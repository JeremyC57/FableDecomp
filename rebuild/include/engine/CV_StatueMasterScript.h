#ifndef FABLE_ENGINE_CV_STATUEMASTERSCRIPT_H
#define FABLE_ENGINE_CV_STATUEMASTERSCRIPT_H
// Retail opt-in (chunk 837): NScript::CV_StatueMasterScript, new header (no includer before chunk 837).
// Layout: Ego_r.pdb CV_StatueMasterScript (struct_layouts_egor.tsv, 108 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): AnglesToFaceList float[4] +0x48; retail sizeof 0x58 (the landed Alloc 0x00ED4A40 `push 58h`; the
// landed GetStatuePointingPosition 0x00ED4420 reads +0x4c..+0x54; the Main 0x00ED3B30 stores +0x48..+0x54). Script vtable
// 0x012F2E6C (stored by the landed ctor 0x00ED3A50): RegisterMain 0x00ED3A90, Main 0x00ED3B30, Init 0x00ED3A80 (landed but Main).
// Methods: Ego_r.pdb ?GetStatuePointingPosition@CV_StatueMasterScript@NScript@@QAEJXZ (landed 0x00ED4420); GetTimeOfDayFraction
// (landed 0x00ED43D0, not declared here: its PDB spelling is not in the Ego_r name set this header was checked against).
// Nested entity scripts (Ego_r.pdb ?Alloc@<Class>@CV_StatueMasterScript@NScript@@SI...; the landed Alloc rows; the binding-name
// literal is the class name less its `C`): CStatueMasterStatue (Alloc 0x00ED40C0), CStatueMasterCellarDoors (Alloc 0x00ED44C0),
// CStatueMasterChest (Alloc 0x00ED4770), forward-declared only.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the landed ctor 0x00ED3A50).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CV_StatueMasterScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_StatueMasterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00ED3A90
    virtual void Main();                                                 // 0x00ED3B30
    virtual void Init();                                                 // 0x00ED3A80
    long GetStatuePointingPosition();                                    // 0x00ED4420
    float AnglesToFaceList[4];                                           // +0x48
    class CStatueMasterStatue;                                           // Alloc 0x00ED40C0
    class CStatueMasterCellarDoors;                                      // Alloc 0x00ED44C0
    class CStatueMasterChest;                                            // Alloc 0x00ED4770
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_StatueMasterScript) == 0x58);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_STATUEMASTERSCRIPT_H
