// CPersistContext::TransferMap<bool, CCharString>(const char* name, std::map<CCharString, bool>& map, const bool& defaultValue) -- transfer a string-keyed bool map in the context's mode: text save (one "name[key] TRUE|FALSE;\r\n" line per entry), binary load/save through the stream helpers, clear, or text load ("name[key] value;" entries parsed from a copy of the parser, the old entry's value as the default, erased and re-inserted, the consumed text cleaned)
// Grade: RELOCATION_MATCH (1104/1104 on the exe span; the catalog row is OVERLONG at 1184 bytes: it runs past the `ret 0Ch` into
//   the 80 bytes of mode jump table + the next body -- re-cut the row to 1104 before landing, $S/bounds/AUDIT.tsv agrees).
//   Fix (2026-10-24 diag): the empty CPersistTraits<bool>::TransferIn gets its REAL body in the TU under auto_inline(off) +
//   noinline (GOTCHAS 24l recipe); VC7.1 then aliases the empty traits `this` onto the live `start` slot (frame 0x70) exactly
//   as retail. Previous grade: DIFFER(1104v1104 frame 0x74v0x70) -- ours gave the traits its own 4-byte slot.
// Leaf: ??$TransferMap@_NVCCharString@@@CPersistContext@@QAEXPBDAAV?$map@VCCharString@@_NU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@std@@AB_N@Z
// Identity (sweep3): Ego_r/FableWin name sets hold ??$TransferMap@_NVCCharString@@@CPersistContext@@QAEXPBDAAV?$map@VCCharString@@_N...@std@@AB_N@Z:
//   the template's arguments are <value, key> (every PDB TransferMap<V, K> takes map<K, V>, e.g. TransferMap<CCharString, long>
//   takes map<long, CCharString>), so this body is TransferMap<bool, CCharString>; the landed name had them swapped.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AE3D0, length 1084 bytes (+ the mode jump table 0x007AE810), `ret 0Ch`.
//   Catalog label "TransferMap<CObjectAugmentationParticleSet,EObjectAugmentationType>" is a propagated mislabel: the
//   value is bool (byte at node +0x14, "TRUE" 0x0122D6D4 / "FALSE" 0x0122D6CC), the sibling of 0x00756D00.
// Callees: CPersistContext::CheckCRC 0x00404500, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy 0x0099EC30 /
//   dtor 0x0099EAE0 / operator+=(const char*) 0x0099F100, CBasicString<char>::operator==(const char*) 0x004115A0, CStringParser
//   copy 0x0046381B / SkipToItem 0x009B9C00 / ReadAsSymbol 0x009BA790 (inline destructor: ~list 0x004046B0, ~CCharString,
//   ~CBaseClass 0x0099A2E0), CPersistTraits<CCharString> TransferIn 0x0040E510 / TransferOut 0x0040E630, CPersistTraits<bool>
//   TransferIn 0x00403FF0 (TransferOut inline), the map's (STLport 4.6 `_Rb_tree<CCharString, pair<const CCharString, bool>, ..>`)
//   iterator ++ 0x007AB8A0 / _M_find 0x007ACC90 (map::find inlined; landed F012 row -- the landed source called it lower_bound) /
//   erase(const key&) 0x007ADE60 / insert_unique 0x007AD060 (map::insert inlined) / clear 0x007ADAF0, and the binary map writer
//   GFSerialiseMapBinaryOut<bool, CCharString, less<CCharString> > 0x007AD270 (mode 3) and reader GFSerialiseMapBinaryIn<..>
//   0x007ADBF0 (mode 2) (__fastcall stream, map; PDB name set), CPersistContext::Error 0x009BAA90 / EntryTransferred
//   0x009BAB40 / CleanText 0x009BB0D0.
// Literals: "[" 0x01235434, "]" 0x01235430, ";" 0x0123542C, "] " 0x012353BC, ";\r\n" 0x012353B8, "" 0x0122D70E, the three error
//   messages 0x01235404 / 0x012353E0 / 0x012353C0.
// Layout: CPersistContext per Ego_r.pdb (landed 0x00757160 / 0x00463334 models); CStringParser retail 0x28 bytes; STLport map
//   node key +0x10, value +0x14, header pointer at the map's +0.
// Shape: STLport 4.6 map<CCharString, bool> (12 bytes) with map::find / insert / erase / clear inlined over the out-of-line
//   tree members. The text save copies the loop iterator into `entry` and reads (*entry).first / .second: VC7.1 then holds
//   the node in esi across the appends as retail does; reading (*it) off the loop iterator (its address escapes into the
//   out-of-line operator++) reloads it after every call instead (DIFFER 1100v1104, one callee-saved register short).
#include <string.h>
#pragma intrinsic(strcmp)
#pragma pack(push, 1)
template <class T> class CBasicString { public:
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;           // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator+=(const char* text);         // 0x0099F100
    bool operator<(const CCharString& other) const;    // (map key order; only inside the out-of-line tree code)
    const char* GetString() const { return PStringData != 0 ? PStringData->Data.PData : ""; }
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CBaseClass {
public:  virtual ~CBaseClass(); };             // 0x0099A2E0
struct CStringParserCommentList {                      // std::list<CStringParserCommentBoundaries>
    void* _M_node;
    ~CStringParserCommentList();                       // 0x004046B0
};
struct CCursorPos {
    bool InQuotes;                                     // +0x00
    long StringPos;                                    // +0x04
    long Line;                                         // +0x08
};
class CStringParser : public CBaseClass {                   // retail 0x28 bytes
public:
    char*                    String;                   // +0x04
    unsigned long            BaseLineOffset;           // +0x08
    CCharString              Filename;                 // +0x0c
    CCharString              TempCharString;           // +0x10
    CStringParserCommentList Comments;                 // +0x14
    void (*ErrorCallback)(CCharString*);               // +0x18
    CCursorPos               CursorPos;                // +0x1c
    CStringParser(const CStringParser& other);         // 0x0046381B
    bool SkipToItem(const CCharString& name);          // 0x009B9C00
    CCharString ReadAsSymbol();                        // 0x009BA790
    CCharString ReadAsString();                        // 0x009BA6F0
};
class CPersistContext;
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, T& value);    // 0x0040E510
    void TransferOut(CPersistContext& context, CCharString& out, const T& value);  // 0x0040E630
};
template <> struct CPersistTraits<bool> {              // empty traits (retail `this` aliases an unrelated stack slot)
    void TransferIn(CPersistContext& context, CStringParser& parser, bool& value);   // 0x00403FF0, real body below
    void TransferOut(CPersistContext& context, CCharString& out, const bool& value)
    {
        if (value)
            out += "TRUE";
        else
            out += "FALSE";
    }
};
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8
// CPersistTraits<bool>::TransferIn 0x00403FF0 with its real shape in the TU (GOTCHAS 24l recipe: auto_inline(off) +
// noinline): the parsed token is compared case-insensitively with the TRUE/FALSE spellings. With the body visible VC7.1
// aliases the empty traits `this` onto the live `start` slot as retail does. Not landed at 0x00403FF0.
#pragma auto_inline(off)
__declspec(noinline) void CPersistTraits<bool>::TransferIn(CPersistContext& context, CStringParser& parser, bool& value)
{
    CCharString token = parser.ReadAsString();
    if (_stricmp(token.GetString(), CCharString("TRUE").GetString()) == 0 ||
        _stricmp(token.GetString(), CCharString("YES").GetString()) == 0)
        value = true;
    else if (_stricmp(token.GetString(), CCharString("FALSE").GetString()) == 0 ||
             _stricmp(token.GetString(), CCharString("NO").GetString()) == 0)
        value = false;
}
#pragma auto_inline(on)
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> struct _Const_traits {
    typedef _Tp value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
struct _Rb_tree_node_base {
    bool                _M_color;                     // +0x00
    _Rb_tree_node_base* _M_parent;                    // +0x04
    _Rb_tree_node_base* _M_left;                      // +0x08 (in the header: leftmost)
    _Rb_tree_node_base* _M_right;                     // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                            // +0x10 (key), +0x14 (bool)
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    reference operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Self& operator++();                               // 0x007AB8A0 for this tree (out of line)
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Key key_type;
    typedef _Value value_type;
    typedef unsigned int size_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> > const_iterator;
    unsigned int _M_node_count;                        // +0x04
    _Compare _M_key_compare;                           // +0x08 (empty)
    private:
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;          // 0x007ACC90 (out of line)
    public:
    iterator begin() { return iterator(this->_M_header._M_data->_M_left); }
    iterator end() { return iterator(this->_M_header._M_data); }
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
    pair<iterator, bool> insert_unique(const value_type& __x);     // 0x007AD060
    size_type erase(const key_type& __x);                          // 0x007ADE60
    void clear();                                                  // 0x007ADAF0
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                            // STLport 4.6 (SGI): 12 bytes
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    typedef typename _Rep_type::size_type size_type;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    iterator find(const key_type& __x) { return _M_t.find(__x); }
    pair<iterator, bool> insert(const value_type& __x) { return _M_t.insert_unique(__x); }
    size_type erase(const key_type& __x) { return _M_t.erase(__x); }
    void clear() { _M_t.clear(); }
};
}
class CMemoryDataInputStream;
class CDataOutputStream;
template <class _Tp, class _Key, class _Compare>
void __fastcall GFSerialiseMapBinaryOut(CDataOutputStream& stream, const std::map<_Key, _Tp, _Compare>& map);   // 0x007AD270
template <class _Tp, class _Key, class _Compare>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, std::map<_Key, _Tp, _Compare>& map);     // 0x007ADBF0

