#!/usr/bin/env python3
"""wl425.py: worklist of the CLONE / COPY rtti_identified family.
Input: work/family.tsv (OPEN.tsv rows, status rtti_identified, rtti column ?Clone@ / ?Copy@).
Output: evidence/worklist.tsv with per row: address, rtti symbol, class, kind, retail length, normalized body signature, call / jmp
targets (with the table's symbols), new-size immediate, vtable stores, HEAD self leaf, HEAD source, HEAD includes, lock, t3 flag."""
import csv, sys, os, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import nf, capstone
R = '/home/user/FableDecomp'; A = str(nf.A)
csv.field_size_limit(10**9)
fam = [l.rstrip('\n').split('\t') for l in open(A + '/work/family.tsv')]
cat = nf.catalog()
tab = collections.defaultdict(list)
for r in csv.DictReader(open(R + '/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t'):
    tab[r['target'].lower()].append(r['symbol'])
vt = collections.defaultdict(list)
for r in csv.DictReader(open(R + '/rebuild/manifest/rtti-vtables.tsv'), delimiter='\t'):
    vt[r['func_va'].lower()].append((r['class'], r['vtable_va'], r['slot']))
t3set = set()
for l in open(R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv').read().splitlines()[1:]:
    q = l.split('\t')
    if len(q) > 2:
        for x in re.findall(r'0x([0-9a-fA-F]{8})', q[1] + ' ' + q[2]): t3set.add(x.lower())
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
def sym(t):
    s = tab.get('%08x' % t)
    return s[0] if s else 'NOSYM'
out = csv.writer(open(A + '/evidence/worklist.tsv', 'w'), delimiter='\t', lineterminator='\n')
out.writerow(['address', 'kind', 'rtti', 'klass', 'length', 'sig', 'calls', 'newsize', 'vtstores', 'self_leaf', 'source', 'test', 'includes',
              'lock', 't3', 'vtables'])
for f in fam:
    a = f[0]; rt = f[12]; self_ = f[7]; src = f[10]
    kind = 'Clone' if rt.startswith('?Clone@') else 'Copy'
    m = re.match(r'\?(?:Clone|Copy)@(.+?)@@[UEM]', rt); klass = '::'.join(reversed(m.group(1).split('@'))) if m else '?'
    c = cat.get(a)
    if not c:
        out.writerow([a, kind, rt, klass, 'NOCAT'] + [''] * 11); continue
    n = int(c['length']); b = bytes.fromhex(c['bytes']); va = int(a, 16)
    sig = []; calls = []; newsize = ''; vts = []
    for i in md.disasm(b, va):
        ops = i.op_str
        if i.mnemonic in ('call', 'jmp') and re.match(r'0x[0-9a-f]+$', ops):
            t = int(ops, 16)
            if va <= t < va + n: sig.append(i.mnemonic + ' L'); continue
            calls.append('%s:%08x:%s' % (i.mnemonic, t, sym(t))); sig.append(i.mnemonic + ' X'); continue
        if re.match(r'j\w+$', i.mnemonic) and re.match(r'0x[0-9a-f]+$', ops): sig.append(i.mnemonic + ' L'); continue
        mm = re.search(r'dword ptr \[(\w+)\], (0x[0-9a-f]{7,8})$', ops)
        if i.mnemonic == 'mov' and mm: vts.append(mm.group(2)); sig.append('mov [%s],VT' % mm.group(1)); continue
        if i.mnemonic == 'push' and re.match(r'0x[0-9a-f]+$', ops) and not newsize and len(calls) == 0:
            newsize = ops
        sig.append(i.mnemonic + ' ' + re.sub(r'0x[0-9a-f]{6,8}', 'IMM', ops))
    s = nf.src_of(a); t = nf.test_of(a)
    incs = ''
    if s and s.exists():
        incs = ','.join(re.findall(r'^#include\s+"([^"]+)"', s.read_text(errors='replace'), re.M))
    out.writerow([a, kind, rt, klass, n, ' ; '.join(sig), ' '.join(calls), newsize, ' '.join(vts), self_,
                  str(s).replace(R + '/rebuild/src/compiled/', '') if s else 'NOSRC', str(t).replace(R + '/rebuild/tests/', '') if t else 'NOTEST',
                  incs, nf.lock_owner(a), 'T3' if a in t3set else '', ' '.join('%s@%s#%s' % x for x in vt.get(a, []))])
print('ok', len(fam))
