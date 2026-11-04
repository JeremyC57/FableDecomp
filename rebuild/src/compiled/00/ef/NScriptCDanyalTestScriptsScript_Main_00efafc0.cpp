// NScript::CDanyalTestScriptsScript::Main() -- a script body: pump the game interface (at +0x40) until this script's active thread is being terminated
// Leaf: ?Main@CDanyalTestScriptsScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFAFC0, length 37 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (called directly on `this`: this class
// derives from CScriptBase, whose +0x2c thread pointer the callee reads), game-interface vtable slot 7
// (+0x1c, Pump) through the pointer at +0x40.
// The class is a script-module CScriptBase-derived script whose vtable (0x012F6AA8, no RTTI in that
// module: slot 0 the deleting destructor, slot 1 "CGlobal_WatchForHeroDeathScript::Main" @ 0x00EFAFF0,
// slot 2 this body) names it; the manifest's "CCOE_AltarEvil::Main" label is propagated from a
// byte-alike. Same shape as 0x00E6A7A0 with a non-virtual predicate: `while (!IsActiveThreadTerminating())`
// rotated into test + do/while with the `lea esp,[esp]` alignment pad.
// Naming wave 35: identity CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00efb0b0 at 00cda3be with name "DanyalTestScripts"; Alloc news + stores script vtable 012f6aa8; PDB ?Alloc@CDanyalTestScriptsScript@NScript@@SIPAVCScriptBase@@; script vtable slot 2 (slot 1 pushes "Main" 0x012C2F94); PDB Main; refs=1; was CVtable_012f6aa8::PumpUntilTerminating_00efafc0 (wave3/labels_alloc.tsv); PDB ?Main@CDanyalTestScriptsScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?PumpUntilTerminating_00efafc0@CVtable_012f6aa8@@QAEXXZ`; this source now defines the PDB member (class CVtable_012f6aa8 renamed CDanyalTestScriptsScript); same bytes.
struct CGameScriptInterface_Calls {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void Pump();                                    // slot 7 (+0x1c)
};

struct CScriptBase_Retail {
    virtual void v00();
    unsigned char _pad[0x3c];                               // +0x04 .. +0x40 (the active thread is at +0x2c)
    unsigned char IsActiveThreadTerminating();              // 0x00CB7940
};

namespace NScript {
class CDanyalTestScriptsScript : public CScriptBase_Retail {
public:
    CGameScriptInterface_Calls* m_pInterface;               // +0x40
public:
    virtual void Main();                          // PDB ?Main@CDanyalTestScriptsScript@NScript@@UAEXXZ
};
}
using NScript::CDanyalTestScriptsScript;

void NScript::CDanyalTestScriptsScript::Main()
{
    while (!IsActiveThreadTerminating())
        m_pInterface->Pump();
}