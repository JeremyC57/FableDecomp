// Grade: DIFFER(1116v1116) -- 2026-10-24 diag: real-body same-TU noinline CPersistTraits<CLongPair>::TransferIn/TransferOut stand-ins (GOTCHAS 24l recipe) fix the found-branch pair copy (was 1120v1116); the one residue left is the empty valueTraits `this`: ours overlays it on value.second ([esp+0x4c]), retail on the CMapValue temporary's first word ([esp+0x50]); declaration before `value`, a function-scope traits object and a by-value temporary (own slot, frame +4) are inert.
// CPersistContext::TransferMap<CCharString, pair<long,long> >(const char* name, map<CCharString, pair<long,long> >& map, const pair<long,long>& defaultValue) -- transfer a string-keyed long-pair map in the context's mode: text save (one "name[key] value;\r\n" line per entry), binary load/save through the stream helpers, clear, or text load ("name[key] value;" entries parsed from a copy of the parser, the old entry's value as the default, erased and re-inserted, the consumed text cleaned)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00756D00, length 1098 bytes (+ the mode jump table), `ret 0Ch`. Called by the
//   landed CPersistContext::TransferMap(const char*, CObjectAugmentationParticleSetMap&) 0x00757160 (catalog label
//   "TransferMap<CObjectAugmentationParticleSet,EObjectAugmentationType>": the value is CPersistTraits<pair<long,long> >).
// Callees: CPersistContext::CheckCRC 0x00404500, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy 0x0099EC30 /
//   dtor 0x0099EAE0 / operator+=(const char*) 0x0099F100, CBasicString<char>::operator==(const char*) 0x004115A0, CStringParser
//   copy 0x0046381B / SkipToItem 0x009B9C00 / ReadAsSymbol 0x009BA790 (its inline destructor: ~list 0x004046B0, ~CCharString,
//   ~CBase 0x0099A2E0), CPersistTraits<CCharString> TransferIn 0x0040E510 / TransferOut 0x0040E630, CPersistTraits<pair<long,
//   long> > TransferIn 0x00755F40 / TransferOut 0x00755A30, the map's iterator ++ 0x00755BB0 / lower_bound 0x00756350 / erase
//   0x00756C60 / insert 0x00756250 / clear 0x007563E0, the binary map writer 0x00756540 (mode 3, save stream) and reader
//   0x007565E0 (mode 2, load stream) (__fastcall stream, map), CPersistContext::Error 0x009BAA90 / EntryTransferred 0x009BAB40 /
//   CleanText 0x009BB0D0.
// Literals: "[" 0x01235434, "]" 0x01235430, ";" 0x0123542C, "] " 0x012353BC, ";\r\n" 0x012353B8, "" 0x0122D70E, the three error
//   messages 0x01235404 / 0x012353E0 / 0x012353C0.
// Layout: CPersistContext per Ego_r.pdb (landed 0x00757160 / 0x00463334 models); CStringParser retail 0x28 bytes; STLport map
//   node key +0x10, value +0x14, header pointer at the map's +0.
// Shape: the /O2 sibling of the landed /Os CPersistContext::TransferMap<CCharString, long> 0x00463334: every switch case ends
//   in `return`; the parser copy's destructor is inline; the value traits object doubles as the value (its DefaultVal is
//   zeroed, overwritten by the old entry or the default, and transferred in place).
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
};
struct CPersistContext;
struct CLongPair {                                     // std::pair<long, long>
    long first;
    long second;
    CLongPair() : first(0), second(0) { }
};
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);   // 0x0040E510
    void TransferOut(CPersistContext* context, CCharString* out, T* value);       // 0x0040E630
};
template <> struct CPersistTraits<CLongPair> {         // empty: VC passes `this` as the value's own address
    void TransferIn(CPersistContext* context, CStringParser* parser, CLongPair* value);   // 0x00755F40
    void TransferOut(CPersistContext* context, CCharString* out, CLongPair* value);       // 0x00755A30
};
void __fastcall TransferInLong_009e2590(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590
void __fastcall TransferOutLong_00755bb0(CPersistContext* context, CCharString* out, long* value);        // long writer (declared only)
// CPersistTraits<CLongPair>::TransferOut 0x00755A30 with its real shape in the TU (24l recipe, `__declspec(noinline)`):
// "(first,second)" through the long writer; not landed at 0x00755A30.
__declspec(noinline) void CPersistTraits<CLongPair>::TransferOut(CPersistContext* context, CCharString* out, CLongPair* value)
{
    *out += "(";
    TransferOutLong_00755bb0(context, out, &value->first);
    *out += ",";
    TransferOutLong_00755bb0(context, out, &value->second);
    *out += ")";
}
// CPersistTraits<CLongPair>::TransferIn 0x00755F40 with its real shape in the TU (GOTCHAS 24l recipe, `__declspec(noinline)`,
// 2026-10-24 diag): "( first , second )" read through the long reader; not landed at 0x00755F40.
__declspec(noinline) void CPersistTraits<CLongPair>::TransferIn(CPersistContext* context, CStringParser* parser, CLongPair* value)
{
    if (parser->ReadAsSymbol() == "(") {
        long read;
        TransferInLong_009e2590(context, parser, &read);
        value->first = read;
        if (parser->ReadAsSymbol() == ",") {
            TransferInLong_009e2590(context, parser, &read);
            value->second = read;
            parser->ReadAsSymbol();
        }
    }
}
struct SMapNode {                                      // STLport _Rb_tree_node<pair<const CCharString, pair<long,long> > >
    long        _M_color;                              // +0x00
    SMapNode*   _M_parent;                             // +0x04
    SMapNode*   _M_left;                               // +0x08 (in the header: leftmost)
    SMapNode*   _M_right;                              // +0x0c
    CCharString first;                                 // +0x10
    CLongPair   second;                                // +0x14
};
struct CMapIterator {                                  // STLport _Rb_tree_iterator
    SMapNode* _M_node;
    CMapIterator& operator++();                        // 0x00755BB0
};
struct CMapValue {                                     // pair<const CCharString, pair<long,long> >
    CCharString first;
    CLongPair   second;
    CMapValue(const CCharString& key, const CLongPair& value) : first(key), second(value) { }
};
struct CMapInsertResult {                              // pair<iterator, bool> (returned through a hidden pointer)
    SMapNode* _M_node;
    bool      second;
    CMapInsertResult() { }
};
struct CStringLongPairMap {                            // STLport map<CCharString, pair<long,long> >
    SMapNode* _M_header;                               // +0x00
    SMapNode* lower_bound(const CCharString& key);     // 0x00756350
    unsigned long erase(const CCharString& key);       // 0x00756C60
    CMapInsertResult insert(const CMapValue& value);   // 0x00756250
    void clear();                                      // 0x007563E0
};
struct CMemoryDataInputStream;
struct CDataOutputStream;
void __fastcall TransferMapOut_00756540(CDataOutputStream* stream, CStringLongPairMap& map);      // 0x00756540
void __fastcall TransferMapIn_007565e0(CMemoryDataInputStream* stream, CStringLongPairMap& map);   // 0x007565E0

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
    void TransferMap(const char* name, CStringLongPairMap& map, const CLongPair& defaultValue);
};

void CPersistContext::TransferMap(const char* name, CStringLongPairMap& map, const CLongPair& defaultValue)
{
    CheckCRC(name);
    CPersistTraits<CCharString> keyTraits;
    switch (Mode) {
    case 4:
        map.clear();
        return;
    case 3:
        TransferMapOut_00756540(PSaveStream, map);
        return;
    case 2:
        TransferMapIn_007565e0(PLoadStream, map);
        return;
    case 1: {
        CMapIterator it;
        for (it._M_node = map._M_header->_M_left; it._M_node != map._M_header; ++it) {
            SMapNode* node = it._M_node;
            *PStringToSaveTo += name;
            *PStringToSaveTo += "[";
            keyTraits.TransferOut(this, PStringToSaveTo, &node->first);
            *PStringToSaveTo += "] ";
            CPersistTraits<CLongPair> valueTraits;
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
                    CLongPair value;
                    SMapNode* old = map.lower_bound(key);
                    if (old != map._M_header)
                        value = old->second;
                    else if (UseDefaults)
                        value = defaultValue;
                    CPersistTraits<CLongPair> valueTraits;
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
