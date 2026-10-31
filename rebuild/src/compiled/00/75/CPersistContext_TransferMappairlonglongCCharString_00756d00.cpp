// CPersistContext::TransferMap<pair<long, long>, CCharString>(const char* name, map<CCharString, pair<long, long> >& map, const pair<long, long>& defaultValue) -- transfer a string-keyed long-pair map in the context's mode: text save (one "name[key] (first,second);\r\n" line per entry), binary load/save through GFSerialiseMapBinaryIn/Out, clear, or text load ("name[key] value;" entries parsed from a copy of the parser, the old entry's value as the default, erased and re-inserted, the consumed text cleaned)
// Leaf: ??$TransferMap@U?$pair@JJ@std@@VCCharString@@@CPersistContext@@QAEXPBDAAV?$map@VCCharString@@U?$pair@JJ@std@@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@ABU?$pair@JJ@2@@Z
// Grade: RELOCATION_MATCH (chunk 621). The value traits follow the PDB layout (Ego_r CPersistTraits<std::pair<long,long> >
//   is 8 bytes, DefaultVal at +0). The default branch zeroes valueTraits.DefaultVal (one `xor edi,edi`, two stores; the
//   register is reused by the later `cmp ecx,edi`), overwrites it with the old entry or the default and transfers it IN
//   PLACE (`this` == &DefaultVal, retail [esp+0x44]).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00756D00, 1098 bytes + the 4-entry mode jump table (oracle 1116), `ret 0Ch`.
// Identity: Ego_r ??$TransferMap@U?$pair@JJ@std@@VCCharString@@@CPersistContext@@...ABU?$pair@JJ@2@@Z (donor 0x00768180;
//   PDB TransferMap<V, K> takes map<K, V>). Its no-default overload is the landed 0x00757160, whose only callee is this row;
//   CTCHeroRememberCrimes::OnSerialise 0x007556B0 passes RegionNamesToCrimeDurationAndFine (map<CCharString, pair<long,long> >).
//   The catalog label TransferMap<CObjectAugmentationParticleSet,EObjectAugmentationType> is propagated and wrong.
// Callees (names: PDB name sets + donor order + reloc_bindings; NOTES.md / NAME_FIXES.tsv):
//   CPersistContext::CheckCRC 0x00404500 (protected), PersistError 0x009BAA90 (Ego_r donor order InitClearMode -> PersistError
//   -> SetAsFoundItem -> WriteString = retail 0x009BAA70 / 0x009BAA90 / 0x009BAB40 / 0x009BAB50), SetAsFoundItem 0x009BAB40,
//   CleanOutLine 0x009BB0D0 (protected);
//   CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy 0x0099EC30 / dtor 0x0099EAE0 / operator+=(const char*)
//   0x0099F100, CBasicString<char>::operator==(const char*) 0x004115A0;
//   CStringParser copy 0x0046381B / SkipPastWholeString 0x009B9C00 (donor order SkipUntilWholeString -> SkipPastString ->
//   SkipPastWholeString -> SkipWhitespace = retail 0x009B9AA0 / 0x009B9BA0 / 0x009B9C00 / 0x009B9C60) / ReadAsSymbol 0x009BA790,
//   its compiler-generated destructor inline: std::list<CStringParserCommentBoundaries>::~list 0x004046B0, ~CCharString x2,
//   ~CBaseClass 0x0099A2E0;
//   CPersistTraits<CCharString>::TransferIn 0x0040E510 / TransferOut 0x0040E630 (PDB reference forms);
//   CPersistTraits<std::pair<long,long> >::TransferIn 0x00755F40 / TransferOut 0x00755A30 (same-TU noinline bodies, GOTCHAS
//   24l recipe; PDB reference forms), their long helpers TransferIn 0x009E2590 / TransferOutLong 0x009E2560 (__fastcall);
//   the tree's _Rb_tree_iterator::operator++ 0x00755BB0, _Rb_tree::_M_find 0x00756350 (the iterator-free lookup: the result is
//   compared with the header in eax), erase(const key_type&) 0x00756C60, insert_unique 0x00756250, clear 0x007563E0;
//   GFSerialiseMapBinaryOut 0x00756540 (mode 3, save stream) / GFSerialiseMapBinaryIn 0x007565E0 (mode 2, load stream), __fastcall.
// Literals: "[" 0x01235434, "]" 0x01235430, ";" 0x0123542C, "] " 0x012353BC, ";\r\n" 0x012353B8, "" 0x0122D70E, the three error
//   messages 0x01235404 / 0x012353E0 / 0x012353C0.
// Layout: CPersistContext per Ego_r.pdb (no engine header; HAND_OWNED, as the landed 0x00757160 / 0x00463334 models);
//   CStringParser retail 0x28 bytes (engine/CStringParser.h is the Ego_r 0x30 layout); STLport 4.6 map: _Rb_tree header node
//   pointer at +0, node value at +0x10 (key) / +0x14 (pair<long,long>).
// Shape: every switch case ends in `return`. The parser copy's generated destructor is inline. Case 1's value traits use the
//   real zeroing pair() constructor; VC7.1 drops its two stores as dead (retail case 1 has none), so no constructor workaround.
//   The save loop takes a non-escaping copy (`iterator entry = it;`) so the node stays in esi across the TransferOut calls.
//   0x00756350 is the private const _Rb_tree::_M_find (one push, link* returned in eax and compared with the header), which the
//   inline map::find wraps as iterator(_M_find(k)); find itself would return the iterator through a hidden pointer.
#include <string.h>
#pragma intrinsic(strcmp)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

