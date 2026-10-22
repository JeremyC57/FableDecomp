// NUISystem::CText::~CText() -- text component dtor: release both primitive handles and the font bank, release the def's font by name through the system font bank, then the member dtors (two CEnginePrimitiveHandles, the font counted pointer, the value string) and ~CChangingStateComponent
// Grade: DIFFER(506v510) -- in the two inlined member CEnginePrimitiveHandle destructors retail stores the handle vtable 0x0122F598 first and loads the primitive into eax (`mov ecx,eax` before the vcall); ours hoists the load above the vtable store into ecx (the explicit Release() calls in the body match)
// Leaf: ??1CText
// Callees: LoadAndReturnDef 0x530EC0 (static in a dtor; label CEventKilledCreature is wrong), CDefStringTable::GetString 0x9D49B0 on 0x13CA828, GFGetSystemManager 0x9A4EC0,
//   font bank ReleaseFont 0x9E2B40, ~CCharString 0x99EAE0, ~CBase 0x99A2E0 (handle base), ~CWideString 0x99B510, ~CChangingStateComponent 0x52CCF0 (label CTCIdleScheduler::Clear is wrong), operator delete 0xBFE9BC.
// Layout: retail CText = Ego_r.pdb minus 0x30 (landed ctor 0x54F5C0 / GetDataFromDef 0x54ED90): Value +0x15C, PFont +0x160/+0x164, Handle +0x168, HandleDropShadowBottomRight +0x170.
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
struct CCharString {
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};
struct CDefStringTable { CCharString GetString(long name); };   // 0x009D49B0
extern CDefStringTable g_DefStringTable;                    // 0x013CA828
struct CFontManager { void ReleaseFont(const CCharString& name); };   // 0x009E2B40
struct CSystemManager {
    unsigned char _pad00[0x84];
    CFontManager* FontBank;                                 // +0x84
    CFontManager* GetFontBank() const { return FontBank; }
};
CSystemManager* __fastcall GFGetSystemManager();            // 0x009A4EC0
struct CBase {
    virtual ~CBase();                                       // 0x0099A2E0
};
struct CEnginePrimitive {
    virtual void v00();
    virtual void DeleteThis();                              // slot 1
    virtual void v02();
    virtual void OnLastHandleReleased(struct CEnginePrimitiveHandle* handle);   // slot 3
    long RefCount;                                          // +0x04
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CEnginePrimitivePointer {
    CEnginePrimitive* Ptr;
    __forceinline void Clear() { if (Ptr) { Ptr->Release(); Ptr = 0; } }
    __forceinline void operator=(CEnginePrimitive* p) { Clear(); Ptr = p; }
    __forceinline ~CEnginePrimitivePointer() { Clear(); }
};
struct CEnginePrimitiveHandle : CBase {                     // vtable 0x0122F598
    CEnginePrimitivePointer Primitive;                      // +0x04
    __forceinline void Release()
    {
        if (Primitive.Ptr && Primitive.Ptr->RefCount == 1)
            Primitive.Ptr->OnLastHandleReleased(this);
        Primitive = 0;
    }
    __forceinline virtual ~CEnginePrimitiveHandle() { Release(); }
};
struct CBaseObject;
struct CCPPointerInfo {
    long RefCount;                                          // +0x00
    void (__thiscall* Deleter)(void* object);               // +0x04 hmm
    void* Object;                                           // +0x08
};
struct CFontBank;
struct CFontBankPointer {
    CFontBank* Data;
    CCPPointerInfo* Info;
    __forceinline void ReleaseRef()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->Deleter(Info->Object);
                delete Info;
            }
        }
    }
    __forceinline void Clear() { ReleaseRef(); Info = 0; Data = 0; }
    __forceinline ~CFontBankPointer() { ReleaseRef(); Data = 0; Info = 0; }
};
struct CDefString {
    long Index;
    CCharString GetString() const { return g_DefStringTable.GetString(Index); }
};
struct CRefCounted {                                        // second base of CDefBase: the count at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};
struct CUIDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x50 - 0x08];
    CDefString Font;                                        // +0x50
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CDefPointer_CUIDef {
    const CUIDef* Data;
    ~CDefPointer_CUIDef() { if (Data) const_cast<CUIDef*>(Data)->ReleaseTemporary(); }
    const CUIDef* operator->() const { return Data; }
};
namespace NUISystem {
struct IComponentA { virtual void va00(); unsigned char _state[0x10]; };
struct IComponentB { virtual void vb00(); };
struct CComponentRoot { virtual ~CComponentRoot(); };
struct CChangingStateComponent : CComponentRoot, IComponentA, IComponentB {
    unsigned char _body[0x15c - 0x1c];
    virtual ~CChangingStateComponent();                     // 0x0052CCF0
    CDefPointer_CUIDef LoadAndReturnDef() const;            // 0x00530EC0
};
struct CText : CChangingStateComponent {
    CWideString Value;                                      // +0x15c
    CFontBankPointer PFont;                                 // +0x160
    CEnginePrimitiveHandle Handle;                          // +0x168
    CEnginePrimitiveHandle HandleDropShadowBottomRight;     // +0x170
    virtual ~CText();
    virtual void va00();
    virtual void vb00();
};
CText::~CText()
{
    Handle.Release();
    HandleDropShadowBottomRight.Release();
    PFont.Clear();
    GFGetSystemManager()->GetFontBank()->ReleaseFont(LoadAndReturnDef()->Font.GetString());
}
}
