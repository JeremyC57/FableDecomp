# bulkport.py <worklist.tsv> <outdir>: port each landed CTC* constructor row that calls 0x00686800 through a local base model onto
# engine/CTCBase.h. Writes <outdir>/<addr>.cpp and <outdir>/plan.tsv (addr, status, leaf, flags, detail). Rows it cannot handle
# mechanically get status MANUAL with the reason; nothing is written for them.
import re, sys, glob, csv
wl, outdir = sys.argv[1:3]
pdb = set()
for p in ('ghidra_out/egor_pdb_names.tsv', 'ghidra_out/_fablewin_api.tsv'):
    for l in open(p, encoding='utf-8', errors='replace'):
        r = l.rstrip('\n').split('\t'); pdb.add(r[-1])
cat = {l.split('\t')[0]: l.split('\t')[1] for l in open('rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8') if '\t' in l}
plan = []
def code_sub(pat, rep, text):
    out = []
    for line in text.split('\n'):
        if line.lstrip().startswith('//'): out.append(line); continue
        k = line.find('//')
        out.append(re.sub(pat, rep, line[:k]) + line[k:] if k >= 0 else re.sub(pat, rep, line))
    return '\n'.join(out)
for r in csv.DictReader(open(wl), delimiter='\t'):
    a, sym = r['caller_row'], r['symbol_bound']
    if r['target'] != '00686800': continue
    f = glob.glob('rebuild/src/compiled/*/*/*_%s.cpp' % a)[0]
    t = open(f, encoding='utf-8', newline='').read()
    if '\r\n' in t: plan.append((a, 'MANUAL', '', '', 'CRLF source')); continue
    name = cat[a]; parts = name.split('::')
    if len(parts) != 2 or parts[0] != parts[1]: plan.append((a, 'MANUAL', '', '', 'not a plain ctor: ' + name)); continue
    cls = parts[0]
    leaf = '??0%s@@QAE@AAVCThing@@@Z' % cls
    if leaf not in pdb: plan.append((a, 'MANUAL', leaf, '', 'leaf not in either PDB')); continue
    m = re.match(r'\?\?0(\w+)@@', sym)
    base = m.group(1) if m else None
    if not base: plan.append((a, 'MANUAL', leaf, '', 'unparsed base symbol ' + sym)); continue
    L = t.split('\n')
    try:
        i = next(k for k, l in enumerate(L) if re.match(r'^(struct|class) %s\b[^;]*\{' % re.escape(base), l))
    except StopIteration:
        plan.append((a, 'MANUAL', leaf, '', 'local base block %s not found' % base)); continue
    j = next(k for k in range(i, len(L)) if L[k].startswith('};'))
    if base != 'CTCBase' and re.search(r'\bCTCBase\b', '\n'.join(l.split('//')[0] for l in L)):
        plan.append((a, 'MANUAL', leaf, '', 'code already names CTCBase besides ' + base)); continue
    del L[i:j + 1]
    if i < len(L) and L[i].strip() == '' and i > 0 and L[i - 1].strip() == '': del L[i]
    k = next(n for n, l in enumerate(L) if re.match(r'^(#pragma pack|#define|#include|class |struct |namespace )', l))
    L[k:k] = ['#define FABLE_RETAIL_CTCBASE', '#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z']
    t2 = '\n'.join(L)
    t2 = code_sub(r'\b%s\b' % re.escape(base), 'CTCBase', t2)
    n0 = len(re.findall(r'\b%s\((?:const )?\w+\s*\* ?(\w+)\)' % re.escape(cls), t2))
    t2 = code_sub(r'\b%s\((?:const )?(?:CThing|CEngineInternalPrimitive2DClockSprite|void)\s*\* ?(\w+)\)' % re.escape(cls), r'%s(CThing& \1)' % cls, t2)
    n1 = len(re.findall(r'\b%s\(CThing& \w+\)' % re.escape(cls), t2))
    if n1 < 2: plan.append((a, 'MANUAL', leaf, '', 'derived ctor signature not recognised (%d/%d)' % (n0, n1))); continue
    mm = re.search(r'^(struct|class) %s\b[^\n]*\{' % re.escape(cls), t2, re.M)
    if not mm: plan.append((a, 'MANUAL', leaf, '', 'derived class block not found')); continue
    e = t2.index('\n};', mm.start())
    t2 = t2[:mm.start()] + re.sub(r'\n[ \t]*virtual void v00\(\);[^\n]*', '', t2[mm.start():e]) + t2[e:]
    t2 = t2.replace('CTCBase::CTCBase(CThing*)', 'CTCBase::CTCBase(CThing&)')
    t2 = re.sub(r'\b(%s::%s)\((?:CThing|CEngineInternalPrimitive2DClockSprite)\*\)' % (re.escape(cls), re.escape(cls)), r'\1(CThing&)', t2)
    if re.search(r'^// Leaf:.*$', t2, re.M):
        t2 = re.sub(r'^// Leaf:.*$', '// Leaf: ' + leaf, t2, count=1, flags=re.M)
    else:
        m2 = re.search(r'^// Retail:.*$', t2, re.M) or re.search(r'^//.*$', t2, re.M)
        t2 = t2[:m2.start()] + '// Leaf: ' + leaf + '\n' + t2[m2.start():]
    note = ('// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model %s (which bound %s to 0x00686800) is gone;\n'
            '//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (%s).\n' % (base, sym, leaf))
    m3 = re.search(r'^// Leaf:.*\n', t2, re.M)
    t2 = t2[:m3.end()] + note + t2[m3.end():]
    fl = re.search(r'^// Flags:\s*(.*)$', t2, re.M)
    open('%s/%s.cpp' % (outdir, a), 'w', encoding='utf-8').write(t2)
    plan.append((a, 'PORTED', leaf, ' '.join(x for x in (fl.group(1).split() if fl else []) if x != 'none'), base))
with open(outdir + '/plan.tsv', 'w') as o:
    for p in plan: o.write('\t'.join(p) + '\n')
import collections
print(collections.Counter(p[1] for p in plan)); print(collections.Counter(p[4] for p in plan if p[1] == 'MANUAL').most_common(12))
