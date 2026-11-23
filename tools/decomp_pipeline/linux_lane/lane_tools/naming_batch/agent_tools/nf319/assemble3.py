# assemble3.py: STATUS.tsv / APPLIED.tsv / RENAMES.tsv / REJECTED.tsv / rows.txt for NF319 (alias rows, header route).
# done rows: evidence/libcp_c2.txt (+ libcp_c3.txt for the Leaf-pinned rows), work/recheck_c2n.log / recheck_c3n.log grades.
import re
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319')
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
FD = {int(l.split('\t')[0]): l.rstrip('\n').split('\t') for l in open(A / 'work/feas_decl.tsv')}
done = [int(x) for x in open(A / 'work/done_idx.txt').read().split()]
grade = {}
for f in ('recheck_fn.log',):
    for l in open(A / 'work' / f):
        p = l.split()
        if len(p) > 2 and re.fullmatch(r'[0-9a-f]{8}', p[0]) and p[2] in ('PASS', 'FAIL', 'LINK_FAIL'): grade[p[0]] = (p[1], p[2], f)
lc = {}
for f in ('libcp_fin.txt',):
    for l in open(A / 'evidence' / f): lc[l.split()[0]] = (l.rstrip('\n'), f)
slot = {}
for l in open(A / 'work/probe_final.log'):
    p = l.split()
    slot.setdefault(p[0], []).append(p[1] + ' ' + p[2])
st = {}
for i in done:
    a = E[i - 1][0]; g = grade[a]; L, lf = lc[a]
    assert 'LIBCP_OK' in L and g[1] == 'PASS', a
    dr = '.drectve = HEAD + /DEFAULTLIB:"libcp" (NF318 Q1)' if 'libcp"' in L else '.drectve equal'
    route = ('declared under %s (switches landed rows define; no header change)' % ' + '.join(FD[i][3].split(','))) if i in FD else \
            ('add-only opt-in FABLE_RETAIL_%s_RTTI_DECLS in engine/%s.h (hdr/; override of a base virtual: probe PASS, made-up-name control FAIL)' % (
                re.match(r'\?[^@]+@([^@]+)@@', E[i - 1][3]).group(1).upper(), re.match(r'\?[^@]+@([^@]+)@@', E[i - 1][3]).group(1)))
    st[i] = ('done', 'alias -> real class, %s; vland %s %s%s; objsec: one DEF rename %s -> PDB spelling, UND/DEF sets equal, no ??_7/??_R, every non-debug section equal, %s; graded symbol = the PDB/RTTI symbol; link HEAD v new: 0 duplicates (evidence/link_fin.txt); probe: %s; evidence/%s' % (
        route, g[0], g[1], ' (new test rows/004e0f34_test.cpp: the old one called the alias)' if a == '004e0f34' else ' (landed test unchanged)', E[i - 1][4], dr,
        'override of a base declaration, control FAIL' if i not in FD else 'n/a (X declares M itself)', lf))
def put(idxs, status, reason):
    for i in idxs:
        if i not in st: st[i] = (status, reason)
flat = [int(l.split('\t')[0]) for l in open(A / 'evidence/flat_census.tsv') if l[0].isdigit()]
put(flat, 'skipped-port', 'census (NF299 Q4 form, evidence/flat_census.tsv; 1-in-10 sample opened, NOTES): engine/<X>.h has only the flat model (`void* __vftable` member, no `virtual`, no based model anywhere in rebuild/include) while retail places the member in X\'s own vftable; a virtual declaration there adds a vptr and cannot sit at the RTTI slot: needs the class-model port')
put([142], 'skipped-port', 'NF317 Q4: the HEAD int return (QAEHXZ) grades MATCH, the PDB bool return grades DIFFER(22v25) on the same body; retail code wins, not respelled (PDB revision difference); draft work/drafts/00934100.cpp')
put([2, 20, 21, 134], 'skipped-port', 'the base class model does not declare the virtual (override probe FAIL: work/probe.log); retail puts it at a base slot (vtslot.py), so an add-only declaration in X would open a new slot: needs the base model port')
put([1], 'skipped-port', 'engine/CDataInputStream.h declares GetPosition with an in-class inline body under FABLE_RETAIL_CDATAINPUTSTREAM; an out-of-line definition conflicts (NF318: in-class inline body)')
put([5], 'skipped-port', 'engine/CCombatAbilityAttackBase.h models the class on ONE line (`class X:public CSubDefClassBase {public:...};`): any declaration needs an edit of that existing line, not an add-only opt-in')
put([163], 'skipped-port', 'engine/CStreamingFontDataBank.h has no base (VIRTUAL_DTOR opt-in: a base-less class with the dtor at slot 0); retail places GetLastModifiedTime at slot 18 of its own vftable 0x0129D5CC, so a declaration would sit at slot 1: needs the base (CBankFile chain) model port')
put([59], 'skipped-port', 'NF318 Q4: the body calls made-up free stand-ins taking the alias pointer (UnsheatheWeapons_CheckPredicate / _BaseFinishAction (CCreatureAction_UnsheatheWeapons_Methods*)); moving needs callee respells with their own second source: T3 unit')
body = {4: 'flat/stand-in member names (FallenInLoveSoundCriteria_PStringData ...)', 34: 'flat member field10', 63: 'flat member f10', 64: 'BaseInit undeclared / flat member m8', 68: 'flat members m_a / m_b',
        150: 'alias accessor Brain() + local CTCAIScratchpad / event models', 151: 'alias accessor Brain() + local CTCAIScratchpad / event models', 152: 'local CThing / CAIBrain / CTCShop models',
        157: 'local CCharString model', 159: 'local CMeshData / CIVCountedPointer / C3DMesh2 models + C3DMeshLODInfo_Methods', 166: 'calls RemoveRegion(), which the real class model does not declare (clang: undeclared identifier; a callee identity, NF318 Q4)',
        167: 'flat members m_disabled / _pad_0x10', 173: 'local CCharString model + Data member', 176: 'flat member names LastCameraPosition_X ...', 177: 'local CMatrix3x4 / CMatrix4x4 models',
        179: 'local CCharString model', 181: 'calls RenderSunFlare / RenderSky, which the real class model does not declare (clang: undeclared identifier)', 184: 'local CEngineComponentRenderParams / CDisplayManager models', 185: 'local vector models with stand-in resize callees',
        187: 'alias accessor Children() + local C3DBoundingBox / dispatch models', 188: 'local simulation / C3DMesh2 models', 189: 'local CIndexBuffer / water sub-patch models'}
