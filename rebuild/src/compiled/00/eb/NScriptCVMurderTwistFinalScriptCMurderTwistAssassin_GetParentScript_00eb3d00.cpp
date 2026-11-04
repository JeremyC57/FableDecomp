// NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB3D00, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00eaeb31 "MurderTwistAssassin" in 00eaea20; Alloc 00eb3c60; vtable 012f00bc slot 3; PDB ?GetParentScript@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00eb3d00.
//   ?GetParentScript@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00EB3C60 (rebuild/src/compiled/00/eb/NScriptCVMurderTwistFinalScriptCMurderTwistAssassin_Alloc_00eb3c60.cpp); the Alloc body (if any) is not part of this graded body.
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CScriptGameResourceObjectScriptedThingBase {      // 16 bytes (Ego_r): vptr, +4, PImp +8
public:
    virtual ~CScriptGameResourceObjectScriptedThingBase();
    unsigned char _base04[4];
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
};

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MurderTwistFinalScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CMurderTwistAssassin : public CEntityScriptBase {
    public:
        CV_MurderTwistFinalScript* ParentClass;         // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        CScriptGameResourceObjectScriptedThingBase MyEntityHandle;// +0x1C (Ego_r)
        unsigned long NextLine;                         // +0x2C
        bool HasBeenThreatened;                         // +0x30
        float ThreatDistSq;                             // +0x34
        bool HasGivenIntroToHero;                       // +0x38
        bool PreemptiveAttackInProgress;                // +0x39
        CMurderTwistAssassin(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CMurderTwistAssassin();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_MurderTwistFinalScript::CMurderTwistAssassin::Alloc(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CMurderTwistAssassin(parent, data, me));
}
}

CScriptBase* NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::GetParentScript() const
{
    return ParentClass;
}