#pragma pack(push, 1)
template <class T> class CBasicString {
public:
    T*            PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const T* text) const;              // 0x004115A0 for char
};
class CCharStringData {
public:
    CBasicString<char> Data;
    long               NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator+=(const char* text);   // 0x0099F100
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

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
};
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
template <class _Arg1, class _Arg2, class _Result> struct binary_function { };
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> { };
template <class _Pair> struct _Select1st { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport 4.6 list: the sentinel node pointer
public:
    ~list();                                           // 0x004046B0 for CStringParserCommentBoundaries (out of line)
    void* _M_node;
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                              // +0x00
    _Base_ptr   _M_parent;                             // +0x04
    _Base_ptr   _M_left;                               // +0x08 (header: leftmost)
    _Base_ptr   _M_right;                              // +0x0c (header: rightmost)
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                             // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    pointer operator->() const { return &(operator*()); }
    _Self& operator++();                               // 0x00755BB0 for this map (out of line)
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
protected:
    size_type _M_node_count;                           // +0x04
    _Compare _M_key_compare;                           // +0x08
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
public:
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }
    pair<iterator, bool> insert_unique(const value_type& __x);   // 0x00756250
    size_type erase(const key_type& __x);              // 0x00756C60
    void clear();                                      // 0x007563E0 (out of line)
    iterator find(const key_type& __x) { return iterator(_M_find(__x)); }
private:
    _Rb_tree_node<_Value>* _M_find(const key_type& __k) const;   // 0x00756350
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    typedef typename _Rep_type::size_type size_type;
private:
    _Rep_type _M_t;
public:
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    pair<iterator, bool> insert(const value_type& __x) { return _M_t.insert_unique(__x); }
    size_type erase(const key_type& __x) { return _M_t.erase(__x); }
    void clear() { _M_t.clear(); }
    iterator find(const key_type& __x) { return _M_t.find(__x); }
};
}

class CStringParserCommentBoundaries {
public:
    long Start;
    long End;
};
struct CCursorPos {
    bool InQuotes;                                     // +0x00
    long StringPos;                                    // +0x04
    long Line;                                         // +0x08
};
class CStringParser : public CBaseClass {             // retail 0x28 bytes; no user-declared destructor
public:
    char*                    String;                   // +0x04
    unsigned long            BaseLineOffset;           // +0x08
    CCharString              Filename;                 // +0x0c
    CCharString              TempCharString;           // +0x10
    std::list<CStringParserCommentBoundaries> Comments;   // +0x14
    void (__fastcall* ErrorCallback)(const CCharString&); // +0x18
    CCursorPos               CursorPos;                // +0x1c
    CStringParser(const CStringParser& other);         // 0x0046381B
    bool SkipPastWholeString(const CCharString& text); // 0x009B9C00
    CCharString ReadAsSymbol();                        // 0x009BA790
};

class CDefinitionManager;
class CSymbolMap;
class CMemoryDataInputStream;
class CDataOutputStream;
class CPersistContext {                                // Ego_r.pdb, 0x2c bytes
public:
    CDefinitionManager*     PDefinitionManager;           // +0x00
    CSymbolMap*             PSymbolMap;                   // +0x04
    CStringParser*          PParser;                      // +0x08
    void (__fastcall*       ErrorCallback)(const CCharString&); // +0x0c
    CCharString*            PStringToClean;               // +0x10
    bool                    UseDefaults;                  // +0x14
    bool                    UseCRCCheck;                  // +0x15
    bool                    SingleTransfer;               // +0x16
    bool                    Finished;                     // +0x17
    long                    Mode;                         // +0x18 0 text load, 1 text save, 2 binary load, 3 binary save, 4 clear
    bool                    Binary;                       // +0x1c
    CCharString*            PStringToSaveTo;              // +0x20
    CMemoryDataInputStream* PLoadStream;                  // +0x24
    CDataOutputStream*      PSaveStream;                  // +0x28
    void PersistError(const CStringParser& parser, const CCharString& message) const;   // 0x009BAA90
    void SetAsFoundItem();                                // 0x009BAB40
    template <class _Tp, class _Key> void TransferMap(const char* name, std::map<_Key, _Tp>& map, const _Tp& defaultValue);
protected:
    void CheckCRC(const char* name);                      // 0x00404500
    void CleanOutLine(long start, long end);              // 0x009BB0D0
};

