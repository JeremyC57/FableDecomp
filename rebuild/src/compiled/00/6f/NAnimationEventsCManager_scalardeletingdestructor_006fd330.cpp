// NAnimationEvents::CManager::`scalar deleting destructor'(unsigned int flags) -- destroy the extra-events map vector and the event symbol map, then free
// Leaf: ??_GCManager@NAnimationEvents@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FD330, 65 bytes, `ret 4`.
// Label correction: catalog "CPatchTesselationEdgeStrip_dtor" is propagated; this is the scalar deleting destructor of
//   NAnimationEvents::CManager.
// Callees: __destroy_aux 0x006FB8A0 (separate row; same-TU noinline body so the empty tag shares the live flags slot),
//   free 0x00BFEA14, ~CSymbolMap 0x006FD040, operator delete 0x00BFE9BC.
// Shape: the class declares no destructor (compiler-generated: no vtable reset); members die in reverse (the extra
//   events vector, then the symbol map); `??_G` is emitted for a `delete` helper.
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C
void __cdecl operator delete(void* block);     // MSVCR71 import thunk 0x00BFE9BC
extern "C" void __cdecl free(void* block);     // MSVCR71 import thunk 0x00BFEA14

namespace NAnimationEvents { struct CEvent { long m_a, m_b; }; }

template<class T, int N> struct CSmallVector {
    T* Start;                                   // +0x00
    int Size;                                   // +0x04
    void Copy(const CSmallVector& other);       // 0x006FB5C0
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
    void Clear()
    {
        delete[] Start;
        Start = 0;
        Size = 0;
    }
    ~CSmallVector() { Clear(); }
};

template<class K, class V> struct pair {
    K first;
    V second;
};

typedef pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > SKeyedEvents;

struct __false_type { __false_type() {} };

template<class T> inline void _Destroy(T* pointer) { pointer->~T(); }

#pragma auto_inline(off)
// 0x006FB8A0 (separate row; same-TU body so the empty tag can share the live flags slot)
template<class ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(ForwardIterator first, ForwardIterator last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
#pragma auto_inline(on)

template<class ForwardIterator> inline void _Destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy_aux(first, last, __false_type());
}

template<class T> class allocator { public: allocator() {} };

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

template<class K, class V> struct CKeyPairCompareLess { };

template<class K, class V, class Compare = CKeyPairCompareLess<K, V> > class CVectorMap {
public:
    vector<pair<K, V> > Map;                    // +0x00
    Compare             Comp;                   // +0x0c
    bool                Dirty;                  // +0x0d
};

class CMeshDataBank;
class CSymbolMap {                              // retail 0x24 bytes
public:
    unsigned char _body[0x24];
    ~CSymbolMap();                              // 0x006FD040
};

namespace NAnimationEvents {
class CManager {
public:
    virtual void GetEvents(unsigned long anim, void* events) const;
    const CMeshDataBank* Bank;                                                   // +0x04
    CSymbolMap EventSymbols;                                                     // +0x08
    CVectorMap<unsigned long, CSmallVector<CEvent, 32> > AnimToExtraEventsMap;   // +0x2c
    bool Dirty;                                                                  // +0x3c
    bool UseEmbeddedEvents;                                                      // +0x3d
};
}

void DeleteManager(NAnimationEvents::CManager* manager) { delete manager; }