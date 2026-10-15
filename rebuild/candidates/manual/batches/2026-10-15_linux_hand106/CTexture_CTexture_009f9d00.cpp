// CTexture::CTexture(const CTexture& other) -- copy constructor: share the other texture's Direct3D texture, copy the 28-bit ByteLength and the 4-bit AllocationSource, and AddRef the shared COM texture
// Retail: Fable.exe @ 0x009F9D00, 68 bytes (+ int3 padding). Callee: IDirect3DTexture9::AddRef (COM slot 1, `call [ecx+4]`, `this` pushed).
// Layout: PDB CTexture (8 bytes): PD3DTexture +0, then `unsigned long ByteLength : 28, AllocationSource : 4` at +4.
// engine/CTexture.h flattens the bitfield word into byte arrays, which cannot express the masked copies, so the
// class is modelled locally with the PDB names and bit widths.
// Shape: each bitfield copy is VC7.1's `((src ^ dst) & mask) ^ dst` merge -- ByteLength with the 0x0fffffff mask
// against the old word, then AllocationSource the same mask the other way round (`(src ^ new) & 0x0fffffff ^ src`);
// the texture pointer is re-read from the member for the AddRef null test.
struct IDirect3DTexture9 {                         // IUnknown slots of the COM texture
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1 (+0x04)
    virtual unsigned long __stdcall Release();                               // slot 2 (+0x08)
};

class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLength : 28;            // +0x04 bits 0..27
    unsigned long      AllocationSource : 4;       // +0x04 bits 28..31
    CTexture(const CTexture& other);
};

CTexture::CTexture(const CTexture& other)
{
    PD3DTexture = other.PD3DTexture;
    ByteLength = other.ByteLength;
    AllocationSource = other.AllocationSource;
    if (PD3DTexture)
        PD3DTexture->AddRef();
}
