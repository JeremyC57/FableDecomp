// CGameScriptInterface::CameraUseCameraPoint(const CCharString&, const C3DVector*, const CRightHandedSet*, float, long, long) const -- look the camera point up by script name and forward to the CScriptThing/position overload
// Leaf: ?CameraUseCameraPoint@CGameScriptInterface@@UBEXABVCCharString@@PBVC3DVector@@PBVCRightHandedSet@@MJJ@Z
// Retail: Fable.exe @ 0x00892380, 137 bytes, `ret 18h`; Ego_r.pdb same symbol (slot 412, +0x670).
// Body: CScriptThing point = GetThingWithScriptName(name) (slot 72, +0x120); CameraUseCameraPoint(point, position,
// orientation, time, mode1, mode2) (slot 411, +0x66c, 0x0089E190); then point's inline destructor (vptr 0x01238C8C,
// ~CCountedPointer, operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0). Twin of the matched 0x008922F0.
// Layout: as 0x008922F0. The four CameraUseCameraPoint overloads occupy slots 411..414; VC7.1 lays an overload group out
// in reverse declaration order, so they are declared 414, 413, 412, 411.
// Shape: /O2 (16-aligned entry, no frame).
class CCharString;
class C3DVector;
class CRightHandedSet;
class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass {
public:
    CCountedPointer<CScriptThing> PImp;                  // +0x04
    CScriptThing(const CScriptThing& other);
    virtual ~CScriptThing() {}
};
#define V10(p) virtual void p##0(); virtual void p##1(); virtual void p##2(); virtual void p##3(); virtual void p##4(); \
               virtual void p##5(); virtual void p##6(); virtual void p##7(); virtual void p##8(); virtual void p##9();
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::CameraUseCameraPoint(const CCharString& name, const C3DVector* position,
                                                const CRightHandedSet* orientation, float time,
                                                long mode1, long mode2) const
{
    CScriptThing point = GetThingWithScriptName(name);
    CameraUseCameraPoint(point, position, orientation, time, mode1, mode2);
}