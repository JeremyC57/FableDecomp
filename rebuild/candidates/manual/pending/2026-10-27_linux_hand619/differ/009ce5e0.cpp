// CBankFile::AddEntryMultipleFiles(const CCharString& symbol, unsigned long type, const CArray<CCharString>& filenames, CCountedPointer<CBankStateBlock> stateBlock) -- add or refresh a bank entry built from several source files
// Grade: DIFFER(741v741) -- respelled: class keys (ABVCCharString / ABV?$CArray@VCCharString@@ / V?$CCountedPointer, full leaf now binds), map at +0xb8 is OldSymbolIndices (engine/CBankFile.h; SymbolIndices +0xac is what InitEntry fills) modelled as the real STLport map whose find inlines _Rb_tree::_M_find (private const, 0x009D2DD0), CArray<T> : std::vector<T> with vector<..>::resize callees, CBankFile::CRuntimeData nested, private Equals/SetFilenames (friend CBankFile) and private InitEntry, CPackedUIntArray::Size (Ego_r) for the packed test; residue unchanged: symbol in ebp/index in ebx (retail ebx/ebp) and the first-branch filename / "" temporary / resize zero temporaries in rotated frame words -- tried separate per-branch index locals (identical)
// Leaf: ?AddEntryMultipleFiles@CBankFile@@UAEXABVCCharString@@KABV?$CArray@VCCharString@@@@V?$CCountedPointer@VCBankStateBlock@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CE5E0, 741 bytes, ret 0x14; vtable 0x0129B54C slot 6; Ego_r.pdb
//   ?AddEntryMultipleFiles@CBankFile@@UAEXABVCCharString@@KABV?$CArray@VCCharString@@@@V?$CCountedPointer@VCBankStateBlock@@@@@Z.
//   A symbol already in OldSymbolIndices (retail +0xb8; SymbolIndices +0xac is the one InitEntry fills) must not exist yet in this
//   update pass (CBankCreatorException "Duplicate symbol: <symbol>"); its record is flagged for update when the type, filenames
//   or state-block CRC changed; a new symbol takes the first free index (growing RuntimeData / UpdateData / Symbols / Checksums
//   when none is free) and a fresh update record; both paths finish with InitEntry(index, symbol, first filename or "").
// Callees: _Rb_tree<CCharString, pair<const CCharString, unsigned long>, ...>::_M_find 0x009D2DD0 (map::find inline),
//   CCharString() / copy / (const char*, long) 0x0099E4B0 / 0x0099EC30 / 0x0099EBF0, ~CCharString 0x0099EAE0, operator=
//   0x0099EFB0, operator+(const char*, const CCharString&) 0x0099F690 (__fastcall), CBankCreatorException copy 0x009D47E0 /
//   dtor 0x009D47C0 (throw info 0x013693B4, _CxxThrowException 0x00BFEB84), CBankFileEntryUpdateData::Equals (private const)
//   0x009CC750 / SetFilenames (private) 0x009CC840, CBankStateBlock slot 2 GetCRC, vector<CBankFile::CRuntimeData>::resize
//   0x009D4010, vector<CBankFileEntryUpdateData*>::resize 0x009D3DF0, vector<CCharString>::resize(unsigned) 0x0049B760,
//   vector<unsigned long>::resize 0x00464931, operator new 0x00BFEA1A, CBankFile::InitEntry (private) 0x009CE050,
//   CPackedUIntArray::operator[] const 0x00A629F0 (inline EntryExists), operator delete 0x00BFE9BC. "Duplicate symbol: "
//   0x0129B5AC, "" 0x0122D70E.
// Layout: rebuild/include/engine/CBankFile.h retail offsets (Size +4, Symbols +8, Checksums +0x14, RuntimeData +0x20,
//   UpdateData +0x2c, PackedValid +0x68, OldSymbolIndices +0xb8); the header stores containers as opaque bytes, so the class is
//   modelled locally with the real STLport 4.6 templates at the same offsets.
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* block);
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* p);                 // 0x00BFE9BC
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const CCharString& other);             // 0x0099EC30
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
class CBankCreatorException {                          // 8 bytes (Ego_r.pdb)
public:
    CCharString EntrySymbol;
    CCharString ErrorText;
    CBankCreatorException(const CCharString& text) : ErrorText(text) { }
    CBankCreatorException(const CBankCreatorException& other);   // 0x009D47E0
    ~CBankCreatorException();                                    // 0x009D47C0
};
class CCPPointerInfo { public:
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void*);              // +4
    void* Data;                                        // +8
};
class CBankStateBlock { public:
    virtual void v0(); virtual void v1();
    virtual unsigned long GetCRC();                    // slot 2 (+0x08)
};
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
    }
    T* operator->() const { return Data; }
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    unsigned int size() const { return unsigned int(end() - begin()); }
    bool empty() const { return begin() == end(); }
    _Tp& operator[](unsigned int __n) { return *(begin() + __n); }
    const _Tp& operator[](unsigned int __n) const { return *(begin() + __n); }
    void resize(unsigned int __new_size, const _Tp& __x);
    void resize(unsigned int __new_size);
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };
template <class _Tp> struct _Nonconst_traits { };
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
struct _Rb_tree_base_iterator { _Rb_tree_node_base* _M_node; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _STLP_alloc_proxy<_Rb_tree_node<_Value>*, _Rb_tree_node<_Value>, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    iterator end() { return iterator(_M_header._M_data); }
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
private:
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;   // 0x009D2DD0
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator end() { return _M_t.end(); }
    iterator find(const _Key& __x) { return _M_t.find(__x); }
};
}
template <class T> class CArray : public std::vector<T> { };
class CPackedUIntArray { public:                       // 0x10 bytes (Ego_r.pdb)
    unsigned long* PackedInts;                         // +0x00
    unsigned long  Size;                               // +0x04
    unsigned char  Bits;                               // +0x08
    unsigned long  Bias;                               // +0x0c
    unsigned long operator[](unsigned long index) const;   // 0x00A629F0
};
class CSmallVectorCCharString8 { public: CCharString* Start; signed char Size; CSmallVectorCCharString8() : Start(0), Size(0) { } };   // CSmallVector<CCharString,8>
class CBankFile;
class CBankFileEntryUpdateData {                       // 0x18 bytes retail (Ego_r 0x20 with the StateBlock pointer)
public:
    unsigned long            StateBlockCRC;            // +0x00
    unsigned long            InfoSize;                 // +0x04
    char*                    InfoBuffer;               // +0x08
    CSmallVectorCCharString8 Filenames;                // +0x0c
    bool                     RequiresUpdate;           // +0x14
    bool                     Exists;                   // +0x15
    CBankFileEntryUpdateData() : StateBlockCRC(0), InfoSize(0), InfoBuffer(0), RequiresUpdate(false), Exists(true) { }
private:
    friend class CBankFile;
    bool Equals(const CArray<CCharString>& filenames, CCountedPointer<CBankStateBlock> stateBlock) const;   // 0x009CC750
    void SetFilenames(const CArray<CCharString>& filenames);   // 0x009CC840
};
class CBankFile {
public:
    class CRuntimeData { public:
        unsigned long DataOffset;                      // +0x00
        unsigned long DataSize;                        // +0x04
        unsigned char DataType;                        // +0x08
        bool          Valid;                           // +0x09
    };
    virtual ~CBankFile();                                                      // +0x00 vptr
    unsigned long                               Size;                          // +0x04
    CArray<CCharString>                         Symbols;                       // +0x08
    CArray<unsigned long>                       Checksums;                     // +0x14
    CArray<CRuntimeData>                        RuntimeData;                   // +0x20
    CArray<CBankFileEntryUpdateData*>           UpdateData;                    // +0x2c
    CPackedUIntArray                            PackedDataOffset;              // +0x38
    CPackedUIntArray                            PackedDataSize;                // +0x48
    CPackedUIntArray                            PackedDataType;                // +0x58
    CPackedUIntArray                            PackedValid;                   // +0x68
    unsigned char                               _pad_0x78[0xac - 0x78];        // OpenFlags .. FileSize
    std::map<CCharString, unsigned long>        SymbolIndices;                 // +0xac
    std::map<CCharString, unsigned long>        OldSymbolIndices;              // +0xb8
    bool EntryExists(unsigned long index) const
    {
        if (index != 0 && index < Size) {
            if (PackedValid.Size)
                return PackedValid[index - 1] != 0;
            return RuntimeData[index].Valid;
        }
        return false;
    }
    virtual void AddEntryMultipleFiles(const CCharString& symbol, unsigned long type, const CArray<CCharString>& filenames, CCountedPointer<CBankStateBlock> stateBlock);
private:
    void InitEntry(unsigned long index, const CCharString& symbol, const CCharString& filename);   // 0x009CE050
};

