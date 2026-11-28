#!/usr/bin/env python3
"""prep_evidence.py: extract the per-row PDB evidence the pass-27 build.py packages (run once before build.py).

  EGOD_EXTRACT/<key>.txt, <key>.lines.txt   Ego_d locals (egod_locals.py) and line table (egod_proc.py + egod_lines.py)
  REL_EXTRACT/<addr>.Ego_r.txt, .FableWin.txt   Ego_r / FableWin procedure records (pdb_proc.py)

env: EGOD_PDB (Ego_d.pdb), EGOD_EXTRACT, REL_EXTRACT, EGOR_SYMBOLS / FABLEWIN_SYMBOLS (llvm-pdbutil dump -symbols of the 4 KB repacks).
Only names, types and line sizes are extracted; no PDB is copied anywhere.
"""
import os, re, subprocess, sys
HERE = os.path.dirname(os.path.abspath(__file__)); PKG = os.path.dirname(HERE)
R = '/home/user/FableDecomp'
import hashlib
def egod_key(en):
    """file stem for an Ego_d name; long template names fall back to a short hash (build.py uses the same rule)"""
    k = re.sub(r'[^A-Za-z0-9_~.-]', '_', en)
    return k if len(k) <= 120 else k[:80] + '_' + hashlib.sha1(en.encode()).hexdigest()[:12]
def demangle_leaf(leaf):
    m = re.match(r'\?([A-Za-z_]\w*)@((?:[A-Za-z_]\w*@)*)@', leaf or '')
    if not m: return None
    return '::'.join(list(reversed([s for s in m.group(2).split('@') if s])) + [m.group(1)])
def leaf_of(path):
    try:
        for x in open(path, encoding='utf-8', errors='replace'):
            if x.startswith('// Leaf:'):
                s = x.split(); return s[2] if len(s) > 2 else None
    except OSError: pass
    return None
PDB = os.environ['EGOD_PDB']; EGX = os.environ['EGOD_EXTRACT']; REL = os.environ['REL_EXTRACT']
os.makedirs(EGX, exist_ok=True); os.makedirs(REL, exist_ok=True)
man = {}
for l in open(f'{R}/rebuild/manifest/functions.tsv', encoding='utf-8-sig'):
    f = l.rstrip('\n').split('\t')
    if f and re.fullmatch(r'[0-9a-fA-F]{8}', f[0]): man[f[0].lower()] = f
hdr = open(f'{R}/rebuild/manifest/functions.tsv', encoding='utf-8-sig').readline().rstrip('\n').split('\t')
mi, ni = hdr.index('module'), hdr.index('name')
sel = [l.rstrip('\n').split('\t') for l in open(f'{PKG}/rows.tsv') if l.strip()]
names = []
for a, ln, grade, path, en, ob, summ, extras in sel:
    m = man.get(a); mod, nm = (m[mi], m[ni]) if m else ('?', '?')
    qs = []
    if en != '-': qs.append(en)
    dl = demangle_leaf(leaf_of(f'{R}/{path}'))
    if dl and '::' in dl: qs.append(dl)
    if nm.startswith('?'):
        d2 = demangle_leaf(nm)
        if d2 and '::' in d2: qs.append(d2)
    elif '::' in nm: qs.append(nm)
    elif mod not in ('_global', '_gapscan', '?', ''): qs.append(f'{mod}::{nm}')
    qs = [q for i, q in enumerate(qs) if q not in qs[:i] and not re.search(r'(vfunc|sub_|FUN_|Unproven|_[0-9a-f]{8})', q)]
    names.append((a, qs))
    if en == '-': continue
    key = egod_key(en)
    lo = subprocess.run([sys.executable, f'{HERE}/egod_locals.py', PDB, en], capture_output=True, text=True).stdout
    open(f'{EGX}/{key}.txt', 'w').write(lo)
    pr = subprocess.run([sys.executable, f'{HERE}/egod_proc.py', PDB, en], capture_output=True, text=True).stdout
    pm = re.search(r'proc off=(\d+) seg=(\d+) len=(\d+)', pr)
    if pm:
        li = subprocess.run([sys.executable, f'{HERE}/egod_lines.py', PDB, pm.group(2), pm.group(1), pm.group(3)],
                            capture_output=True, text=True).stdout
        open(f'{EGX}/{key}.lines.txt', 'w').write(f'# {en}: Ego_d proc off={pm.group(1)} seg={pm.group(2)} len={pm.group(3)}\n' + pr + li)
    print(a, en, 'locals', len(lo.splitlines()), 'lines', 'yes' if pm else 'no')
nf = f'{REL}/names.tsv'
with open(nf, 'w') as o:
    for a, qs in names: o.write('\t'.join([a] + (qs or ['-'])) + '\n')
for label, env in (('Ego_r', 'EGOR_SYMBOLS'), ('FableWin', 'FABLEWIN_SYMBOLS')):
    subprocess.run([sys.executable, f'{HERE}/pdb_proc.py', os.environ[env], nf, REL, label], check=True)
