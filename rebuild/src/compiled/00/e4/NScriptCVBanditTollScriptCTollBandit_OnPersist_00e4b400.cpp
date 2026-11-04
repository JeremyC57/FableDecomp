// NScript::CV_BanditTollScript::CTollBandit::OnPersist(CPersistContext& context) -- saves/loads TollState by name (default 0L)
// Leaf: ?OnPersist@CTollBandit@CV_BanditTollScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E4B400, 35 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<long> 0x00410BE0 (??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z, already bound).
// Identity (naming wave 35, family op): reg 00e47af5 "TollBandit" in 00e479b0; Alloc 00e48b40; vtable 012e4d88 slot 4; PDB ?OnPersist@CTollBandit@CV_BanditTollScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_TollState_00e4b400.
//   ?OnPersist@CTollBandit@CV_BanditTollScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00e4b400::PersistTransfer_TollState_00e4b400(CPersistContext* context) -- persists the TollState dword at +0x1c with a zero default Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4B400, oracle length 35 b... (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00E48B40 (rebuild/src/compiled/00/e4/NScriptCVBanditTollScriptCTollBandit_Alloc_00e48b40.cpp); the PDB OnPersist declaration added; its long TollState. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BanditTollScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CTollBandit : public CEntityScriptBase {
    public:
        CV_BanditTollScript* ParentClass;               // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long TollState;                                 // +0x1C
        bool TollBanditOnlyCollectsToll;                // +0x20
        bool KilledReluctantTrader;                     // +0x21
        long TollBanditPayMeDistance;                   // +0x24
        CScriptThing BanditEscapeMarker;                // +0x28 CScriptThing (Ego_r)
        CTollBandit(CV_BanditTollScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CTollBandit();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BanditTollScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CTollBandit@CV_BanditTollScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_BanditTollScript::CTollBandit::Alloc(CV_BanditTollScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CTollBandit(parent, data, me));
}
}

void NScript::CV_BanditTollScript::CTollBandit::OnPersist(CPersistContext& context)
{
    context.Transfer("TollState", TollState, 0L);
}
