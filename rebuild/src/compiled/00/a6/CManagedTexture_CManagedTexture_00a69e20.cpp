// CManagedTexture::~CManagedTexture() -- when only this handle and the texture manager still hold the managed data, release it from the manager; then drop the counted reference and run ~CBaseClass
// Leaf: ??1CManagedTexture@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A69E20, 61 bytes, thiscall, tail `jmp ~CBaseClass`.
// Identity (proven): stores vtable 0x0129C880 (RTTI .?AVCManagedTexture@@ : CBaseClass, rtti-vtables.tsv) first; its
//   slot-0 `??_G` 0x00A00620 calls this body; Ego_r.pdb / FableWin.pdb ??1CManagedTexture@@UAE@XZ (Ego_r 0x00B7D740, after
//   Release 0x00B7D1E0, before Sizeof 0x00B7D7B0); the landed CGraphicInfoFrame::CFrameData dtor 0x009FE6C0 and
//   CCompositeSurfaceCache dtor 0x00B2B910 call it as ~CManagedTexture (reloc_bindings ??1CManagedTexture@@UAE@XZ). The
//   catalog label "CEnginePrimitiveHandle::Clear" is propagated.
// Callees: CManagedTexture::Release 0x00A69AD0 (?Release@CManagedTexture@@QAE_NXZ), CBaseClass::~CBaseClass 0x0099A2E0;
//   the pointee's slot-1 Delete through the vtable.
// Shape: `if (Data.GetRefCount() == 2) Release();` -- the PDB accessor ?GetRefCount@?$CIVCountedPointer@VCManagedTextureData@@@@
//   QBEKXZ is `Object ? Object->GetRefCount() : 0`, so a null pointer skips the call; then the implicit member destructor runs
//   the inline ?Release@?$CIVCountedPointer@..@@QAEXXZ, which copies Object into a local first (reading the member twice
//   reloads it after the `dec`: 64v61, chunk 656) and stores the zero only on the non-null path.
// Layout: CManagedTexture (Ego_r.pdb, 8 bytes): CBaseClass vptr +0, Data CIVCountedPointer<CManagedTextureData> +4 (Object).
//   CIVCountedPointeeBase is the retail model of engine/CIVCountedPointeeBase.h (FABLE_RETAIL_DEF_CLASS_BASE: vptr, slot 1
//   Delete, IVRefCount +4 with the inline AddRef / Release / GetRefCount accessors).
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"

class CManagedTextureData;

template <class T> class CIVCountedPointer {
public:
    ~CIVCountedPointer() { Release(); }
    unsigned long GetRefCount() const { return Object ? Object->GetRefCount() : 0; }
    void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointeeBase* Object;                       // +0x00
};

class CManagedTexture : public CBaseClass {             // vtable 0x0129C880
public:
    virtual ~CManagedTexture();
    bool Release();                                      // 0x00A69AD0
    CIVCountedPointer<CManagedTextureData> Data;         // +0x04
};

CManagedTexture::~CManagedTexture()
{
    if (Data.GetRefCount() == 2)
        Release();
}