// NScript::CV_BanditCampPathScript::CBCP_Bandit::Init() -- SetThingPersistent(Me, true) through the game interface
// Leaf: ?Init@CBCP_Bandit@CV_BanditCampPathScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E475B0, 20 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the entity vtable 0x012E4B44 that the landed CBCP_Bandit::Alloc 0x00E47500 stores; FableWin.pdb
//   ?Init@CBCP_Bandit@CV_BanditCampPathScript@NScript@@EAEXXZ (private virtual).
// Callees: CGameScriptInterface::SetThingPersistent(const CScriptThing&, bool) const -- slot 477 (+0x774) of the interface at +4
//   (ghidra_out/cgsi_slots_aligned.tsv).
// Shape (family K069, representative 0x00DD4480): `this` copied to eax, Game loaded through it, the constant pushed, then &Me.
class CScriptThing {                                    // 12 bytes (Ego_r): vptr, PImp
public:
    virtual ~CScriptThing();
    void* PImpData;                                     // +0x04
    void* PImpInfo;                                     // +0x08
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase {                               // 20 bytes (Ego_r)
public:
    virtual ~CEntityScriptBase();
    virtual void GetParentScript() const;               // slot 1
private:
    virtual void Init();                                // slot 2
public:
    CGameScriptInterface* Game;                         // +0x04 (CGameScriptInterfaceBase* in the PDB; the derived interface here)
    CScriptThing Me;                                    // +0x08
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BanditCampPathScript {
public:
    class CBCP_Bandit : public CEntityScriptBase {
    private:
        virtual void Init();
    public:
        CV_BanditCampPathScript* ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    };
};
void CV_BanditCampPathScript::CBCP_Bandit::Init()
{
    Game->SetThingPersistent(Me, true);
}
}