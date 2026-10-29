// CVertexShaderData::`scalar deleting destructor'(unsigned int) -- Release the declaration and shader COM objects, CBase destructor, free on bit 0
// Leaf: ??_GCVertexShaderData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A630D0, length 64 bytes.
// Callees: CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300 (restores the CBase vtable 0x01231710), operator delete @ 0x00BFE9BC;
// the two COM objects are released through IUnknown slot 2 (`push eax ; call [ecx+8]`, __stdcall with `this`).
// Vtable 0x0129DA50 = CVertexShaderData (rtti-vtables.tsv; constructor at 0x00A62FC0 in this chunk).
// Shape: the vtable store is scheduled between the first member's null test and its branch; the declaration
// (+8) is released before the shader (+4), i.e. in the destructor body in that order; the base destructor is
// out of line; the `test byte ptr [esp+8],1` free tail is the scalar deleting destructor.
// Layout as the constructor: {vptr, shader +4, declaration +8, handle +0xc, refcount +0x10}; engine header
// CVertexShader.h only describes the owner, so the data class is modelled here.
struct IUnknownRetail {
    virtual long __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                 // 0x0099A300 (out of line, restores the CBase vtable)
};

struct CVertexShaderData : CBaseClassNonCopyable {            // vtable 0x0129DA50
    IUnknownRetail* m_pShader;                // +0x04
    IUnknownRetail* m_pDeclaration;           // +0x08
    long            m_handle;                 // +0x0c
    long            m_refCount;               // +0x10
    __forceinline virtual ~CVertexShaderData() // __forceinline keeps the releases in the ??_G body (else it collapses to `call ??1`)
    {
        if (m_pDeclaration)
            m_pDeclaration->Release();
        if (m_pShader)
            m_pShader->Release();
    }
};

CVertexShaderData* CreateVertexShaderData() { return new CVertexShaderData; }   // pulls the vtable (and ??_G) into the TU