void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value);        // 0x009E2590
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);       // 0x009E2560

template <class T> class CPersistTraits {
public:
    T DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, T& value);         // 0x0040E510 for CCharString
    void TransferOut(CPersistContext& context, CCharString& out, const T& value);       // 0x0040E630 for CCharString
};
// CPersistTraits<std::pair<T1, T2> > (Ego_r: DefaultVal pair at +0, TransferIn/TransferOut by reference). The two members
// are given their real shape in the TU, never inlined (GOTCHAS 24l: the same-TU bodies fix the found-branch pair copy).
template <class T1, class T2> class CPersistTraits<std::pair<T1, T2> > {
public:
    std::pair<T1, T2> DefaultVal;
    __declspec(noinline) void TransferIn(CPersistContext& context, CStringParser& parser, std::pair<T1, T2>& value)   // 0x00755F40
    {
        if (parser.ReadAsSymbol() == "(") {
            long read;
            ::TransferIn(&context, &parser, &read);
            value.first = read;
            if (parser.ReadAsSymbol() == ",") {
                ::TransferIn(&context, &parser, &read);
                value.second = read;
                parser.ReadAsSymbol();
            }
        }
    }
    __declspec(noinline) void TransferOut(CPersistContext& context, CCharString& out, std::pair<T1, T2>& value)       // 0x00755A30
    {
        out += "(";
        TransferOutLong(&context, &out, &value.first);
        out += ",";
        TransferOutLong(&context, &out, &value.second);
        out += ")";
    }
};

template <class _Tp, class _Key, class _Compare>
void __fastcall GFSerialiseMapBinaryOut(CDataOutputStream& stream, const std::map<_Key, _Tp, _Compare>& map);      // 0x00756540
template <class _Tp, class _Key, class _Compare>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, std::map<_Key, _Tp, _Compare>& map);        // 0x007565E0

template <class _Tp, class _Key>
void CPersistContext::TransferMap(const char* name, std::map<_Key, _Tp>& map, const _Tp& defaultValue)
{
    typedef typename std::map<_Key, _Tp>::iterator iterator;
    typedef typename std::map<_Key, _Tp>::value_type value_type;
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
        for (iterator it = map.begin(); it != map.end(); ++it) {
            iterator entry = it;
            *PStringToSaveTo += name;
            *PStringToSaveTo += "[";
            keyTraits.TransferOut(*this, *PStringToSaveTo, entry->first);
            *PStringToSaveTo += "] ";
            CPersistTraits<_Tp> valueTraits;
            valueTraits.TransferOut(*this, *PStringToSaveTo, entry->second);
            *PStringToSaveTo += ";\r\n";
        }
        return;
    }
    default: {
        CStringParser parser(*PParser);
        long nameLength = strlen(name);
        while (!Finished && parser.SkipPastWholeString(name)) {
            long start = parser.CursorPos.StringPos - nameLength;
            if (parser.ReadAsSymbol() == "[") {
                CPersistTraits<_Key> traits;
                _Key key;
                traits.TransferIn(*this, parser, key);
                if (parser.ReadAsSymbol() == "]") {
                    CPersistTraits<_Tp> valueTraits;
                    iterator old = map.find(key);
                    if (old != map.end())
                        valueTraits.DefaultVal = old->second;
                    else if (UseDefaults)
                        valueTraits.DefaultVal = defaultValue;
                    valueTraits.TransferIn(*this, parser, valueTraits.DefaultVal);
                    SetAsFoundItem();
                    map.erase(key);
                    map.insert(value_type(key, valueTraits.DefaultVal));
                    if (parser.ReadAsSymbol() == ";")
                        CleanOutLine(start, parser.CursorPos.StringPos);
                    else
                        PersistError(parser, "Unable to find terminating semicolon.");
                } else
                    PersistError(parser, "Unable to find terminating bracket.");
            } else
                PersistError(parser, "Unable to find opening bracket.");
        }
        return;
    }
    }
}

template void CPersistContext::TransferMap<std::pair<long, long>, CCharString>(const char*, std::map<CCharString, std::pair<long, long> >&,
                                                                              const std::pair<long, long>&);