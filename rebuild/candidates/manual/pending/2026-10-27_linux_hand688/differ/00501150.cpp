// CWorldMap::LoadLevelFromFiles(long level, CCountedPointer<CAFile> topologyFile, CCountedPointer<CAFile> navigationFile, CCountedPointer<CAFile> thingFile, ELoadType type) -- queue one level for loading from three already-open files: build a CLevelLoader::CObject holding a single CLevelFiles record (the level id and the navigation, thing and topology files) and the map's loadable section names, hand it to the level loader, and for an immediate load pump the loader until it is idle
// Grade: DIFFER(756v756) -- one residue, the documented lane gap of GOTCHAS chunk 149: the empty `__false_type` tag passed to __uninitialized_fill_n is homed by retail in the live `level` argument slot (`lea edx,[esp+50h]`), while our VC7.1 opens a frame byte for it (`sub esp,40h` / `lea edx,[esp+13h]`); every other instruction matches
// Leaf: ?LoadLevelFromFiles@CWorldMap@@IAEXJV?$CCountedPointer@VCAFile@@@@00W4ELoadType@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00501150, 756 bytes, thiscall `ret 20h` (long + three by-value 8-byte counted pointers +
//   the nested enum; the callee releases the by-value pointers last). Ego_r.pdb names the member (protected); its sole caller
//   is CWorldMap::LoadLevel 0x00502620 (`?LoadLevel@CWorldMap@@IAEXJW4ELoadType@1@@Z`). The catalog's "LoadLevel_ParseObjects"
//   is a placeholder.
// Callees: operator new 0x00BFEA1A (the CObject, the CCPPointerInfo count block), malloc 0x00BFEA0E (the set header, the
//   vector storage), CCountedPointer<CLevelLoader::CObject>::DeleteData 0x0051AF00 (stored in the count block; landed as
//   "CTCMapwho::OnDie" -- NAME_FIXES.tsv), std::__uninitialized_fill_n<CLevelFiles*,unsigned int,CLevelFiles> 0x00513570
//   (`__fastcall`; landed as a descriptive UninitFillN stand-in -- NAME_FIXES.tsv), CLevelLoader::CObject::CLevelFiles::~CLevelFiles
//   0x0050F980 (the fill's source record and the vector's element), CWorldMap virtual slot 26 GetLoadableSections 0x004FF490
//   (set<CCharString> by hidden pointer), CLevelLoader::CObject::SetAsLoading 0x006C27A0 (landed as "SMappedRecords::Set" --
//   NAME_FIXES.tsv), CLevelLoader::Add 0x006C2120, CLevelLoader::IsBusy 0x006C20A0, CLevelLoader virtual slot 1
//   (CSuspendableProcessMicrothread::RunSuspendableProcess 0x00A44880), set<CCharString>::_M_erase 0x004ABA70, free
//   0x00BFEA14, operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CLevelLoader::CObject at the retail 12-byte STLport sizes (0x20 bytes: LoadableSections set +0,
//   PLevelLoader +0xC, RegionLevelFiles vector +0x10, RegionIndex +0x1C -- the inline constructor zeroes everything but
//   RegionIndex); CLevelFiles 0x1C {LevelID, PTopologyFile +4, PNavigationFile +0xC, PThingFile +0x14}; CWorldMap::PLevelLoader
//   +0xBC (engine/CWorldMap.h; the local class carries the 27 virtual slots the data-only header lacks). The counted
//   pointer is the shared engine/CCountedPointer.h (its operator= is the inline ShareData, its destructor the Data-first
//   release). The argument names are descriptive (the PDB keeps only the types); the record fields they are assigned to
//   are the PDB's, in retail's store order (navigation, thing, topology).
// Shape: `while (type == LOAD_TYPE_IMMEDIATE && PLevelLoader->IsBusy())` gives retail's untransformed loop (the guard
//   tested once, `jmp` back to the IsBusy test); `if (type == 0) while (...)` rotates the loop (772v756).
#include "engine/CCountedPointer.h"
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);               // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
class CAFile;
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Tp> struct _Identity { const _Tp& operator()(const _Tp& __x) const { return __x; } };
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);   // 0x00513570 for CLevelFiles
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    typedef _Tp* iterator;
    vector() : _Vector_base<_Tp, _Alloc>(allocator_type()) { }
    explicit vector(size_type __n) : _Vector_base<_Tp, _Alloc>(__n, allocator_type())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                 // +0x00
    _Base_ptr _M_parent;                                // +0x04
    _Base_ptr _M_left;                                  // +0x08
    _Base_ptr _M_right;                                 // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _Rb_tree_base(const _Alloc&) : _M_header(allocator<_Node>(), (_Rb_tree_node_base*)0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    unsigned int _M_node_count;                         // +0x04
    _Compare     _M_key_compare;                        // +0x08
