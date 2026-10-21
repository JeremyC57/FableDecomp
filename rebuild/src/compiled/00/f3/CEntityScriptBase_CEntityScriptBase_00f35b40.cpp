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
struct CCPPointerInfo {
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
class CBaseClass {
public:
    virtual ~CBaseClass();                               // 0x0099A2E0
};
class CScriptThing : public CBaseClass {
public:
    CCountedPointer<CScriptThing> PImp;                  // +0x04
    virtual ~CScriptThing() {}
};
class CGameScriptInterfaceBase;
class CEntityScriptBase {
public:
    CGameScriptInterfaceBase* Game;                      // +0x04
    CScriptThing              Me;                        // +0x08
    virtual ~CEntityScriptBase();
};
CEntityScriptBase::~CEntityScriptBase()
{
}