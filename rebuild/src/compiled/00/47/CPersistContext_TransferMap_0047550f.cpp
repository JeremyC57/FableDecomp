// CPersistContext::TransferMap<CCharString, long>(const char* name, map<CCharString, long>& map, const long& defaultValue) -- transfer a string-keyed long map in the context's mode: text load ("name[key] value;" entries parsed from a copy of the parser, inserted, the consumed text cleaned), text save (one "name[key] value;\r\n" line per entry), binary load/save through the stream helpers, or clear
// Leaf: TransferMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047550F, 850 bytes, `ret 0Ch` (/Os, ebp frame).
// Callees: CPersistContext::CheckCRC 0x00404500, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy 0x0099EC30 /
//   dtor 0x0099EAE0 / operator+=(const char*) 0x0099F100, CStringParser copy 0x0046381B / dtor 0x004047A0 / SkipToItem
//   0x009B9C00 / ReadAsSymbol 0x009BA790, the CBasicString compare 0x00411570 (__fastcall), CPersistTraits<CCharString>
//   TransferIn 0x0040E510 / TransferOut 0x0040E630, the long transfer helpers 0x009E2590 / 0x009E2560 (__fastcall),
//   map<CCharString,long> find 0x00475861 / erase(key) 0x00475BF6 / insert 0x004758E3 / clear 0x00453630, the tree
//   increment 0x004292C0, the binary map writer 0x00475A51 (mode 2, save stream) and reader 0x00475AA6 (mode 3, load stream) (__fastcall stream, map), CPersistContext::Error 0x009BAA90 and
//   the two context helpers 0x009BAB40 (after an entry) / 0x009BB0D0 (clean the consumed text, start..end).
// Layout: CPersistContext per Ego_r.pdb (landed 0x00757160 / 0x009BBA40 models); CStringParser retail 0x28 bytes (landed
//   copy ctor 0x0046381B); STLport map node key +0x10, value +0x14, header pointer at the map's +0.
// Literals: "[" 0x01235434, "]" 0x01235430, ";" 0x0123542C, "] " 0x012353BC, ";\r\n" 0x012353B8, the three error messages
//   0x01235404 / 0x012353E0 / 0x012353C0.
// Shape (RST2 advisor): every switch case ends in `return` (not `break`), so the function-scope key traits' destructor is
//   referenced once per exit and its slot outranks the conditional-temp flag ([ebp-8], as retail); the binary cases are
//   mode 2 = TransferMapIn(PLoadStream) and mode 3 = TransferMapOut(PSaveStream) (the parked source had them swapped,
//   which the masked jump-target diff hid).
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
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
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
        return Compare_00411570(PStringData->Data.PData, text) == 0;
    }
};
struct CBase { virtual ~CBase(); };                    // 0x0099A2E0
struct CStringParserCommentList { void* head; };
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
    ~CStringParser();                                  // 0x004047A0
    bool SkipToItem(const CCharString& name);          // 0x009B9C00
    CCharString ReadAsSymbol();                        // 0x009BA790
};
struct CPersistContext;
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);   // 0x0040E510 for CCharString
    void TransferOut(CPersistContext* context, CCharString* out, T* value);       // 0x0040E630 for CCharString
};
void __fastcall TransferInLong_009e2590(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590
void __fastcall TransferOutLong_009e2560(CPersistContext* context, CCharString* out, long* value);       // 0x009E2560

struct SMapNode {                                      // STLport _Rb_tree_node<pair<const CCharString, long> >
    long        _M_color;                              // +0x00
    SMapNode*   _M_parent;                             // +0x04
    SMapNode*   _M_left;                               // +0x08 (in the header: leftmost)
    SMapNode*   _M_right;                              // +0x0c
    CCharString first;                                 // +0x10
    long        second;                                // +0x14
};
SMapNode* __cdecl _M_increment(SMapNode* node);        // 0x004292C0
struct CStringLongPair {
    CCharString first;
    long        second;
    CStringLongPair(const CCharString& key, const long& value) : first(key), second(value) { }
};
struct CStringLongInsertResult {                       // pair<iterator, bool> (non-POD: returned through a hidden pointer)
    SMapNode* _M_node;
    bool      second;
    CStringLongInsertResult() { }
};
struct CStringLongMap {                                // STLport map<CCharString, long>
    SMapNode* _M_header;                               // +0x00
    SMapNode* find(const CCharString& key);            // 0x00475861
    unsigned long erase(const CCharString& key);       // 0x00475BF6
    CStringLongInsertResult insert(const CStringLongPair& value);   // 0x004758E3
    void clear();                                      // 0x00453630
};
struct CMemoryDataInputStream;
struct CDataOutputStream;
void __fastcall TransferMapOut_00475a51(CDataOutputStream* stream, CStringLongMap& map);      // 0x00475A51
void __fastcall TransferMapIn_00475aa6(CMemoryDataInputStream* stream, CStringLongMap& map);   // 0x00475AA6

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
    void TransferMap(const char* name, CStringLongMap& map, const long& defaultValue);
};

void CPersistContext::TransferMap(const char* name, CStringLongMap& map, const long& defaultValue)
{
    CheckCRC(name);
    CPersistTraits<CCharString> keyTraits;
    switch (Mode) {
    case 1:
        for (SMapNode* it = map._M_header->_M_left; it != map._M_header; it = _M_increment(it)) {
            *PStringToSaveTo += name;
            *PStringToSaveTo += "[";
            keyTraits.TransferOut(this, PStringToSaveTo, &it->first);
            *PStringToSaveTo += "] ";
            long value = it->second;
            TransferOutLong_009e2560(this, PStringToSaveTo, &value);
            *PStringToSaveTo += ";\r\n";
        }
        return;
    case 2:
        TransferMapIn_00475aa6(PLoadStream, map);
        return;
    case 3:
        TransferMapOut_00475a51(PSaveStream, map);
        return;
    case 4:
        map.clear();
        return;
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
                    map.find(key);
                    long value;
                    TransferInLong_009e2590(this, &parser, &value);
                    EntryTransferred();
                    map.erase(key);
                    map.insert(CStringLongPair(key, value));
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