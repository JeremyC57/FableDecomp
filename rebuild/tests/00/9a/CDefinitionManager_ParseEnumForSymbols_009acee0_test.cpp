// Behaviour test for 0x009ACEE0 CDefinitionManager::ParseEnumForSymbols (links the REAL row object, with the COMDATs it carries:
// CBasicString<char>::operator==(const char*) / Compare, CParsedItem's implicit ctor / dtor, CPersistTraits<long>).
// The engine callees outside the TU are defined here over raw native-layout storage; offsets the row reads: CDefinitionManager
// PSymbolMap +0x58 (0x009AD11F / 0x009AD159), ErrorCallback +0xB0 (0x009AD122); CParsedItem Type +0 (0x009ACF67), String +8;
// CCharString = one data pointer (null = empty string), its data block's text pointer at +0 (0x009AD0D1 `mov eax,[edi]`).
// A scripted token stream stands in for the parser. Expected (independent state-machine model below): optional string type name
// (else "NULL"), one symbol skipped, then per entry: "}" + ";" ends (else "Unexpected symbol in enum."), a non-string item is
// "Expected string in enum", then the separator: "," keeps the running value, "=" reads a long through CPersistTraitsSLONG with a
// CPersistContext(parser, ErrorCallback, true, this, PSymbolMap, 0) and makes it the running value, "}" ends WITHOUT adding the
// entry (";" expected), anything else is "Unknown symbol after '<name>'."; an added entry is AddSymbolDirty(type, name, value), an
// optional following "," item is consumed and the running value advances by one. Every string temporary must be destroyed.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StrData { char* text; unsigned long length; unsigned long bits; long refs; };
static int g_strings;
static StrData* MakeData(const char* text) {
    if (!text || !*text) return 0;                       // the empty string is the null data pointer
    StrData* d = (StrData*)calloc(1, sizeof(StrData)); d->length = (unsigned long)strlen(text);
    d->text = (char*)malloc(d->length + 1); memcpy(d->text, text, d->length + 1); d->refs = 1; ++g_strings; return d; }
static void FreeData(StrData* d) { if (d) { free(d->text); free(d); --g_strings; } }
static const char* Text(const StrData* d) { return d ? d->text : ""; }
static int g_objects;                                    // live CCharString objects (constructed minus destroyed)
class CCharString {
public:
    StrData* d;
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
    const CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
};
CCharString::CCharString() { d = 0; ++g_objects; }
CCharString::CCharString(const char* text, long length) { if (length != -1) printf("FAIL length arg %ld\n", length); d = MakeData(text); ++g_objects; }
CCharString::CCharString(const CCharString& other) { d = MakeData(Text(other.d)); ++g_objects; }
CCharString::~CCharString() { FreeData(d); d = (StrData*)0xDDDDDDDD; --g_objects; }
const CCharString& CCharString::operator=(const CCharString& other) { if (this != &other) { StrData* n = MakeData(Text(other.d)); FreeData(d); d = n; } return *this; }
const CCharString& CCharString::operator=(const char* text) { FreeData(d); d = MakeData(text); return *this; }
static CCharString Concat(const char* a, const char* b) { char buf[512]; strcpy(buf, a); strcat(buf, b); return CCharString(buf); }
CCharString __fastcall operator+(const CCharString& left, const CCharString& right) { return Concat(Text(left.d), Text(right.d)); }
CCharString __fastcall operator+(const CCharString& left, const char* right) { return Concat(Text(left.d), right); }

static int failures;
static void check(bool ok, const char* what, int script) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (script %d)\n", what, script); } }

