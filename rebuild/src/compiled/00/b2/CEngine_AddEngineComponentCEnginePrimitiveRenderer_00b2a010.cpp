// CEngine::AddEngineComponent<CEnginePrimitiveRenderer>(CEnginePrimitiveRenderer*, CEnginePrimitiveRenderer*&, unsigned char) -- report progress ("Engine: Add Engine Component"), publish the new component through its global pointer, append it to the engine's component list and let it announce itself
// Leaf: AddEngineComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2A010, 163 bytes (+ int3 padding). Mangled (PDB family):
//   ??$AddEngineComponent@V<T>@@@CEngine@@AAEXPAV<T>@@AAPAV1@E@Z (private; component, reference to the global pointer, an
//   unsigned char flag this instantiation never reads).
// Label correction: the catalog says AddEngineComponent<CEngineLightingManager>; the single caller (CEngine ctor at
//   0x00B2671E) passes `new` 0x350 bytes built by 0x00B33B50, which stores vtables 0x012A1380 / 0x012A1348 =
//   CEnginePrimitiveRenderer (RTTI), and the global 0x01436E80; so this is the CEnginePrimitiveRenderer instantiation.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, DisplayProgress 0x009E9F40 (`__fastcall`: text in ecx,
//   the first bool in dl, the float and the second bool on the stack), ~CCharString 0x0099EAE0,
//   CEngineComponent::SetGlobalPointerAddress 0x00B59750 (`GlobalPointerAddress = p`, +4), STLport
//   vector<CEngineComponent*>::_M_insert_overflow 0x00B292B0, CEngineComponent 0x00B59760 (calls its own virtual slot 9 for a
//   CCharString and drops it; named Announce here).
// Layout: the retail CEnginePrimitiveRenderer is 0x350 bytes with its CEngineComponent base (vtable 0x012A1348;
//   PDB CEngineComponent {vptr, void** GlobalPointerAddress +4, bool RenderEnabled +8, uchar KeepAlive +9}) at +0x268;
//   CEngine's EngineComponents vector sits at +0x168 in retail (engine/CEngine.h +0x178: retail STLport vectors are 12 bytes,
//   the PDB's 16), so `this` is a local model.
// Shape: the progress text is a named CCharString in its own block; the push_back argument is the null-checked T* ->
//   CEngineComponent* conversion temporary (homed in the dead `component` slot, `neg ; sbb ; and`), the __true_type tag
//   also addressed there; the member calls go through the unchecked base address kept in edi.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);        // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
void* FindRef_free_DisplayProgress_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b)
{
    void* p = FindRef_free_DisplayProgress_1();
    if (!p)
        return;
    *(void**)&text = p;
    
}
#pragma auto_inline(on)

inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() { } };

struct CEngineComponent {                              // vtable 0x012A2544 (as its own object)
    virtual void v00();
    void**        GlobalPointerAddress;                // +0x04
    bool          RenderEnabled;                       // +0x08
    unsigned char KeepAlive;                           // +0x09
    void SetGlobalPointerAddress(void** address);      // 0x00B59750
    void Announce();                                   // 0x00B59760
};

struct CEnginePrimitiveRendererBody {                  // primary part (vtable 0x012A1380)
    virtual void v00();
    unsigned char _body[0x264];
};
struct CEnginePrimitiveRenderer : CEnginePrimitiveRendererBody, CEngineComponent {   // 0x350 bytes in retail
    unsigned char _tail[0x350 - 0x274];
};

struct CEngineComponentVector {                        // STLport vector<CEngineComponent*> (12 bytes)
    CEngineComponent** _M_start;
    CEngineComponent** _M_finish;
    CEngineComponent** _M_end_of_storage;
    void _M_insert_overflow(CEngineComponent** position, CEngineComponent* const& x, const __true_type&, unsigned int fill, bool atEnd);   // 0x00B292B0
    void push_back(CEngineComponent* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CEngineComponent*(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
void* FindRef_CEngineComponentVector__M_insert_overflow_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CEngineComponentVector::_M_insert_overflow(CEngineComponent** position, CEngineComponent* const& x, const __true_type& p2, unsigned int fill, bool atEnd)
{
    void* p = FindRef_CEngineComponentVector__M_insert_overflow_0();
    if (!p)
        return;
    if (*(void**)&x != p)
        return;
}
#pragma auto_inline(on)

struct CEngine {
    unsigned char          _pad000[0x168];
    CEngineComponentVector EngineComponents;           // +0x168 (retail; PDB +0x178)
    template <class T> void AddEngineComponent(T* component, T*& globalPointer, unsigned char flag);
};

template <class T> void CEngine::AddEngineComponent(T* component, T*& globalPointer, unsigned char flag)
{
    {
        CCharString text("Engine: Add Engine Component", -1);
        DisplayProgress(text, -1.0f, false, false);
    }
    globalPointer = component;
    component->SetGlobalPointerAddress(reinterpret_cast<void**>(&globalPointer));
    EngineComponents.push_back(component);
    component->Announce();
}

template void CEngine::AddEngineComponent<CEnginePrimitiveRenderer>(CEnginePrimitiveRenderer*, CEnginePrimitiveRenderer*&, unsigned char);