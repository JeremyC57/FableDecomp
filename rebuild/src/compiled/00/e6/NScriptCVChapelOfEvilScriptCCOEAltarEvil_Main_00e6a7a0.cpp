// NScript::CV_ChapelOfEvilScript::CCOE_AltarEvil::Main() -- a script action body: pump the game interface until the script thread is being terminated
// Leaf: ?Main@CCOE_AltarEvil@CV_ChapelOfEvilScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6A7A0, length 37 bytes.
// Callees: IsFinished @ 0x00F35B30 (`this->GetScriptBase()` through vtable slot 3, then
// CScriptBase::IsActiveThreadTerminating @ 0x00CB7940; named as in the landed CAT_BanditContactEnd::Main
// 0x00F05780, which shares the interface-at-+4 layout and the same callee), game-interface vtable
// slot 7 (+0x1c, Pump) through the pointer at +4.
// The class is a script-module action whose vtable (0x012E84A0, no RTTI in that module: slot 0 the
// deleting destructor, slot 1 this Main) names it; the manifest's "CCOE_AltarEvil::Main" label is
// propagated over the byte-identical bodies of the whole family. The loop is a plain `while
// (!((const CEntityScriptBase*)this)->IsActiveThreadTerminating())` -- rotated by the compiler into test + do/while with the 4-byte `lea esp,[esp]`
// alignment pad before the loop head.
// Naming wave 35: identity reg 00e6949f "COE_AltarEvil" in 00e693f0; Alloc 00e6a6e0; vtable 012e84a0 slot 1; PDB ?Main@CCOE_AltarEvil@CV_ChapelOfEvilScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was CVtable_012e84a0::Main (wave3/labels_alloc.tsv); PDB ?Main@CCOE_AltarEvil@CV_ChapelOfEvilScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CVtable_012e84a0@@QAEXXZ`; this source now defines the PDB member (class CVtable_012e84a0 renamed CCOE_AltarEvil); same bytes.
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

class CEntityScriptBase { public:   // the PDB class the accessor belongs to
    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
};
namespace NScript {
class CV_ChapelOfEvilScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CCOE_AltarEvil;
};
}
using NScript::CV_ChapelOfEvilScript;
namespace NScript {
class CV_ChapelOfEvilScript::CCOE_AltarEvil {
public:
    virtual void v00();
    CGameScriptInterface_Calls* m_pInterface;               // +0x04
private:
    virtual void Main();                          // PDB ?Main@CCOE_AltarEvil@CV_ChapelOfEvilScript@NScript@@EAEXXZ
};
}

void NScript::CV_ChapelOfEvilScript::CCOE_AltarEvil::Main()
{
    while (!((const CEntityScriptBase*)this)->IsActiveThreadTerminating())
        m_pInterface->Pump();
}