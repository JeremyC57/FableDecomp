#!/usr/bin/env python3
"""Wave 3b: the per-script template vtables that hang off the same evidence.
  CEntityScriptBinding<T>  = the binding vtable stored at each registration site in T::Main (2 slots: GetEntityScriptName, CreateActiveEntityScript)
  CActiveEntityScript<T>   = the vtable CEntityScriptBinding<T>::CreateActiveEntityScript stores after operator new
  CSpawnedFunc<T>          = the vtable T::RegisterMain stores after operator new(0x3c) (the "Main" spawned function)
Rows only for refs==1 slot functions whose PDB member exists for that exact instantiation and whose current name is a placeholder
(or provably wrong)."""
import sys, os, re, json, struct, collections, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import regscan as RS
E = RS.E
W = os.path.dirname(os.path.abspath(__file__)) + '/..'
R = '/home/user/FableDecomp'
J = json.load(open(W + '/scan.json'))
review = list(csv.DictReader(open(W + '/review.tsv'), delimiter='\t'))
cat = {}
for l in open(R + '/rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8'):
    c = l.rstrip('\n').split('\t')
    if len(c) > 1: cat[c[0]] = c[1]
man = {}
for r in csv.DictReader(open(R + '/rebuild/manifest/functions.tsv', encoding='utf-8-sig'), delimiter='\t'):
    m = r['module']; n = r['name']
    man[r['address']] = n if m in ('_global', '_gapscan', '', '_unattributed') or n.startswith(m + '::') else m + '::' + n
pdb = set()
for f in ('ghidra_out/fablewin_pdb_names.tsv', 'ghidra_out/egor_pdb_names.tsv'):
    for l in open(R + '/' + f, errors='replace'):
        c = l.rstrip('\n').split('\t')
        if len(c) > 1: pdb.add(c[1])
PH = re.compile(r"sub_|CVtable_|CUnproven|vfunc_|^\?|_gapscan|^None$|Alloc_CVtable|^_unattributed::|_[0-9a-f]{8}$|^FUN_")
def curname(a): return cat.get(a) or man.get(a) or 'None'
raw = E.d
def refs(va): return raw.count(struct.pack('<I', va))
name_holders = collections.defaultdict(set)
for a, n in cat.items(): name_holders[n].add(a)

def tmpl_tags(tmpl, outer):
    pre = '?$%s@V%s@NScript@@@@' % (tmpl, outer)
    tags = set()
    for s in pdb:
        if pre in s:
            m = re.match(r'^\?(\w+)@\?\$', s)
            if m: tags.add(m.group(1))
            m = re.match(r'^\?\?(_G|_E|1|0)\?\$', s)
            if m: tags.add('??' + m.group(1))
    return tags

# outer -> binding vt (registrations), and enclosing function -> outer
outer_bvt = collections.defaultdict(set); outer_func = {}
site_by = {('%08x' % s['site']): s for s in J['sites']}
for r in review:
    s = site_by[r['site']]
    outer_bvt[r['outer']].add(s['binding_vt']); outer_func[r['outer']] = int(r['func'], 16)
# script vtables from labels_alloc evidence (outer script vtable XXXXXXXX)
script_vt = {k: int(v[0], 16) for k, v in json.load(open(W + '/script_vts.json')).items()}

rows = []; log = []
def propose(addr, newname, ev):
    cur = curname(addr)
    if cur == newname: log.append((addr, newname, 'SAME', '')); return
    if PH.search(cur): act, note = 'RENAME', 'was %s' % cur
    elif cur.replace('NScript::', '') == newname.replace('NScript::', ''): act, note = 'RENAME', 'was %s (namespace spelling)' % cur
    else:
        others = name_holders.get(cur, set()) - {addr}
        if others: act, note = 'RENAME_WRONG', 'replaces %s: current name also held by %s' % (cur, ','.join(sorted(others)[:3]))
        else: act, note = 'DISAGREE', 'current %s kept' % cur
    log.append((addr, newname, act, note))
    if act.startswith('RENAME'): rows.append((addr, newname, ev + '; ' + note))

