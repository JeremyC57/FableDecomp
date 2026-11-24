// CTCSkeletalMorph::CalculateSkeletalMorph() -- rebuild the skeletal morph: clear its parameters, then for every morph entry fetch the named morph target from the (lazily created) resource manager, wrap it in a new counted morph parameter, add it and set its value; finally drop the cached combined morph
// Leaf: ?CalculateSkeletalMorph@CTCSkeletalMorph@@ABEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00786700, 368 bytes (`ret`).
// Callees: C3DSkeletalMorph parameter clear 0x00A45410 / add 0x00A45470 / combined-morph reset 0x00A45330 (catalog labels are
//          propagated twins; names descriptive); CCountedPointer<CSkeletalMorphResourceManager>::Reset 0x004A9A80 on the global
//          0x013BAB10; CSkeletalMorphResourceManager::GetMorphTarget 0x006C3620; C3DSkeletalMorphParameter ctor 0x00A455A0 and
//          SetCurrentValue 0x00A45370; DeleteData<C3DSkeletalMorphParameter> 0x00786A30 (__fastcall deleter in the count block);
//          operator new 0x00BFEA1A, malloc 0x00BFEA0E (the map header), operator delete 0x00BFE9BC.
// Layout: retail CTCSkeletalMorph Morphs +0x10 (12-byte entries: name +4, value +8), PSkeletalMorph +0x1c (PDB +0x20, CArray
//          16 bytes in the PDB v 12 in release); CSkeletalMorphResourceManager vtable 0x01238F84, CachedMorphs header +4, count +8.
// Shape: the manager's map is its own member object (inline ctor), so the vtable store precedes the header malloc; the
//          parameter's count block is a `new SCountBlock(1, DeleteData, object)` expression; the counted-pointer dtor nulls both words.
// NF52 (round 7): moved onto the shared headers: the made-up global g_SkeletalMorphResourceManager (0x013BAB10) is
//   CTBaseSingleton<CSkeletalMorphResourceManager>::PClass reached through the inline static Get() (engine/CTBaseSingleton.h:
//   `if (PClass.IsNull()) PClass.Reset(new T); return *PClass;`, Reset out of line 0x004A9A80), the manager is
//   engine/CSkeletalMorphResourceManager.h (overlay opt-ins _INLINE_CTOR and _GETMORPHTARGET, PDB GetMorphTarget returns a
//   non-const reference), the counted pointers are engine/CCountedPointer.h, CCharString is fable_string.h. Local
//   SCountBlock / CCountedPointer / CMorphMap / CSkeletalMorphResourceManager / CCharString models deleted.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSKELETALMORPHRESOURCEMANAGER_INLINE_CTOR
#define FABLE_RETAIL_CSKELETALMORPHRESOURCEMANAGER_GETMORPHTARGET
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CSkeletalMorphResourceManager.h"
class C3DSkeletalMorphTarget;
class C3DSkeletalMorphParameter { public:
    unsigned char _d[0x14];
    C3DSkeletalMorphParameter(const C3DSkeletalMorphTarget& target);   // 0x00A455A0 (NF377: Ego_r + FableWin ??0C3DSkeletalMorphParameter@@QAE@ABVC3DSkeletalMorphTarget@@@Z)
    void SetCurrentValue(float value);              // 0x00A45370
};
class C3DSkeletalMorph { public:
    void Reset();                                   // 0x00A45410 (NF334: Ego_r + FableWin ?Reset@C3DSkeletalMorph@@QAEXXZ, the landed self)
    void AddMorphParameter(const CCountedPointer<C3DSkeletalMorphParameter>& parameter);   // 0x00A45470
    void NotifyParamUpdate();                       // 0x00A45330 (Ego_r + FableWin; NF292, was ResetCombinedMorph)
};
struct CSkeletalMorphEntry {                        // CTCSkeletalMorph::CSkeletalMorph, 12 bytes
    long        _00;
    CCharString Name;                               // +0x04
    float       Value;                              // +0x08
};
class CTCSkeletalMorph {
public:
    virtual void v00();
    unsigned char        _pad04[0x10 - 0x04];
    CSkeletalMorphEntry* m_morphsStart;             // +0x10 Morphs
    CSkeletalMorphEntry* m_morphsFinish;            // +0x14
    CSkeletalMorphEntry* m_morphsEnd;               // +0x18
    C3DSkeletalMorph*    PSkeletalMorph;            // +0x1c (CCountedPointer object)
    private:
    void CalculateSkeletalMorph() const;
    public:
};

void CTCSkeletalMorph::CalculateSkeletalMorph() const
{
    PSkeletalMorph->Reset();
    for (CSkeletalMorphEntry* morph = m_morphsStart; morph != m_morphsFinish; ++morph) {
        CCountedPointer<C3DSkeletalMorphTarget> target = CSkeletalMorphResourceManager::Get().GetMorphTarget(morph->Name);
        CCountedPointer<C3DSkeletalMorphParameter> parameter(new C3DSkeletalMorphParameter(*target.Data));
        PSkeletalMorph->AddMorphParameter(parameter);
        parameter->SetCurrentValue(morph->Value);
    }
    PSkeletalMorph->NotifyParamUpdate();
}