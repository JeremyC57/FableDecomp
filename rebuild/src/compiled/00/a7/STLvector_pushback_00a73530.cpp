// _STL::vector::push_back(const SStringKeyedChunk& x) -- append a 16-byte (CCharString key, {name, counted chunk}) record: copy-construct it at the finish when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A73530, 105 bytes (+ int3 padding to 112), thiscall `ret 4`.
// Callers: the landed 0x00A73840 ("C3DAnimFile::AddStringKeyedChunk", the string-keyed record map's add). Callees: CCharString
//   copy constructor 0x0099EC30, vector<...>::_M_insert_overflow(pointer, const T&, const __false_type&, size_type, bool)
//   0x00A72DD0. The catalog label "Vector_Insert_ElementAt" is descriptive.
// Element: pair<CCharString, {CCharString, CCountedPointer}> (the records sorted by the 0x00A733B0 family).
// Shape: STLport push_back: `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, __false_type(), 1, true);` -- the placement-new null test, pair's explicit copy
//   ctor (key string copy, then the record's implicit copy through its own address: string copy, inline counted copy),
//   finish re-read and advanced by 10h. The empty tag is homed on x's argument slot because _M_insert_overflow has a same-TU noinline
//   body that reads x and leaves the tag unnamed (the 0x007CA350 recipe).
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

struct SCountBlock {
    long m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4
    void* m_pObject;                                    // +8
};

template <class T>
class CCountedPointer {
public:
    T* m_pObject;                                       // +0
    SCountBlock* m_pCount;                              // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
};

class C3DAnimFileRootChunk;

struct SNamedChunk {                                    // 12 bytes
    CCharString                             Name;       // +0x00
    CCountedPointer<C3DAnimFileRootChunk>   Chunk;      // +0x04
};

namespace _STL {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class T> class vector {
public:
    typedef T value_type;
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    __declspec(noinline) void _M_insert_overflow(T* __position, const T& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00A72DD0
    void push_back(const T& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

// Same-TU stand-in for the out-of-line growth path (only the target row is graded): it reads the value through
// its reference and never touches the empty tag.
template <class T>
void vector<T>::_M_insert_overflow(T* __position, const T& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    if (__x.second.Chunk.m_pObject == 0 && __atend)
        this->_M_finish = __position + __fill_len;
}
}

typedef _STL::pair<CCharString, SNamedChunk> SStringKeyedChunk;      // 0x10 bytes
template class _STL::vector<SStringKeyedChunk>;