#!/bin/bash
# symchk.sh <wd> <row>...: defined-symbol diff (new vs HEAD probe obj) and the entry symbols referenced
D=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF181
wd=$1; shift
for r in "$@"; do
  n=$(find $wd -name "$r.obj" | head -1); h=$D/pS/probe/$r.obj
  def(){ objdump -t "$1" 2>/dev/null | awk '$0 ~ /\(sec +[1-9]/ && $0 !~ /\$/ {print $NF}' | grep -v '^\.' | sort -u; }
  und(){ objdump -t "$1" 2>/dev/null | awk '$0 ~ /\(sec +0\)/ {print $NF}' | sort -u; }
  echo "== $r"; [ -f "$h" ] || echo "  (no HEAD obj)"
  diff <(def $h) <(def $n) | grep '^[<>]' | sed 's/^/  def /'
  diff <(und $h) <(und $n) | grep '^[<>]' | sed 's/^/  und /'
done; true
