"""batch_rerow.py RENAMES.tsv rows_dir skip_file: apply section 1 of a naming hand-back's RENAMES.tsv to landed rows.

For each row (address 0x..., module = class or `_global`/`_static_init`, name = method only) it:
  - renames the landed source and test to <Module>_<Name>_<addr>[_test].cpp (file names drop template arguments and are capped
    at 80 characters, as the landed files are),
  - writes the new source text from rows_dir/<addr>.cpp,
  - updates build_candidates.ps1 (Module / Source / TestSource; PassPattern is KEPT),
  - updates the catalog name (rebuild/oracles/auto-re-candidates.tsv) and the manifest name/module.
Rows whose address is listed in skip_file are left alone. Prints the renamed addresses to $OUT (default stdout) and a summary.
Run from the repo root. Copy the remaining (non-renamed) rows over their landed sources separately, then recheck.
After a landing that moves files, run coordinator/funclet_includes_check.py: EH funclet rows include their parent by path.
"""
import csv
import os
import re
import sys

ren_p, rows_d, skip_p = sys.argv[1:4]
skip = {l.strip().lower() for l in open(skip_p) if l.strip()}
san = lambda s: re.sub(r'[^A-Za-z0-9]', '', s.replace('::', '')) or 'global'


def untemplate(s):
    out, d = '', 0
    for c in s:
        if c == '<':
            d += 1
        elif c == '>':
            d -= 1
        elif d == 0:
            out += c
    return out


fsan = lambda s: san(untemplate(s))[:80] or 'global'
ren = [r for r in csv.DictReader(open(ren_p), delimiter='\t') if r.get('address', '').lower().startswith('0x')]
ren = [r for r in ren if r['address'].lower()[2:].zfill(8) not in skip]
ps_p = 'rebuild/build_candidates.ps1'
raw = open(ps_p, 'rb').read()
bom = raw.startswith(b'\xef\xbb\xbf')
crlf = b'\r\n' in raw[:4096]
ps = raw.decode('utf-8-sig').replace('\r\n', '\n')
newname, moves = {}, []
for r in ren:
    a = r['address'].lower()[2:].zfill(8)
    mod, name = r['module'], r['name']
    m = re.search(r"(\[pscustomobject\]@\{\s*\n\s*Address = '%s'.*?\})" % a, ps, re.S)
    if not m:
        print('NO BLOCK', a)
        continue
    blk = m.group(1)
    kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
    d = kv['Source'].rsplit('/', 1)[0]
    base = f"{fsan(mod) if mod != '_global' else 'global'}_{fsan(name)}_{a}"
    nsrc, ntst = f'{d}/{base}.cpp', f'{d}/{base}_test.cpp'
    for old, new, root in ((kv['Source'], nsrc, 'rebuild/src/compiled/'), (kv.get('TestSource'), ntst, 'rebuild/tests/')):
        if old and old != new:
            if os.path.exists(root + new):
                print('TARGET EXISTS', a, new)
                raise SystemExit(0)
            moves.append((root + old, root + new))
    nb = blk.replace(f"Module = '{kv['Module']}'", f"Module = '{mod}'").replace(f"Source = '{kv['Source']}'", f"Source = '{nsrc}'")
    if kv.get('TestSource'):
        nb = nb.replace(f"TestSource = '{kv['TestSource']}'", f"TestSource = '{ntst}'")
    ps = ps.replace(blk, nb)
    newname[a] = (mod, name, 'rebuild/src/compiled/' + nsrc)
for old, new in moves:
    os.rename(old, new)
for a, (mod, name, src) in newname.items():
    p = os.path.join(rows_d, a + '.cpp')
    if os.path.exists(p):
        open(src, 'w').write(open(p).read())
out_ps = ps.replace('\n', '\r\n') if crlf else ps
open(ps_p, 'wb').write((b'\xef\xbb\xbf' if bom else b'') + out_ps.encode('utf-8'))
for path, col in (('rebuild/oracles/auto-re-candidates.tsv', 'cat'), ('rebuild/manifest/functions.tsv', 'man')):
    lines = open(path, encoding='utf-8').read().split('\n')
    n = 0
    for i, l in enumerate(lines):
        a = l.split('\t', 1)[0]
        if a in newname:
            c = l.split('\t')
            mod, name, _ = newname[a]
            if col == 'cat':
                c[1] = f'{mod}::{name}' if mod != '_global' else name
            else:
                c[1] = name
                c[2] = mod
            lines[i] = '\t'.join(c)
            n += 1
    open(path, 'w', encoding='utf-8').write('\n'.join(lines))
    print(path, n)
open(os.environ.get('OUT', '/dev/stdout'), 'w').write('\n'.join(sorted(newname)) + '\n')
print('renamed', len(newname), 'moves', len(moves))
