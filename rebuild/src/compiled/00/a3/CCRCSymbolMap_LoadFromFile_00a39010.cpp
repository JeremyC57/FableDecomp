// CCRCSymbolMap::LoadFromFile(const CWideString& filename, const CCharString& enumName) -- load a CRC symbol file: clear the map, read the whole file into a zeroed byte buffer, strip comments, parse the wanted enum and trim the map
// Leaf: ?LoadFromFile@CCRCSymbolMap@@QAE_NABVCWideString@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A39010, 378 bytes, thiscall, `ret 8`, returns bool.
// Identity (proven): Ego_r.pdb ?LoadFromFile@CCRCSymbolMap@@QAE_NABVCWideString@@ABVCCharString@@@Z (donor 00B95030, after
//   ParseBuffer / Clear / DeallocateUnusedMemory / UpdateDirty / UpdateDirtyQuick, the retail order 0x00A38E50 .. 0x00A38FE0
//   then this row); reloc_bindings already binds 0x00A39010 to that symbol (two callers 0x004CDB46, 0x00A01A4F).
// Callees: CVectorMap<unsigned long,long>::DeallocateMemory 0x00A39900 and set<unsigned long>'s _Rb_tree::_M_erase
//   0x004CF810 (the inlined Clear, landed 0x00A38550), CWideString::operator=(const CWideString&) 0x0099B7D0 /
//   (const unsigned short*) 0x0099B800 on the static CCRCSymbolMap::CurrentFilename 0x013D27E8 (Ego_r dynamic initializer
//   name), the inlined CDiskFileWin32 default ctor / dtor (CBaseClassNonCopyable 0x0099A2F0 / 0x0099A300, vtable 0x0122D06C,
//   CWideString() 0x0099AED0 / ~ 0x0099B510, Close 0x0099A920) as in the landed CConsole::RunScript 0x009EC890, Open
//   0x0099A6A0, Read 0x0099A960, malloc / free (the inlined STLport vector<char>(n): allocate, memset, free),
//   GFRemoveComments 0x00A60410 x3 ("//" 0x0129B204 to newline 0x0129B208 / carriage return 0x0129B200; "/*" 0x0129B1F8 to "*/"
//   0x0129B1FC), ParseBuffer 0x00A38E50 (Ego_r: protected, const char*, const CCharString&), the inlined UpdateDirty
//   (retail 0x00A38FB0: CVectorMap::Update = Sort, std::sort 0x009B8520 when Dirty, then CArray::DeallocateUnusedMemory
//   0x00A39A00), L"" 0x0122D70C.
// Layout: Ego_r.pdb CCRCSymbolMap re-laid at retail sizes (landed 0x00A38550): SLONGMap +0x04 (CVectorMap 0x10: Data
//   +0, Compare +0xc, Dirty +0xd), NamesUsed set +0x14 (12 bytes), CheckForDuplicateSymbols +0x20. Local models (the
//   generated engine/CDiskFileWin32.h is a flat pack(1) layout without the vtable; NOTES.md header debt).
// Shape: Clear and UpdateDirty are plain members defined earlier in the file (retail keeps both out of line and inlines
//   them here); the result flag is a stack bool set only on success; the buffer is the real STLport vector<char>.
//   CurrentFilename's access is unproven (no PDB data-symbol dump): written public.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl memset(void*, int, unsigned int);
#pragma intrinsic(memset)
class CCharString { public:
    void* PStringData;
};
class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);       // 0x0099B7D0
    const CWideString& operator=(const unsigned short* text);     // 0x0099B800
};
class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                            // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                   // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;
class CDiskFileWin32 : public CBaseClassNonCopyable { public:     // vtable 0x0122D06C, 0x1c bytes
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
    virtual void Close();                               // 0x0099A920
    virtual void Read(void* data, long size, CASuspendableProcess* process);   // 0x0099A960
    FileAccessType AccessType;                          // +0x04
    long        Position;                               // +0x08
    long        Length;                                 // +0x0c
    void*       WinFileHandle;                          // +0x10
    bool        Opened;                                 // +0x14
    CWideString Pathname;                               // +0x18
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
};
void __fastcall GFRemoveComments(char* text, const char* start, const char* end);   // 0x00A60410

namespace std {
struct __true_type { __true_type() { } };
template <class _Tp> struct less { bool operator()(const _Tp& a, const _Tp& b) const { return a < b; } };
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _A, class _B> struct pair { _A first; _B second; };
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x009B8520
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(unsigned int __n) : _Vector_base<_Tp, _Alloc>(__n, _Alloc())
    {
        memset(this->_M_start, 0, __n);
        this->_M_finish = this->_M_start + __n;
    }
    _Tp& operator[](unsigned int __n) { return *(this->_M_start + __n); }
    _Tp* begin() { return this->_M_start; }
    _Tp* end() { return this->_M_finish; }
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Tp> struct _Identity { };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
protected:
    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                        // 0x004CF810
public:
    void clear() {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;
    void clear() { _M_t.clear(); }
};
}
template <class T> class CArray : public std::vector<T> {
public:
    void DeallocateUnusedMemory();                      // 0x00A39A00
};
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap { public:
    CArray<std::pair<K, V> > Data;                      // +0x00
    C                        Compare;                   // +0x0c
    bool                     Dirty;                     // +0x0d
    void DeallocateMemory();                            // 0x00A39900
    void Sort() { if (Dirty) { std::sort(Data.begin(), Data.end(), Compare); Dirty = false; } }
    void Update() { Sort(); Data.DeallocateUnusedMemory(); }
};

class CCRCSymbolMap : public CBaseClassNonCopyable { public:      // vtable 0x0129CF84
    mutable CVectorMap<unsigned long, long> SLONGMap;   // +0x04
    std::set<unsigned long>                 NamesUsed;  // +0x14
    bool                                    CheckForDuplicateSymbols;   // +0x20
    static CWideString CurrentFilename;                 // 0x013D27E8
    void Clear();
    void UpdateDirty() const;
    bool LoadFromFile(const CWideString& filename, const CCharString& enumName);
protected:
    bool ParseBuffer(const char* buffer, const CCharString& enumName);   // 0x00A38E50
};

void CCRCSymbolMap::Clear()
{
    SLONGMap.DeallocateMemory();
    NamesUsed.clear();
}

void CCRCSymbolMap::UpdateDirty() const
{
    SLONGMap.Update();
}

bool CCRCSymbolMap::LoadFromFile(const CWideString& filename, const CCharString& enumName)
{
    bool result = false;
    Clear();
    CurrentFilename = filename;
    CDiskFileWin32 file;
    file.Open(filename, FILE_ACCESS_READ, 2);
    std::vector<char> buffer(file.Length + 1);
    buffer[file.Length] = 0;
    file.Read(&buffer[0], file.Length, 0);
    GFRemoveComments(&buffer[0], "//", "\n");
    GFRemoveComments(&buffer[0], "//", "\r");
    GFRemoveComments(&buffer[0], "/*", "*/");
    if (ParseBuffer(&buffer[0], enumName))
        result = true;
    UpdateDirty();
    CurrentFilename = (const unsigned short*)L"";
    return result;
}