protected:
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
public:
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(_Alloc()), _M_node_count(0) { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
private:
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        _M_root() = 0;
        _M_leftmost() = (_Link_type)this->_M_header._M_data;
        _M_rightmost() = (_Link_type)this->_M_header._M_data;
    }
    void _M_erase(_Link_type __x);                      // 0x004ABA70 for set<CCharString>
public:
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = (_Link_type)this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = (_Link_type)this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    typedef _Key key_type;
    typedef _Key value_type;
    typedef _Compare key_compare;
    typedef _Rb_tree<key_type, value_type, _Identity<value_type>, key_compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                     // +0x00
    set() : _M_t() { }
};
}   // namespace std

class CLevelLoader {                                    // retail: CSuspendableProcessMicrothread base (vptr), ObjectList +0x14
public:
    class CObject {
    public:
        class CLevelFiles {                             // 0x1C
        public:
            long                     LevelID;           // +0x00
            CCountedPointer<CAFile>  PTopologyFile;     // +0x04
            CCountedPointer<CAFile>  PNavigationFile;   // +0x0C
            CCountedPointer<CAFile>  PThingFile;        // +0x14
            CLevelFiles() { }
            ~CLevelFiles();                             // 0x0050F980
        };
        std::set<CCharString>    LoadableSections;      // +0x00
        CLevelLoader*            PLevelLoader;          // +0x0C
        std::vector<CLevelFiles> RegionLevelFiles;      // +0x10
        long                     RegionIndex;           // +0x1C
        CObject() : PLevelLoader(0) { }
        void SetAsLoading(const std::vector<CLevelFiles>& files, const std::set<CCharString>& sections, long regionIndex);   // 0x006C27A0
    };
    virtual void v00();
    virtual void RunSuspendableProcess();               // slot 1 (0x00A44880, CSuspendableProcessMicrothread)
    bool IsBusy() const;                                // 0x006C20A0
    void Add(const CCountedPointer<CObject>& object);   // 0x006C2120
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25();
    virtual std::set<CCharString> GetLoadableSections() const;   // slot 26 (+0x68) 0x004FF490
    enum ELoadType { LOAD_TYPE_IMMEDIATE = 0, LOAD_TYPE_BACKGROUND = 1 };
    unsigned char _pad04[0xbc - 0x04];
    CCountedPointer<CLevelLoader> PLevelLoader;         // +0xbc
protected:
    void LoadLevelFromFiles(long level, CCountedPointer<CAFile> topologyFile, CCountedPointer<CAFile> navigationFile,
                            CCountedPointer<CAFile> thingFile, ELoadType type);
};

void CWorldMap::LoadLevelFromFiles(long level, CCountedPointer<CAFile> topologyFile, CCountedPointer<CAFile> navigationFile,
                                   CCountedPointer<CAFile> thingFile, ELoadType type)
{
    CCountedPointer<CLevelLoader::CObject> object(new CLevelLoader::CObject);
    std::vector<CLevelLoader::CObject::CLevelFiles> files(1);
    files[0].LevelID = level;
    files[0].PNavigationFile = navigationFile;
    files[0].PThingFile = thingFile;
    files[0].PTopologyFile = topologyFile;
    std::set<CCharString> sections = GetLoadableSections();
    object->SetAsLoading(files, sections, 0);
    PLevelLoader->Add(object);
    while (type == LOAD_TYPE_IMMEDIATE && PLevelLoader->IsBusy())
        PLevelLoader->RunSuspendableProcess();
}
