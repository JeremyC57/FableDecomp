// CScriptGameResourceObjectScriptedThing::CreateNullHandle(CGameScriptInterface&) -- builds a scripted-thing handle that owns a new, unbound scripted-thing implementation
// Leaf: ?CreateNullHandle@
// Retail 0x00903810, 0xB2 bytes, static __fastcall (ecx = hidden result, edx = scriptInterface), plain `ret`.
//   Manifest label "CGameScriptInterface_CreateScriptedEntity" (_global); PDB (FableWin) name set has
//   `?CreateNullHandle@CScriptGameResourceObjectScriptedThing@@SI?AVCScriptGameResourceObjectScriptedThingBase@@AAVCGameScriptInterface@@@Z`.
//   Only caller: CGameScriptInterface::StartScriptingEntity 0x0089B5B0 (call at 0x0089B71B).
// What it does: default-constructs a CScriptGameResourceObjectScriptedThingBase handle (vtable 0x0127094C, empty PImp),
//   gives it a `new CScriptGameResourceObjectScriptedThing(scriptInterface)` (0x28 bytes), returns a copy (PImp AddRef)
//   and destroys the local (PImp release, CScriptGameResourceObjectBase vtable 0x0126008C, CBaseObject dtor).
// Callees: CBaseObject::CBaseObject() 0x0099A380, CBaseObject::CBaseObject(const CBaseObject&) 0x0099A390,
//   CBaseObject::~CBaseObject() 0x0099A430, operator new 0x00BFEA1A, operator delete 0x00BFE9BC,
//   CScriptGameResourceObjectScriptedThing(CGameScriptInterface&) 0x00903710, and the handle's impl setter 0x008AB960
//   (`add ecx,8; jmp 0x008AB230` = PImp.Reset(T*), a folded out-of-line member; its real name is not in the PDB name sets).
// Shape: no NRVO in VC7.1, so local + copy ctor + local dtor. CCountedPointer has a user copy ctor (null-tested AddRef)
//   and an inline dtor whose release is null-tested, then zeroes Data and Info; the intermediate
//   CScriptGameResourceObjectBase vptr store of the constructors is dead and dropped, the one in its user dtor stays.
class CCPPointerInfo { public:
    unsigned long RefCount;                              // +0x00
    void (__fastcall* DeleteFunc)(void* data);           // +0x04
    void*         Data;                                  // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
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
class CBaseObject {
public:
    void* IntelligentPointer;                            // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    CBaseObject(const CBaseObject& other);               // 0x0099A390
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectBase : public CBaseObject {
public:
    CScriptGameResourceObjectBase() {}
    virtual ~CScriptGameResourceObjectBase() {}
};
class CGameScriptInterface;
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {
public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    void SetImplementation(CScriptGameResourceObjectScriptedThingBase* imp);   // 0x008AB960
};
class CScriptGameResourceObjectScriptedThing : public CScriptGameResourceObjectScriptedThingBase {
public:
    CGameScriptInterface* ScriptInterface;               // +0x10
    void* PThing[2];                                     // +0x14
    long ScriptID;                                       // +0x1c
    bool NullResource;                                   // +0x20
    long ActionPriority;                                 // +0x24
    CScriptGameResourceObjectScriptedThing(CGameScriptInterface& scriptInterface);   // 0x00903710
    static CScriptGameResourceObjectScriptedThingBase __fastcall CreateNullHandle(CGameScriptInterface& scriptInterface);
};
CScriptGameResourceObjectScriptedThingBase __fastcall
CScriptGameResourceObjectScriptedThing::CreateNullHandle(CGameScriptInterface& scriptInterface)
{
    CScriptGameResourceObjectScriptedThingBase handle;
    handle.SetImplementation(new CScriptGameResourceObjectScriptedThing(scriptInterface));
    return handle;
}