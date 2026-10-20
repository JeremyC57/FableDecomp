// CPreallocTexturePool::Initialise(long poolIndex, unsigned long maxSize, unsigned long count) -- size the pool: remember the pool index / max size / count, make room for count texture slots, and fill each with a new managed texture (fresh texture holder, the pool's size and index) registered in the pool's resource list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6A540, length 290 bytes, `ret 0xc`.
// Callees: vector<CIVCountedPointer<CManagedTextureData>>::resize(n, const value&) 0x00A6C020 (propagated label
// "vector<unsigned int>::resize", a byte twin), operator new 0x00BFEA1A (0x4c-byte texture data, 8-byte texture),
// CResource::CResource 0x009FBEC0 (CManagedTextureData vtable 0x0129DC24 stored after it), the old texture data's
// virtual slot 1 (the intrusive release), CCountedPointer<CTexture>::Reset(CTexture*) 0x005669C0 (propagated label),
// CResourceList::AddResource(CResource*) 0x009FBFE0.
// Layout: Ego_r.pdb CPreallocTexturePool (Count +0x3c, MaxSize +0x40, PoolIndex +0x44, Textures +0x48) and
// CManagedTextureData (76 bytes: Width +0x1c, Height +0x1e, Texture +0x20, VolumeTexture +0x28, Owner +0x30, D3DHeader
// +0x34, D3DVolumeHeader +0x38, Data +0x3c, AllocSize +0x40, PoolIndex +0x44, Allocated/Locked/BusyFlag +0x48..+0x4a).
// Shape: the managed-texture and texture constructors are __forceinline inside their new-expressions (every zero
// through ebx); the slot's intrusive operator=(T*) releases the old object through the pointee's inline Release()
// (`dec [ecx+4] ; jne`; an open-coded decrement re-reads the count) and stores a zero
// before the new pointer; the loop index and `this` live in frame words (the resize's default value shares the index's).
void* operator new(unsigned int size);                 // 0x00BFEA1A

struct CResourceList {
    void AddResource(struct CResource* resource);      // 0x009FBFE0
};

struct CResource {                                     // 0x1c bytes
    virtual void v00();
    virtual void DeleteThis();                         // slot 1 (+0x04)
    long RefCount;                                     // +0x04
    unsigned char _pad08[0x14];
    CResource();                                       // 0x009FBEC0
    void Release()
    {
        if (--RefCount == 0)
            DeleteThis();
    }
};

struct CTexture {                                      // 8 bytes
    void* m_pFirst;
    void* m_pSecond;
    __forceinline CTexture() : m_pFirst(0), m_pSecond(0) { }
};
struct CVolumeTexture;
template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
    void Reset(T* object);                             // CCountedPointer<CTexture>: 0x005669C0
};
template <class T> struct scoped_ptr {
    T* ptr;
    scoped_ptr() : ptr(0) { }
};
struct CManagedTextureOwner;
struct IDirect3DTexture9;
struct IDirect3DVolumeTexture9;

struct CManagedTextureData : CResource {               // vtable 0x0129DC24
    unsigned short                  Width;             // +0x1c
    unsigned short                  Height;            // +0x1e
    CCountedPointer<CTexture>       Texture;           // +0x20
    CCountedPointer<CVolumeTexture> VolumeTexture;     // +0x28
    CManagedTextureOwner*           Owner;             // +0x30
    IDirect3DTexture9*              D3DHeader;         // +0x34
    scoped_ptr<IDirect3DVolumeTexture9> D3DVolumeHeader;   // +0x38
    void*                           Data;              // +0x3c
    unsigned long                   AllocSize;         // +0x40
    unsigned long                   PoolIndex;         // +0x44
    bool                            Allocated;         // +0x48
    bool                            Locked;            // +0x49
    bool                            BusyFlag;          // +0x4a
    __forceinline CManagedTextureData()
        : Width(0), Height(0), Owner(0), D3DHeader(0), Data(0), AllocSize(0), PoolIndex(0),
          Allocated(false), Locked(false), BusyFlag(false) { }
    virtual void v00();
};

template <class T> struct CIVCountedPointer {          // intrusive: the object carries the count
    T* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer& operator=(T* object)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = object;
        return *this;
    }
};
template <class T> struct CArray {                     // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void resize(unsigned int n, const T& x);           // 0x00A6C020
    T& operator[](unsigned long n) { return *(_M_start + n); }
};

struct CPreallocTexturePool : CResourceList {
    void*         vfptr;                               // +0x00
    unsigned char _pad04[0x38];                        // the resource-list base
    unsigned long Count;                               // +0x3c
    unsigned long MaxSize;                             // +0x40
    long          PoolIndex;                           // +0x44
    CArray<CIVCountedPointer<CManagedTextureData> > Textures;   // +0x48
    void Initialise(long poolIndex, unsigned long maxSize, unsigned long count);
};

void CPreallocTexturePool::Initialise(long poolIndex, unsigned long maxSize, unsigned long count)
{
    PoolIndex = poolIndex;
    MaxSize = maxSize;
    Count = count;
    Textures.resize(count, CIVCountedPointer<CManagedTextureData>());
    for (unsigned long i = 0; i < count; ++i) {
        Textures[i] = new CManagedTextureData();
        CManagedTextureData* texture = Textures[i].Object;
        texture->Texture.Reset(new CTexture());
        texture->AllocSize = maxSize;
        texture->PoolIndex = poolIndex;
        AddResource(Textures[i].Object);
    }
}