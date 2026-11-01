# h26_pred.py: naming wave 26, family pred -- the NScript predicate re-model of the SetPredicate 0x00F35B10 /
#  IsActiveThreadTerminating 0x00F35B30 binders still on row-local spellings (wave 25 DEFERRED, family esb leftovers):
#  ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z / ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ
#  (Ego_r.pdb / FableWin.pdb: global CEntityScriptBase, const accessor); the filters derive from NScript::CPredicateBase
#  (engine/CPredicateBase.h, inline ctor / dtor as retail).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
NOTE = ('// Naming wave 26 (family pred): CEntityScriptBase is the global PDB class (Ego_r.pdb / FableWin.pdb) with\n'
        '//   ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z 0x00F35B10 and the const\n'
        '//   ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ 0x00F35B30; the predicate parameter is an NScript::CPredicateBase.\n'
        '//   Was: ')
def note(a, leaf=None, was=''):
    p, rel, t = he._rw(a, 'src')
    if 'family pred' in t: return
    L = t.split('\n'); i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    if not i:   # no Leaf line: add the symbol the compiler emits for the row's own function (graded on it, not on the largest)
        L.insert(1, '// Leaf: ' + leaf); i = [1]
    i = i[0]
    L.insert(i + 1, NOTE + was + '.'); he._w(p, '\n'.join(L))

# 0x00D9F270 NScript::CQ_MinionCampScript::CMC_Minion::Main (CIsThingConscious; the filter's destructor stays inline here)
he.edit('00d9f270', 'src', '''namespace NScript {
#pragma inline_depth(0)
class CIsThingConscious { public:                              // vtable 0x012C2FE8: a filter holding a script thing
    virtual void f00();
    CScriptThing m_thing;                               // +0x04
    CIsThingConscious(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingConscious() { }
};
#pragma inline_depth()
struct CEntityScriptBase {                              // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    void SetPredicate(const CIsThingConscious& predicate);   // 0x00F35B10
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
''', '''#include "engine/CPredicateBase.h"
namespace NScript {
#pragma inline_depth(0)
class CIsThingConscious : public CPredicateBase { public:      // vtable 0x012C2FE8 (engine/CIsThingConscious.h; the destructor inline here)
    virtual bool operator()() const;                    // slot 0  0x00CDDAB0
    virtual CPredicateBase* Clone() const;              // slot 1  0x00CDDAE0
    CScriptThing Thing;                                 // +0x04
    CIsThingConscious(const CScriptThing& thing);
    virtual ~CIsThingConscious() { }                    // slot 2; out of line 0x00EF7AB0
};
#pragma inline_depth()
// the constructor outside the inline_depth(0) region: retail expands the CPredicateBase constructor inside it
inline CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
}
class CEntityScriptBase { public:                       // global PDB class, retail 0x14 bytes
    virtual ~CEntityScriptBase();                       // slot 0  0x00F35B40
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00F35B10
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
namespace NScript {
''')
note('00d9f270', was='NScript::CEntityScriptBase (non-PDB scope) with SetPredicate on the local CIsThingConscious')
# the row's Leaf line names the PDB nesting (?Main@CMC_Minion@CQ_MinionCampScript@NScript, Ego_r.pdb + FableWin.pdb) but the row defined
#  ?Main@CMC_Minion@NScript@@ (the verifier graded the largest function): nest CMC_Minion in CQ_MinionCampScript (name-only)
he.edit('00d9f270', 'src', """class CQ_MinionCampScript;
struct CMC_Minion : CEntityScriptBase {
    CQ_MinionCampScript* ParentClass;                   // +0x14
    void*                PMasterData;                   // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};
""", """class CQ_MinionCampScript { public:
struct CMC_Minion : CEntityScriptBase {                 // Ego_r.pdb: nested in CQ_MinionCampScript
    CQ_MinionCampScript* ParentClass;                   // +0x14
    void*                PMasterData;                   // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};
};
""")
he.edit('00d9f270', 'src', 'void CMC_Minion::Main()\n', 'void CQ_MinionCampScript::CMC_Minion::Main()\n')

