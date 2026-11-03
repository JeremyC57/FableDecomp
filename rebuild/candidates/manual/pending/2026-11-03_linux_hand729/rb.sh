#!/bin/bash
# rb.sh addr...: reloc_bindings rows whose target is addr (distinct symbols with counts)
cd /home/user/FableDecomp
for a in "$@"; do a=$(echo $a | tr A-Z a-z); echo "== $a"; awk -F'\t' -v a="$a" 'tolower($2)==a' rebuild/oracles/reloc_bindings.tsv | cut -f1 | sort | uniq -c | sort -rn | head -4; done