for outer in sorted(o for o in outer_bvt if o):
    bvts = outer_bvt[outer]
    if len(bvts) != 1:
        log.append(('-', outer, 'SKIP', 'binding vtables %s' % sorted(bvts))); continue
    bvt = next(iter(bvts))
    T = 'NScript::' + outer
    # --- CEntityScriptBinding<T>
    btags = tmpl_tags('CEntityScriptBinding', outer)
    for slot, meth in ((0, 'GetEntityScriptName'), (1, 'CreateActiveEntityScript')):
        f = E.dword(bvt + 4 * slot); n = refs(f)
        body_ok = (meth != 'GetEntityScriptName') or E.read(f, 4) == bytes.fromhex('8d4104c3')
        if n == 1 and meth in btags and body_ok:
            propose('%08x' % f, 'CEntityScriptBinding<%s>::%s' % (T, meth),
                    'binding vtable %08x slot %d: stored at every CEntityScriptBinding registration in %s (%08x); PDB ?%s@?$CEntityScriptBinding@V%s@NScript@@@@; refs=1%s'
                    % (bvt, slot, outer, outer_func[outer], meth, outer, '; body lea eax,[ecx+4];ret' if slot == 0 else ''))
        else: log.append(('%08x' % f, 'CEntityScriptBinding<%s>::%s' % (T, meth), 'SKIP', 'refs=%d pdb=%s body=%s' % (n, meth in btags, body_ok)))
    # --- CActiveEntityScript<T>
    cae = E.dword(bvt + 4)
    avt, actor = RS.last_vt_store(cae, set())
    atags = tmpl_tags('CActiveEntityScript', outer)
    if avt:
        for slot, meth in ((0, 'sdd'), (4, 'SuspendableProcess')):
            f = E.dword(avt + 4 * slot); n = refs(f)
            has = ('??_G' in atags or '??_E' in atags or '??1' in atags) if meth == 'sdd' else meth in atags
            nm = 'CActiveEntityScript<%s>::%s' % (T, "`scalar deleting destructor'" if meth == 'sdd' else meth)
            if n == 1 and has:
                propose('%08x' % f, nm, 'CActiveEntityScript<%s> vtable %08x (stored after operator new by CEntityScriptBinding<%s>::CreateActiveEntityScript %08x, binding vtable %08x) slot %d; PDB %s; refs=1'
                        % (outer, avt, outer, cae, bvt, slot, 'has ??_G/??_E/??1' if meth == 'sdd' else '?%s@?$CActiveEntityScript@V%s@NScript@@@@' % (meth, outer)))
            else: log.append(('%08x' % f, nm, 'SKIP', 'refs=%d pdb=%s' % (n, has)))
    # --- CSpawnedFunc<T> via RegisterMain
    svt = script_vt.get(outer)
    if svt:
        rm = E.dword(svt + 4)
        fvt, _ = RS.last_vt_store(rm, set())
        stags = tmpl_tags('CSpawnedFunc', outer)
        if fvt:
            for slot, meth in ((0, 'sdd'), (4, 'SuspendableProcess')):
                f = E.dword(fvt + 4 * slot); n = refs(f)
                has = ('??_G' in stags or '??_E' in stags or '??1' in stags) if meth == 'sdd' else meth in stags
                nm = 'CSpawnedFunc<%s>::%s' % (T, "`scalar deleting destructor'" if meth == 'sdd' else meth)
                if n == 1 and has:
                    propose('%08x' % f, nm, 'CSpawnedFunc<%s> vtable %08x (stored after operator new(0x3c) by %s::RegisterMain %08x, script vtable %08x slot 1) slot %d; PDB %s; refs=1'
                            % (outer, fvt, outer, rm, svt, slot, 'has ??_G/??_E/??1' if meth == 'sdd' else '?%s@?$CSpawnedFunc@V%s@NScript@@@@' % (meth, outer)))
                else: log.append(('%08x' % f, nm, 'SKIP', 'refs=%d pdb=%s' % (n, has)))

