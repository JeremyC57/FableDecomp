#!/usr/bin/env python3
"""pdb_proc.py <symbols dump> <names file> <out dir> <label>

Per-function records from Ego_r.pdb / FableWin.pdb (pass 27). Both are VS 2010 (cl 16.00.40219) builds of the same engine, not the
VC7.1 retail build: Ego_r is the game's release build (`release\*.obj`), FableWin the editor's debug build (`debug\*.obj`, edit and
continue). Their names, parameter / local names, types and call-site signatures (S_CALLSITEINFO) are source evidence; code sizes,
frame offsets and registers are NOT a retail oracle. A local missing from an Ego_r record was optimised away, not absent from the
source; Ego_d is the locals oracle (FableWin's debug records can corroborate it).

Make the symbols dump first (both PDBs use 1 KB blocks that llvm-pdbutil refuses; repack them once):
    python3 tools/repack4k.py Ego_r.pdb Ego_r.4k.pdb
    llvm-pdbutil dump -symbols Ego_r.4k.pdb > Ego_r.symbols.txt
The names file has one `key<TAB>qualified name[<TAB>qualified name...]` per line (key = output file stem, e.g. the row address).
A name matches a procedure exactly, or by its last two `::` parts when that tail names one procedure only. Every matching record is
written (overloads, statics and several objects each get their own block, headed by the object file), with S_LABEL32 lines dropped.
Writes <out dir>/<key>.<label>.txt; prints one summary line per key.
"""
import collections, os, re, sys
dump, namesf, outd, label = sys.argv[1:5]
os.makedirs(outd, exist_ok=True)
want = []
for l in open(namesf, encoding='utf-8'):
    f = l.rstrip('\n').split('\t')
    if len(f) >= 2: want.append((f[0], [x for x in f[1:] if x and x != '-']))
names = set(x for _, qs in want for x in qs)
tails = collections.defaultdict(set)
for q in names: tails['::'.join(q.split('::')[-2:])].add(q)
REC = re.compile(r'^\s*(\d+) \| (S_\w+)')
PROC = re.compile(r'^\s*\d+ \| S_[GL]PROC32 \[size = \d+\] `(.*)`\s*$')
found = collections.defaultdict(list)     # proc name -> [(object, [lines])]
seen_tail = collections.defaultdict(set)  # tail -> proc names seen in the dump
mod = None; cur = None; endoff = None; buf = None; skip = False
with open(dump, encoding='utf-8', errors='replace') as fh:
    for line in fh:
        if line.startswith('Mod ') or line.startswith('  Mod '):
            m = re.search(r'`(.*)`', line); mod = m.group(1) if m else line.strip(); cur = None; continue
        if cur is not None:
            m = REC.match(line)
            if m:
                if int(m.group(1)) == endoff:
                    buf.append(line.rstrip()); found[cur].append((mod, buf)); cur = None; continue
                skip = m.group(2) == 'S_LABEL32'
            if not skip: buf.append(line.rstrip())
            continue
        m = PROC.match(line)
        if not m: continue
        nm = m.group(1); tl = '::'.join(nm.split('::')[-2:])
        if nm in names or tl in tails:
            seen_tail[tl].add(nm)
            cur = nm; buf = [line.rstrip()]; skip = False
            nxt = next(fh)
            buf.append(nxt.rstrip())
            em = re.search(r'end = (\d+)', nxt)
            endoff = int(em.group(1)) if em else -1
for key, qs in want:
    blocks = []; how = []
    for q in qs:
        if q in found:
            blocks = [(q, ob, ls) for ob, ls in found[q]]; how.append(f'exact {q}'); break
        tl = '::'.join(q.split('::')[-2:])
        cand = sorted(n for n in seen_tail.get(tl, ()) if n in found and '::' in tl)
        if len(cand) == 1:
            blocks = [(cand[0], ob, ls) for ob, ls in found[cand[0]]]; how.append(f'tail {tl} -> {cand[0]}'); break
    if not blocks:
        print(f'{key}\t{label}\tnone\t' + ' | '.join(qs)); continue
    with open(f'{outd}/{key}.{label}.txt', 'w') as o:
        kind = {'Ego_r': 'VS 2010 release build of the game', 'FableWin': 'VS 2010 debug build of the editor'}.get(label, 'VS 2010 build')
        o.write(f'# {label}.pdb ({kind}, same engine source; names / types / call-site signatures are evidence, '
                f'sizes / frame offsets / registers are not retail). Matched by {how[0]}; {len(blocks)} record(s).\n')
        for q, ob, ls in blocks:
            o.write(f'\n== {q}  [{ob}]\n')
            for x in ls: o.write(x + '\n')
    print(f'{key}\t{label}\t{len(blocks)}\t{how[0]}')
