#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
#include <string.h>
#pragma intrinsic(strcmp)
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
};
class CCharStringData { public:
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
class CStringParser : public CBase { public:                         // retail 0x28 bytes
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
    unsigned long erase(const CCharString& key);       // 0x0047536C
    CStringLongInsertResult insert(const CStringLongPair& value);   // 0x00431606
    void clear();                                      // 0x0043027A
};
class CMemoryDataInputStream;
class CDataOutputStream;
void __fastcall TransferMapOut_004314c8(CDataOutputStream* stream, CStringLongMap& map);      // 0x004314C8
void __fastcall TransferMapIn_0043151d(CMemoryDataInputStream* stream, CStringLongMap& map);   // 0x0043151D

class CDefinitionManager;
class CSymbolMap;
class CPersistContext { public:
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
    protected: void CheckCRC(const char* name); public:                      // 0x00404500
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
        TransferMapIn_0043151d(PLoadStream, map);
        return;
    case 3:
        TransferMapOut_004314c8(PSaveStream, map);
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

static int g_live = 0, g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("check failed line %d\n", __LINE__); ++g_fail; } } while (0)
static CCharStringData* MakeData(const char* t) {
    CCharStringData* d = (CCharStringData*)malloc(sizeof(CCharStringData));
    size_t n = strlen(t); d->Data.PData = (char*)malloc(n + 1); memcpy(d->Data.PData, t, n + 1);
    d->Data.StringLength = (unsigned long)n; d->Data.DataLength = (unsigned long)n; d->Data.UseFastExtend = false; d->NoRefs = 1; ++g_live; return d;
}
static void FreeData(CCharStringData* d) { if (d) { free(d->Data.PData); free(d); --g_live; } }
static const char* Text(const CCharString& c) { return c.PStringData ? c.PStringData->Data.PData : ""; }
long __fastcall Compare_00411570(const char* a, const char* b) { return strcmp(a, b); }
CCharString::CCharString() : PStringData(0) { }
CCharString::CCharString(const char* text, long) : PStringData(MakeData(text)) { }
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData ? MakeData(other.PStringData->Data.PData) : 0) { }
CCharString::~CCharString() { FreeData(PStringData); PStringData = 0; }
CCharString& CCharString::operator+=(const char* text) {
    const char* old = Text(*this); char* buf = (char*)malloc(strlen(old) + strlen(text) + 1);
    strcpy(buf, old); strcat(buf, text); FreeData(PStringData); PStringData = MakeData(buf); free(buf); return *this;
}
CBase::~CBase() { }
static const char* g_tokens[8]; static int g_tok = 0, g_skips = 0, g_skipLimit = 0;
CStringParser::CStringParser(const CStringParser& o) : CBase(o), String(o.String), BaseLineOffset(o.BaseLineOffset), Filename(o.Filename),
    TempCharString(o.TempCharString), Comments(o.Comments), ErrorCallback(o.ErrorCallback), CursorPos(o.CursorPos) { }
CStringParser::~CStringParser() { }
bool CStringParser::SkipToItem(const CCharString& name) { CHECK(strcmp(Text(name), "Name") == 0); if (g_skips++ >= g_skipLimit) return false; CursorPos.StringPos = 20; return true; }
CCharString CStringParser::ReadAsSymbol() { CursorPos.StringPos += 2; return CCharString(g_tokens[g_tok++]); }
template <class T> void CPersistTraits<T>::TransferIn(CPersistContext*, CStringParser*, T* value) { value->~T(); new (value) T("Key1"); }
template <class T> void CPersistTraits<T>::TransferOut(CPersistContext*, CCharString* out, T* value) { *out += Text(*value); }
void __fastcall TransferInLong_009e2590(CPersistContext*, CStringParser*, long* value) { *value = 42; }
void __fastcall TransferOutLong_009e2560(CPersistContext*, CCharString* out, long* value) { char b[32]; sprintf(b, "%ld", *value); *out += b; }
SMapNode* __cdecl _M_increment(SMapNode* node) { return node->_M_right; }
static int g_find = 0, g_erase = 0, g_insert = 0, g_clear = 0, g_in = 0, g_out = 0; static char g_insKey[32]; static long g_insVal = 0; static void* g_stream = 0;