seen = set(); out = []
for a, n, ev in sorted(rows):
    if a in seen: continue
    seen.add(a); out.append((a, n, ev))
with open(W + '/labels_templates.tsv', 'w') as f:
    f.write('address\tname\tevidence\n')
    for a, n, ev in out: f.write('%s\t%s\t%s\n' % (a, n, ev.replace('\t', ' ')))
with open(W + '/templates_log.tsv', 'w') as f:
    f.write('address\tname\taction\tnote\n')
    for x in log: f.write('\t'.join(x) + '\n')
print('template rows', len(out), collections.Counter(x[2] for x in log))

# ---- 3c: CSpawnedFunc<X> vtables of scripts without entity registrations: slot 0 already named CSpawnedFunc<NScript::X> in the
# catalog AND the functions that store the vtable (text refs) are catalog-named members of NScript::X (a RegisterMain among them).
done = {a for a, _, _ in out}
extra = []; xlog = []
RD_LO, RD_HI = [(x[1], x[1] + x[2]) for x in E.secs if x[0] == '.rdata'][0]
SDDRX = re.compile(r"^CSpawnedFunc<NScript::(\w+)>::`scalar deleting destructor'$")
o0 = E.off(RD_LO)
import bisect
for a, n in cat.items():
    m = SDDRX.match(n)
    if not m: continue
    X = m.group(1); f0 = int(a, 16)
    vts = [RD_LO + i - o0 for i in range(o0, o0 + (RD_HI - RD_LO) - 3, 4) if False]
    p = struct.pack('<I', f0); i = raw.find(p, o0, o0 + (RD_HI - RD_LO))
    if i < 0 or raw.find(p, i + 1, o0 + (RD_HI - RD_LO)) >= 0: xlog.append((a, X, 'SKIP', 'sdd not referenced exactly once in .rdata')); continue
    vt = RD_LO + i - o0
    f4 = E.dword(vt + 16); fa = '%08x' % f4
    if fa in done: continue
    # storers of the vtable
    q = struct.pack('<I', vt); t0 = E.off(E.tlo); j = t0; storers = set()
    while True:
        j = raw.find(q, j, t0 + len(E.text))
        if j < 0: break
        storers.add(RS.fstart(E.tlo + j - t0)); j += 1
    snames = [curname('%08x' % s_) for s_ in storers]
    ok_store = bool(snames) and all(sn.startswith('NScript::%s::' % X) for sn in snames) and any(sn.endswith('::RegisterMain') for sn in snames)
    has = 'SuspendableProcess' in tmpl_tags('CSpawnedFunc', X)
    n4 = refs(f4)
    nm = 'CSpawnedFunc<NScript::%s>::SuspendableProcess' % X
    if ok_store and has and n4 == 1:
        before = len(rows)
        propose(fa, nm, 'CSpawnedFunc<%s> vtable %08x (slot 0 %s already catalog-named CSpawnedFunc<NScript::%s>; vtable stored after operator new only by %s) slot 4; PDB ?SuspendableProcess@?$CSpawnedFunc@V%s@NScript@@@@; refs=1'
                % (X, vt, a, X, ', '.join(sorted(snames)), X))
        if len(rows) > before: extra.append(rows[-1])
    else: xlog.append((fa, nm, 'SKIP', 'storers=%s pdb=%s refs=%d' % (snames, has, n4)))
with open(W + '/labels_templates.tsv', 'a') as f:
    for a, n, ev in sorted(extra): f.write('%s\t%s\t%s\n' % (a, n, ev.replace('\t', ' ')))
with open(W + '/templates_log.tsv', 'a') as f:
    for x in xlog: f.write('\t'.join(x) + '\n')
print('extra CSpawnedFunc rows', len(extra), 'skips', len(xlog))
for x in xlog: print(x)
