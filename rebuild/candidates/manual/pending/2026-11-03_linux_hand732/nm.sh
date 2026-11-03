#!/bin/bash
# nm.sh addr...: catalog name + bound symbols (reloc_bindings target column)
cd /home/user/FableDecomp
for a in "$@"; do a=$(echo $a | tr 'A-F' 'a-f' | sed 's/^0x//'); a=$(printf "%08x" 0x$a)
 echo "== $a cat: $(grep -P "^$a\t" rebuild/oracles/auto-re-candidates.tsv | cut -f2 | head -1)"
 awk -F'\t' -v t=$a '$2==t{print "   "$1}' rebuild/oracles/reloc_bindings.tsv | sort | uniq -c | sort -rn | head -5
done
