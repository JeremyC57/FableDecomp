#ifndef FABLE_ENGINE_CVERTEXBUFFERBASE_H
#define FABLE_ENGINE_CVERTEXBUFFERBASE_H
// Hand-written (chunk 723): CVertexBufferBase : CBaseClassNonCopyable : CBaseClass (RTTI .?AVCVertexBufferWin32@@ chain). Ego_r.pdb
// members (0x34 bytes, no containers, retail = Ego_r): PVertexBuffer +0x04, Length +0x08, Type +0x0C, Stride +0x10 (landed
// renderers read the stride byte at +0x10), CompressionParams +0x14 (CVertexCompressionParams, 0x20 bytes, opaque here). The four
// Do* virtuals (slots 1..4) are pure here and private in CVertexBufferWin32. The destructor is the implicit one (Ego_r.pdb
// ??1CVertexBufferBase@@UAE@XZ is its virtual COMDAT): expanded owners store no CVertexBufferBase vtable (0x00B71880).
// Ctor: Ego_r.pdb ??0CVertexBufferBase@@IAE@XZ (protected).
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CVertexCompressionParams.h"
struct IDirect3DVertexBuffer9;
enum EVertexType;                                                        // MS forward enum
class CVertexBufferBase : public CBaseClassNonCopyable {
protected:
    CVertexBufferBase()                                                  // ??0CVertexBufferBase@@IAE@XZ (inline, 0x00BC14E0)
        : Length(0), Type((EVertexType)0) { }
private:
    virtual unsigned char* DoLock(unsigned long offset, unsigned long size, unsigned long flags) = 0;   // slot 1
    virtual void DoUnlock() = 0;                                         // slot 2
    virtual bool DoIsBusy() const = 0;                                   // slot 3
    virtual unsigned long DoSizeof() const = 0;                          // slot 4
public:
    IDirect3DVertexBuffer9* GetD3DVertexBuffer();                        // ?GetD3DVertexBuffer@CVertexBufferBase@@QAEPAUIDirect3DVertexBuffer9@@XZ
    unsigned char* Lock(unsigned long offset, unsigned long flags, unsigned long size);   // 0x00AC20A0 ?Lock@CVertexBufferBase@@QAEPAEKKK@Z (DoLock)
    void Unlock();                                                       // 0x00AC20B0 ?Unlock@CVertexBufferBase@@QAEXXZ (DoUnlock)
    long GetSize() const { return Length; }                              // ?GetSize@CVertexBufferBase@@QBEJXZ, inline (no out-of-line
                                                                         // copy; 0x00B6EDF0 reads Length at +0x08)
    long GetStride() const;                                              // ?GetStride@CVertexBufferBase@@QBEJXZ
    IDirect3DVertexBuffer9* PVertexBuffer;                               // +0x04
    unsigned long           Length;                                      // +0x08
    EVertexType             Type;                                        // +0x0C
    unsigned char           Stride;                                      // +0x10
    CVertexCompressionParams CompressionParams;                          // +0x14
};
FABLE_STATIC_ASSERT(sizeof(CVertexBufferBase) == 0x34);

#endif // FABLE_ENGINE_CVERTEXBUFFERBASE_H
