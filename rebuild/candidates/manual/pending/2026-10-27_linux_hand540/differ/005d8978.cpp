// CPersistContext::TransferMap<long, unsigned long>(const char* name, std::map<unsigned long, long>& map, const long& defaultValue) -- transfer a long-valued map keyed by unsigned long (CTCInventoryStats CreaturesKilled, "CreaturesKilled2") in the context's mode: text load ("name[key] value;" entries parsed from a copy of the parser; each replaces any existing entry for its key, the consumed text is cleaned), text save (one "name[key] value;\r\n" line per entry), binary save/load through the stream helpers, or clear
// Grade: DIFFER(754v754) -- residue: frame slots only: the discarded insert result temp sits at -0x3C (retail -0x30), so the value, the ";" temp and start each sit one slot off; the value traits object takes its own slot (-0x38) where retail homes both traits objects in the dead map argument slot [ebp+0Ch]; and the binary save/load case blocks are emitted load-first (retail save-first); lane gap: frame-slot packing | tried: PDB-named rewrite (TransferMap<long,unsigned long> on map<unsigned long,long>, CPersistTraits<unsigned long>/<long>, CPersistTraitsSLONG::TransferIn, _Rb_global<bool>::_M_increment), traits objects block-scoped / declared before the loop / reversed, traits temporaries (56), case 3 before case 2 (inert), insert returning pair<iterator,bool> (frame +4); the earlier single-object traits device reached NDIFF 9 but is not a real type | next: none known
// Leaf: ??$TransferMap@JK@CPersistContext@@QAEXPBDAAV?$map@KJU?$less@K@std@@V?$allocator@U?$pair@$$CBKJ@std@@@2@@std@@ABJ@Z (Ego_r.pdb; caller Transfer wrapper 0x005D8C6A from 0x005D7FAB)
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D8978, 754 bytes, `ret 0Ch` (/Os, ebp frame).
// Callees: CPersistContext::CheckCRC 0x00404500, CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0 / operator+=(const
//   char*) 0x0099F100, CBasicString<char>::operator== 0x004115A0, CStringParser copy 0x0046381B / dtor 0x004047A0 /
//   SkipToItem 0x009B9C00 / ReadAsSymbol 0x009BA790, CPersistTraitsSLONG::TransferIn(CPersistContext&, CStringParser&,
//   long&) 0x009E2590 (FableWin.pdb: 0x30 after TransferOut 0x009E2560, as 0x00B2BE00 after 0x00B2BDD0; both traits'
//   TransferIn are inline), CPersistTraits<unsigned long> / <long>::TransferOut 0x005D8185 / 0x005CC6C0, map erase(key)
//   0x005D8730 / insert 0x0045CA09 (modelled returning the iterator: retail homes a 4-byte result) / clear 0x00451046,
//   std::_Rb_global<bool>::_M_increment 0x004292C0, the binary map writer 0x0045C91D (mode 2, save stream) and reader
//   0x0045C95A (mode 3, load stream), CPersistContext::Error 0x009BAA90 and the context helpers 0x009BAB40 (after an
//   entry) / 0x009BB0D0 (clean the consumed text, start..end).
// Layout: CPersistContext per Ego_r.pdb (landed 0x00757160 / 0x009BBA40 models); CStringParser retail 0x28 bytes (landed
//   copy ctor 0x0046381B); STLport map node key +0x10, value +0x14, header pointer at the map's +0. The catalog label
//   "TransferMap<long,long>" is wrong: Ego_r.pdb spells the instantiation TransferMap<long, unsigned long>.
// Literals: "[" 0x01235434, "]" 0x01235430, ";" 0x0123542C, "] " 0x012353BC, ";\r\n" 0x012353B8, the three error messages
//   0x01235404 / 0x012353E0 / 0x012353C0.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <string.h>
#pragma intrinsic(strcmp)
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;           // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator+=(const char* text);         // 0x0099F100
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
class CBase { public: virtual ~CBase(); };                    // 0x0099A2E0
struct CStringParserCommentList { void* head; };
struct CCursorPos {
    bool InQuotes;                                     // +0x00
    long StringPos;                                    // +0x04
    long Line;                                         // +0x08
};
class CStringParser : public CBase {                  // retail 0x28 bytes
public:
    char*                    String;                   // +0x04
    unsigned long            BaseLineOffset;           // +0x08
    CCharString              Filename;                 // +0x0c
    CCharString              TempCharString;           // +0x10
    CStringParserCommentList Comments;                 // +0x14
    void (*ErrorCallback)(CCharString*);               // +0x18
    CCursorPos               CursorPos;                // +0x1c
    CStringParser(const CStringParser& other);         // 0x0046381B
    ~CStringParser();                                  // 0x004047A0
    bool SkipToItem(const CCharString& name);          // 0x009B9C00
    CCharString ReadAsSymbol();                        // 0x009BA790
};
class CPersistContext;
class CPersistTraitsSLONG {
public:
    static void __fastcall TransferIn(CPersistContext& context, CStringParser& parser, long& value);   // 0x009E2590
};
template <class T> class CPersistTraits;
template <> class CPersistTraits<unsigned long> {
public:
    unsigned long DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const unsigned long& value);   // 0x005D8185
};
template <> class CPersistTraits<long> {
public:
    long DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, long& value)
    {
        long read;
        CPersistTraitsSLONG::TransferIn(context, parser, read);
        value = read;
    }
    void TransferOut(CPersistContext& context, CCharString& out, const long& value);   // 0x005CC6C0
};
namespace std {
template <class T> struct less { };
template <class T> class allocator { };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
struct _Rb_tree_node_base {
    bool                _M_color;                      // +0x00
    _Rb_tree_node_base* _M_parent;                     // +0x04
    _Rb_tree_node_base* _M_left;                       // +0x08 (in the header: leftmost)
    _Rb_tree_node_base* _M_right;                      // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                             // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node);   // 0x004292C0 (_Rb_global<bool>)
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node<value_type>* _M_header;              // +0x00
    unsigned int erase(const _Key& key);               // 0x005D8730
    iterator insert(const value_type& value);   // 0x0045CA09
    void clear();                                      // 0x00451046
};
}
typedef std::map<unsigned long, long> CULongLongMap;
typedef std::_Rb_tree_node<std::pair<const unsigned long, long> > SMapNode;
struct CMemoryDataInputStream;
struct CDataOutputStream;
void __fastcall TransferMapOut_0045c91d(CDataOutputStream* stream, CULongLongMap& map);       // 0x0045C91D
void __fastcall TransferMapIn_0045c95a(CMemoryDataInputStream* stream, CULongLongMap& map);   // 0x0045C95A

