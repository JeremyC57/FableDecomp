// C3DPrimitive2::LoadBinary(CMemoryDataInputStream&, unsigned long, unsigned char**, CMovableResourceMemoryPool*) -- read the primitive header, place and load the static / animated blocks in the caller's memory block, build and fill the vertex and index buffers, then load every cloth primitive
// Grade: DIFFER(2283v2274) -- register colouring only: retail keeps the repeating-mesh rep count in edi (spilled to the dead memory-pool param slot around the params rep movsd, frame 0x34) and the cloth count in that slot; VC7.1 gives reps its own frame slot (0x38) and the count edi, and loads the Initialise pool into ecx instead of edx. Every call, store order and constant otherwise equal (masked udiff 143 lines, all register/esp-offset).
// Next idea: reuse a dead parameter as the rep count (e.g. compute reps into the memory-pool parameter after its last use) or fold reps into VertexCount*reps / IndexCount*reps temporaries so VC7.1 stops giving it a slot.
// Leaf: ?LoadBinary@C3DPrimitive2@@QAE_NAAVCMemoryDataInputStream@@KPAPAEPAVCMovableResourceMemoryPool@@@Z
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CINDEXBUFFER
#include "engine/CMemoryDataInputStream.h"
#include "engine/C3DVector.h"
#include "engine/CVertexCompressionParams.h"
#include "engine/CIndexBuffer.h"
#include "engine/CCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"

void* __cdecl operator new(unsigned int size);                    // 0x00BFEA1A
inline void* __cdecl operator new(unsigned int, void* p) { return p; }

enum EIndexBufferFormat { INDEX_BUFFER_FORMAT_16 = 0x65 };
enum EIndexBufferPool { INDEX_BUFFER_POOL_1 = 1, INDEX_BUFFER_POOL_2 = 2 };
enum EPrimitiveType { PRIMITIVE_TYPE_TRIANGLE_LIST = 4 };
enum EVertexType { };

class CMovableResourceMemoryPool;

class C3DClothPrimitive : public CBaseClassNonCopyable {
public:
    C3DClothPrimitive() { }
    virtual ~C3DClothPrimitive();
    bool LoadBinary(CMemoryDataInputStream& stream, CMovableResourceMemoryPool* pool);   // 0x00AC3BD0
    CCountedPointer<class CParticleProgram> ParticleProgram;      // +0x04
    unsigned long PrimitiveIndex;                                 // +0x0C
    unsigned long MaterialIndex;                                  // +0x10
};

extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                              _OutputIter __result, const random_access_iterator_tag&, _Distance*);   // 0x00A96430
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00A96940
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00A95750
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                      const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                              // 0x00A959E0
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*)
{                                                                                              // 0x00A95D10
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                           _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                              // 0x00A96350
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x00A97E40
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x00A97AB0
};
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
#pragma auto_inline(on)
}
template <class T> class CArray : public std::vector<T> { };

namespace N3DPrimitive {
class CTexture;
class CBaseBlock {
public:
    unsigned long PrimitiveCount;                       // +0x00
    unsigned long StartIndex;                           // +0x04
    bool          IsStrip;                              // +0x08
    bool          DegenerateTriangles;                  // +0x09
    unsigned char ChangeFlags;                          // +0x0A
    CBaseBlock() : PrimitiveCount(0), StartIndex(0), IsStrip(false), DegenerateTriangles(false), ChangeFlags(0) { }
};
class CStaticBlock : public CBaseBlock {
public:
    unsigned long DiffuseTextureFixupOffset;            // +0x0C
    unsigned long BumpTextureFixupOffset;               // +0x10
    CTexture*     DiffuseTexture;                       // +0x14
    CTexture*     BumpTexture;                          // +0x18
    long          MaterialIndex;                        // +0x1C
    CStaticBlock() : DiffuseTextureFixupOffset(0), BumpTextureFixupOffset(0), DiffuseTexture(0), BumpTexture(0), MaterialIndex(-1) { }
    void LoadBinary(CMemoryDataInputStream& stream);    // 0x00A8EA70
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    CIVCountedPointer() : Object(0) { }
};
class CAnimatedBlock : public CBaseBlock {
public:
    class CDeviceIndependent;
    unsigned long  BoneConstantsFixupOffset;            // +0x0C
    unsigned long  VertexCount;                         // +0x10
    unsigned short BonesPerVertex;                      // +0x14
    bool           PalettedFlag;                        // +0x16
    unsigned char  GroupCount;                          // +0x17
    unsigned char  Groups[18];                          // +0x18
    CIVCountedPointer<CDeviceIndependent> Data;         // +0x2C
    CAnimatedBlock() { }
    void LoadBinary(CMemoryDataInputStream& stream);    // 0x00A8EB10
};
class CDeviceIndependent;
}

