// NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::Alloc(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script factory of the "MurderTwistAssassin" binding: new CMurderTwistAssassin(parent, data, me) (CEntityScriptBase(*parent.Game, me), ParentClass = &parent, PMasterData = data) handed to a fresh CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EB3C60, 153 bytes (+ int3 padding), static `__fastcall` (hidden result
//   pointer ecx, parent edx, data and me on the stack), `ret 8`, returns the result pointer.
// Identity: Ego_r.pdb ?Alloc@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z is exactly the symbol this source emits.
//   CV_MurderTwistFinalScript::Main 0x00EAEA20 (slot 2 of the script vtable 0x012EF978, which the "V_MurderTwistFinal" script's Alloc stores at its CScriptInfoManager::RegisterScripts registration) stores this address as the factory of the CEntityScriptBinding<CV_MurderTwistFinalScript> record named "MurderTwistAssassin" (site 0x00EAEB34).
//   Was landed as _global::Create_CVtable_012f00bc (named by the entity vtable address); same shape as the family-pass F014 Alloc rows.
// Callees: scalar operator new 0x00BFEA1A (0x3C = sizeof(CMurderTwistAssassin), then 0x0C for the count block),
//   CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me), CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase() 0x007E72A0 (MyEntityHandle +0x1C), and the count block's deleter
//   0x00CDEE00 (stored, not called: `if (p) p->~CEntityScriptBase()` deleting, `__fastcall`).
// Data: vtables CEntityScriptBase 0x012C3224 (stored before the Me copy) and CMurderTwistAssassin 0x012F00BC.
// Layout: Ego_r.pdb CV_MurderTwistFinalScript::CMurderTwistAssassin (60 bytes = the retail new size): ParentClass +0x14, PMasterData +0x18, MyEntityHandle +0x1C, NextLine +0x2C, HasBeenThreatened +0x30, ThreatDistSq +0x34, HasGivenIntroToHero +0x38, PreemptiveAttackInProgress +0x39.
//   CEntityScriptBase: vptr, Game +0x04, Me +0x08 (Ego_r 20 bytes). The parent's Game is CLevelScriptBase's +0x40.
// Shape: the PDB ctor (parent, data, me) inlined: base and ParentClass in the init list, PMasterData assigned in the body;
//   STLport-free counted pointer ctor: Data first, the null test on the argument, the 12-byte {1, deleter, Data} block
//   read back from Data, three exits (block, failed new, null object).
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
