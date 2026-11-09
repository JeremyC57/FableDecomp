#!/usr/bin/env python3
"""gen.py <base.cpp> <edits.py> [names...]: one variant per named edit (or '+'-joined combos), compile-only fitness via cf.sh,
one line each (tools/srch/SEARCH.tsv). edits.py defines EDITS = {name: [(old, new, occurrence_or_None), ...]}; an occurrence
index picks the n-th match (0-based), None requires a unique match. Variant sources are deleted unless KEEP=1."""
import sys, os, subprocess, runpy
from pathlib import Path
here = Path(__file__).resolve().parent
base = Path(sys.argv[1]).read_text()
E = runpy.run_path(sys.argv[2])['EDITS']
names = sys.argv[3:] or list(E)
def apply(text, edits):
    for old, new, occ in edits:
        n = text.count(old)
        if occ is None:
            if n != 1: raise ValueError(f'{old[:60]!r} occurs {n}x')
            text = text.replace(old, new)
        else:
            i = -1
            for _ in range(occ + 1):
                i = text.find(old, i + 1)
                if i < 0: raise ValueError(f'{old[:60]!r} occurrence {occ} missing')
            text = text[:i] + new + text[i + len(old):]
    return text
for nm in names:
    edits = []
    for part in nm.split('+'): edits += E[part]
    try: v = apply(base, edits)
    except ValueError as ex: print(nm, 'EDIT_FAIL', ex); continue
    p = here / 'v' / f'{nm}.cpp'; p.write_text(v)
    r = subprocess.run(['bash', str(here / 'cf.sh'), str(p), nm], capture_output=True, text=True)
    print(r.stdout.strip()[:400], flush=True)
    if not os.environ.get('KEEP'): p.unlink()
    o = here / 'obj' / f'{nm}.obj'
    if not os.environ.get('KEEPOBJ') and o.exists(): o.unlink()
