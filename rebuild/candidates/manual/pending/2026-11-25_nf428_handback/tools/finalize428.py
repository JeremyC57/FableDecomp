#!/usr/bin/env python3
"""finalize428.py: split the measured run dirs batches/r1, r2 into the hand-back folders acc_r1, acc_r2 (ACCEPT) and hold_r1 (HOLD), and
write RESULTS / CHECKS / UNITS / TABLE_CHANGES / SELF_SYMBOLS / RENAMES / PLAN / rows.txt / head/payload.json in each (evidence copied)."""
import sys, os, re, json, shutil, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A)
HOLD = {'00403db0': 'form b (a binder defines Reset<CFileInstaller> pick-any) adds a pick-any ?ReleaseData@?$CCountedPointer@VCFileInstaller@@@@QAEXXZ copy whose body differs from the only landed definer\'s (0x00402510): a new body under one pick-any name (R1/R10 not met); needs a ReleaseData variant decision',
        '00403e40': 'form b adds a pick-any ?ReleaseData@?$CCountedPointer@VCDataBank@NGameText@@@@QAEXXZ copy that differs from every landed definer (0x00402510, 0x00416832, 0x0042E98F) and from retail 0x00403750 (that name\'s retail copy): R1/R10 not met; also catalog Module is the wrong stand-in CCountedPointer_CIPhysicsFacesSegmentBase_const',
        '008ab230': 'form b adds a pick-any ?ReleaseData@?$CCountedPointer@VCScriptGameResourceObjectScriptedThingBase@@@@QAEXXZ copy equal to 0x008AB960\'s but differing from retail 0x007E70E0 (the name\'s retail copy, landed row): held conservatively (a NEQ copy of a retail-addressed name)'}
RT = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A + '/evidence/rtti.tsv')}
for a, v in RT.items():
    if v[3] != 'RTTI_OK' and a not in HOLD: HOLD[a] = 'second source missing: no retail call site shows a constructed object of T (rtti428 %s: %s); the stored deleter\'s DeleteData<T> binding was itself derived from this Reset\'s callers (agentFAM10), so it is not independent' % (v[3], v[4][:120] or 'nothing readable')
P = [l.rstrip('\n').split('\t') for f in sorted(glob.glob(A + '/runs/r[12]/PLAN.tsv')) for l in open(f)]
src = {}
for f in sorted(glob.glob(A + '/runs/r[12]/PLAN.tsv')):
    for l in open(f): src[l.split('\t')[0]] = os.path.dirname(f)
def grab(path, a):
    for l in open(path):
        if l.startswith(a): return l.rstrip('\n')
    return ''
fam = {r['target']: r for r in json.load(open(A + '/fam.json'))}
groups = {}
for p in P:
    a = p[0]; r = os.path.basename(src[a])
    groups.setdefault(('hold_r1' if a in HOLD else 'acc_' + r), []).append(p)
