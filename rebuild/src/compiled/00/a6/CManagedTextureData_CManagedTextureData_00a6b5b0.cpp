// CManagedTextureData::~CManagedTextureData() -- compiler-generated destructor: delete the D3D volume-texture header (boost::scoped_ptr), release the VolumeTexture and Texture counted pointers, then ~CResource
// Leaf: ??1CManagedTextureData@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6B5B0, length 107 bytes, tail jump to ~CResource.
// Identity (proven): the only caller is the scalar deleting destructor 0x00A6B590 = slot 0 of vtable 0x0129DC24
//   (RTTI .?AVCManagedTextureData@@); Ego_r.pdb / FableWin.pdb ??1CManagedTextureData@@UAE@XZ. Label correction: the catalog's
//   "C3DMeshStripifier::~C3DMeshStripifier" is propagated (the class has no destructor at this address).
// Callees: operator delete @ 0x00BFE9BC (the scoped D3D header, then each count block), CResource::~CResource @ 0x009FC550
//   (Ego_r ??1CResource@@UAE@XZ; tail jump); the count block's __fastcall DeleteFunc (+0x04, with its Data +0x08).
// Layout (Ego_r.pdb CManagedTextureData, 0x4C, no STL members so the offsets hold in retail): CResource base 0x1C, Width +0x1C,
//   Height +0x1E, Texture CCountedPointer<CTexture> +0x20, VolumeTexture CCountedPointer<CVolumeTexture> +0x28, Owner +0x30,
//   D3DHeader +0x34, D3DVolumeHeader boost::scoped_ptr<IDirect3DVolumeTexture9> +0x38, Data +0x3C, AllocSize +0x40,
//   PoolIndex +0x44, Allocated / Locked / BusyFlag +0x48..+0x4A.
// Shape: no vtable store = a compiler-generated destructor; the members in reverse order: scoped_ptr's checked_delete of a
//   trivially-destroyed COM interface is a bare `operator delete` (no null test), then the two engine/CCountedPointer.h
//   destructors (Data then Info zeroed; one shared zero register). The implicit ??1 is emitted the way the retail TU emits it:
//   the real out-of-line constructor (retail 0x00A69980) is defined here, which emits the vtable, ??_G and the implicit ??1.
//   engine/CManagedTextureData.h is a flattened layout (vptr as a data member, no CResource base): local model (header debt).
#include "rebuild_abi.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CCountedPointer.h"

void __cdecl operator delete(void* block);                  // 0x00BFE9BC

namespace boost {
template <class T> inline void checked_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
    (void) sizeof(type_must_be_complete);
    delete x;
}
class noncopyable {
protected:
    noncopyable() { }
    ~noncopyable() { }
private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};
template <class T> class scoped_ptr : noncopyable { public:
    explicit scoped_ptr(T* p = 0) : ptr(p) { }
    ~scoped_ptr() { boost::checked_delete(ptr); }
private:
    T* ptr;
};
} // namespace boost

struct IDirect3DTexture9;
struct IDirect3DVolumeTexture9 {                            // COM interface (d3d9.h): no virtual destructor
    virtual long __stdcall QueryInterface(const void* riid, void** object) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
class CTexture;
class CVolumeTexture;
class CManagedTextureOwner;

class CResource : public CIVCountedPointeeBase { public:   // 0x1C bytes
    CResource();                                            // 0x009FBEC0
    virtual ~CResource();                                   // 0x009FC550
    unsigned char _pad08[0x1C - 0x08];
};

class CManagedTextureData : public CResource { public:      // vtable 0x0129DC24
    CManagedTextureData();
    unsigned short                             Width;               // +0x1C
    unsigned short                             Height;              // +0x1E
    CCountedPointer<CTexture>                  Texture;             // +0x20
    CCountedPointer<CVolumeTexture>            VolumeTexture;       // +0x28
    CManagedTextureOwner*                      Owner;               // +0x30
    IDirect3DTexture9*                         D3DHeader;           // +0x34
    boost::scoped_ptr<IDirect3DVolumeTexture9> D3DVolumeHeader;     // +0x38
    void*                                      Data;                // +0x3C
    unsigned long                              AllocSize;           // +0x40
    unsigned long                              PoolIndex;           // +0x44
    bool                                       Allocated;           // +0x48
    bool                                       Locked;              // +0x49
    bool                                       BusyFlag;            // +0x4A
};

CManagedTextureData::CManagedTextureData()
    : Width(0), Height(0), Owner(0), D3DHeader(0), Data(0), AllocSize(0), PoolIndex(0), Allocated(false), Locked(false), BusyFlag(false)
{
}