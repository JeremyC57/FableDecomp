# h23_emit.py: naming wave 23, family emit -- DECISIONS_W22_RESOLVED item 1 (CAmbientEmitter is the one class for 0x00C44EB0..0x00C44F80).
#  0x00C44EB0: the constructor own row re-authored on CAmbientEmitter with the bound caller spelling ??0CAmbientEmitter@@QAE@PAUCResourceWorld@@PAX@Z
#    (0x00C36180 `new CAmbientEmitter(this, owner)`); its world is the CResourceWorld of that caller, Init2 is the landed
#    ?AcquireSounds@CAmbientEmitter@@QAEXXZ 0x00C44CE0, Init1 is 0x00C44C50 under its kept stand-in member name InitializeThingFilters.
#  0x00C61C70: the 16-byte record 0x00C44C50 allocates and hands to the bound ?Reschedule@CAmbientEvent@@QAEXPAVCAmbientEmitter@@@Z 0x00C61CA0
#    is the CAmbientEvent (was the junk CThingFilter_SolidToThing label): ??0CAmbientEvent@@QAE@PAX_N@Z (retail loads the flag as a byte:
#    `mov dl,[esp+8]`; the definition pointer at +0 is what Reschedule reads through [this]).
#  0x00C44C50: the member that builds the events runs on the CAmbientEmitter (it passes `this` to CAmbientEvent::Reschedule, whose
#    parameter is CAmbientEmitter*), so its class is CAmbientEmitter; the member keeps its stand-in name (no evidence names it; DECISIONS_W23).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
a = '00c44eb0'
NE = '// Naming (naming wave 23, DECISIONS_W22_RESOLVED item 1): the class is the CAmbientEmitter (destructor 0x00C44F80; `new CAmbientEmitter(this, owner)` in CResourceWorld::AllocateFor 0x00C36180, whose bound spelling this definition takes); was the stand-in CResourceAllocation_00c44eb0 on CWorld_00c44eb0.'
p, rel, t = he._rw(a, 'src')
if NE in t: print('already', a)
else:
    t = t.replace('// CWorld_00c44eb0::CResourceAllocation(world, owner) --', '// CAmbientEmitter::CAmbientEmitter(CResourceWorld* world, void* owner) --', 1)
    i = t.index('\n') + 1
    t = t[:i] + '// Leaf: ??0CAmbientEmitter@@QAE@PAUCResourceWorld@@PAX@Z\n' + NE + '\n' + t[i:]
    t = t.replace('struct CWorld_00c44eb0 {', 'struct CResourceWorld {')
    t = t.replace('struct CResourceAllocation_00c44eb0 {', 'class CAmbientEmitter { public:')
    t = t.replace('    CWorld_00c44eb0* m_pWorld;             // +0', '    CResourceWorld* m_pWorld;              // +0')
    t = t.replace('    void Init1();                          // 0x00C44C50', '    void InitializeThingFilters();         // 0x00C44C50 (stand-in member name kept: DECISIONS_W22_RESOLVED item 1)')
    t = t.replace('    void Init2();                          // 0x00C44CE0', '    void AcquireSounds();                  // 0x00C44CE0 (landed ?AcquireSounds@CAmbientEmitter@@QAEXXZ)')
    t = t.replace('    CResourceAllocation_00c44eb0(CWorld_00c44eb0* world, COwner_00c44eb0* owner);', '    CAmbientEmitter(CResourceWorld* world, void* owner);    // ??0CAmbientEmitter@@QAE@PAUCResourceWorld@@PAX@Z')
    t = t.replace('CResourceAllocation_00c44eb0::CResourceAllocation_00c44eb0(CWorld_00c44eb0* world, COwner_00c44eb0* owner)\n    : m_pWorld(world), m_pOwner(owner), m_8(owner->m_pPart->Get1C()),',
                  'CAmbientEmitter::CAmbientEmitter(CResourceWorld* world, void* owner)\n    : m_pWorld(world), m_pOwner(static_cast<COwner_00c44eb0*>(owner)), m_8(static_cast<COwner_00c44eb0*>(owner)->m_pPart->Get1C()),')
    t = t.replace('    Init1();\n    Init2();\n', '    InitializeThingFilters();\n    AcquireSounds();\n')
    code = '\n'.join(l for l in t.split('\n') if not l.startswith('//'))
    assert 'CResourceAllocation_00c44eb0' not in code and 'CWorld_00c44eb0' not in code and 'Init1' not in code, rel
    he._w(p, t); print('ok', a)

