#!/bin/bash
# rb.sh addr... : landed spellings bound to retail target addresses (reloc_bindings col 2)
cd /home/user/FableDecomp
for a in "$@"; do a=$(echo $a | tr A-F a-f | sed 's/^0x//'); echo "$a: $(awk -F'\t' -v a=$a '$2==a{print $1}' rebuild/oracles/reloc_bindings.tsv | sort | uniq -c | sort -rn | head -4 | awk '{print $2"("$1")"}' | tr '\n' ' ') | cat: $(grep -P "^$a\t" rebuild/oracles/auto-re-candidates.tsv | cut -f2)"; done