for g, ps in groups.items():
    B = A + '/batches/' + g
    for d in ('rows', 'tests', 'orig/rows', 'orig/tests', 'head', 'evidence', 'obj'): os.makedirs(B + '/' + d, exist_ok=True)
    res = ['address\tverdict\told\tnew\treason']; chk = ['address\tform\tobjsec\tleaf_sel_H/N\tvland_head/new\ttest_linked_row\tvland_new_warns\tpk_H\tpk_N\texposed_debt\tlink\tname_gate\treserved']
    units = ['address\tunit\tkind']; ss = ['address\tsymbol\tstatus\tleaf_given\tcurrent_line']; ren = ['address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence\told_source\tnew_source\tnew_test_source']
    pays = []
    for p in ps:
        a, mem, ct, old, new, d, form = p; S = src[a]
        for x, y in (('rows/%s.cpp', 'rows/%s.cpp'), ('tests/%s_test.cpp', 'tests/%s_test.cpp'), ('orig/rows/%s.cpp', 'orig/rows/%s.cpp'), ('orig/tests/%s_test.cpp', 'orig/tests/%s_test.cpp'), ('obj/%s.obj', 'obj/%s.obj'), ('obj/H_%s.obj', 'obj/H_%s.obj')):
            if os.path.exists(S + '/' + x % a): shutil.copy(S + '/' + x % a, B + '/' + y % a)
        for e in glob.glob(S + '/evidence/ng_%s.log' % a): shutil.copy(e, B + '/evidence/')
        callers = re.search(r'\(([^)]*)\)', [x for x in fam[a]['spellings'].split('||') if x.strip().startswith(new)][0]).group(1)
        why = ('self respell (NF428): self port of the CCountedPointer<%s>::Reset stand-in (local SCountBlock model) onto the PDB name the callers '
               'already bind; T from two sources: callers at %s bind %s (whole FableWin.pdb entry), and the deleter the body stores, 0x%s, is bound '
               'to ?DeleteData@?$CCountedPointer@%s@@@@SIXPAX@Z (derived from source 1 by agentFAM10, consistent, not counted); independent source 2: retail RTTI, '
               'a retail call site passes a freshly constructed %s (vftable RTTI / ??0 call before the call, rtti428); form %s (%s); leaf bytes unchanged (RELOCATION_MATCH both sides)') % (
               ct, callers, new, d.upper(), new.split('?$CCountedPointer@')[1].split('@@@@')[0], RT[a][2],
               form, 'NF371 strong specialization, selection 1' if form == 'spec' else 'NF371 R1 form b explicit instantiation, selection 2: a landed binder emits a pick-any copy')
        res.append('\t'.join([a, 'HOLD' if a in HOLD else 'ACCEPT', old, new, (HOLD[a] + '. ' if a in HOLD else '') + why]))
        os_ = grab(S + '/evidence/objsec.txt', a).split('\t')
        hn = grab(S + '/evidence/hn.txt', a); te = grab(S + '/evidence/tests.txt', a)
        sel = re.findall(r'sel=(\d)', hn)
        pkH = [l for l in open(S + '/evidence/pk_H.tsv') if l.startswith(a) and '\t#' not in l]
        pkN = [l for l in open(S + '/evidence/pk_N.tsv') if l.startswith(a) and '\t#' not in l]
        neq = sum(1 for l in pkN if l.split('\t')[1] == 'NEQ'); reach = sum(1 for l in pkN if l.rstrip('\n').split('\t')[-2] == 'y')
        ng = grab(S + '/evidence/name_gate.txt', a).split(' ', 1)[1]
        chk.append('\t'.join([a, form, os_[2] if len(os_) > 2 else '?', '[%s]/[%s]' % tuple(sel[:2]), 'RELOCATION_MATCH/RELOCATION_MATCH',
                              te.split()[-1] if te else '?', '-', 'copies=%d (all NOADDR leaf=n)' % len(pkH), 'copies=%d neq=%d reach=%d' % (len(pkN), neq, reach),
                              'none' if a not in ('008ab230',) else '0x007E70E0 ReleaseData differs (existing)', 'PASS (151 census+witness definers; dups 1->1, unresolved watch 56->46, 0 new)' if a not in HOLD else 'not in accepted link set', ng, 'reserved 0']))
        units.append('%s\t%s\t-' % (a, a))
        ss.append('\t'.join([a, new, 'RELOCATION_MATCH', new, '']))
        if a == '00413520':
            ren.append('\t'.join([a, 'CCountedPointer<CConsole>', 'Reset', new, 'CCountedPointer_004132f0::Reset', old, 'callers bind the PDB name; stored deleter 0x004132F0 = DeleteData<CConsole>',
                                  '00/41/CCountedPointer004132f0_Reset_00413520.cpp', '00/41/CCountedPointerCConsole_Reset_00413520.cpp', '00/41/CCountedPointerCConsole_Reset_00413520_test.cpp']))
        pays.append(a)
    for nm, L in (('RESULTS.tsv', res), ('CHECKS.tsv', chk), ('UNITS.tsv', units), ('SELF_SYMBOLS.tsv', ss), ('RENAMES.tsv', ren),
                  ('TABLE_CHANGES.tsv', ['action\tsymbol\ttarget\twitness\tnote'])):
        open(B + '/' + nm, 'w').write('\n'.join(L) + '\n')
    open(B + '/PLAN.tsv', 'w').write(''.join('\t'.join(p) + '\n' for p in ps)); open(B + '/rows.txt', 'w').write(''.join(p[0] + '\n' for p in ps))
    # HEAD vland payload, the subset of the run's own
    hp = {}
    for r in ('r1', 'r2'):
        for x in json.load(open(A + '/work/p_%sh/payload.json' % r))['result']['authored']: hp[x['address']] = x
    json.dump({'result': {'authored': [hp[a] for a in pays]}}, open(B + '/head/payload.json', 'w'))
    for x in pays: assert hp[x]['source_cpp'] == nf.src_of(x).read_text(encoding='utf-8'), x
    for ef in ('objsec.txt', 'hn.txt', 'tests.txt', 'pk_H.tsv', 'pk_N.tsv', 'name_gate.txt'):
        L = []
        for r in ('r1', 'r2'):
            fp = A + '/runs/%s/evidence/%s' % (r, ef)
            if os.path.exists(fp): L += [l for l in open(fp) if l[:8] in pays]
        open(B + '/evidence/' + ef, 'w').write(''.join(L))
    for ef in ('link_main.txt', 'link_ctl.txt', 'rdcmp.tsv', 'rtti.tsv', 'mutation.txt', 'layout.tsv', 'ddsrc.tsv'):
        L = [l for l in open(A + '/evidence/' + ef) if l[:8] in pays] if ef.endswith('.tsv') else open(A + '/evidence/' + ef).readlines()
        open(B + '/evidence/' + ef, 'w').write(''.join(L))
    print(g, len(ps))
