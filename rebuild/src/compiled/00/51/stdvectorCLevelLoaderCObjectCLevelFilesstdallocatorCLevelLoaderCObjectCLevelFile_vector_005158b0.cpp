// std::vector<CLevelLoader::CObject::CLevelFiles,std::allocator<CLevelLoader::CObject::CLevelFiles> >::vector(unsigned int n) -- STLport vector(n) of the level loader's 28-byte level-file records: allocate n records, copy-construct each from a default record, destroy the default record
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005158B0, 125 bytes, `ret 4` (thiscall: n). No direct caller (an inlined use
//   elsewhere); the catalog's "StdVector_AllocateConstructPairScriptBase" is descriptive and the element is not a
//   script pair.
// Element: Ego_r.pdb CLevelLoader::CObject::CLevelFiles, 0x1C bytes {long LevelID +0; CCountedPointer<CAFile>
//   PTopologyFile +4, PNavigationFile +0xC, PThingFile +0x14} -- as the landed CLevelFilesVector::PushBackEmpty
//   0x0051B620 and ~CLevelFiles 0x0050F980: the implicit default constructor leaves LevelID unset and the three
//   counted pointers' inline constructors zero their six words from the shared zero register.
// Callees: malloc @ 0x00BFEA0E; __uninitialized_fill_n<CLevelFiles*, unsigned int, CLevelFiles>(first, n, x, const
//   __false_type&) @ 0x00513570 (`__fastcall`, ret 8; manifest "_Uninit_fill_n<pair<unsigned __int64,
//   CCountedPointer<CActiveEntityScriptBase>>*..>" is propagated); CLevelFiles::~CLevelFiles @ 0x0050F980.
// Shape: STLport `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish =
//   uninitialized_fill_n(_M_start, n, _Tp()); }`; the default record fills the 0x1C-byte frame, its zero stores land
//   after the argument pushes, the empty tag is homed in the dead `n` slot, the record is destroyed after the call.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CAFile;

template <class T> class CCountedPointer {
public:
    T*    Object;                                   // +0
    void* Info;                                     // +4 (count block)
    CCountedPointer() : Object(0), Info(0) { }
};

class CLevelLoader {
public:
    class CObject {
    public:
        class CLevelFiles {                         // Ego_r.pdb, 0x1C bytes
        public:
            long                    LevelID;            // +0x00
            CCountedPointer<CAFile> PTopologyFile;      // +0x04
            CCountedPointer<CAFile> PNavigationFile;    // +0x0C
            CCountedPointer<CAFile> PThingFile;         // +0x14
            ~CLevelFiles();                             // 0x0050F980
        };
    };
};

namespace std {

struct __false_type { __false_type() { } };

template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&);   // 0x00513570

template <class ForwardIter, class Size, class T>
inline ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};

template <class T, class A> class _Vector_base {
public:
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    explicit vector(unsigned int n) : _Vector_base<T, A>(n, A())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};

template class vector<CLevelLoader::CObject::CLevelFiles, allocator<CLevelLoader::CObject::CLevelFiles> >;

}