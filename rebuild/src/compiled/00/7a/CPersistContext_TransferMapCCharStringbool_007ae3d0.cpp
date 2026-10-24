// Grade: RELOCATION_MATCH (1104/1104 on the exe span; the catalog row is OVERLONG at 1184 bytes: it runs past the `ret 0Ch` into
//   the 80 bytes of mode jump table + the next body -- re-cut the row to 1104 before landing, $S/bounds/AUDIT.tsv agrees).
//   Fix (2026-10-24 diag): the empty CPersistTraits<bool>::TransferIn gets its REAL body in the TU under auto_inline(off) +
//   noinline (GOTCHAS 24l recipe); VC7.1 then aliases the empty traits `this` onto the live `start` slot (frame 0x70) exactly
//   as retail. Previous grade: DIFFER(1104v1104 frame 0x74v0x70) -- ours gave the traits its own 4-byte slot.
// Leaf: TransferMap
// CPersistContext::TransferMap<CCharString, bool>(const char* name, map<CCharString, bool>& map, const bool& defaultValue) -- transfer a string-keyed bool map in the context's mode: text save (one "name[key] TRUE|FALSE;\r\n" line per entry), binary load/save through the stream helpers, clear, or text load ("name[key] value;" entries parsed from a copy of the parser, the old entry's value as the default, erased and re-inserted, the consumed text cleaned)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AE3D0, length 1084 bytes (+ the mode jump table 0x007AE810), `ret 0Ch`.
//   Catalog label "TransferMap<CObjectAugmentationParticleSet,EObjectAugmentationType>" is a propagated mislabel: the
//   value is bool (byte at node +0x14, "TRUE" 0x0122D6D4 / "FALSE" 0x0122D6CC), the sibling of 0x00756D00.
// Callees: CPersistContext::CheckCRC 0x00404500, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy 0x0099EC30 /
//   dtor 0x0099EAE0 / operator+=(const char*) 0x0099F100, CBasicString<char>::operator==(const char*) 0x004115A0, CStringParser
//   copy 0x0046381B / SkipToItem 0x009B9C00 / ReadAsSymbol 0x009BA790 (inline destructor: ~list 0x004046B0, ~CCharString,
//   ~CBase 0x0099A2E0), CPersistTraits<CCharString> TransferIn 0x0040E510 / TransferOut 0x0040E630, CPersistTraits<bool>
//   TransferIn 0x00403FF0 (TransferOut inline), the map's iterator ++ 0x007AB8A0 / lower_bound 0x007ACC90 / erase 0x007ADE60 /
//   insert 0x007AD060 / clear 0x007ADAF0, the binary map writer 0x007AD270 (mode 3) and reader 0x007ADBF0 (mode 2)
//   (__fastcall stream, map), CPersistContext::Error 0x009BAA90 / EntryTransferred 0x009BAB40 / CleanText 0x009BB0D0.
// Literals: "[" 0x01235434, "]" 0x01235430, ";" 0x0123542C, "] " 0x012353BC, ";\r\n" 0x012353B8, "" 0x0122D70E, the three error
//   messages 0x01235404 / 0x012353E0 / 0x012353C0.
// Layout: CPersistContext per Ego_r.pdb (landed 0x00757160 / 0x00463334 models); CStringParser retail 0x28 bytes; STLport map
//   node key +0x10, value +0x14, header pointer at the map's +0.
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
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator+=(const char* text);         // 0x0099F100
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
struct CBase { virtual ~CBase(); };                    // 0x0099A2E0
struct CStringParserCommentList {                      // std::list<CStringParserCommentBoundaries>
    void* _M_node;
    ~CStringParserCommentList();                       // 0x004046B0
};
struct CCursorPos {
    bool InQuotes;                                     // +0x00
    long StringPos;                                    // +0x04
    long Line;                                         // +0x08
};
struct CStringParser : CBase {                         // retail 0x28 bytes
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
struct CPersistContext;
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);   // 0x0040E510
    void TransferOut(CPersistContext* context, CCharString* out, T* value);       // 0x0040E630
};
template <> struct CPersistTraits<bool> {              // empty traits (retail `this` aliases an unrelated stack slot)
    void TransferIn(CPersistContext* context, CStringParser* parser, bool* value);   // 0x00403FF0, real body below
    void TransferOut(CPersistContext* context, CCharString* out, bool* value)
    {
        if (*value)
            *out += "TRUE";
        else
            *out += "FALSE";
    }
};
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8
// CPersistTraits<bool>::TransferIn 0x00403FF0 with its real shape in the TU (GOTCHAS 24l recipe: auto_inline(off) +
// noinline): the parsed token is compared case-insensitively with the TRUE/FALSE spellings. With the body visible VC7.1
// aliases the empty traits `this` onto the live `start` slot as retail does. Not landed at 0x00403FF0.
#pragma auto_inline(off)
__declspec(noinline) void CPersistTraits<bool>::TransferIn(CPersistContext* context, CStringParser* parser, bool* value)
{
    CCharString token = parser->ReadAsString();
    if (_stricmp(token.GetString(), CCharString("TRUE").GetString()) == 0 ||
        _stricmp(token.GetString(), CCharString("YES").GetString()) == 0)
        *value = true;
    else if (_stricmp(token.GetString(), CCharString("FALSE").GetString()) == 0 ||
             _stricmp(token.GetString(), CCharString("NO").GetString()) == 0)
        *value = false;
}
#pragma auto_inline(on)
struct SMapNode {                                      // STLport _Rb_tree_node<pair<const CCharString, bool> >
    long        _M_color;                              // +0x00
    SMapNode*   _M_parent;                             // +0x04
    SMapNode*   _M_left;                               // +0x08 (in the header: leftmost)
    SMapNode*   _M_right;                              // +0x0c
    CCharString first;                                 // +0x10
    bool        second;                                // +0x14
};
struct CMapIterator {                                  // STLport _Rb_tree_iterator
    SMapNode* _M_node;
    CMapIterator& operator++();                        // 0x007AB8A0
};
struct CMapValue {                                     // pair<const CCharString, bool>
    CCharString first;
    bool        second;
    CMapValue(const CCharString& key, const bool& value) : first(key), second(value) { }
};
struct CMapInsertResult {                              // pair<iterator, bool> (returned through a hidden pointer)
    SMapNode* _M_node;
    bool      second;
    CMapInsertResult() { }
};
struct CStringBoolMap {                                // STLport map<CCharString, bool>
    SMapNode* _M_header;                               // +0x00
    SMapNode* lower_bound(const CCharString& key);     // 0x007ACC90
    unsigned long erase(const CCharString& key);       // 0x007ADE60
    CMapInsertResult insert(const CMapValue& value);   // 0x007AD060
    void clear();                                      // 0x007ADAF0
};
struct CMemoryDataInputStream;
struct CDataOutputStream;
void __fastcall TransferMapOut_007ad270(CDataOutputStream* stream, CStringBoolMap& map);      // 0x007AD270
void __fastcall TransferMapIn_007adbf0(CMemoryDataInputStream* stream, CStringBoolMap& map);   // 0x007ADBF0

