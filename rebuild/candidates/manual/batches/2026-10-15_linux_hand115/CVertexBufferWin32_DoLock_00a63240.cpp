// CVertexBufferWin32::DoLock(unsigned long flags, unsigned long offset, unsigned long size) -- lock `size` bytes (the whole buffer when 0) of the D3D vertex buffer after notifying the render manager; returns the mapped pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A63240, length 82 bytes.
// Callees: GFGetSystemManager @ 0x009A4EC0, the render manager's buffer notification @ 0x00A0AA70
// ((buffer, 1.0f), a `ret 8` stub), IDirect3DVertexBuffer::Lock (slot 11, +0x2c: (offset, size, &data, flags)).
// Shape: `size` in edi replaced by m_size (+8) when zero; the chain fetch and the 1.0f push as in
// Uninitialise; the Lock pushes right to left (flags = arg 1, size, the out pointer, offset = arg 2, `this`),
// with the out pointer `data` homed in the dead `size` argument slot (`lea edx,[esp+18h]`, the
// `mov [esp+24h],0` initialiser scheduled after the pushes) and returned from that slot; `ret 0Ch`.
// Layout: engine/CVertexBufferWin32.h is an opaque 0x34-byte record; +4 buffer and +8 size are modelled.
struct IDirect3DVertexBuffer {
    virtual long __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall v03(); virtual long __stdcall v04(); virtual long __stdcall v05();
    virtual long __stdcall v06(); virtual long __stdcall v07(); virtual long __stdcall v08();
    virtual long __stdcall v09(); virtual long __stdcall v10();
    virtual long __stdcall Lock(unsigned long offset, unsigned long size, void** data, unsigned long flags);   // slot 11 (+0x2c)
};

struct CRenderManagerCore {
    void VertexBufferLocked(IDirect3DVertexBuffer* buffer, float weight);   // 0x00A0AA70
};

struct CDisplayManager {                       // PDB: +4 SystemManager, +8 PRenderManager
    void*               __vftable;             // +0x00
    void*               SystemManager;         // +0x04
    CRenderManagerCore* PRenderManager;        // +0x08
};

struct CSystemManager {                        // PDB: +0x60 CCountedPointer<CDisplayManager> PDisplayManager
    unsigned char    _pad00[0x60];
    CDisplayManager* PDisplayManager;          // +0x60
    void*            PDisplayManagerCount;     // +0x64
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

struct CVertexBufferWin32 {
    virtual void v00();
    IDirect3DVertexBuffer* m_pBuffer;          // +0x04
    unsigned long          m_size;             // +0x08
    void* DoLock(unsigned long flags, unsigned long offset, unsigned long size);
};

void* CVertexBufferWin32::DoLock(unsigned long flags, unsigned long offset, unsigned long size)
{
    if (size == 0)
        size = m_size;
    CSystemManager* system = GFGetSystemManager();   // the call result in a local: evaluated before the pushes
    system->PDisplayManager->PRenderManager->VertexBufferLocked(m_pBuffer, 1.0f);
    void* data = 0;
    m_pBuffer->Lock(offset, size, &data, flags);
    return data;
}