# 0x00EE76F0 NScript::CTrophyDealerInCave::Main (CIsThingAlive; destructor out of line 0x00E241B0 -> engine/CIsThingAlive.h)
he.edit('00ee76f0', 'src', '''namespace NScript {
class CIsThingAlive { public:                                  // vtable 0x012C32F0: a filter holding a script thing
    virtual void f00();
    CScriptThing Thing;
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}
    virtual ~CIsThingAlive();                                   // 0x00E241B0
};

struct CEntityScriptBase {
    virtual void e00();
    CGameScriptInterface* GSI;                          // +0x04
    CScriptThing          Me;                           // +0x08
    void SetPredicate(const CIsThingAlive& predicate);  // 0x00F35B10
    bool IsActiveThreadTerminating();                   // 0x00F35B30
};
''', '''#include "engine/CIsThingAlive.h"                     // NScript::CIsThingAlive : CPredicateBase, vtable 0x012C32F0, ~ 0x00E241B0
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}   // FableWin-only: retail expands it inline

class CEntityScriptBase { public:                       // global PDB class, retail 0x14 bytes
    virtual ~CEntityScriptBase();                       // slot 0  0x00F35B40
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00F35B10
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};

namespace NScript {
''')
he.resub('00ee76f0', 'src', r'\bGSI\b(?!\w)', 'Game')
note('00ee76f0', was='NScript::CEntityScriptBase with SetPredicate on the local CIsThingAlive and a non-const\n//   IsActiveThreadTerminating')

# the four CVtable0xxxxxxx entity rows: SetPredicate moves from the entity onto the empty global CEntityScriptBase
for a, cls in (('00d408b0', 'CVtable012CDF28'), ('00e11bb0', 'CVtable012E04FC'), ('00e4a060', 'CVtable012E4EF8'), ('00ec46b0', 'CVtable012F16E0')):
    he.edit(a, 'src', '''    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
};
struct %s : public CEntityScriptBase {void* VTable;GameCalls* Game;CScriptThing Me;NativeParent* Parent;void* Master;
 void SetPredicate(const NScript::CPredicateBase&); //00F35B10
''' % cls, '''    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10 (Ego_r.pdb CEntityScriptBase)
};
struct %s : public CEntityScriptBase {void* VTable;GameCalls* Game;CScriptThing Me;NativeParent* Parent;void* Master;
''' % cls)
    note(a, was='SetPredicate declared on the entity class (?SetPredicate@%s@@...)' % cls)

# CGoalChest 0x00EF29E0 / CDoor 0x00EF3000: derive from the empty CEntityScriptBase (offset 0) instead of casting `this` to it
he.edit('00ef29e0', 'src', '''    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ)
};
struct CGoalChest {
 void* VTable; GameCalls* Game; ThingCalls Me; CPersonalScript_EmptyScript* ParentClass; void* PMasterData;
 void SetPredicate(const NScript::CPredicateBase&); 
''', '''    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ)
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10 (Ego_r.pdb CEntityScriptBase)
};
struct CGoalChest : CEntityScriptBase {                 // the PDB base (empty here: offset 0)
 void* VTable; GameCalls* Game; ThingCalls Me; CPersonalScript_EmptyScript* ParentClass; void* PMasterData;
''')
he.edit('00ef3000', 'src', '''    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ)
};
struct CDoor {void* VTable;GameCalls* Game;CScriptThing Me;
 void SetPredicate(const NScript::CPredicateBase&);void Main();
''', '''    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ)
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10 (Ego_r.pdb CEntityScriptBase)
};
struct CDoor : CEntityScriptBase {void* VTable;GameCalls* Game;CScriptThing Me;   // the PDB base (empty here: offset 0)
 void Main();
''')
for a, lf, cls in (('00ef29e0', '?Main@CGoalChest@@QAEXXZ', 'CGoalChest'), ('00ef3000', None, 'CDoor')):
    he.edit_all(a, 'src', '((CEntityScriptBase*)this)->IsActiveThreadTerminating()', 'IsActiveThreadTerminating()')
    note(a, lf, was='SetPredicate declared on %s (?SetPredicate@%s@@...), which cast `this` to CEntityScriptBase for the accessor' % (cls, cls))

