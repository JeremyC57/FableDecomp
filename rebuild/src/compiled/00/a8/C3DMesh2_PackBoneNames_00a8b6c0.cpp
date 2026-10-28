// C3DMesh2::PackBoneNames(const CArray<CCharString>& names) -- pack the bone names into one movable-pool block: a 16-bit offset per name followed by the null-terminated name strings (built in a memory output stream), then point BoneNameIndices / BoneNameData into the block
// Leaf: ?PackBoneNames@C3DMesh2@@AAE_NABV?$CArray@VCCharString@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8B6C0, 348 bytes, thiscall `ret 4`.
// Identity (proven): Ego_r.pdb donor order GetUsedBones 0xBF87D0, PackBoneNames 0xBF8920, RemapBoneIndices 0xBF8AA0 maps onto
//   retail 0x00A8B5F0 (landed C3DMesh2::GetUsedBones), this row, 0x00A8B820 (landed C3DMesh2::RemapBoneIndices); the body
//   fills exactly BoneNameMemoryHandle / BoneNameIndices / BoneNameData / BoneNameDataSize.
// Callees: vector<unsigned short>(n) 0x00A8D750 (same-TU noinline stand-in), CMemoryDataOutputStream ctor 0x00996AF0 / GetPosition 0x009945B0 /
//   GetLength 0x00994B40 / PeekData 0x00995E70 / dtor 0x00996B30, CDataOutputStream::WriteNullTerminatedString 0x00995AE0,
//   CMovableResourceMemoryPool::AllocateResource 0x00A62E40, CHandleBasedMemoryPool::GetPointerFromHandle 0x00A026B0,
//   free 0x00BFEA14 (the index array's destructor); memcpy inline (rep movsd / movsb).
// Layout (retail): C3DMeshStats (base, 0x7C) holds MemoryPool at +0x4; C3DMesh2 BoneNameMemoryHandle +0x84, BoneCount +0x98,
//   BoneNameIndices +0xA4, BoneNameData +0xA8, BoneNameDataSize +0xAC -- the Ego_r.pdb C3DMesh2 offsets minus 8, as the
//   landed C3DMesh2 rows model it. CMemoryDataOutputStream is modelled locally: engine/CMemoryDataOutputStream.h is a flat
//   layout without its virtual GetPosition / destructor (NOTES.md, Header debt).
// Shape: InitialiseBoneNamePointers inline (pointer setup only for a non-empty block); the index copy reads from the
//   array's first element, or from the array object itself when it is empty.
#include "engine/CMovableResource.h"
#define CHandleBasedMemoryPool CHandleBasedMemoryPoolLayout
#include "engine/CHandleBasedMemoryPool.h"
#undef CHandleBasedMemoryPool
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)

class CCharString { public: void* PStringData; };

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _Tp* _M_end_of_storage;                                // +0x08
    explicit vector(unsigned int __n);                     // 0x00A8D750 (vector<unsigned short>)
    ~vector() { if (_M_start != 0) free(_M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
    const _Tp& operator[](unsigned int __n) const { return *(_M_start + __n); }
};
// Same-TU body of 0x00A8D750 (the TU's own vector<unsigned short>(n) instance, out of line): with it VC7.1 sees that the
// constructor does not capture the array, so the array's start stays in a register across the loop's calls, as retail.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) vector<_Tp, _Alloc>::vector(unsigned int __n)
{
    _M_start = __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0;
    _M_finish = _M_start;
    _M_end_of_storage = _M_start + __n;
    for (; __n > 0; --__n, ++_M_finish)
        *_M_finish = _Tp();
}
#pragma auto_inline(on)
}
template <class T> class CArray : public std::vector<T> {
public:
    explicit CArray(unsigned long count) : std::vector<T>(count) { }
    const T* GetData() const { return this->size() > 0 ? &(*this)[0] : (const T*)this; }
};

class CDataOutputStream {                                  // retail: vptr +0, NewLine CCharString +4
public:
    virtual ~CDataOutputStream();
    CCharString NewLine;                                   // +0x04
    void WriteNullTerminatedString(const CCharString& text);   // 0x00995AE0
};
class CMemoryDataOutputStream : public CDataOutputStream { // retail 0x18 bytes
public:
    CMemoryDataOutputStream();                             // 0x00996AF0
    virtual ~CMemoryDataOutputStream();                    // 0x00996B30
    virtual unsigned long GetPosition() const;             // 0x009945B0
    unsigned long GetLength() const;                       // 0x00994B40
    const void* PeekData() const;                          // 0x00995E70
    CArray<char>  Data;                                    // +0x08
    unsigned long CurrentPosition;                         // +0x14
};

class CHandleBasedMemoryPool : public CHandleBasedMemoryPoolLayout {
public:
    void* GetPointerFromHandle(long handle);               // 0x00A026B0
};
class CMovableResourceMemoryPool : public CHandleBasedMemoryPool {
public:
    bool AllocateResource(long* handle, CMovableResource* owner, unsigned long size, unsigned long flags);   // 0x00A62E40
};

class C3DMeshStats : public CMovableResource {             // retail 0x7C bytes
public:
    CMovableResourceMemoryPool* MemoryPool;                // +0x04
    unsigned char _pad08[0x7c - 0x08];
};
class C3DMesh2 : public C3DMeshStats {
public:
    long           MemoryHandle;                           // +0x7C
    long           PrimMemoryHandle;                       // +0x80
    long           BoneNameMemoryHandle;                   // +0x84
    unsigned long  MaterialCount;                          // +0x88
    void*          Materials;                              // +0x8C
    unsigned long  PrimitiveCount;                         // +0x90
    void*          Primitives;                             // +0x94
    unsigned long  BoneCount;                              // +0x98
    void*          Bones;                                  // +0x9C
    void*          BoneKeyframes;                          // +0xA0
    unsigned short* BoneNameIndices;                       // +0xA4
    char*          BoneNameData;                           // +0xA8
    unsigned long  BoneNameDataSize;                       // +0xAC
private:
    void InitialiseBoneNamePointers(unsigned long size)
    {
        if (size != 0) {
            BoneNameIndices = (unsigned short*)MemoryPool->GetPointerFromHandle(BoneNameMemoryHandle);
            BoneNameData = (char*)(BoneNameIndices + BoneCount);
        }
    }
    bool PackBoneNames(const CArray<CCharString>& names);
};

bool C3DMesh2::PackBoneNames(const CArray<CCharString>& names)
{
    unsigned long count = names.size();
    if (count == 0)
        return true;
    CArray<unsigned short> indices(count);
    CMemoryDataOutputStream stream;
    for (unsigned long i = 0; i < count; ++i) {
        indices[i] = (unsigned short)stream.GetPosition();
        stream.WriteNullTerminatedString(names[i]);
    }
    unsigned long name_bytes = stream.GetLength();
    unsigned long index_bytes = indices.size() * sizeof(unsigned short);
    BoneNameDataSize = name_bytes;
    if (!MemoryPool->AllocateResource(&BoneNameMemoryHandle, this, index_bytes + name_bytes, 0))
        return false;
    InitialiseBoneNamePointers(index_bytes + name_bytes);
    memcpy(BoneNameIndices, indices.GetData(), index_bytes);
    memcpy(BoneNameData, stream.PeekData(), BoneNameDataSize);
    return true;
}