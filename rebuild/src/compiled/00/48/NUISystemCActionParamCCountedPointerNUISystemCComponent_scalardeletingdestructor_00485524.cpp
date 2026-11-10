// NUISystem::CActionParamCCountedPointer<NUISystem::CComponent>::`scalar deleting destructor'(unsigned int flags) -- the component action parameter's deleting destructor: release the held counted pointer, restore the CActionParam vtable, free when asked
// Leaf: ??_G?$CActionParamCCountedPointer@VCComponent@NUISystem@@@NUISystem@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00485524, 43 bytes, thiscall `ret 4`; size-optimised TU (odd address). Bytes: vptr 0x01236794
//   (RTTI .?AV?$CActionParamCCountedPointer@VCComponent@NUISystem@@@NUISystem@@), `lea ecx,[esi+8] ; call 0x004291DE` (the inline
//   ~CCountedPointer<CComponent> of the +8 value: its ReleaseData), CActionParam vptr 0x0122F83C (inline empty base destructor),
//   operator delete 0x00BFE9BC on flags & 1.
// Names: Ego_r.pdb / FableWin.pdb ??0?$CActionParamCCountedPointer@VCComponent@NUISystem@@@NUISystem@@QAE@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z,
//   ??1..UAE@XZ (virtual), ??_E.. (the debug build's vector deleting form); retail emits the scalar form here. Value +8 (the
//   ctor's CCountedPointer& argument; the ctor's type value UI_ACTION_PARAM_TYPE_COMPONENT is assumed, not graded). Was landed as an extern "C" stand-in calling
//   @SplitBranch_BaseSubObject_dtor@4 (catalog module CLandscapeBackgroundSubNodeSplitBranch, propagated).
// Release: 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA; the destructor
//   is the 5-byte jmp COMDAT 0x00429C53, Ego_r ??1 5 bytes).
#pragma optimize("s",on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
template <class T> class CActionParamCCountedPointer : public CActionParam {
public:
    CActionParamCCountedPointer(CCountedPointer<T>& value) : CActionParam(UI_ACTION_PARAM_TYPE_COMPONENT), Value(value) { }
    virtual ~CActionParamCCountedPointer() { }
    CCountedPointer<T> Value;                                           // +0x08
};
}
// The constructor instantiation emits the vtable and with it the scalar deleting destructor (the destructor expanded inside it).
template NUISystem::CActionParamCCountedPointer<NUISystem::CComponent>::CActionParamCCountedPointer(CCountedPointer<NUISystem::CComponent>& value);
