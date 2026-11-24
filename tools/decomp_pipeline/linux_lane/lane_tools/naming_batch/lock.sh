#!/usr/bin/env bash
# Claim (or release) a row lock for a naming batch agent.
#   lock.sh <AGENT> <addr> [release]      addr: 8 hex digits, any case, with or without 0x
# Locks live in $S/w22/nflocks/<addr>/owner (plain lowercase 8-hex dir, no 0x). A lock is free when the owner file is missing
# or reads `released`. Prints `OK <addr>` when the caller holds it afterwards, `HELD <addr> <owner>` (exit 1) otherwise.
set -u
[ -n "${S:-}" ] || { echo "S not set (source \$S/wine/env.sh)"; exit 2; }
AG=$1; A=$(echo "$2" | tr 'A-F' 'a-f' | sed 's/^0x//'); MODE=${3:-claim}
[[ $A =~ ^[0-9a-f]{8}$ ]] || { echo "bad addr $2"; exit 2; }
L=$S/w22/nflocks/$A; mkdir -p "$L"
exec 9>"$L/.flock"; flock 9
cur=$(cat "$L/owner" 2>/dev/null || echo released)
if [ "$MODE" = release ]; then
  if [ "$cur" = "$AG" ]; then echo released > "$L/owner"; echo "RELEASED $A"; exit 0; fi
  echo "NOT_OWNER $A $cur"; exit 1
fi
if [ "$cur" = released ] || [ "$cur" = "$AG" ]; then echo "$AG" > "$L/owner"; echo "OK $A"; exit 0; fi
echo "HELD $A $cur"; exit 1