# 0x00E0E2E0 CUndeadRisingZombieAttackerBoss::Main: the global CPredicateBase { IsTrue } stand-in becomes NScript::CPredicateBase
#  (engine/CPredicateBase.h) and the filter NScript::CIsThingConscious (inline destructor here, as retail)
he.edit('00e0e2e0', 'src', '''struct CPredicateBase {
    virtual bool IsTrue() = 0;
};
struct CIsThingConscious : CPredicateBase {             // vtable 0x012C2FE8
    CScriptThing Thing;                                 // +0x04
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
    ~CIsThingConscious() { }
    virtual bool IsTrue();
};
''', '''#include "engine/CPredicateBase.h"
namespace NScript {
class CIsThingConscious : public CPredicateBase { public:      // vtable 0x012C2FE8 (engine/CIsThingConscious.h; the destructor inline here)
    virtual bool operator()() const;                    // slot 0  0x00CDDAB0
    virtual CPredicateBase* Clone() const;              // slot 1  0x00CDDAE0
    CScriptThing Thing;                                 // +0x04
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
    virtual ~CIsThingConscious() { }                    // slot 2; out of line 0x00EF7AB0
};
}
using NScript::CIsThingConscious;
''')
he.edit('00e0e2e0', 'src', '''    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
};''', '''    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00F35B10 (Ego_r.pdb CEntityScriptBase)
};''')
he.edit('00e0e2e0', 'src', '''    void SetPredicate(const CPredicateBase& predicate); // 0x00F35B10
''', '')
note('00e0e2e0', '?Main@CUndeadRisingZombieAttackerBoss@@QAEXXZ', was='SetPredicate on the entity over a global CPredicateBase { IsTrue } stand-in\n//   (?SetPredicate@CUndeadRisingZombieAttackerBoss@@QAEXABUCPredicateBase@@@Z)')

# 0x00E8B020 CRingFighter::Main: the CScriptPredicate stand-in (vtable 0x012C2FF4) is NScript::CPredicateBase, slot 0 operator()
p, rel, t = he._rw('00e8b020', 'src')
if 'struct CScriptPredicate {' in t:
    t = t.replace('''struct CScriptPredicate {                               // vtable 0x012C2FF4
    virtual bool Evaluate() const = 0;
    virtual CScriptPredicate* Clone() const = 0;
    virtual ~CScriptPredicate() {}
};
''', '#include "engine/CPredicateBase.h"                    // NScript::CPredicateBase, vtable 0x012C2FF4\n')
    t = t.replace('class CIsThingAlive : public CScriptPredicate {', 'class CIsThingAlive : public CPredicateBase {')
    t = t.replace('CScriptPredicate', 'NScript::CPredicateBase').replace('virtual bool Evaluate() const;', 'virtual bool operator()() const;')
    t = t.replace('((CEntityScriptBase*)this)->IsActiveThreadTerminating()', 'IsActiveThreadTerminating()')
    he._w(p, t); print('ok 00e8b020 src')
note('00e8b020', was='SetPredicate(const CScriptPredicate&) over a global CScriptPredicate { Evaluate } stand-in of the predicate base')

# 0x00D3A1A0 CDF_Fireheart::Main: the "entity thing setup" 0x00F35B10 is CEntityScriptBase::SetPredicate and the 16-byte
#  CScriptThingHolder (vtable 0x012C32F0) is NScript::CIsThingAlive (Ego_r.pdb 0x10: Thing +0x04), destroyed inline here
he.edit('00d3a1a0', 'src', '''struct CScriptThingHolder {                            // 16 bytes: its own vptr (0x012C32F0) then the thing
    CScriptThing Thing;
    CScriptThingHolder(const CScriptThing& thing) : Thing(thing) { }
    virtual ~CScriptThingHolder() { }
};
''', '''#define FABLE_RETAIL_NSCRIPT
#include "engine/CPredicateBase.h"
namespace NScript {
class CIsThingAlive : public CPredicateBase { public:   // vtable 0x012C32F0 (engine/CIsThingAlive.h; the destructor inline here)
    virtual bool operator()() const;                    // slot 0  0x00CDF120
    virtual CPredicateBase* Clone() const;              // slot 1  0x00CDF130
    CScriptThing Thing;                                 // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    virtual ~CIsThingAlive() { }                        // slot 2; out of line 0x00E241B0
};
}
''')
he.edit('00d3a1a0', 'src', '''    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
};''', '''    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00F35B10 (Ego_r.pdb CEntityScriptBase)
};''')
he.edit('00d3a1a0', 'src', '''    void SetupThing(const CScriptThingHolder& thing);  // 0x00F35B10
''', '')
he.edit('00d3a1a0', 'src', 'SetupThing(CScriptThingHolder(Me));', 'SetPredicate(NScript::CIsThingAlive(Me));')
he.edit('00d3a1a0', 'src', '// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840, entity thing setup 0x00F35B10, IsInterrupted',
        '// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840, CEntityScriptBase::SetPredicate 0x00F35B10, IsActiveThreadTerminating')
note('00d3a1a0', '?Main@CDF_Fireheart@@QAEXXZ', was='CDF_Fireheart::SetupThing(const CScriptThingHolder&) over a 16-byte holder stand-in of the CIsThingAlive filter')