for i in list(range(119, 124)) + list(range(125, 134)):
    body[i] = 'local CBaseIntelligentPointer / CIntelligentPointer models or the CIntelligentPointer_Retail stand-in (CScriptThing in-TU members)'
for i, r in body.items(): put([i], 'skipped-port', 'body port (NF318: body ports / callee identities): on the real class model the body does not compile: ' + r + '; the move is more than an add-only declaration')
held = {3: 'CDisplayEngine is polymorphic (base CBaseClass, virtuals) only under FABLE_RETAIL_CDISPLAYENGINE_INITENGINE_PASS27', 32: 'class branch only under FABLE_RETAIL_CREADABLEDEF_USEINFO_PASS24',
        81: 'class branch only under FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27', 82: 'class switch FABLE_RETAIL_CAISTATEGROUP_MAZESHIELDEDCHASE: no landed row defines it', 103: 'class branch only under FABLE_RETAIL_ROCKTROLL_UPDATE_PASS27',
        111: 'declared only under FABLE_RETAIL_JACK_SETUPARENA_PASS23', 113: 'class branch only under FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_ATTACK_PASS23', 149: 'class branch only under FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_ATTACK_PASS23',
        124: 'declared only under FABLE_RETAIL_SCRIPTEDTHING_FOLLOW_ROUTE: no landed row defines it (the row is also a body port like its 15 CScriptGameResourceObjectScriptedThing siblings)', 139: 'class branch only under FABLE_RETAIL_WIFE_GIFT_UPDATE_PASS24',
        147: 'class branch only under FABLE_RETAIL_MINION_TRESPASS_UPDATE_PASS24', 190: 'class switch FABLE_RETAIL_LANDSCAPE_BACKGROUND_SUB_NODE_SPLIT_BRANCH: no landed row defines it'}
for i, r in held.items(): put([i], 'skipped-held', 'NF318 Q2: ' + r)
missing = [i for i in range(1, len(E) + 1) if i not in st]
assert not missing, missing
with open(A / 'STATUS.tsv', 'w') as o:
    print('address\tstatus\tpdb_spelling\treason', file=o)
    for i in range(1, len(E) + 1): print(E[i - 1][0], st[i][0], E[i - 1][3], st[i][1], sep='\t', file=o)
with open(A / 'REJECTED.tsv', 'w') as o: print('address\tpdb_spelling\treason', file=o)
with open(A / 'APPLIED.tsv', 'w') as o:
    print('address\trow_source\tchange', file=o)
    for i in done: print(E[i - 1][0], E[i - 1][7], 'alias class -> real class member (%s)' % E[i - 1][3], sep='\t', file=o)
with open(A / 'RENAMES.tsv', 'w') as o:
    print('## section 1: self renames (address, old leaf, new leaf = pdb_spelling)', file=o)
    for i in done: print(E[i - 1][0], E[i - 1][4], E[i - 1][3], sep='\t', file=o)
    print('## section 2: caller respellings: none (no landed row or table line binds an old spelling; exe census: work/census.tsv)', file=o)
(A / 'rows.txt').write_text('\n'.join(E[i - 1][0] for i in done) + '\n')
from collections import Counter
print(Counter(v[0] for v in st.values()))