// ---- the scripted token stream ----
enum EParsedItemType { PARSED_ITEM_NULL = 0, PARSED_ITEM_SYMBOL = 1, PARSED_ITEM_STRING = 2, PARSED_ITEM_INTEGER = 3, PARSED_ITEM_FLOAT = 4 };
struct Token { int type; const char* text; };
static const Token* g_tok; static int g_ntok, g_pos;
class CParsedItem { public: int Type; long Integer; CCharString String; const CCharString& GetString() const; const CCharString& GetSymbol() const; };
static int g_getstring, g_getsymbol;
const CCharString& CParsedItem::GetString() const { ++g_getstring; return String; }
const CCharString& CParsedItem::GetSymbol() const { ++g_getsymbol; return String; }
class CStringParser {
public:
    EParsedItemType PeekNextItemType();
    CCharString ReadAsString();
    CCharString ReadAsSymbol();
    bool ReadNextItem(CParsedItem& item);
    bool PeekNextItem(CParsedItem& item);
    int tag;
};
static CStringParser g_parser;
static void Fill(CParsedItem& item, const Token& t) { item.Type = t.type; item.String = t.text; }
EParsedItemType CStringParser::PeekNextItemType() { if (this != &g_parser) ++failures; return g_pos < g_ntok ? (EParsedItemType)g_tok[g_pos].type : PARSED_ITEM_NULL; }
CCharString CStringParser::ReadAsString() { if (this != &g_parser) ++failures; return CCharString(g_pos < g_ntok ? g_tok[g_pos++].text : ""); }
CCharString CStringParser::ReadAsSymbol() { if (this != &g_parser) ++failures; return CCharString(g_pos < g_ntok ? g_tok[g_pos++].text : ""); }
bool CStringParser::ReadNextItem(CParsedItem& item) { if (this != &g_parser) ++failures; if (g_pos >= g_ntok) return false; Fill(item, g_tok[g_pos++]); return true; }
bool CStringParser::PeekNextItem(CParsedItem& item) { if (this != &g_parser) ++failures; if (g_pos >= g_ntok) return false; Fill(item, g_tok[g_pos]); return true; }

// ---- the manager, the symbol map, the persist context ----
class CSymbolMap { public: void AddSymbolDirty(const CCharString& group, const CCharString& name, long value); int tag; };
static CSymbolMap g_map;
struct Added { char group[64]; char name[64]; long value; };
static Added g_added[64]; static int g_nadded;
void CSymbolMap::AddSymbolDirty(const CCharString& group, const CCharString& name, long value) {
    if (this != &g_map) ++failures;
    if (g_nadded < 64) { strcpy(g_added[g_nadded].group, Text(group.d)); strcpy(g_added[g_nadded].name, Text(name.d)); g_added[g_nadded].value = value; }
    ++g_nadded; }
static void __fastcall TheErrorCallback(const CCharString&) { }
static union { unsigned char b[0xD0]; double align; } g_manager;
class CDefinitionManager {
public:
    static void Run(CStringParser& parser) { ((CDefinitionManager*)g_manager.b)->ParseEnumForSymbols(parser); }
protected:
    void ParseEnumForSymbols(CStringParser& parser);
    void Error(const CStringParser& parser, const CCharString& message);
};
static char g_errors[4][128]; static int g_nerrors;
void CDefinitionManager::Error(const CStringParser& parser, const CCharString& message) {
    if ((void*)this != (void*)g_manager.b || &parser != &g_parser) ++failures;
    if (g_nerrors < 4) strcpy(g_errors[g_nerrors], Text(message.d));
    ++g_nerrors; }
class CPersistContext {
public:
    CPersistContext(CStringParser& parser, void (__fastcall* callback)(const CCharString&), bool flag, const CDefinitionManager* manager, const CSymbolMap* map, CCharString* text);
    unsigned char body[0x40];
};
static int g_contexts; static const CPersistContext* g_last_context;
CPersistContext::CPersistContext(CStringParser& parser, void (__fastcall* callback)(const CCharString&), bool flag, const CDefinitionManager* manager, const CSymbolMap* map, CCharString* text) {
    if (&parser != &g_parser || callback != TheErrorCallback || flag != true || (const void*)manager != (const void*)g_manager.b || map != &g_map || text != 0) { ++failures; printf("FAIL context args\n"); }
    ++g_contexts; g_last_context = this; }
class CPersistTraitsSLONG { public: static void __fastcall TransferIn(CPersistContext& context, CStringParser& parser, long& value); };
static int g_transfers;
void __fastcall CPersistTraitsSLONG::TransferIn(CPersistContext& context, CStringParser& parser, long& value) {
    if (&context != g_last_context || &parser != &g_parser) { ++failures; printf("FAIL transfer args\n"); }
    ++g_transfers; value = g_pos < g_ntok ? atol(g_tok[g_pos++].text) : -777; }

