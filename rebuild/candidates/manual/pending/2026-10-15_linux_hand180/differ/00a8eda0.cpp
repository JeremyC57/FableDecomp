// Grade: DIFFER(166v165) -- lane gap: retail tests the flag argument in place (test byte ptr [esp+1Ch],1) where ours builds mov bl,[esp+1Ch] ; mov edx,1 ; test dl,bl, which also shifts the stride/pool register choice (retail esi/eax, ours edx/eax); the 1.0f register load sits one store earlier. tried: chained vs separate scale stores (separate fixes the store order), a buffer pointer local
// C3DPrimitive2::SetupVertexBuffer(EVertexType, unsigned long, unsigned long, unsigned long) -- replace the vertex buffer with a fresh CVertexBufferWin32 and initialise it (one stream of 8-byte, or with flag bit 0 two streams of 0x18-byte, vertex records)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8EDA0, length 165 bytes (`ret 10h`).
// GRAPHICS LAYER (replacement work later): Ego_r.pdb ?SetupVertexBuffer@C3DPrimitive2@@AAE_NW4EVertexType@@KKK@Z; the new
// object stores vtable 0x0129BA20 (rtti-vtables.tsv: CVertexBufferWin32, 0x34 bytes per the PDB).
// Callees: operator new @ 0x00BFEA1A, CBase::CBase @ 0x0099A2F0 (stores 0x0129A7C4), CVertexBufferWin32::Initialise(EVertexType,
// long, long, unsigned long, EVertexBufferPool) @ 0x00A63150 (returned), the old buffer's virtual deleting dtor (slot 0, flag 1).
// Retail offsets: C3DPrimitive2 VertexBuffer (scoped_ptr<CVertexBufferWin32>) +0x30 (PDB +0x34: the CArray before it is
// 12 bytes in retail). CVertexBufferWin32 words +8/+0xc, a bool +0x10, a scale C4DVector (1,1,1,1) +0x14 and a bias
// C4DVector (0,0,0,0) +0x24 are set by the inline ctor (all but +4/+0x10 in a novtable intermediate base, before the vtable store).
// Shape: scoped_ptr::reset (store the new pointer, then delete the old one), the vertex count clamped to >= 1, the
// stride/pool pair picked by flag bit 0, then the tail call's bool result.
// Leaf: SetupVertexBuffer@C3DPrimitive2
struct CBase {
    CBase();                                        // 0x0099A2F0
    virtual ~CBase();
};
struct __declspec(novtable) CVertexBufferBase : CBase {
    CVertexBufferBase() : m_08(0), m_0c(0)
    {
        ScaleX = 1.0f; ScaleY = 1.0f; ScaleZ = 1.0f; ScaleW = 1.0f;
        BiasX = 0; BiasY = 0; BiasZ = 0; BiasW = 0;
    }
    void* m_04;                                     // +0x04 the device buffer
    long m_08;                                      // +0x08
    long m_0c;                                      // +0x0c
    bool m_locked;                                  // +0x10
    float ScaleX, ScaleY, ScaleZ, ScaleW;           // +0x14
    long BiasX, BiasY, BiasZ, BiasW;                // +0x24
};
struct CVertexBufferWin32 : CVertexBufferBase {
    CVertexBufferWin32() { m_04 = 0; m_locked = false; }
    virtual ~CVertexBufferWin32();
    bool Initialise(long vertexType, long vertexCount, long usage, unsigned long stride, long pool);   // 0x00A63150
};

template<class T> struct scoped_ptr {
    T* ptr;
    void reset(T* p) { T* old = ptr; ptr = p; if (old) delete old; }
    T* operator->() const { return ptr; }
};

struct C3DPrimitive2 {
    unsigned char _pad[0x30];
    scoped_ptr<CVertexBufferWin32> VertexBuffer;    // +0x30
    bool SetupVertexBuffer(long vertexType, unsigned long usage, unsigned long vertexCount, unsigned long flags);
};

bool C3DPrimitive2::SetupVertexBuffer(long vertexType, unsigned long usage, unsigned long vertexCount, unsigned long flags)
{
    VertexBuffer.reset(new CVertexBufferWin32);
    CVertexBufferWin32* buffer = VertexBuffer.ptr;
    if (vertexCount < 1)
        vertexCount = 1;
    unsigned long stride = 8;
    long pool = 1;
    if (flags & 1) {
        stride = 0x18;
        pool = 2;
    }
    return buffer->Initialise(vertexType, vertexCount, usage, stride, pool);
}