a = '00c61c70'
SRC = '''// CAmbientEvent::CAmbientEvent(void* def, bool flag) -- constructor of the 16-byte ambient event record: the event definition (+0), a cleared due time (+4), the flag argument (+8), an index of -1 (+0xC)
// Leaf: ??0CAmbientEvent@@QAE@PAX_N@Z
// Naming (naming wave 23, DECISIONS_W22_RESOLVED item 1): the record 0x00C44C50 allocates (`operator new(0x10)`, (entry, false)) and hands to the bound ?Reschedule@CAmbientEvent@@QAEXPAVCAmbientEmitter@@@Z 0x00C61CA0 is the CAmbientEvent; was the junk CThingFilter_SolidToThing label (the PDB CThingFilter_SolidToThing is 8 bytes).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C61C70, length 32 bytes.
// Callees: none.
// Library: libpk ambient sound (no PDB/RTTI): descriptive member names (+4 is the due time CAmbientEmitter::ProcessEvents 0x00C44BD0 compares;
//   Reschedule reads the definition through [this]); the definition type is not named, hence void*. Retail loads the flag as a byte (`mov dl,[esp+8]`).
// Shape: plain init-list stores in member order; `this` returned in eax.
class CAmbientEvent { public:
    void*         m_pDef;                           // +0x00
    unsigned long m_time;                           // +0x04
    bool          m_flag;                           // +0x08
    long          m_index;                          // +0x0c
    CAmbientEvent(void* def, bool flag);
};

CAmbientEvent::CAmbientEvent(void* def, bool flag)
    : m_pDef(def)
    , m_time(0)
    , m_flag(flag)
    , m_index(-1)
{
}'''
he.put(a, 'src', SRC)
TST = '''// Behaviour test for 0x00C61C70 CAmbientEvent::CAmbientEvent
#include <cstdio>
class CAmbientEvent { public:
    void*         m_pDef;
    unsigned long m_time;
    bool          m_flag;
    long          m_index;
    CAmbientEvent(void* def, bool flag)
        : m_pDef(def), m_time(0), m_flag(flag), m_index(-1) {}
};
int main()
{
    int def;
    CAmbientEvent a(&def, true), b(0, false);
    bool ok = a.m_pDef == &def && a.m_time == 0 && a.m_flag && a.m_index == -1
           && b.m_pDef == 0 && !b.m_flag && b.m_index == -1;
    if (!ok) { std::printf("00c61c70_TEST FAIL\\n"); return 1; }
    std::printf("00c61c70_TEST PASS\\n");
    return 0;
}'''
p, rel, t = he._rw(a, 'tst')
if t.endswith('\n'): TST += '\n'
he.put(a, 'tst', TST)

a = '00c44c50'
NC = '// Naming (naming wave 23, DECISIONS_W22_RESOLVED item 1): the member runs on the CAmbientEmitter (it passes `this` to CAmbientEvent::Reschedule(CAmbientEmitter*) 0x00C61CA0) and allocates CAmbientEvent records (ctor 0x00C61C70); the member keeps its stand-in name (no evidence names it). Was CGame_00c44c50 / CFilter_00c44c50 / Bind.'
p, rel, t = he._rw(a, 'src')
if NC in t: print('already', a)
else:
    t = t.replace('// CGame_00c44c50::InitializeThingFilters() --', '// CAmbientEmitter::InitializeThingFilters() --', 1)
    i = t.index('\n') + 1
    t = t[:i] + '// Leaf: ?InitializeThingFilters@CAmbientEmitter@@QAEXXZ\n' + NC + '\n' + t[i:]
    t = t.replace('struct CGame_00c44c50;\nstruct CFilter_00c44c50 {                  // 0x10 bytes\n    unsigned char _body[0x10];\n    CFilter_00c44c50(void* entry, long mode);   // 0x00C61C70\n    void Bind(CGame_00c44c50* game);       // 0x00C61CA0\n};',
                  'class CAmbientEmitter;\nclass CAmbientEvent { public:                // 0x10 bytes (libpk ambient event record)\n    unsigned char _body[0x10];\n    CAmbientEvent(void* def, bool flag);    // 0x00C61C70\n    void Reschedule(CAmbientEmitter* emitter);  // 0x00C61CA0 (bound ?Reschedule@CAmbientEvent@@QAEXPAVCAmbientEmitter@@@Z)\n};')
    t = t.replace('struct CGame_00c44c50 {', 'class CAmbientEmitter { public:')
    t = t.replace('void CGame_00c44c50::InitializeThingFilters()', 'void CAmbientEmitter::InitializeThingFilters()')
    t = t.replace('        CFilter_00c44c50* filter = new CFilter_00c44c50(table->m_pEntries[i], 0);\n        filter->Bind(this);',
                  '        CAmbientEvent* event = new CAmbientEvent(table->m_pEntries[i], false);\n        event->Reschedule(this);')
    code = '\n'.join(l for l in t.split('\n') if not l.startswith('//'))
    assert 'CGame_00c44c50' not in code and 'CFilter_00c44c50' not in code, rel
    he._w(p, t); print('ok', a)
