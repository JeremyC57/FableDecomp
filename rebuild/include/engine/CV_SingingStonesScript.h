#ifndef FABLE_ENGINE_CV_SINGINGSTONESSCRIPT_H
#define FABLE_ENGINE_CV_SINGINGSTONESSCRIPT_H
// Retail opt-in (chunk 804): NScript::CV_SingingStonesScript, new header (no includer before chunk 804).
// Layout: Ego_r.pdb CV_SingingStonesScript (struct_layouts_egor.tsv, 180 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48), then - 4 after the STLport vector EffectList (PDB 16, retail 12): CurrentPlayListIndex +0x48,
// PlayList +0x4c, RudePlayList +0x5c, YourPersonalPlayList +0x6c, EffectList +0x7c, ListText +0x88, DoorManIntroComplete +0x98 ..
// DoorManComplete +0x9b, sizeof 0x9c. Checked against retail: CManWithDoorName::Main 0x00ED17D0 writes +0x98 / +0x99 / +0x9a /
// +0x9b and reads +0x98 .. +0x9b (the four OnPersist 0x00ED1010 literals name the same members).
// Nested CManWithDoorName (Ego_r.pdb, 28 bytes): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18.
// Retail methods: the Ego_r.pdb name set ?<Name>@CV_SingingStonesScript@NScript@@...; addresses from ghidra_out labels (script
// vtable 0x012F2B0C, CManWithDoorName vtable 0x012F2C98: 0 sdd 0x00ED17B0, 1 Main, 2 Init, 3 GetParentScript).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the entity scripts' PMasterData is CQ_SunnyvaleMasterData*).
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_SingingStonesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00ED0F50 (slot 1)
    virtual void Main();                                                 // 0x00ED10A0 (slot 2)
    virtual void Init();                                                 // 0x00ED0EC0 (slot 3)
    virtual void OnPersist(CPersistContext&);                            // 0x00ED1010 (slot 4)
    long CurrentPlayListIndex;                                           // +0x48
    long PlayList[4];                                                    // +0x4c
    long RudePlayList[4];                                                // +0x5c
    long YourPersonalPlayList[4];                                        // +0x6c
    std::vector<CScriptThing> EffectList;                                // +0x7c
    CCharString ListText[4];                                             // +0x88
    bool DoorManIntroComplete;                                           // +0x98
    bool DoorManAttackedByHero;                                          // +0x99
    bool DoorManHasBribe;                                                // +0x9a
    bool DoorManComplete;                                                // +0x9b
    class CSingingStone;                                                 // Alloc 0x00ED1420
    class CManWithDoorName : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_SingingStonesScript&, CScriptDataBase*, const CScriptThing&); // 0x00ED16E0
        virtual CScriptBase* GetParentScript() const;                    // 0x00ED1780
    protected:
        CV_SingingStonesScript& ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        virtual void Init();                                             // 0x00ED1790
        virtual void Main();                                             // 0x00ED17D0
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_SingingStonesScript) == 0x9C);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_SingingStonesScript::CManWithDoorName) == 0x1C);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_SINGINGSTONESSCRIPT_H
