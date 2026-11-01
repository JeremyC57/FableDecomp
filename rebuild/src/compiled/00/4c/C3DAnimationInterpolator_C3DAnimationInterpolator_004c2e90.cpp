// C3DAnimationInterpolator::~C3DAnimationInterpolator() -- the compiler-generated destructor: destroy EndState and StartState (each releasing its shared-data intrusive pointer, then ~CBaseClass), then the CBaseClass base destructor
// Leaf: ??1C3DAnimationInterpolator@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C2E90, 81 bytes, thiscall, tail `jmp ~CBaseClass`.
// Identity (proven): the slot-0 `??_G` 0x004C3D10 of vtable 0x01239840 (RTTI .?AVC3DAnimationInterpolator@@ : CBaseClass)
//   calls this body; the other callers destroy C3DAnimationInterpolator locals and members (CTCGraphicAppearance
//   RefreshPrimitiveAnimationAndPos / GetHelperPoint / GetDummyObject, the landed CEngineInternalPrimitiveAnimatedMesh dtor
//   0x00BD9250 binding ~C3DAnimationInterpolator, the 0x004C3360 implicit dtor of the class holding it at +0, CTCAssassinRush,
//   CAIStateGroup_SummonerCharge). Ego_r.pdb (0x004C5F00) and FableWin.pdb (0x01D09060) spell ??1C3DAnimationInterpolator@@UAE@XZ.
//   No 0x01239840 store: the destructor is the implicit one (and so is the members': no C3DAnimationBlendState vtable store).
// Callees: the shared data's slot-1 Delete through the vtable, CBaseClass::~CBaseClass 0x0099A2E0 (three times: EndState,
//   StartState, then the tail jump for this object).
// Shape: members destroyed in reverse order (EndState +0xC, StartState +4); each C3DAnimationBlendState implicit dtor inlines the
//   CIVCountedPointer<C3DAnimationBlendState::CSharedData>::Release of Data (Object copied to a local, `dec [obj+4]`, Delete at
//   zero, zeroed) and calls ~CBaseClass out of line. DestroyOne (a destruction by name, ungraded) makes VC7.1 emit this
//   out-of-line implicit `??1` (chunks 656 / 657 / 696).
// Layout: Ego_r.pdb C3DAnimationInterpolator (0x14): CBaseClass vptr +0, StartState +4, EndState +0xC; C3DAnimationBlendState
//   (8): CBaseClass vptr +0, Data CIVCountedPointer<CSharedData> +4. No engine headers exist for either class (local models);
//   CBaseClass / CIVCountedPointeeBase are the shared retail models.
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"

template <class T> class CIVCountedPointer {
public:
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointeeBase* Object;                       // +0x00
};

class C3DAnimationBlendState : public CBaseClass {       // 8 bytes
public:
    class CSharedData;
    C3DAnimationBlendState();
    CIVCountedPointer<CSharedData> Data;                 // +0x04
};

class C3DAnimationInterpolator : public CBaseClass {     // vtable 0x01239840, 0x14 bytes
public:
    C3DAnimationInterpolator();
    C3DAnimationBlendState StartState;                   // +0x04
    C3DAnimationBlendState EndState;                     // +0x0C
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(C3DAnimationInterpolator* interpolator)
{
    interpolator->C3DAnimationInterpolator::~C3DAnimationInterpolator();
}