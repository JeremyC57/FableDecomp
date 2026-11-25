#!/usr/bin/env bash
# gsum.sh <batch>: one-screen gate summary (objgate non-EQ, pk BAD, link, vland grade diffs, LITERAL, new warn kinds)
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; W=$S/w22/agentNF443; b=$1; cd $W
echo "rows $(wc -l < batches/$b/rows.txt)"; grep -v ALL_EQ work/og_$b.out | cut -c1-200; grep -v "	OK	" work/pk_$b.out | cut -c1-200 | tail -4
cut -c1-250 work/lk_$b.out
diff <(grep -oE "^[0-9a-f]{8}\s+(RELOC|MATCH|DIFF|COMP|FAIL)\S*\s+\S+" work/vland_${b}h.log) <(grep -oE "^[0-9a-f]{8}\s+(RELOC|MATCH|DIFF|COMP|FAIL)\S*\s+\S+" work/vland_${b}n.log) && echo grades-equal
echo "LITERAL $(grep -c LITERAL work/vland_${b}n.log)"; python3 tools/warncmp.py $b | grep -v "\['SHARED_HEADER_AVAILABLE'\]"
