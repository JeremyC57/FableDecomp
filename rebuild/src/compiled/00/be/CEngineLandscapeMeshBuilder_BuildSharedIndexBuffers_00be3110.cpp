// CEngineLandscapeMeshBuilder::BuildSharedIndexBuffers() -- for every power-of-two patch size 2..16 x 2..16 build the index list into a temporary CArray, copy it into a new 16-bit triangle-strip index buffer and keep it in SharedIndexBuffers[x][y]
// Leaf: ?BuildSharedIndexBuffers@CEngineLandscapeMeshBuilder@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE3110, 386 bytes, thiscall, plain `ret`; 8-aligned frame (the C2DCoordI[289] local).
// Identity: Ego_r.pdb ?BuildSharedIndexBuffers@CEngineLandscapeMeshBuilder@@AAEXXZ (private); its one callee is BuildSharedIndexBufferGenerationTables 0x00BE2CD0
//   (Ego_r ?BuildSharedIndexBufferGenerationTables@CEngineLandscapeMeshBuilder@@QAEXPAVC2DCoordI@@PAG1JJAAJ@Z).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport vector storage), BuildSharedIndexBufferGenerationTables 0x00BE2CD0
//   (same-TU stand-in body below), operator new 0x00BFEA1A, CEngineResourceManager::GetIndexBufferUsage 0x00B3B060 /
//   GetIndexBufferMemPool 0x00B3B0A0, CIndexBuffer::CIndexBuffer 0x00AC2010 / Lock 0x00AC1FB0 / Unlock 0x00AC1F00; memcpy
//   intrinsic; g_pResourceManager 0x01436E98.
// Stand-in: BuildSharedIndexBufferGenerationTables is a same-TU noinline stand-in for the real retail callee (its writes through
//   the index pointer keep VC7.1 from dropping the vector's zero-fill stores); it is not part of this row.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __x;
    return __first;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    explicit vector(size_type __n) : _Vector_base<_Tp, _Alloc>(__n, _Alloc())
    {
        this->_M_finish = __uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
};
}
template <class T> class CArray : public std::vector<T> {
public:
    explicit CArray(unsigned long size) : std::vector<T>(size) { }
    T* GetAsCArray() { return this->size() > 0 ? this->_M_start : GetZeroSizeArray(); }
private:
    T* GetZeroSizeArray() { return reinterpret_cast<T*>(this); }
};

class C2DCoordI { public: long X, Y; };
enum EIndexBufferFormat { };
enum EIndexBufferPool { };
enum EPrimitiveType { };
class CIndexBuffer { public:
    CIndexBuffer(unsigned long size, unsigned long usage, EIndexBufferFormat format, EIndexBufferPool pool, EPrimitiveType type);   // 0x00AC2010
    unsigned char* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC1FB0
    void Unlock();                                                                        // 0x00AC1F00
    void* __vftable;
    unsigned char _pad04[0x10];
};
class CEngineResourceManager { public:
    unsigned long GetIndexBufferUsage();               // 0x00B3B060
    EIndexBufferPool GetIndexBufferMemPool();          // 0x00B3B0A0
};
extern CEngineResourceManager* g_pResourceManager;     // 0x01436E98

class CEngineLandscapeMeshBuilder { public:
    void*         __vftable;
    CIndexBuffer* SharedIndexBuffers[17][17];          // +0x04
    void BuildSharedIndexBufferGenerationTables(C2DCoordI* coords, unsigned short* indices, unsigned short* indices2, long sizeX, long sizeY, long& count);   // 0x00BE2CD0
private:
    void BuildSharedIndexBuffers();                     // Ego_r.pdb: private (AAE)
};

#pragma auto_inline(off)
__declspec(noinline) void CEngineLandscapeMeshBuilder::BuildSharedIndexBufferGenerationTables(C2DCoordI* coords, unsigned short* indices, unsigned short* indices2, long sizeX, long sizeY, long& count)
{
    indices[0] = 0;
    count = sizeX * sizeY;
}
#pragma auto_inline(on)

void CEngineLandscapeMeshBuilder::BuildSharedIndexBuffers()
{
    for (long x = 2; x <= 16; x *= 2) {
        for (long y = 2; y <= 16; y *= 2) {
            CArray<unsigned short> indices(x * y * 12);
            C2DCoordI coords[289];
            long count = 0;
            BuildSharedIndexBufferGenerationTables(coords, indices.GetAsCArray(), 0, x, y, count);
            CIndexBuffer* buffer = new CIndexBuffer(count * 2, g_pResourceManager->GetIndexBufferUsage(), (EIndexBufferFormat)0x65,
                                                    g_pResourceManager->GetIndexBufferMemPool(), (EPrimitiveType)5);
            unsigned char* data = buffer->Lock(0, 0, 0);
            memcpy(data, indices.GetAsCArray(), count * 2);
            buffer->Unlock();
            SharedIndexBuffers[x][y] = buffer;
        }
    }
}