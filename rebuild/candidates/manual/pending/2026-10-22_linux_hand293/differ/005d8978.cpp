// Grade: DIFFER(754v754) -- frame slots only: the discarded insert result temp sits at -0x3c (retail -0x30), so value, the ";" temp and start each sit one slot off; everything else (incl. the save-loop map-traits object on [ebp+0xc]) matches (advisor RST2 best)
// CPersistContext::TransferMap<EKey, EValue>(const char* name, map<EKey, EValue>& map, const EValue& defaultValue) -- transfer an enum-keyed enum map in the context's mode: text load ("name[key] value;" entries parsed from a copy of the parser; each replaces any existing entry for its key, the consumed text is cleaned), text save (one "name[key] value;\r\n" line per entry), binary save/load through the stream helpers, or clear
// Leaf: TransferMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D8978, 754 bytes, `ret 0Ch` (/Os, ebp frame).
// Callees: CPersistContext::CheckCRC 0x00404500, CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0 / operator+=(const
//   char*) 0x0099F100, CBasicString<char>::operator== 0x004115A0, CStringParser copy 0x0046381B / dtor 0x004047A0 /
//   SkipToItem 0x009B9C00 / ReadAsSymbol 0x009BA790, the long reader 0x009E2590 (__fastcall; both traits' TransferIn are
//   inline), the key / value traits' TransferOut 0x005D8185 / 0x005CC6C0, map erase(key) 0x005D8730 / insert 0x0045CA09 /
//   clear 0x00451046, the tree increment 0x004292C0, the binary map writer 0x0045C91D (mode 2, save stream) and reader
//   0x0045C95A (mode 3, load stream), CPersistContext::Error 0x009BAA90 and the context helpers 0x009BAB40 (after an
//   entry) / 0x009BB0D0 (clean the consumed text, start..end).
// Layout: CPersistContext per Ego_r.pdb (landed 0x00757160 / 0x009BBA40 models); CStringParser retail 0x28 bytes (landed
//   copy ctor 0x0046381B); STLport map node key +0x10, value +0x14, header pointer at the map's +0. The catalog label
//   "TransferMap<long,long>" names the shape: both traits read a long; the enum types themselves are not recovered.
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
void __fastcall TransferInLong_009e2590(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590
enum EKey { EKEY_FIRST };
enum EValue { EVALUE_FIRST };
template <class T> struct CPersistTraits {             // enum traits: read as a long, written out of line
    T DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value)
    {
        long read;
        TransferInLong_009e2590(context, parser, &read);
        *value = (T)read;
    }
    void TransferOut(CPersistContext* context, CCharString* out, T* value);   // 0x005D8185 (EKey), 0x005CC6C0 (EValue)
};

struct SMapNode {                                      // STLport _Rb_tree_node<pair<const EKey, EValue> >
    long      _M_color;                                // +0x00
    SMapNode* _M_parent;                               // +0x04
    SMapNode* _M_left;                                 // +0x08 (in the header: leftmost)
    SMapNode* _M_right;                                // +0x0c
    EKey      first;                                   // +0x10
    EValue    second;                                  // +0x14
};
SMapNode* __cdecl _M_increment(SMapNode* node);        // 0x004292C0
struct CEnumMapTraits {                                // one traits object for the map entry (key and value writers)
    void TransferOut(CPersistContext* context, CCharString* out, EKey* value);     // 0x005D8185
    void TransferOut(CPersistContext* context, CCharString* out, EValue* value);   // 0x005CC6C0
};
struct CEnumPair {
    EKey   first;
    EValue second;
    CEnumPair(const EKey& key, const EValue& value) : first(key), second(value) { }
};
struct CEnumMapIterator {                              // STLport _Rb_tree_iterator (non-POD: returned through a hidden pointer)
    SMapNode* _M_node;
    CEnumMapIterator() { }
};
struct CEnumMap {                                      // STLport map<EKey, EValue>
    SMapNode* _M_header;                               // +0x00
    unsigned long erase(const EKey& key);              // 0x005D8730
    CEnumMapIterator insert(const CEnumPair& value);   // 0x0045CA09
    void clear();                                      // 0x00451046
};
struct CMemoryDataInputStream;
struct CDataOutputStream;
void __fastcall TransferMapOut_0045c91d(CDataOutputStream* stream, CEnumMap& map);       // 0x0045C91D
void __fastcall TransferMapIn_0045c95a(CMemoryDataInputStream* stream, CEnumMap& map);   // 0x0045C95A

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
    void TransferMap(const char* name, CEnumMap& map, const EValue& defaultValue);
};

void CPersistContext::TransferMap(const char* name, CEnumMap& map, const EValue& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case 1:
        for (SMapNode* it = map._M_header->_M_left; it != map._M_header; it = _M_increment(it)) {
            CEnumMapTraits traits;
            *PStringToSaveTo += name;
            *PStringToSaveTo += "[";
            traits.TransferOut(this, PStringToSaveTo, &it->first);
            *PStringToSaveTo += "] ";
            traits.TransferOut(this, PStringToSaveTo, &it->second);
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
                CPersistTraits<EValue> valueTraits;
                long read;
                TransferInLong_009e2590(this, &parser, &read);
                EKey key = (EKey)read;
                if (parser.ReadAsSymbol() == "]") {
                    EValue value;
                    valueTraits.TransferIn(this, &parser, &value);
                    EntryTransferred();
                    map.erase(key);
                    map.insert(CEnumPair((EKey)read, value));
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