unsigned long CStringLongMap::erase(const CCharString& key) { CHECK(strcmp(Text(key), "Key1") == 0); ++g_erase; return 0; }
CStringLongInsertResult CStringLongMap::insert(const CStringLongPair& value) { ++g_insert; strcpy(g_insKey, Text(value.first)); g_insVal = value.second; CStringLongInsertResult r; r._M_node = _M_header; return r; }
void CStringLongMap::clear() { ++g_clear; }
void __fastcall TransferMapOut_004314c8(CDataOutputStream* stream, CStringLongMap&) { ++g_out; g_stream = stream; }
void __fastcall TransferMapIn_0043151d(CMemoryDataInputStream* stream, CStringLongMap&) { ++g_in; g_stream = stream; }
static int g_crc = 0, g_err = 0, g_entries = 0; static long g_cleanStart = -1, g_cleanEnd = -1;
void CPersistContext::CheckCRC(const char* name) { CHECK(strcmp(name, "Name") == 0); ++g_crc; }
void CPersistContext::Error(const CStringParser&, const CCharString&) { ++g_err; }
void CPersistContext::EntryTransferred() { ++g_entries; }
void CPersistContext::CleanText(long start, long end) { g_cleanStart = start; g_cleanEnd = end; }

int main()
{
    CPersistContext c; memset(&c, 0, sizeof(c));
    CCharString saved; c.PStringToSaveTo = &saved;
    c.PLoadStream = (CMemoryDataInputStream*)0x1111; c.PSaveStream = (CDataOutputStream*)0x2222;
    SMapNode header, n1, n2; header._M_left = &n1; n1._M_right = &n2; n2._M_right = &header;
    new (&n1.first) CCharString("A"); n1.second = 1; new (&n2.first) CCharString("B"); n2.second = 2;
    CStringLongMap map; map._M_header = &header; long def = 0;
    int live0 = g_live;
    c.Mode = 1; c.TransferMap("Name", map, def);
    CHECK(strcmp(Text(saved), "Name[A] 1;\r\nName[B] 2;\r\n") == 0); CHECK(g_live == live0 + 1);
    c.Mode = 2; c.TransferMap("Name", map, def); CHECK(g_in == 1 && g_out == 0 && g_stream == (void*)0x1111);
    c.Mode = 3; c.TransferMap("Name", map, def); CHECK(g_in == 1 && g_out == 1 && g_stream == (void*)0x2222);
    c.Mode = 4; c.TransferMap("Name", map, def); CHECK(g_clear == 1);
    static char raw[sizeof(CStringParser)]; memset(raw, 0, sizeof(raw));
    CStringParser* proto = (CStringParser*)raw; c.PParser = proto;
    g_tokens[0] = "["; g_tokens[1] = "]"; g_tokens[2] = ";"; g_tok = 0; g_skips = 0; g_skipLimit = 1;
    c.Mode = 0; c.Finished = false; c.TransferMap("Name", map, def);
    CHECK(g_insert == 1 && strcmp(g_insKey, "Key1") == 0 && g_insVal == 42);
    CHECK(g_erase == 1); CHECK(g_entries == 1 && g_err == 0);
    CHECK(g_cleanStart == 20 - 4 && g_cleanEnd == 26);
    g_tokens[0] = "("; g_tok = 0; g_skips = 0; g_skipLimit = 1; c.TransferMap("Name", map, def);
    CHECK(g_err == 1 && g_insert == 1);
    c.Finished = true; g_tok = 0; g_skips = 0; c.TransferMap("Name", map, def); CHECK(g_tok == 0);
    CHECK(g_live == live0 + 1 && g_crc == 7);
    if (g_fail) { printf("00475026_TEST FAIL\n"); return 1; }
    printf("00475026_TEST PASS\n");
    return 0;
}