struct CDefinitionManager;
struct CSymbolMap;
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
    void CheckCRC(const char* name);                      // 0x00404500
    void Error(const CStringParser& parser, const CCharString& message);   // 0x009BAA90
    void EntryTransferred();                              // 0x009BAB40
    void CleanText(long start, long end);                 // 0x009BB0D0
    template <class _Tp, class _Key> void TransferMap(const char* name, std::map<_Key, _Tp>& map, const _Tp& defaultValue);
};

template <>
void CPersistContext::TransferMap<long, unsigned long>(const char* name, std::map<unsigned long, long>& map, const long& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case 1:
        for (std::_Rb_tree_node_base* it = map._M_header->_M_left; it != map._M_header; it = std::_Rb_global<bool>::_M_increment(it)) {
            CPersistTraits<unsigned long> keyTraits;
            CPersistTraits<long> valueTraits;
            *PStringToSaveTo += name;
            *PStringToSaveTo += "[";
            keyTraits.TransferOut(*this, *PStringToSaveTo, ((SMapNode*)it)->_M_value_field.first);
            *PStringToSaveTo += "] ";
            valueTraits.TransferOut(*this, *PStringToSaveTo, ((SMapNode*)it)->_M_value_field.second);
            *PStringToSaveTo += ";\r\n";
        }
        break;
    case 2:
        TransferMapOut_0045c91d(PSaveStream, map);
        break;
    case 3:
        TransferMapIn_0045c95a(PLoadStream, map);
        break;
    case 4:
        map.clear();
        break;
    default: {
        CStringParser parser(*PParser);
        long nameLength = strlen(name);
        while (!Finished && parser.SkipToItem(name)) {
            long start = parser.CursorPos.StringPos - nameLength;
            if (parser.ReadAsSymbol() == "[") {
                CPersistTraits<long> valueTraits;
                long read;
                CPersistTraitsSLONG::TransferIn(*this, parser, read);
                unsigned long key = read;
                if (parser.ReadAsSymbol() == "]") {
                    long value;
                    valueTraits.TransferIn(*this, parser, value);
                    EntryTransferred();
                    map.erase(key);
                    map.insert(std::map<unsigned long, long>::value_type(read, value));
                    if (parser.ReadAsSymbol() == ";")
                        CleanText(start, parser.CursorPos.StringPos);
                    else
                        Error(parser, "Unable to find terminating semicolon.");
                } else
                    Error(parser, "Unable to find terminating bracket.");
            } else
                Error(parser, "Unable to find opening bracket.");
        }
        break;
    }
    }
}
