// CVtable_012cc60c::CVtable_012cc60c(CScriptDataBase* data, CGameScriptInterfaceBase* iface) -- quest-script constructor: CScriptBase(), the data/interface pair at +0x44/+0x40, own vtable, an empty scripted-thing handle at +0x50 and an empty CScriptThing at +0x60
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D2F6B0, length 84 bytes.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (the manifest's "CHeroMorphDef" label is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 (the handle's base: vtable 0x0129A7D8, +4 nulled),
// CBaseClass::CBaseClass @ 0x0099A2D0 (the CScriptThing's base).
// Vtable 0x012CC60C has no RTTI locator (quest-script module); the chunk label "CV_MurderTwistFinalScript" is
// carried by five different constructors in the manifest (propagated), so the class is named by its vtable.
// Shape (landed 0x00CE1A00 model): the base runs on `this`, the first argument is stored at +0x44 and the
// second at +0x40 BEFORE this class's vtable store (an inlined `__declspec(novtable)` intermediate base), then
// the two member objects: each base constructor is called on `lea edi,[esi+N]`, its vtable stored, and its two
// counted-pointer words zeroed from ebx by the inline constructor of an embedded {Data, Info} member object
// (plain init-list zeros would hoist the `xor` above the vtable store); `ret 8`, `this` in eax.
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

struct __declspec(novtable) CScriptObject : CScriptBase {   // the argument-carrying intermediate base: both words stored before the derived vtable
    void* m_pInterface;                            // +0x40 (second argument, CGameScriptInterfaceBase*)
    void* m_pData;                                 // +0x44 (first argument, CScriptDataBase*)
    CScriptObject(void* data, void* iface) : CScriptBase(), m_pInterface(iface), m_pData(data) { }
};

class CCPPointerInfo { public:                         // CCountedPointer's shared block (Ego_r.pdb CCPPointerInfo)
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4 destroys the pointee
    void* Data;                                        // +8
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
template <class T> class CCountedPointer { public:     // retail CCountedPointer<T>: Data +0, Info +4; the release
    T*              Data;                              // dtor is 0x007E74D0's inline PImp release (wave 6 GRO rule:
    CCPPointerInfo* Info;                              // VC7.1's per-site inlining of the implicit STB dtor follows
    CCountedPointer() : Data(0), Info(0) {}            // from this size)
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

class CScriptThing : public CBaseClass { public:                 // vtable 0x01238C8C, 12 bytes (engine/CScriptThing.h: PImp_Data +4, PImp_Info +8)
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;                     // +0x04 Data, +0x08 Info
    CScriptThing() : CBaseClass(), PImp() { }
};

struct CVtable_012cc60c : CScriptObject {          // vtable 0x012CC60C
    unsigned char _pad48[8];                       // +0x48..+0x4f
    CScriptGameResourceObjectScriptedThingBase m_scriptedThing;   // +0x50
    CScriptThing  m_thing;                         // +0x60
    virtual void v00();
    CVtable_012cc60c(void* data, void* iface);
};

CVtable_012cc60c::CVtable_012cc60c(void* data, void* iface)
    : CScriptObject(data, iface), m_scriptedThing(), m_thing()
{
}