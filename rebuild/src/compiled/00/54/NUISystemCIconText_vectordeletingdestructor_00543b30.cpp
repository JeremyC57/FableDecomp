// NUISystem::CIconText::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x00543C70
// Leaf: ??_ECIconText@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00543B30, 8 bytes: `sub ecx,4 ; jmp 0x00543C70`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CIconText's CObserver vtable 0x012470A8 at +4 (rtti-vtables.tsv, RTTI .?AVCIconText@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x00543C70 is slot 0 of the primary vtable 0x012470CC, the class's ??_G (it calls
//   ~CIconText 0x005424B0; FableWin.pdb ??_ECIconText@NUISystem@@UAEPAXI@Z). VC7.1 names the slot-0 entry ??_E (a weak alias of ??_G
//   when the TU has no array new) and spells the thunk with the adjustor (W3 = 4); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; the definer is the landed constructor
//   CIconText(long) 0x00542330 (rebuild/src/compiled/00/54/CIconText_CIconText_00542330.cpp, reused verbatim below, as the chunk-765
//   precedent for big classes). That function is not graded here (it is its own row); only the thunk is this row.
// Callees: ??_GCIconText@NUISystem@@UAEPAXI@Z 0x00543C70 (bound as its ??_E alias; landed placeholder ?Override_00543c70@CMulti_00543c70@@UAEXXZ, NAME_FIXES.tsv).
// Twin: 0x00543B40 is the +0x18 (CActionParam) adjustor thunk of the same vtable set.
#include "fable_string.h"
#include "engine/_quarantine/CComponent.h"
#include "engine/CObserver.h"
typedef ::CComponent CComponentDonorLayout;
typedef ::CObserver CObserverDonorLayout;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CEnginePersistentPrimitiveBase;
class CEnginePrimitiveHandle : public CBaseClass { public:
 CEnginePersistentPrimitiveBase* Data;
 CEnginePrimitiveHandle() : Data(0) {} //own VT0122F598, genuine inline default ctor
 virtual ~CEnginePrimitiveHandle();
};
template<class T> struct CNativeVector {
 T *First,*Last,*End;
 CNativeVector() : First(0),Last(0),End(0) {}
};
namespace NUISystem {
struct CUIInterface { virtual ~CUIInterface(); };
class CObserver { public:
 virtual ~CObserver();
 void* EventsToObserve[3];
 bool PreventObservation;
};
class CActionParam { public:
 virtual ~CActionParam();
 fable_i32 ActionParamType;
};
class CComponent : public CUIInterface,public CObserver,public CActionParam { public:
 unsigned char ComponentMembers[0x134-0x20]; //opaque native base data; unrelated to this constructor
 virtual ~CComponent();
};
class CChangingStateComponent : public CComponent { public:
 fable_i32 StatesBeingDone,StatesDone;
 void* StatesToDo;
 float ParentUpdateTime;
 fable_u32 CurrentState,TargetState,RequestedState;
 float UpdateTime;
 bool PreviousUpdateChanged;
 fable_u32 PreviousState;
 CChangingStateComponent(long); //0052CC50
 virtual ~CChangingStateComponent();
};
class CFontBank;
struct CFontPointer { CFontBank* Data;void* Info;CFontPointer():Data(0),Info(0) {} };
class CIconText : public CChangingStateComponent { public:
 CWideString Text,DisplayText;
 CFontPointer PFont;
 bool Unavailable;
 CEnginePrimitiveHandle TextHandle,DropShadowHandle;
 CNativeVector<CEnginePrimitiveHandle> IconHandles;
 struct CEmbeddedIcon;
 CNativeVector<CEmbeddedIcon> Icons;
 float Window[4],PositionOffset[2];
 fable_u8 NoRenderNextFrame;
 CIconText(long);
 virtual ~CIconText();
 void GetDataFromDef(); //00541FD0 ?GetDataFromDef@CIconText@NUISystem@@QAEXXZ (Ego_r.pdb 0x005BB900 / FableWin.pdb 0x02262EA0; NF28: was UpdateFont)
};
FABLE_STATIC_ASSERT(sizeof(CChangingStateComponent)==0x15c);
CIconText::CIconText(long parent) : CChangingStateComponent(parent) {
 NoRenderNextFrame=0;
 GetDataFromDef();
 Unavailable=false;
}
}