struct CDefinitionManager;
struct CSymbolMap;
struct CPersistContext {
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
    void TransferMap(const char* name, CStringBoolMap& map, const bool& defaultValue);
};

void CPersistContext::TransferMap(const char* name, CStringBoolMap& map, const bool& defaultValue)
{
    CheckCRC(name);
    CPersistTraits<CCharString> keyTraits;
    switch (Mode) {
    case 4:
        map.clear();
        return;
    case 3:
        TransferMapOut_007ad270(PSaveStream, map);
        return;
    case 2:
        TransferMapIn_007adbf0(PLoadStream, map);
        return;
    case 1: {
        CMapIterator it;
        for (it._M_node = map._M_header->_M_left; it._M_node != map._M_header; ++it) {
            SMapNode* node = it._M_node;
            *PStringToSaveTo += name;
            *PStringToSaveTo += "[";
            keyTraits.TransferOut(this, PStringToSaveTo, &node->first);
            *PStringToSaveTo += "] ";
            CPersistTraits<bool> valueTraits;
            valueTraits.TransferOut(this, PStringToSaveTo, &node->second);
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
                CPersistTraits<CCharString> traits;
                CCharString key;
                traits.TransferIn(this, &parser, &key);
                if (parser.ReadAsSymbol() == "]") {
                    bool value;
                    SMapNode* old = map.lower_bound(key);
                    if (old != map._M_header)
                        value = old->second;
                    else if (UseDefaults)
                        value = defaultValue;
                    CPersistTraits<bool> valueTraits;
                    valueTraits.TransferIn(this, &parser, &value);
                    EntryTransferred();
                    map.erase(key);
                    map.insert(CMapValue(key, value));
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