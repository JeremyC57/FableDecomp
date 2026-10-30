// CGameScriptInterface::CameraUseCameraPoint(const CCharString&, const CScriptThing&, float, long, long) const -- look the camera point up by script name and forward to the CScriptThing overload
// Leaf: ?CameraUseCameraPoint@CGameScriptInterface@@UBEXABVCCharString@@ABVCScriptThing@@MJJ@Z
// Retail: Fable.exe @ 0x008922F0, 132 bytes, `ret 14h`; Ego_r.pdb same symbol.
// Body: CScriptThing point = GetThingWithScriptName(name) (vtable slot 72, +0x120, 0x008A7D60, hidden result);
// CameraUseCameraPoint(point, target, time, mode1, mode2) (slot 413, +0x674, 0x00891070, the CScriptThing overload);
// then point's inline destructor: vptr = 0x01238C8C, ~CCountedPointer (if Info: --RefCount, at zero
// DeleteFunc(Data) __fastcall + operator delete 0x00BFE9BC; Data = Info = 0), out-of-line ~CBaseClass 0x0099A2E0.
// Layout: engine/CScriptThing.h (vptr, PImp CCountedPointer<CScriptThing> +4/+8) flattens the vptr into a data member,
// so the polymorphic classes are declared here as in the landed 0x00F35B40 model; the CGameScriptInterface slots come
// from slots.py on its retail vtable (PDB order). VC7.1 lays an overload group out in reverse declaration order, so
// this overload (slot 414) is declared before the CScriptThing one (slot 413).
// Shape: /O2 (16-aligned entry, no frame).
class CCharString;
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
#define V10(p) virtual void p##0(); virtual void p##1(); virtual void p##2(); virtual void p##3(); virtual void p##4(); \
               virtual void p##5(); virtual void p##6(); virtual void p##7(); virtual void p##8(); virtual void p##9();
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::CameraUseCameraPoint(const CCharString& name, const CScriptThing& target, float time,
                                                long mode1, long mode2) const
{
    CScriptThing point = GetThingWithScriptName(name);
    CameraUseCameraPoint(point, target, time, mode1, mode2);
}