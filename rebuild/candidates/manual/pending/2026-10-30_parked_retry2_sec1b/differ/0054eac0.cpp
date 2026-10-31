// Grade: DIFFER(241v243) (sec1b 2026-10-31, graded over the true extent 243: catalog 395 overran the tail-call jmp into the next function, BOUNDS_FIX.tsv) -- in the inlined member CEnginePrimitiveHandle dtor the Data load goes straight into ecx and is scheduled above the handle vtable store (retail: vtable store, eax load, mov ecx,eax); the body's Release() block matches (tried operator->/Get, a data local, plain inline dtor, written-out dtor)
// NUISystem::CViewport::~CViewport() -- release the engine primitive handle, detach the fake children, then the component teardown
// Leaf: ??1CViewport
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
struct CBaseClass {                                // polymorphic root (0x0099A2E0 dtor)
    virtual ~CBaseClass();                         // 0x0099A2E0
};
struct CEnginePrimitiveHandle;
struct CEnginePersistentPrimitiveBase {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3 (+0x0c)
    long RefCount;                                 // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CIVCountedPointer {      // intrusive counted pointer
    T* Data;
    void Clear() { if (Data) { Data->Release(); Data = 0; } }
    CIVCountedPointer& operator=(T* p) { Clear(); Data = p; return *this; }
    ~CIVCountedPointer() { Clear(); }
};
struct CEnginePrimitiveHandle : CBaseClass {       // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
    void Release()
    {
        if (Data.Data && Data.Data->RefCount == 1)
            Data.Data->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline ~CEnginePrimitiveHandle() { Release(); }
};
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
};
namespace NUISystem {
struct CComponentRoot { virtual ~CComponentRoot(); };
struct CInputListener { virtual void v00(); unsigned char _pad04[0x14 - 0x04]; };
struct CThirdBase { virtual void v00(); };
class CComponent : public CComponentRoot, public CInputListener, public CThirdBase {
public:
    ~CComponent();                                 // 0x0052CCF0
    V16(a) V16(b) V16(c) V16(d) V16(e) virtual void f0(); virtual void f1(); virtual void f2(); virtual void f3(); virtual void f4(); virtual void f5(); virtual void f6(); virtual void f7(); virtual void f8();
    virtual void SetPParent(CComponent* parent);   // slot 90 (+0x168)
    unsigned char _pad1c[0x15c - 0x1c];
};
class CViewport : public CComponent {
public:
    CEnginePrimitiveHandle Handle;                 // +0x15c (Ego_r.pdb +0x18c less 0x30)
    vector<CComponent*>    FakeChildren;           // +0x164
    unsigned long          GraphicIndex;           // +0x170
    virtual ~CViewport();
};

CViewport::~CViewport()
{
    Handle.Release();
    for (unsigned long i = 0; i < FakeChildren.size(); ++i)
        FakeChildren[i]->SetPParent(0);
}
}