// ---- the independent model: a state machine over the same tokens ----
struct Model { Added added[64]; int nadded; char errors[4][128]; int nerrors; int pos; int contexts; };
static const char* Tok(const Token* t, int n, int& pos) { return pos < n ? t[pos++].text : ""; }
static void RunModel(const Token* t, int n, Model& m) {
    memset(&m, 0, sizeof m);
    int pos = 0; long running = 0; char type[64];
    if (pos < n && t[pos].type == PARSED_ITEM_STRING) strcpy(type, t[pos++].text); else strcpy(type, "NULL");
    Tok(t, n, pos);                                                       // the opening symbol, unchecked
    enum { ENTRY, CLOSE, DONE } state = ENTRY;
    while (state != DONE) {
        if (state == CLOSE) {
            if (strcmp(Tok(t, n, pos), ";") != 0) strcpy(m.errors[m.nerrors++], "Unexpected symbol in enum.");
            state = DONE; continue;
        }
        if (pos >= n) { state = DONE; continue; }
        const Token& item = t[pos++];
        if (item.type == PARSED_ITEM_SYMBOL && strcmp(item.text, "}") == 0) { state = CLOSE; continue; }
        if (item.type != PARSED_ITEM_STRING) { strcpy(m.errors[m.nerrors++], "Expected string in enum"); state = DONE; continue; }
        const char* sep = Tok(t, n, pos);
        long value = running;
        if (strcmp(sep, ",") == 0) { }
        else if (strcmp(sep, "=") == 0) { ++m.contexts; value = pos < n ? atol(t[pos++].text) : -777; running = value; }
        else if (strcmp(sep, "}") == 0) { state = CLOSE; continue; }
        else { sprintf(m.errors[m.nerrors++], "Unknown symbol after '%s'.", item.text); state = DONE; continue; }
        strcpy(m.added[m.nadded].group, type); strcpy(m.added[m.nadded].name, item.text); m.added[m.nadded].value = value; ++m.nadded;
        if (pos < n && t[pos].type == PARSED_ITEM_SYMBOL && strcmp(t[pos].text, ",") == 0) ++pos;
        ++running;
    }
    m.pos = pos;
}

static int g_cases;
static void RunScript(const Token* t, int n, int id) {
    g_tok = t; g_ntok = n; g_pos = 0; g_nadded = 0; g_nerrors = 0; g_contexts = 0; g_transfers = 0;
    int strings0 = g_strings, objects0 = g_objects;
    CDefinitionManager::Run(g_parser);
    Model m; RunModel(t, n, m);
    check(g_strings == strings0, "string data leaked or over-freed", id);
    check(g_objects == objects0, "string objects not balanced", id);
    check(g_pos == m.pos, "tokens consumed", id);
    check(g_nadded == m.nadded, "number of symbols added", id);
    for (int i = 0; i < m.nadded && i < g_nadded; ++i) {
        check(strcmp(g_added[i].group, m.added[i].group) == 0, "symbol type name", id);
        check(strcmp(g_added[i].name, m.added[i].name) == 0, "symbol name", id);
        check(g_added[i].value == m.added[i].value, "symbol value", id);
    }
    check(g_nerrors == m.nerrors, "number of errors", id);
    for (int j = 0; j < m.nerrors && j < g_nerrors; ++j) check(strcmp(g_errors[j], m.errors[j]) == 0, "error text", id);
    check(g_contexts == m.contexts && g_transfers == m.contexts, "persist contexts / transfers", id);
    ++g_cases;
}
#define S(x) { PARSED_ITEM_STRING, x }
#define Y(x) { PARSED_ITEM_SYMBOL, x }
#define I(x) { PARSED_ITEM_INTEGER, x }

static unsigned long g_seed = 12345;
static unsigned long Rnd() { g_seed = g_seed * 1103515245UL + 12345UL; return (g_seed >> 16) & 0x7FFF; }

