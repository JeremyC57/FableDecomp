#!/bin/bash
# usage: stage_pending.sh <agentNN> <name>
# Copy a finished hand-authoring chunk ($S/w22/agentNN) into rebuild/candidates/manual/pending/<name>/ so the
# work survives a container reset. This does NOT grade or land anything: the final re-grade and landing is
# land_pending.sh (run by the Fable 5.1 session, see docs/pipeline/FABLE_LANDING_QUEUE.md).
# Staged: payload.json + oracle.tsv (matched rows only, the same per-function oracle rows a landed batch
# commits), src/ test/ differ/ sources, NOTES.md, STATUS.tsv, and rows.tsv (address/module/name/length of all
# 40 chunk rows, NO bytes -- oracle_from_exe.py rebuilds the parked rows' oracles from Fable.exe into scratch).
set -e
: "${S:?export S=<scratch lane root> first}"
cd "$(git rev-parse --show-toplevel)"
A=$S/w22/$1; P=rebuild/candidates/manual/pending/$2
[ -f "$A/payload.json" ] || { echo "no payload.json in $A (agent not finished?)"; exit 1; }
if grep -lE "__asm|naked|_emit" "$A"/src/*.cpp 2>/dev/null; then echo "asm in matched sources -- refuse"; exit 1; fi
mkdir -p "$P/src" "$P/test" "$P/differ"
cp "$A/payload.json" "$A/oracle.tsv" "$A/NOTES.md" "$P/"
[ -f "$A/STATUS.tsv" ] && cp "$A/STATUS.tsv" "$P/"
cp "$A"/src/*.cpp "$P/src/" 2>/dev/null || true
cp "$A"/test/*.cpp "$P/test/" 2>/dev/null || true
cp "$A"/differ/*.cpp "$P/differ/" 2>/dev/null || true
chunk=${1/agent/chunk}
cut -f1-4 "$S/w22/$chunk.tsv" > "$P/rows.tsv"
python3 - "$P" <<'PY'
import json,sys,os
p=sys.argv[1]; n=len(json.load(open(p+'/payload.json'))['result']['authored'])
d=len([f for f in os.listdir(p+'/differ') if f.endswith('.cpp')])
print(f"staged {p}: {n} matched, {d} parked sources")
PY
