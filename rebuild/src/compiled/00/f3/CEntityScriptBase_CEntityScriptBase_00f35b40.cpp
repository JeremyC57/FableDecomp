// CEntityScriptBase::~CEntityScriptBase() -- base destructor of the entity (personal) scripts: release the Me script-thing handle
// Leaf: ??1CEntityScriptBase@@UAE@XZ
// Retail: Fable.exe @ 0x00F35B40, 73 bytes, tail `jmp` to CBaseClass::~CBaseClass (0x0099A2E0); 30+ callers, each a
// script class's ??_G (e.g. 0x00CDEBF0, slot 0 of the vtable 0x012C3224 stored here). Catalog label
// NUISystem::CFrontEndScreen::~CFrontEndScreen is wrong: the members are Ego_r.pdb CEntityScriptBase's.
// Body: vptr = 0x012C3224; Me (+0x08, CScriptThing) is destroyed inline -- its vptr = CScriptThing's (0x01238C8C, RTTI
// .?AVCScriptThing@@ : CBaseClass), PImp's ~CCountedPointer (if Info, --RefCount, at zero DeleteFunc(Data) __fastcall and
// operator delete(Info) 0x00BFE9BC; Data = Info = 0), then the out-of-line ~CBaseClass.
// Layout: Ego_r.pdb CEntityScriptBase (0x14): vptr +0, Game +4 (CGameScriptInterfaceBase*), Me +8 (CScriptThing, 0xc:
// vptr, PImp CCountedPointer<CScriptThing> +4/+8 -- engine/CScriptThing.h flattens the vptr into a data member, so the
// polymorphic classes are declared here).
// Shape: /O2.
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
class CGameScriptInterfaceBase;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
CEntityScriptBase::~CEntityScriptBase()
{
}