// CIconText::CIconText(long) -- initialize UI text, font pointer, primitive handles and icon vectors
// Retail00542330,178 bytes. Actual fable_string.h CWideString and actual donor UI base headers retained.
// Actual global CComponent/CObserver donor views are348/24 bytes; native projections below
// are308/20 bytes because of retail container layouts (RTTI base offsets0/4/18).
// CIconText,CChangingStateComponent,CUIInterface,CActionParam,CBaseClass and handle headers do not exist.
// RTTI12470CC confirms actual bases CChangingStateComponent/CComponent/CUIInterface+0/CObserver+4/CActionParam+18.
// 0052CC50 is CChangingStateComponent ctor (manifest propagated CDraggableInto is wrong).
// Native UI containers shrink CComponent348 donor ->308 retail and changing396 ->348.
// Genuine virtual destructors in all3 bases create native vptr stores, no fake slots.
// CWideString ctor0099AED0; CBaseClass ctor0099A2D0; UpdateFont00541FD0.
#include "fable_string.h"
#include "engine/_quarantine/CComponent.h"
#include "engine/CObserver.h"
typedef ::CComponent CComponentDonorLayout;
typedef ::CObserver CObserverDonorLayout;
class CBaseClass { public:
 CBaseClass(); //0099A2D0 VT01231710
 virtual ~CBaseClass();
};
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
 void UpdateFont(); //00541FD0
};
FABLE_STATIC_ASSERT(sizeof(CChangingStateComponent)==0x15c);
CIconText::CIconText(long parent) : CChangingStateComponent(parent) {
 NoRenderNextFrame=0;
 UpdateFont();
 Unavailable=false;
}
}