template <class T> void __fastcall GFExplicitNew(T* p, unsigned long n)
{
    for (unsigned long i = 0; i < n; ++i)
        new (&p[i]) T;
}

class CVertexBufferWin32;
template <class T> class scoped_ptr {
public:
    T* Pointer;
    void reset(T* p)
    {
        T* old = Pointer;
        Pointer = p;
        if (old)
            delete old;
    }
    T* operator->() const { return Pointer; }
};

class C3DPrimitive2 : public CMovableResource {
public:
    long                                     MaterialIndex;               // +0x04
    long                                     RepeatingMeshReps;           // +0x08
    C3DVector                                BoundingCenter;              // +0x0C
    float                                    BoundingRadius;              // +0x18
    float                                    AverageTextureStretch;       // +0x1C
    CArray<CCountedPointer<C3DClothPrimitive> > ClothPrimitives;          // +0x20
    scoped_ptr<N3DPrimitive::CDeviceIndependent> Data;                    // +0x2C
    scoped_ptr<CVertexBufferWin32>           VertexBuffer;                // +0x30
    scoped_ptr<CIndexBuffer>                 IndexBuffer;                 // +0x34
    unsigned long                            VertexBufferStatus;          // +0x38
    unsigned long                            IndexBufferStatus;           // +0x3C
    unsigned long                            StaticBlockCount;            // +0x40
    N3DPrimitive::CStaticBlock*              StaticBlocks;                // +0x44
    unsigned long                            AnimatedBlockCount;          // +0x48
    N3DPrimitive::CAnimatedBlock*            AnimatedBlocks;              // +0x4C
    unsigned long                            AnimatedBlocksStatus;        // +0x50
    unsigned long                            RepeatedBlocksStatus;        // +0x54
    unsigned long                            VertexCount;                 // +0x58
    unsigned long                            TriangleCount;               // +0x5C
    unsigned long                            IndexCount;                  // +0x60
    bool                                     UploadedFlag;                // +0x64
    unsigned char                            VertexStreamFlags;           // +0x65
    unsigned long                            InitFlags;                   // +0x68

    virtual void RelocateData(long, void*, void*, long);
    static unsigned long __fastcall CalculateVertexSize(unsigned long flags, bool animated, bool repeating);   // 0x00A910E0
    bool LoadBinary(CMemoryDataInputStream& stream, unsigned long flags, unsigned char** memory, CMovableResourceMemoryPool* pool);
private:
    void SetupVertexStreamFlags(unsigned long flags, bool animated)
    {
        VertexStreamFlags = 0;
        if (animated)
            VertexStreamFlags = 1;
        if (flags & 2)
            VertexStreamFlags |= 2;
        if (flags & 0x10)
            VertexStreamFlags |= 4;
    }
    bool SetupVertexBuffer(EVertexType type, unsigned long vertexSize, unsigned long vertexCount, unsigned long flags);   // 0x00A8EDA0
    void LoadVertexBuffer(CMemoryDataInputStream& stream, unsigned long vertexSize, unsigned long fileVertexSize,
                          unsigned long fileFlags, CVertexCompressionParams& params);                                 // 0x00A91560
};

