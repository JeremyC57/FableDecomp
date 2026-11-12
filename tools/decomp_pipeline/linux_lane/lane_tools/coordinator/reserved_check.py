"""reserved_check.py <payload.json | rows-dir | file-with-addresses> ...: stop reserved rows reaching a landing.

Collects every address in rebuild/candidates/manual/pending/*/RESERVED.tsv -- from the working tree and, when the ref exists, from
each branch named in $RESERVED_REFS (default: origin/FullDecomp-B, the second account's branch) -- and reports which of the given
addresses are reserved. Inputs: an agent payload.json (result.authored[].address), a directory of <addr>.cpp / <addr>_test.cpp
rows, or any text file (every 8-hex token at the start of a line or after 0x). It also lists every row whose source mentions a reserved address (`0x........`): usually a callee declaration, which is fine, but
a row that DEFINES a body for a reserved function (a "stand-in body") must be held back -- the script flags rows whose text says so,
and the coordinator reads the others. Run it before every landing and before cutting a chunk.
Prints `reserved: N` and the hits; always exits 0.
"""
import glob
import json
import os
import re
import subprocess
import sys

H8 = re.compile(r'^[0-9a-f]{8}$')


def addrs_of(text):
    out = {}
    for l in text.splitlines()[1:]:
        a = l.split('\t')[0].strip().lower().replace('0x', '')
        if H8.match(a):
            out[a] = True
    return set(out)


res = {}
for f in glob.glob('rebuild/candidates/manual/pending/*/RESERVED.tsv'):
    for a in addrs_of(open(f, encoding='utf-8', errors='replace').read()):
        res.setdefault(a, f.split('/')[-2])
for ref in os.environ.get('RESERVED_REFS', 'origin/FullDecomp-B').split():
    ls = subprocess.run(['git', 'ls-tree', '-r', '--name-only', ref, '--', 'rebuild/candidates/manual/pending'],
                        capture_output=True, text=True)
    for f in ls.stdout.split():
        if f.endswith('/RESERVED.tsv'):
            t = subprocess.run(['git', 'show', f'{ref}:{f}'], capture_output=True, text=True).stdout
            for a in addrs_of(t):
                res.setdefault(a, f'{ref}:{f.split("/")[-2]}')

rows, sources = set(), []
for p in sys.argv[1:]:
    if os.path.isdir(p):
        for f in glob.glob(os.path.join(p, '*.cpp')):
            m = re.match(r'([0-9a-f]{8})', os.path.basename(f))
            if m:
                rows.add(m.group(1))
            sources.append((os.path.basename(f), open(f, encoding='utf-8', errors='replace').read()))
    elif p.endswith('.json'):
        for c in json.load(open(p))['result']['authored']:
            rows.add(c['address'].lower().replace('0x', ''))
            sources.append((c['address'], c.get('source_cpp', '')))
    else:
        for l in open(p, encoding='utf-8', errors='replace'):
            m = re.match(r'\s*(?:0x)?([0-9a-fA-F]{8})\b', l)
            if m:
                rows.add(m.group(1).lower())
hits = sorted(a for a in rows if a in res)
print(f'reserved lists: {len(res)} addresses; checked {len(rows)} rows; reserved: {len(hits)}')
for a in hits:
    print('  RESERVED', a, res[a])
mentions = {}
for name, src in sources:
    standin = bool(re.search(r'stand-?in body', src, re.I))
    for m in re.finditer(r'0x([0-9A-Fa-f]{8})', src):
        a = m.group(1).lower()
        if a in res and not name.lower().replace('0x', '').startswith(a):
            mentions[(name, a)] = standin
print(f'rows that mention a reserved address (callee declared, or a body defined for it -- read these): {len(mentions)}')
for (name, a), standin in sorted(mentions.items()):
    print('  MENTIONS', name, a, res[a], '<-- text says "stand-in body": hold the row back' if standin else '')
