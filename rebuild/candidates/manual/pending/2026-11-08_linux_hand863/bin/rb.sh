#!/bin/bash
# rb.sh <hex target> ... : symbols bound to that retail target in reloc_bindings.tsv (count)
for t in "$@"; do t=$(echo $t | sed 's/^0x//' | tr A-F a-f); t=$(printf "%08x" 0x$t); echo "== $t"; awk -F'\t' -v t=$t '$2==t{print $1}' /home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv | sort | uniq -c | sort -rn | head -6; done