bool C3DPrimitive2::LoadBinary(CMemoryDataInputStream& stream, unsigned long flags, unsigned char** memory, CMovableResourceMemoryPool* pool)
{
    MaterialIndex = stream.ReadSLONG();
    RepeatingMeshReps = stream.ReadSLONG();
    BoundingCenter = stream.Read3DVector();
    BoundingRadius = stream.ReadFloat();
    AverageTextureStretch = stream.ReadFloat();
    StaticBlockCount = stream.ReadULONG();
    AnimatedBlockCount = stream.ReadULONG();
    VertexCount = stream.ReadULONG();
    TriangleCount = stream.ReadULONG();
    IndexCount = stream.ReadULONG();
    unsigned long fileFlags = stream.ReadULONG();
    InitFlags = fileFlags & ~4;
    SetupVertexStreamFlags(InitFlags, AnimatedBlockCount > 0);

    StaticBlockCount = stream.ReadULONG();
    AnimatedBlockCount = stream.ReadULONG();
    StaticBlocks = (N3DPrimitive::CStaticBlock*)*memory;
    GFExplicitNew(StaticBlocks, StaticBlockCount);
    *memory += StaticBlockCount * sizeof(N3DPrimitive::CStaticBlock);
    for (unsigned long i = 0; i < StaticBlockCount; ++i)
        StaticBlocks[i].LoadBinary(stream);
    AnimatedBlocks = (N3DPrimitive::CAnimatedBlock*)*memory;
    GFExplicitNew(AnimatedBlocks, AnimatedBlockCount);
    *memory += AnimatedBlockCount * sizeof(N3DPrimitive::CAnimatedBlock);
    for (unsigned long j = 0; j < AnimatedBlockCount; ++j)
        AnimatedBlocks[j].LoadBinary(stream);

    bool animated = AnimatedBlockCount > 0;
    long reps = RepeatingMeshReps;
    if (reps <= 1)
        reps = 1;
    CVertexCompressionParams params;
    stream.Read(&params, sizeof(params));
    bool repeating = false;
    if (RepeatingMeshReps > 0)
        repeating = true;
    unsigned long fileVertexSize = stream.ReadULONG();
    EVertexType vertexType = (EVertexType)stream.ReadULONG();
    unsigned long vertexSize = CalculateVertexSize(InitFlags, animated, repeating);
    if (!SetupVertexBuffer(vertexType, vertexSize, VertexCount * reps, InitFlags))
        return false;
    LoadVertexBuffer(stream, vertexSize, fileVertexSize, fileFlags, params);

    unsigned long indexBytes = IndexCount * reps * sizeof(unsigned short);
    IndexBuffer.reset(new CIndexBuffer);
    EIndexBufferPool indexPool = INDEX_BUFFER_POOL_1;
    unsigned long usage = 8;
    if (flags & 1) {
        usage = 0x18;
        indexPool = INDEX_BUFFER_POOL_2;
    }
    IndexBuffer->Initialise(indexBytes, usage, INDEX_BUFFER_FORMAT_16, indexPool, PRIMITIVE_TYPE_TRIANGLE_LIST);
    unsigned char* indices = IndexBuffer->Lock(0, 0, 0);
    stream.ReadCompressed(indices, indexBytes);
    IndexBuffer->Unlock();

    unsigned long clothCount = stream.ReadULONG();
    ClothPrimitives.resize(clothCount);
    for (unsigned long c = 0; c < clothCount; ++c) {
        CCountedPointer<C3DClothPrimitive>& cloth = ClothPrimitives[c];
        cloth.Reset(new C3DClothPrimitive);
        if (!cloth->LoadBinary(stream, pool)) {
            ClothPrimitives.clear();
            break;
        }
    }
    VertexBufferStatus = 1;
    IndexBufferStatus = 1;
    AnimatedBlocksStatus = 1;
    RepeatedBlocksStatus = 1;
    UploadedFlag = true;
    return true;
}
