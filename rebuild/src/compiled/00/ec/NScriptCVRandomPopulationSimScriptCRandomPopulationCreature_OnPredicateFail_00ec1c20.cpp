// NScript::CV_RandomPopulationSimScript::CRandomPopulationCreature::OnPredicateFail() -- a population creature is gone: count one fewer of its type and, when the hero killed it, lower the current region's population scale by 0.05 for a guard (type 2) or raise it for any other type, clamped to 0..1
// Leaf: ?OnPredicateFail@CRandomPopulationCreature@CV_RandomPopulationSimScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC1C20, 176 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012F1390, which the landed
//   NScript::CV_RandomPopulationSimScript::CRandomPopulationCreature::Alloc 0x00EC1B70 stores (site 0x00EC1BA5); Ego_r.pdb has ?OnPredicateFail@CRandomPopulationCreature@CV_RandomPopulationSimScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CRandomPopulationCreature (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CScriptThing slot 18 MsgIsKilledBy ("" 0x0122D70E temporary); CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0. Float pool: 0.05f 0x0125A15C, 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
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
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_RandomPopulationSimScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    float CurrentPopulationScale[4];                    // +0x48
    bool HighDetailSimulationActive;                    // +0x58
    long CurrentPopulationRegion;                       // +0x5c
    long CreaturesAlive[3];                             // +0x60
    class CRandomPopulationCreature;
};
class CV_RandomPopulationSimScript::CRandomPopulationCreature : public CEntityScriptBase {
public:
    CV_RandomPopulationSimScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    CScriptThing Exit;                                  // +0x1c
    long MyType;                                        // +0x28
    bool AttachedToVillage;                             // +0x2c
private:
    virtual void OnPredicateFail();
};

void CV_RandomPopulationSimScript::CRandomPopulationCreature::OnPredicateFail()
{
    --ParentClass->CreaturesAlive[MyType];
    if (Me.MsgIsKilledBy("")) {
        if (MyType == 2)
            ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion] -= 0.05f;
        else
            ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion] += 0.05f;
        float& scale = ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion];
        if (scale < 0.0f)
            scale = 0.0f;
        else if (scale > 1.0f)
            scale = 1.0f;
    }
}
}