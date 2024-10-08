run 'Command args match' {
	$XS -c 'echo $0 $2 $#*' a b c d e f
}
conds {match $XS b 6}

run 'Failure on empty command' {
	$XS -c >[2] /dev/null
}
conds expect-failure
