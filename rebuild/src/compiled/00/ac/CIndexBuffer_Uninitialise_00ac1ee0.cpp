// CIndexBuffer::Uninitialise() -- release the Direct3D index buffer (its IUnknown::Release, slot 2) and clear the pointer
// Leaf: ?Uninitialise@CIndexBuffer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC1EE0, 25 bytes, thiscall.
// Identity (proven, chunk c672): called right after the CIndexBuffer RTTI vtable 0x0129EF2C store inside the inline
//   ~CIndexBuffer (0x00BABE9A); Ego_r ?Uninitialise@CIndexBuffer@@QAEXXZ.
// Layout: Ego_r.pdb CIndexBuffer PD3DIndexBuffer +0x04 (IDirect3DIndexBuffer9*; its vtable modelled to slot 2, __stdcall).
// Naming wave 16: landed as the descriptive CVolume::ReleaseVolume() (catalog CVolume::ReleaseVolume); respelled on the PDB
//   member (name-only: the bytes are unchanged).
struct IDirect3DIndexBuffer9;
struct IDirect3DIndexBuffer9Vtbl {
    void (__stdcall *QueryInterface)(IDirect3DIndexBuffer9*);
    void (__stdcall *AddRef)(IDirect3DIndexBuffer9*);
    void (__stdcall *Release)(IDirect3DIndexBuffer9*); // slot 2 at offset 0x8
};
struct IDirect3DIndexBuffer9 {
    IDirect3DIndexBuffer9Vtbl* lpVtbl;
};

class CIndexBuffer {
public:
    void Uninitialise();
    void*                  __vftable;                   // +0x00 vptr (flat, as engine/CIndexBuffer.h)
    IDirect3DIndexBuffer9* PD3DIndexBuffer;             // +0x04
};

void CIndexBuffer::Uninitialise()
{
    IDirect3DIndexBuffer9* r = this->PD3DIndexBuffer;
    if (r) {
        r->lpVtbl->Release(r);
        this->PD3DIndexBuffer = 0;
    }
}
