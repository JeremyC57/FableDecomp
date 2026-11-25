#!/usr/bin/env bash
# pn.sh <Method...>: Ego_r.pdb params / locals of CGameScriptInterface::<Method>
for m in "$@"; do n=$(grep -n "S_GPROC32 .*\`CGameScriptInterface::$m\`" /tmp/claude-0/egod_pdb/Ego_r.symbols.txt | head -1 | cut -d: -f1); [ -z "$n" ] && { echo "$m: none"; continue; }
 echo -n "$m:"; sed -n "$((n+1)),$((n+80))p" /tmp/claude-0/egod_pdb/Ego_r.symbols.txt | awk '/S_GPROC32|S_END/{exit} /S_BPREL32|S_REGREL32|S_REGISTER|S_LOCAL/{match($0,/`[^`]*`/); nm=substr($0,RSTART,RLENGTH); getline; match($0,/\(.*\)/); printf " %s%s", nm, substr($0,RSTART,RLENGTH)}'; echo; done
