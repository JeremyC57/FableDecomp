#!/usr/bin/env python3
"""Apply a labels file (address, name, evidence) to the catalog and the manifest.

Owner rule 2026-11-01: when a catalog name is wrong, fix it at landing -- not only for the chunk's own rows (vland does those)
but for every callee / global / neighbour the agent proved (the labels file). This rewrites
  rebuild/oracles/auto-re-candidates.tsv  column `name`            (rows that exist there)
  rebuild/manifest/functions.tsv          columns `name`, `module` (rows that exist there)
to the label's qualified name, splitting `Scope::Member` on the last `::` outside template brackets (module = scope, or
`_global`). Landed rows are then re-synced from their Leaf by tools/sync_catalog_names.py (run it after this script).
    python3 apply_labels.py ghidra_out/labels_chunk657_2026-10-27.tsv [more...] [--dry-run]
"""
import sys, csv, io

def split_scope(name):
    depth = 0; cut = -1
    i = 0
    while i < len(name):
        c = name[i]
        if c == '<': depth += 1
        elif c == '>': depth -= 1
        elif c == ':' and depth == 0 and name[i:i+2] == '::':
            cut = i; i += 2; continue
        i += 1
    if cut < 0: return '_global', name
    return name[:cut], name[cut+2:]

def read_tsv(p):
    with open(p, encoding='utf-8', newline='') as f:
        txt = f.read()
    nl = '\r\n' if '\r\n' in txt else '\n'
    rows = [l.split('\t') for l in txt.split(nl)]
    return rows, nl

def write_tsv(p, rows, nl):
    with open(p, 'w', encoding='utf-8', newline='') as f:
        f.write(nl.join('\t'.join(r) for r in rows))

def main(argv):
    dry = '--dry-run' in argv; files = [a for a in argv if not a.startswith('--')]
    labels = {}
    for p in files:
        for r in csv.DictReader(open(p, encoding='utf-8'), delimiter='\t'):
            a = r['address'].lower().replace('0x', '').zfill(8); n = r['name'].strip()
            if a and n and n not in ('1','2','3'): labels[a] = n
    cat = 'rebuild/oracles/auto-re-candidates.tsv'; man = 'rebuild/manifest/functions.tsv'
    rows, nl = read_tsv(cat); nc = 0
    for r in rows[1:]:
        if len(r) > 1 and r[0].lower() in labels and r[1] != labels[r[0].lower()]:
            print(f'catalog  {r[0]} {r[1]} -> {labels[r[0].lower()]}'); r[1] = labels[r[0].lower()]; nc += 1
    if not dry: write_tsv(cat, rows, nl)
    rows, nl = read_tsv(man); hdr = rows[0]; iN = hdr.index('name'); iM = hdr.index('module'); nm = 0
    for r in rows[1:]:
        if len(r) > iM and r[0].lower() in labels:
            mod, nam = split_scope(labels[r[0].lower()])
            if (r[iN], r[iM]) != (nam, mod):
                print(f'manifest {r[0]} {r[iM]}::{r[iN]} -> {mod}::{nam}'); r[iN] = nam; r[iM] = mod; nm += 1
    if not dry: write_tsv(man, rows, nl)
    print(f'labels {len(labels)}  catalog renamed {nc}  manifest renamed {nm}{"  (dry run)" if dry else ""}')

if __name__ == '__main__': main(sys.argv[1:])
