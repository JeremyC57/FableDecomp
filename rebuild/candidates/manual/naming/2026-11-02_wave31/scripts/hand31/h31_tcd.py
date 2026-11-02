# h31_tcd.py: naming wave 31, family tcd (scope 2, queue row 25: the ~CTCBase 0x00686830 binders): each row is the compiler-generated
#  destructor of a CTCBase-derived component (identity: the slot-0 ??_G of the class's RTTI vtable calls it; FableWin.pdb ??1<class>@@UAE@XZ),
#  releasing its one def pointer then tail-jumping ~CTCBase. Re-authored on engine/CTCBase.h's retail branch (family tcb) and
#  engine/CDefPointer.h: the invented non-virtual bases (BlendStateBase, CBase3DAnimState, BlendBase, Base, C3DAnimBlendBase,
#  CBaseAnimState, CBase_00686830, ...) and the invented own classes (C3DAnimationBlendState, CSharedOwner_<addr>) go.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
# addr: (class, ??_G, vtable, member, def class, offset, size TU, egor, extra callers, retail length)
T = [
 ('004da967', 'CTCActionUseBase', '004dbf76', '0123f09c', 'PActionUseDef', 'CActionUseDef', 0x0c, True, False,
  'and the implicit destructors / ??_G of its derived CTCActionUse* / CTCCarriedActionUse* classes (RTTI : CTCActionUseBase)', 32),
 ('004db004', 'CTCSmashableBase', '007eef60', '0127185c', 'Definition', 'CSmashableDef', 0x14, True, False,
  'and the ??_G of CTCSmashableWindow 0x004DC6E2 / CTCSmashableBarrier 0x004DC71B (RTTI : CTCSmashableBase)', 32),
 ('004dc7c8', 'CTCExpression', '004dc7ac', '012401f4', 'PDef', 'CExpressionDef', 0x0c, True, False, '', 32),
 ('004dcbce', 'CTCDecapitation', '004dcbb2', '01240534', 'PDef', 'CDecapitationDef', 0x10, True, True, '', 32),
 ('004dccd2', 'CTCResurrectionItem', '004dccb6', '01240744', 'PDef', 'CResurrectionItemDef', 0x0c, True, True, '', 32),
 ('004dcdfd', 'CTCDRumble', '004dcde1', '012408bc', 'PDef', 'CRumbleDef', 0x0c, True, False, '', 32),
 ('004dcf4e', 'CTCHasName', '004dcf32', '01240aa4', 'PDef', 'CHasNameDef', 0x0c, True, False, '', 32),
 ('005dae40', 'CTCWeapon', '005dae20', '012554fc', 'WeaponDef', 'CWeaponDef', 0x0c, False, False, '', 35),
 ('00732cb0', 'CTCClock', '00732c90', '01267044', 'PDef', 'CClockDef', 0x0c, False, False, '', 35),
 ('00746d00', 'CTCBed', '00746ce0', '01268964', 'PDef', 'CBedDef', 0x14, False, False, '', 35),
 ('0078afc0', 'CTCHeroRangedAttacks', '0078afa0', '0126baf4', 'PHeroCombatDef', 'CHeroCombatDef', 0x10, False, False, '', 35),
 ('007dfca0', 'CTCFish', '007dfc80', '0126fd2c', 'PDef', 'CFishDef', 0x0c, False, False, '', 35),
 ('007e48e0', 'CTCWillResponse', '007e48c0', '01270534', 'PDef', 'CWillResponseDef', 0x0c, False, False, '', 35),
]
PRE = {  # Ego_r.pdb members before the def pointer (all trivially destructible)
 'CTCSmashableBase': 'LastWorldFrameHit +0x0c, Smashed +0x10', 'CTCDecapitation': 'Decapitated +0x0c',
 'CTCBed': 'Empty / GettingIn / GettingOut / Animating +0x0c..0x0f, BedSleepAnim +0x10', 'CTCHeroRangedAttacks': 'PParentCreature +0x0c',
}
OLD = {}
import csv
for r in csv.reader(open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t', quoting=csv.QUOTE_NONE):
    if len(r) >= 3 and r[1] == '00686830': OLD[r[2]] = r[0]
CAT = {}
for l in open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv'):
    s = l.split('\t')
    if len(s) > 2: CAT[s[0].lower()] = s[1]
for a, cls, g, vt, mem, dfc, off, os_, egor, extra, ln in T:
    p, rel, t = he._rw(a, 'src')
    old_base = [x for x in ('BlendStateBase', 'CBase3DAnimState', 'BlendBase', 'C3DAnimBlendBase', 'CBaseAnimState', 'CBase_00686830', 'Base')
                if ('struct %s ' % x) in t or ('struct %s{' % x) in t or ('class %s ' % x) in t]
    oldb = OLD.get(a, ('??1%s@@QAE@XZ' % old_base[0]) if old_base else '?')
    base = 'CTCDriverBase' if cls == 'CTCDRumble' else 'CTCBase'
    pad = off - 0x0c
    body = ['// %s::~%s() -- the compiler-generated destructor: release the %s def pointer (+0x%X), then ~CTCBase (tail jump)' % (cls, cls, mem, off),
            '// Leaf: ??1%s@@UAE@XZ' % cls,
            '// Naming wave 31 (family tcd): FableWin.pdb%s ??1%s@@UAE@XZ; the slot-0 ??_G 0x%s of vtable 0x%s (RTTI .?AV%s@@ : %s) calls it%s.' %
            (' / Ego_r.pdb' if egor else '', cls, g.upper(), vt.upper(), cls, base, (', ' + extra) if extra else ''),
            '//   Was "%s" over the invented non-virtual base %s (%s bound to 0x00686830): the base is engine/CTCBase.h\'s retail' % (CAT.get(a, '?'), old_base[0] if old_base else '?', oldb),
            '//   branch (family tcb), so the tail is ??1CTCBase@@UAE@XZ 0x00686830 (Ego_r.pdb / FableWin.pdb).',
            '// Retail: Fable.exe (TLC, VC7.1) function @ 0x%s, %d bytes, thiscall, tail `jmp ~CTCBase`%s.' % (a.upper(), ln, ' (size-optimised TU: `and [esi+X],0`)' if os_ else ''),
            '// Layout: Ego_r.pdb %s: %s CDefPointer<const %s> +0x%X%s; the other members are trivially destructible.' %
            (cls, mem, dfc, off, ('; before it ' + PRE[cls]) if cls in PRE else ''),
            '// Shape: no vptr store -- the class declares no destructor (the implicit ??1, as the landed ~CTCLightningOrb 0x004DCEDD); the def pointer',
            '//   is engine/CDefPointer.h\'s (forced-inline release). DestroyOne (a destruction by name, ungraded) makes VC7.1 emit the out-of-line',
            '//   implicit ??1 (chunk 656/657; wave 30 0x00A9D3A0).']
    if os_: body.append('#pragma optimize("s", on)')
    body += ['#define FABLE_RETAIL_DEF_CLASS_BASE',
             '#include "engine/CDefPointer.h"                          // CDefPointer<T> (forced-inline Release)',
             '#define FABLE_RETAIL_CTCBASE',
             '#include "engine/CTCBase.h"                              // : CBaseClassNonCopyable, vtable 0x0125BE0C; ~CTCBase 0x00686830',
             '',
             'class %s;' % dfc]
    if base == 'CTCDriverBase':
        body += ['class CTCDriverBase : public CTCBase {                   // RTTI .?AVCTCDriverBase@@ : CTCBase (no members, implicit destructor)',
                 '};']
    body += ['class %s : public %s {%s// vtable 0x%s' % (cls, base, ' ' * max(1, 40 - len(cls) - len(base)), vt.upper()),
             'public:']
    if pad: body.append('    unsigned char _pad_0x0c[0x%x];%s// +0x0c %s' % (pad, ' ' * (36 - len('%x' % pad)), PRE.get(cls, '')))
    body += ['    CDefPointer<const %s> %s;%s// +0x%02x' % (dfc, mem, ' ' * max(1, 34 - len(dfc) - len(mem)), off),
             '};',
             '',
             '// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.',
             'void DestroyOne(%s* object)' % cls,
             '{',
             '    object->%s::~%s();' % (cls, cls),
             '}']
    he.put(a, 'src', '\n'.join(body))