class CDefinitionManager;
class CSymbolMap;
class CPersistContext {
public:
    CDefinitionManager*     PDefinitionManager;           // +0x00
    CSymbolMap*             PSymbolMap;                   // +0x04
    CStringParser*          PParser;                      // +0x08
    void (__cdecl*          ErrorCallback)(CCharString*); // +0x0c
    CCharString*            PStringToClean;               // +0x10
    bool                    UseDefaults;                  // +0x14
    bool                    UseCRCCheck;                  // +0x15
    bool                    SingleTransfer;               // +0x16
    bool                    Finished;                     // +0x17
    long                    Mode;                         // +0x18
    bool                    Binary;                       // +0x1c
    CCharString*            PStringToSaveTo;              // +0x20
    CMemoryDataInputStream* PLoadStream;                  // +0x24
    CDataOutputStream*      PSaveStream;                  // +0x28
protected:
    void CheckCRC(const char* name);                      // 0x00404500 (protected: PDB ?CheckCRC@CPersistContext@@IAEXPBD@Z)
public:
    void Error(const CStringParser& parser, const CCharString& message);   // 0x009BAA90
    void EntryTransferred();                              // 0x009BAB40
    void CleanText(long start, long end);                 // 0x009BB0D0
    template <class _Tp, class _Key> void TransferMap(const char* name, std::map<_Key, _Tp>& map, const _Tp& defaultValue);
};

