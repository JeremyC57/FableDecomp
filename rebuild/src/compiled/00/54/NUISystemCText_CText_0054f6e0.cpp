// NUISystem::CText::~CText() -- text component dtor: clear both primitive handles and the font bank, release the def's font by name through the system font manager, then the member dtors (two CEnginePrimitiveHandles, the font counted pointer, the value string) and ~CChangingStateComponent
// Leaf: ??1CText@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054F6E0, length 510 bytes; Ego_r.pdb `??1CText@NUISystem@@UAE@XZ` (the three vtable
//   stores 0x01249CCC / 0x01249CA4 / 0x01249C9C are CText's CUIInterface / CObserver (+4) / CActionParam (+0x18) tables; rtti_bases:
//   CText : CChangingStateComponent : CComponent : CUIInterface@0, CObserver@4, CActionParam@24).
// Callees: CComponent::LoadAndReturnDef() const 0x00530EC0 (Ego_r.pdb `?LoadAndReturnDef@CComponent@NUISystem@@UBE?AV?$CDefPointer@$$CBVCUIDef@NUISystem@@@@XZ`,
//   vtable slot 108; called directly because it is a virtual call on `this` inside a destructor), CDefStringTable::GetString(long) const
//   0x009D49B0 on g_DefStringTable 0x013CA828, GFGetSystemManager 0x009A4EC0 (`?GFGetSystemManager@@YIPAVCSystemManager@@XZ`),
//   CFontManager::ReleaseFont(const CCharString&) 0x009E2B40, ~CCharString 0x0099EAE0, ~CBaseClass 0x0099A2E0 (each handle's base),
//   ~CWideString 0x0099B510, ~CChangingStateComponent 0x0052CCF0, operator delete 0x00BFE9BC.
// Layout: retail CText = Ego_r.pdb minus 0x30 (landed ctor 0x0054F5C0 / GetDataFromDef 0x0054ED90): Value +0x15C, PFont +0x160/+0x164,
//   Handle +0x168, HandleDropShadowBottomRight +0x170. CSystemManager::PFontManager (CCountedPointer<CFontManager>) at retail +0x84.
// Shape: CEnginePrimitiveHandle is the landed CPSCDecalRenderer::~CPSCDecalRenderer 0x00B1FFC0 model (Ego_r.pdb Clear(): IsUnique test,
//   NotifyReleased(*this), `Data = 0`; an inline destructor = Clear() + the CIVCountedPointer destructor + ~CBaseClass), which stores the
//   handle vtable before loading Data as retail does (2026-10-28 retry; the parked model hoisted the load).
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};
class CDefStringTable {
public:
    CCharString GetString(long index) const;                // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                    // 0x013CA828
class CDefString {
public:
    long Index;
    CCharString GetString() const { return g_DefStringTable.GetString(Index); }
};

class CCPPointerInfo {                                      // CCountedPointer count block
public:
    long  RefCount;                                         // +0x00
    void (__fastcall* DeleteData)(void* data);              // +0x04
    void* Data;                                             // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    __forceinline void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteData(Info->Data);
                delete Info;
            }
        }
    }
    __forceinline void Reset() { ReleaseData(); Info = 0; Data = 0; }
    __forceinline ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
};

class CFontBank;
class CFontManager {
public:
    void ReleaseFont(const CCharString& name);              // 0x009E2B40
};
class CSystemManager {
public:
    unsigned char                  _pad00[0x84];
    CCountedPointer<CFontManager>  PFontManager;            // +0x84 (retail)
    CFontManager* GetFontManager() { return PFontManager.Data; }
};
CSystemManager* __fastcall GFGetSystemManager();            // 0x009A4EC0

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CEnginePrimitiveHandle;
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable long IVRefCount;                                // +0x04
    long Release() const { long count = --IVRefCount; if (count == 0) Delete(); return count; }
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
    friend class CEnginePrimitiveHandle;
protected:
    virtual bool WasRendered() const;                       // slot 2
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3 (+0x0C)
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->IVRefCount == 1; }
    __forceinline void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(int) { Release(); Object = 0; return *this; }
    __forceinline ~CIVCountedPointer() { Release(); }
};
class CEnginePrimitiveHandle : public CBaseClass {          // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data; // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->NotifyReleased(*this);
        Data = 0;
    }
    __forceinline ~CEnginePrimitiveHandle() { Clear(); }
};

template <class T> class CDefPointer;
namespace NUISystem {
// CUIDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase (rtti_bases, single chain);
// retail Font (CDefString) +0x50 as the landed GetDataFromDef 0x0054ED90.
class CUIDef : public CIVCountedPointeeBase {
public:
    unsigned char _pad08[0x50 - 0x08];
    CDefString    Font;                                     // +0x50
};
}
template <class T> class CDefPointer {
public:
    T* Data;
    ~CDefPointer() { if (Data) Data->Release(); }
    T* operator->() const { return Data; }
};
namespace NUISystem {
class CUIInterface { public: virtual ~CUIInterface(); };
class CObserver { public: virtual ~CObserver(); unsigned char _body[0x14 - 4]; };
class CActionParam { public: virtual ~CActionParam(); };
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    virtual ~CComponent();
    virtual CDefPointer<const CUIDef> LoadAndReturnDef() const;   // 0x00530EC0, slot 108
    unsigned char _body[0x15c - 0x1c];
};
class CChangingStateComponent : public CComponent {
public:
    virtual ~CChangingStateComponent();                     // 0x0052CCF0
};
class CText : public CChangingStateComponent {
public:
    CWideString                 Value;                      // +0x15c
    CCountedPointer<CFontBank>  PFont;                      // +0x160
    CEnginePrimitiveHandle      Handle;                     // +0x168
    CEnginePrimitiveHandle      HandleDropShadowBottomRight;   // +0x170
    virtual ~CText();
};

CText::~CText()
{
    Handle.Clear();
    HandleDropShadowBottomRight.Clear();
    PFont.Reset();
    GFGetSystemManager()->GetFontManager()->ReleaseFont(LoadAndReturnDef()->Font.GetString());
}
}