int main() {
    memset(&g_manager, 0, sizeof g_manager);
    *(CSymbolMap**)(g_manager.b + 0x58) = &g_map;
    *(void (__fastcall**)(const CCharString&))(g_manager.b + 0xB0) = TheErrorCallback;
    static const Token t1[] = { S("Colours"), Y("{"), S("RED"), Y(","), S("GREEN"), Y(","), S("BLUE"), Y(","), Y("}"), Y(";") }; RunScript(t1, 10, 1);
    static const Token t2[] = { Y("{"), S("A"), Y(","), Y("}"), Y(";") }; RunScript(t2, 5, 2);                    // no type name: "NULL"
    static const Token t3[] = { S("T"), Y("{"), S("A"), Y("="), I("5"), Y(","), S("B"), Y(","), S("C"), Y("="), I("-3"), S("D"), Y(","), Y("}"), Y(";") }; RunScript(t3, 15, 3);
    static const Token t4[] = { S("T"), Y("{"), S("A"), Y(","), S("B"), Y("}"), Y(";") }; RunScript(t4, 7, 4);     // "}" separator: B not added
    static const Token t5[] = { S("T"), Y("{"), S("A"), Y(","), S("B"), Y("}"), Y(",") }; RunScript(t5, 7, 5);     // ... and no ";"
    static const Token t6[] = { S("T"), Y("{"), S("A"), Y(","), Y("}"), Y("}") }; RunScript(t6, 6, 6);            // "}" item, no ";"
    static const Token t7[] = { S("T"), Y("{"), S("A"), Y(","), Y("=") }; RunScript(t7, 5, 7);                     // symbol where a string is expected
    static const Token t8[] = { S("T"), Y("{"), S("A"), Y(","), I("7"), Y(",") }; RunScript(t8, 6, 8);             // integer item
    static const Token t9[] = { S("T"), Y("{"), S("Name"), Y(";") }; RunScript(t9, 4, 9);                          // unknown separator
    static const Token t10[] = { S("T"), Y("{"), S("Name"), Y("") }; RunScript(t10, 4, 10);                        // empty (null) separator
    static const Token t11[] = { S("T"), Y("{"), S("A"), Y(","), Y(""), Y(";") }; RunScript(t11, 6, 11);          // empty symbol item
    static const Token t12[] = { S("T"), Y("{"), S(""), Y(","), S("B"), Y(",") }; RunScript(t12, 6, 12);           // empty entry name
    static const Token t13[] = { S("T"), Y("{") }; RunScript(t13, 2, 13);                                          // stream ends
    static const Token t14[] = { S("T") }; RunScript(t14, 1, 14);
    RunScript(t14, 0, 15);                                                                                         // nothing at all
    static const Token t16[] = { S("T"), Y("{"), S("A"), Y("="), I("9"), S("B"), Y("="), I("2"), Y(","), S("C"), Y(","), Y("}"), Y(";") }; RunScript(t16, 13, 16);
    static const Token t17[] = { S("T"), Y("{"), S("A"), Y("==") }; RunScript(t17, 4, 17);                         // "==" is not "="
    static const Token t18[] = { S("T"), Y("{"), S("A"), Y("=x") }; RunScript(t18, 4, 18);
    static const Token t19[] = { S("T"), Y("{"), S("A"), Y(",,") }; RunScript(t19, 4, 19);
    static const Token t20[] = { S("T"), Y("{"), S("A"), Y("}}") }; RunScript(t20, 4, 20);
    static const Token t21[] = { S("T"), Y("{"), S("A"), Y(","), S("}"), Y(","), Y("}"), Y(";;") }; RunScript(t21, 8, 21);   // a STRING "}" is an entry; ";;" is not ";"
    static const Token t22[] = { S("T"), Y("{"), S("A"), Y(","), S(","), Y(","), S("B"), Y("="), I("4"), S(","), Y(","), Y("}"), Y(";") }; RunScript(t22, 13, 22);   // a STRING "," after a value is not consumed
    static const Token t23[] = { S("T"), Y("{"), S("A"), Y("=") }; RunScript(t23, 4, 23);                          // "=" at the end of the stream
    static const Token t24[] = { S("T"), Y("{"), S("A"), Y("<") }; RunScript(t24, 4, 24);                          // '<' sorts before '='
    static const Token t25[] = { S("T"), Y("{"), S("A"), Y(">") }; RunScript(t25, 4, 25);                          // '>' sorts after '='
    static const Token t26[] = { S("T"), Y("{"), S("A"), Y("}"), Y("") }; RunScript(t26, 5, 26);                  // "}" then an empty symbol
    static const Token t27[] = { S("T"), Y("{"), Y("}"), Y("") }; RunScript(t27, 4, 27);
    // random streams over a small alphabet
    static const Token alphabet[] = { S("A"), S("B"), S("Name"), S(""), Y(","), Y(","), Y("="), Y("}"), Y(";"), Y("{"), Y(""), I("3"), I("12"), S("T"), Y(",") , S("C"), Y("}"), Y(";") };
    static Token buf[24];
    for (int r = 0; r < 4000; ++r) {
        int n = (int)(Rnd() % 20);
        for (int i = 0; i < n; ++i) buf[i] = alphabet[Rnd() % (sizeof alphabet / sizeof alphabet[0])];
        if (r & 1) { buf[0] = alphabet[13]; if (n > 1) buf[1] = alphabet[9]; }
        // bias towards well-formed entries: string, separator pairs
        if (r % 3 == 0) for (int k = 2; k + 1 < n; k += 2) { buf[k] = alphabet[Rnd() % 3]; if (Rnd() % 4) buf[k + 1] = alphabet[4]; }
        RunScript(buf, n, 1000 + r);
    }
    if (failures) { printf("009acee0_TEST FAIL %d\n", failures); return 1; }
    printf("009acee0_TEST PASS (%d scripts)\n", g_cases);
    return 0;
}