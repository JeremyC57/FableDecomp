// std::vector::push_back(const CLevelFiles& x) -- append a 28-byte level-file record (three counted file pointers) or grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@VCLevelFiles@CObject@CLevelLoader@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051AE60, 106 bytes, thiscall, `ret 4`.
// Identity: vector<CLevelLoader::CObject::CLevelFiles> (FableWin.pdb has ?push_back@?$vector@VCLevelFiles@CObject@
//   CLevelLoader@@...); callers CWorldMap::LoadThingsWithSectionNames (0x00500F93), CWorldMap::UpdateNavMaps
//   (0x00501AF9) and 0x0051B641. Ego_r.pdb CLevelFiles (28 bytes): long LevelID, CCountedPointer<CAFile>
//   PTopologyFile / PNavigationFile / PThingFile (Data +0, CCPPointerInfo* Info +4, Info->RefCount at +0).
//   Catalog name "vector<CLevelLoader::CObject::CLevelFiles>::push_back" confirmed.
// Callees: _M_insert_overflow(pointer, const CLevelFiles&, const __false_type&, size_type, bool) @ 0x0051A430.
// Shape: STLport 4.6 `push_back(x) { if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x);
//   ++_M_finish; } else _M_insert_overflow(_M_finish, x, __false_type(), 1, true); }`. `_Construct` must be
//   `__forceinline` (else VC7.1 calls it): placement new with its null test, then the implicit copy constructor
//   (each CCountedPointer copy bumps the shared count when Info is non-null), `add dword ptr [ecx+4],1Ch`. The
//   __false_type temporary is homed in x's own argument slot (`lea edx,[esp+0Ch]` -- x is still read from it).
inline void* operator new(unsigned int, void* __p) { return __p; }

class CAFile;

class CCPPointerInfo { public:                       // 12 bytes
    unsigned long RefCount;                   // +0x00
    void (__cdecl *DeleteFunc)(void*);        // +0x04
    void* Data;                               // +0x08
};

template <class T> class CCountedPointer {    // 8 bytes
public:
    T* Data;                                  // +0x00
    CCPPointerInfo* Info;                     // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};

class CLevelLoader {
public:
    class CObject {
    public:
        class CLevelFiles {                   // 28 bytes
        public:
            long LevelID;                     // +0x00
            CCountedPointer<CAFile> PTopologyFile;    // +0x04
            CCountedPointer<CAFile> PNavigationFile;  // +0x0C
            CCountedPointer<CAFile> PThingFile;       // +0x14
        };
    };
};
namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2>
__forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
    void push_back(const _Tp& __x) {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template void vector<CLevelLoader::CObject::CLevelFiles>::push_back(const CLevelLoader::CObject::CLevelFiles&);
}