template <class _Tp, class _Key>
void CPersistContext::TransferMap(const char* name, std::map<_Key, _Tp>& map, const _Tp& defaultValue)
{
    CheckCRC(name);
    CPersistTraits<_Key> keyTraits;
    switch (Mode) {
    case 4:
        map.clear();
        return;
    case 3:
        GFSerialiseMapBinaryOut(*PSaveStream, map);
        return;
    case 2:
        GFSerialiseMapBinaryIn(*PLoadStream, map);
        return;
    case 1: {
        for (typename std::map<_Key, _Tp>::iterator it = map.begin(); it != map.end(); ++it) {
            typename std::map<_Key, _Tp>::iterator entry = it;
            *PStringToSaveTo += name;
            *PStringToSaveTo += "[";
            keyTraits.TransferOut(*this, *PStringToSaveTo, (*entry).first);
            *PStringToSaveTo += "] ";
            CPersistTraits<_Tp> valueTraits;
            valueTraits.TransferOut(*this, *PStringToSaveTo, (*entry).second);
            *PStringToSaveTo += ";\r\n";
        }
        return;
    }
    default: {
        CStringParser parser(*PParser);
        long nameLength = strlen(name);
        while (!Finished && parser.SkipToItem(name)) {
            long start = parser.CursorPos.StringPos - nameLength;
            if (parser.ReadAsSymbol() == "[") {
                CPersistTraits<_Key> traits;
                _Key key;
                traits.TransferIn(*this, parser, key);
                if (parser.ReadAsSymbol() == "]") {
                    _Tp value;
                    typename std::map<_Key, _Tp>::iterator old = map.find(key);
                    if (old != map.end())
                        value = (*old).second;
                    else if (UseDefaults)
                        value = defaultValue;
                    CPersistTraits<_Tp> valueTraits;
                    valueTraits.TransferIn(*this, parser, value);
                    EntryTransferred();
                    map.erase(key);
                    map.insert(typename std::map<_Key, _Tp>::value_type(key, value));
                    if (parser.ReadAsSymbol() == ";")
                        CleanText(start, parser.CursorPos.StringPos);
                    else
                        Error(parser, "Unable to find terminating semicolon.");
                } else
                    Error(parser, "Unable to find terminating bracket.");
            } else
                Error(parser, "Unable to find opening bracket.");
        }
        return;
    }
    }
}

template void CPersistContext::TransferMap<bool, CCharString>(const char*, std::map<CCharString, bool>&, const bool&);
