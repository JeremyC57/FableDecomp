// CObstacleNavigatorExternalInterface::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x01241D04), the member at +8 destroyed (~vector<const CSteeringAgent*> 0x004DF54F), the CNavigatorExternalInterface base's inline destructor stores its vtable (0x01241D74), then free for flag bit 0 (the size-optimised module: `pop ecx`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF507, length 43 bytes.
// Leaf: ??_GCObstacleNavigatorExternalInterface@@UAEPAXI@Z
// The chunk label is propagated; the vtable (0x01241D04) names the class. Shape: the vtable store, the members, `test byte ptr [esp+8],1`, `ret 4`.
// NF399: the member at +8 is Ego_r.pdb DummyAgentList, vector<CSteeringAgent const*> (FableWin ~CObstacleNavigatorExternalInterface,
//   tc_register.obj, destroys it through ~vector<CSteeringAgent const*>); it was the stand-in CMember_004df54f, whose destructor bound
//   ??1CMember_004df54f@@QAE@XZ at 0x004DF54F. Now the shared stlport/fable_containers.h vector, its destructor declared only
//   (FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE: retail calls the ~vector COMDAT 0x004DF54F out of line).
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "stlport/fable_containers.h"
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CSteeringAgent;
class CNavigatorExternalInterface { public:               // vtable 0x01241D74
    virtual void v00();
    long m_4;
    ~CNavigatorExternalInterface() { }             // inline: the vtable store
};
class CObstacleNavigatorExternalInterface : public CNavigatorExternalInterface { public:              // vtable 0x01241D04
    std::vector<const CSteeringAgent*> DummyAgentList;   // +0x08
    FABLE_FORCEINLINE virtual ~CObstacleNavigatorExternalInterface() { }
};

void* MakeOne_004df507()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CObstacleNavigatorExternalInterface;
}
