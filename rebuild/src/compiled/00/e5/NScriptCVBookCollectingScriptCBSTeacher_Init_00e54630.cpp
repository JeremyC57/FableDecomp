// NScript::CV_BookCollectingScript::CBS_Teacher::Init() -- copy the teacher's book thresholds and the morality reward (truncated to long) from the global script def
// Leaf: ?Init@CBS_Teacher@CV_BookCollectingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E54630, 98 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012E5F40, which the landed
//   NScript::CV_BookCollectingScript::CBS_Teacher::Alloc 0x00E54580 stores (site 0x00E545B5); Ego_r.pdb has ?Init@CBS_Teacher@CV_BookCollectingScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CBS_Teacher (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: __ftol2 0x00BFEA70 (the float-to-long conversion).
class CScriptDef {                                      // Ego_r.pdb CScriptDef; retail offsets (Ego_r - 0x44: 17 vectors of 0x10 are 0xC)
public:
    unsigned char _pad0[0x4e0];
    long  BooksNumberWanted;                            // +0x4e0 (Ego_r +0x524)
    long  BooksNumberAccepted;                          // +0x4e4
    long  BooksNumberComment;                           // +0x4e8
    long  BooksNumberTotal;                             // +0x4ec
    long  BadBooksForHat;                               // +0x4f0
    long  GoodBooksForHat;                              // +0x4f4
    float BooksMoralityReward;                          // +0x4f8
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CGameScriptInterface;
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BookCollectingScript {
public:
    class CBS_Teacher;
};
class CV_BookCollectingScript::CBS_Teacher : public CEntityScriptBase {
public:
    CV_BookCollectingScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    long BadBooksForHat;                                // +0x1c
    long GoodBooksForHat;                               // +0x20
    long BooksWanted;                                   // +0x24
    long BooksAccepted;                                 // +0x28
    long BooksComment;                                  // +0x2c
    long MoralityReward;                                // +0x30
private:
    virtual void Init();
};

void CV_BookCollectingScript::CBS_Teacher::Init()
{
    BadBooksForHat = pGlobalScriptDef->BadBooksForHat;
    GoodBooksForHat = pGlobalScriptDef->GoodBooksForHat;
    MoralityReward = (long)pGlobalScriptDef->BooksMoralityReward;
    BooksWanted = pGlobalScriptDef->BooksNumberWanted;
    BooksAccepted = pGlobalScriptDef->BooksNumberAccepted;
    BooksComment = pGlobalScriptDef->BooksNumberComment;
}
}