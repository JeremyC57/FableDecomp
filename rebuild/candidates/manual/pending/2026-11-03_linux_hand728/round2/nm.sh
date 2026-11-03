# nm.sh addr... : binding spellings + manifest label for each retail address
cd /home/user/FableDecomp
for a in "$@"; do a=$(echo $a | tr A-Z a-z | sed 's/^0x//'); A=$(printf "%08x" 0x$a); AU=$(echo $A | tr a-z A-Z)
 echo "== $A  manifest: $(grep -P "^$A\t" rebuild/manifest/functions.tsv | cut -f2,3 | head -1)"
 grep -iP "\t0x0*$a\t|\t0x$AU\t|\t$A\t|0x$AU" rebuild/oracles/reloc_bindings.tsv | cut -f1 | sort | uniq -c | sort -rn | head -4
done
