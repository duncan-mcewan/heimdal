# Make prototypes from .c files
# $Id$

##use Getopt::Std;
require 'getopts.pl';

$brace = 0;
$line = "";
$debug = 0;

do Getopts('o:p:d') || die "foo";

if($opt_d) {
    $debug = 1;
}

while(<>) {
    print $brace, " ", $_ if($debug);
    if(/^\#if 0/) {
	$if_0 = 1;
    }
    if($if_0 && /^\#endif/) {
	$if_0 = 0;
    }
    if($if_0) { next }
    if(/^\s*\#/) {
	next;
    }
    if(/^\s*$/) {
	$line = "";
	next;
    }
    if(/\{/){
	if($line =~ /\)\s$/){
	    $_ = $line;
	    while(s/\*\//\ca/){
		s/\/\*(.|\n)*\ca//;
	    }
	    s/^\s*//;
	    s/\s$//;
	    s/\s+/ /g;
	    if(!/^static/ && !/^PRIVATE/){
		# remove outer ()
		s/\s*\(/@/;
		s/\)$/@/;
		# remove , within ()
		while(s/\(([^()]*),(.*)\)/($1\$$2)/g){}
		s/,\s*/,\n\t/g;
		# fix removed ,
		s/\$/,/g;
		# match function name
		/([a-zA-Z0-9_]+)\s*@/;
		$f = $1;
		# only add newline if more than one parameter
                if(/,/){ 
		    s/@/ __P((\n\t/;
		}else{
		    s/@/ __P((/;
		}
		s/@/))/;
		$_ = $_ . ";";
		# insert newline before function name
		s/(.*)\s([a-zA-Z0-9_]+ __P)/$1\n$2/;
		$funcs{$f} = $_;
	    }
	}
	$line = "";
	$brace++;
    }
    if(/\}/){
	$brace--;
    }
    if(/^\}/){
	$brace = 0;
    }
    if($brace == 0) {
	$line = $line . " " . $_;
    }
}

sub foo {
    local ($arg) = @_;
    $_ = $arg;
    s/.*\/([^\/]*)/$1/;
    s/[^a-zA-Z0-9]/_/g;
    "__" . $_ . "__";
}

if($opt_o) {
    open(OUT, ">$opt_o");
    $block = &foo($opt_o);
} else {
    $block = "__public_h__";
}

if($opt_p) {
    open(PRIV, ">$opt_p");
    $private = &foo($opt_p);
} else {
    $private = "__private_h__";
}

$public_h = "";
$private_h = "";

$public_h_header = "/* This is a generated file */
#ifndef $block
#define $block

#ifdef __STDC__
#include <stdarg.h>
#ifndef __P
#define __P(x) x
#endif
#else
#ifndef __P
#define __P(x) ()
#endif
#endif

";

$private_h_header = "/* This is a generated file */
#ifndef $private
#define $private

#ifdef __STDC__
#include <stdarg.h>
#ifndef __P
#define __P(x) x
#endif
#else
#ifndef __P
#define __P(x) ()
#endif
#endif

";

foreach(sort keys %funcs){
    if(/^(main)$/) { next }
    if(/^_/) {
	$private_h .= $funcs{$_} . "\n\n";
    } else {
	$public_h .= $funcs{$_} . "\n\n";
    }
}


if ($public_h ne "") {
    $public_h = $public_h_header . $public_h . "#endif /* $block */\n";
}
if ($private_h ne "") {
    $private_h = $private_h_header . $private_h . "#endif /* $private */\n";
}

if($opt_o) {
    print OUT $public_h;
} 
if($opt_p) {
    print PRIV $private_h;
} 

close OUT;
close PRIV;