void CBankFile::AddEntryMultipleFiles(const CCharString& symbol, unsigned long type, const CArray<CCharString>& filenames, CCountedPointer<CBankStateBlock> stateBlock)
{
    unsigned long index;
    std::map<CCharString, unsigned long>::iterator it = OldSymbolIndices.find(symbol);
    if (it != OldSymbolIndices.end()) {
        index = (*it).second;
        CBankFileEntryUpdateData* update = UpdateData[index];
        if (update->Exists)
            throw CBankCreatorException("Duplicate symbol: " + symbol);
        update->Exists = true;
        CRuntimeData& data = RuntimeData[index];
        if (data.DataType != type || !update->Equals(filenames, stateBlock)) {
            data.DataType = (unsigned char)type;
            update->SetFilenames(filenames);
            update->StateBlockCRC = stateBlock->GetCRC();
            update->RequiresUpdate = true;
        }
        {
            CCharString filename(filenames.empty() ? CCharString("") : filenames[0]);
            InitEntry(index, symbol, filename);
        }
    } else {
        for (index = 1; index < Size; ++index)
            if (!EntryExists(index))
                break;
        if (index >= Size) {
            CRuntimeData blank;
            blank.DataOffset = 0;
            blank.DataSize = 0;
            blank.DataType = 0;
            blank.Valid = false;
            RuntimeData.resize(index + 1, blank);
            UpdateData.resize(index + 1, 0);
            Symbols.resize(index + 1);
            Checksums.resize(index + 1, 0);
            Size = RuntimeData.size();
        }
        CRuntimeData& data = RuntimeData[index];
        CBankFileEntryUpdateData* update = new CBankFileEntryUpdateData;
        UpdateData[index] = update;
        Symbols[index] = symbol;
        data.Valid = true;
        data.DataType = (unsigned char)type;
        update->SetFilenames(filenames);
        update->StateBlockCRC = stateBlock->GetCRC();
        update->Exists = true;
        update->RequiresUpdate = true;
        CCharString filename(filenames.empty() ? CCharString("") : filenames[0]);
        InitEntry(index, symbol, filename);
    }
}
