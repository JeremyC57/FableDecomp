// NScript::CV_BordelloScript::CBordelloLady::OnPersist(CPersistContext& context) -- saves/loads PartiedAlready and HaveTalked by name (default false)
// Leaf: ?OnPersist@CBordelloLady@CV_BordelloScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E3BA80, 67 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00e39bf5 "BordelloLady" in 00e39b40; Alloc 00e3ab90; vtable 012e3ac8 slot 4; PDB ?OnPersist@CBordelloLady@CV_BordelloScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer2_00e3ba80.
//   ?OnPersist@CBordelloLady@CV_BordelloScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00e3ba80::PersistTransfer2_00e3ba80(CPersistContext* context) -- persists the "PartiedAlready" and "HaveTalked" byte flags at +0x1d/+0x1e Retail: Fable.exe (TLC, VC7.1) function @ 0x00E3BA80, oracle lengt... (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00E3AB90 (rebuild/src/compiled/00/e3/NScriptCVBordelloScriptCBordelloLady_Alloc_00e3ab90.cpp); the PDB OnPersist declaration added; its bool PartiedAlready, bool HaveTalked. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCharString {                                     // 4 bytes
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();
};

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BordelloScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CBordelloLady : public CEntityScriptBase {
    public:
        CV_BordelloScript* ParentClass;                 // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool WalkingDownstairs;                         // +0x1C
        bool PartiedAlready;                            // +0x1D
        bool HaveTalked;                                // +0x1E
        long GoldRequired;                              // +0x20
        bool NoLongerWorking;                           // +0x24
        CCharString Name;                               // +0x28
        CBordelloLady(CV_BordelloScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CBordelloLady();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BordelloScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CBordelloLady@CV_BordelloScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_BordelloScript::CBordelloLady::Alloc(CV_BordelloScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CBordelloLady(parent, data, me));
}
}

void NScript::CV_BordelloScript::CBordelloLady::OnPersist(CPersistContext& context)
{
    context.Transfer("PartiedAlready", PartiedAlready, false);
    context.Transfer("HaveTalked", HaveTalked, false);
}
