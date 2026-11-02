// CManagedTextureData::ReleaseTexture() -- mark the texture data unallocated and ownerless, release the volume texture and the D3D volume header, then move the resource to the LRU end of its list
// Leaf: ?ReleaseTexture@CManagedTextureData@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A69A80, 76 bytes, thiscall, tail `jmp CResource::SetAsLRU`.
// Identity (proven): Ego_r.pdb and FableWin.pdb both spell ?ReleaseTexture@CManagedTextureData@@QAEXXZ; retail keeps Ego_r's
//   CManagedTextureData function order (CreateTexture 0x00A697A0, IsBusy, CPreallocTexturePool::Allocate, the ctor 0x00A69980,
//   CreateVolumeTexture 0x00A699D0, then this row -- Ego_r: CreateTexture, IsBusy, Allocate, ctor, CreateVolumeTexture,
//   ReleaseTexture); its caller CManagedTexture::Release 0x00A69AD0 calls it on the texture data when only the pool and the
//   handle still reference it (refcount 2). The body touches exactly Ego_r's texture-side members: Allocated +0x48, Owner +0x30,
//   VolumeTexture (CCountedPointer<CVolumeTexture>) +0x28/+0x2C, D3DVolumeHeader (scoped_ptr<IDirect3DVolumeTexture9>) +0x38.
//   The catalog label agrees.
// Callees: CCPPointerInfo::DeleteFunc (__fastcall through +4), operator delete 0x00BFE9BC (the count block, then the D3D
//   volume header), CResource::SetAsLRU 0x009FC450 (Ego_r ?SetAsLRU@CResource@@QAEXXZ; tail jump).
// Shape: engine/CCountedPointer.h ReleaseData (Info re-read after the decrement; Info cleared before Data), then boost
//   scoped_ptr::reset() (`this_type(p).swap(*this)`: the header is loaded, the member zeroed, then the temporary's
//   checked_delete; a COM interface has no destructor, so `delete` is a bare operator delete with no null test); FableWin.pdb
//   names scoped_ptr<IDirect3DVolumeTexture9>'s ctor / dtor / reset / swap, all inline here.
// Layout: Ego_r.pdb CManagedTextureData : CResource (0x1C). CResource is modelled locally over engine/CIVCountedPointeeBase.h
//   with the engine/CResource.h retail members because that header does not declare SetAsLRU (header debt);
//   engine/CManagedTextureData.h is the flattened generated layout (counted pointers and scoped_ptr as raw words: header debt).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CCountedPointer.h"

namespace boost {
template <class T> inline void checked_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T)];
    delete x;
}
template <class T> class scoped_ptr {
    T* ptr;
    scoped_ptr(const scoped_ptr&);
    scoped_ptr& operator=(const scoped_ptr&);
    typedef scoped_ptr<T> this_type;
public:
    explicit scoped_ptr(T* p = 0) : ptr(p) { }
    ~scoped_ptr() { boost::checked_delete(ptr); }
    void reset(T* p = 0) { this_type(p).swap(*this); }
    void swap(scoped_ptr& b) { T* tmp = b.ptr; b.ptr = ptr; ptr = tmp; }
};
}   // namespace boost

struct IDirect3DTexture9;
struct IDirect3DVolumeTexture9 {                            // d3d9.h COM interface (no virtual destructor)
    virtual unsigned long __stdcall Release() = 0;
};
class CResourceList;
class CResource : public CIVCountedPointeeBase {            // vtable 0x012354A4, 0x1C bytes
public:
    virtual ~CResource();                                   // 0x009FC550
    void SetAsLRU();                                        // 0x009FC450
    CResourceList* ResourceList;                            // +0x08
    CResource*     PrevResource;                            // +0x0c
    CResource*     NextResource;                            // +0x10
    unsigned long  ResourceSize;                            // +0x14
    unsigned long  LastUsedFrame;                           // +0x18
};
class CTexture;
class CVolumeTexture;
class CManagedTextureOwner;

class CManagedTextureData : public CResource {              // 0x4C bytes
public:
    void ReleaseTexture();
    unsigned short                              Width;          // +0x1c
    unsigned short                              Height;         // +0x1e
    CCountedPointer<CTexture>                   Texture;        // +0x20
    CCountedPointer<CVolumeTexture>             VolumeTexture;  // +0x28
    CManagedTextureOwner*                       Owner;          // +0x30
    IDirect3DTexture9*                          D3DHeader;      // +0x34
    boost::scoped_ptr<IDirect3DVolumeTexture9>  D3DVolumeHeader;    // +0x38
    void*                                       Data;           // +0x3c
    unsigned long                               AllocSize;      // +0x40
    unsigned long                               PoolIndex;      // +0x44
    bool                                        Allocated;      // +0x48
    bool                                        Locked;         // +0x49
    bool                                        BusyFlag;       // +0x4a
};

void CManagedTextureData::ReleaseTexture()
{
    Allocated = false;
    Owner = 0;
    VolumeTexture.ReleaseData();
    D3DVolumeHeader.reset();
    SetAsLRU();
}