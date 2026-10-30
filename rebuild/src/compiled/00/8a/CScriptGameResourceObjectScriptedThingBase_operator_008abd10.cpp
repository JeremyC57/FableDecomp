// CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) -- memberwise assignment: the leading part through its out-of-line operator=, then the counted pointer (release the old block, adopt, add-ref)
// Leaf: ??4CScriptGameResourceObjectScriptedThingBase@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008abd10, 80 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ??4CScriptGameResourceObjectScriptedThingBase@@QAEAAV0@ABV0@@Z (bound in reloc_bindings); RTTI bases CScriptGameResourceObjectBase : CBaseObject : CBaseClass, Ego_r.pdb PImp (CCountedPointer<CScriptGameResourceObjectScriptedThingBase>) at +8; callers CGameScriptInterface::StartScriptingEntity 0x0089B5B0 and the cutscene macro runner 0x00CBFB7D.
// Callees: CBaseObject::operator=(const CBaseObject&) 0x0099A3B0 (Ego_r.pdb IAE), operator delete 0x00BFE9BC (the old count block); CCountedPointer's operator= inline.
// Shape (family L088): the compiler-generated copy assignment, written out (same body) so the translation unit emits it.
class CCPPointerInfo {                                     // 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;                 // the source's fields are read first
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObject : public CBaseClass {
protected:
    CBaseObject& operator=(const CBaseObject& other);      // 0x0099A3B0
    void* PPointer;                                        // +0x04
};
class CScriptGameResourceObjectBase : public CBaseObject {
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {
public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);
};
CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other)
{
    CScriptGameResourceObjectBase::operator=(other);
    PImp = other.PImp;
    return *this;
}