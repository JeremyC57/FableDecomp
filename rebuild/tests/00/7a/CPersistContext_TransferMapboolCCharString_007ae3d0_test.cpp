// Behaviour test for CPersistContext::TransferMap<bool, CCharString> (map<CCharString, bool>) @ 0x007AE3D0 (src/007ae3d0.cpp).
// Self-contained: the callees (CCharString, the string parser, the map, the context helpers) are small stand-ins; the
// TransferMap body and CPersistTraits<bool>::TransferIn are the reconstruction's logic verbatim.
#include <cstdio>
#include <cstring>
#include <cstdlib>

struct CCharString {
    char* Text;
    CCharString() : Text(0) { }
    CCharString(const char* text, long length = -1) { Text = (char*)std::malloc(std::strlen(text) + 1); std::strcpy(Text, text); }
    CCharString(const CCharString& o) { Text = 0; if (o.Text) { Text = (char*)std::malloc(std::strlen(o.Text) + 1); std::strcpy(Text, o.Text); } }
    ~CCharString() { if (Text) std::free(Text); }
    CCharString& operator=(const CCharString& o) { if (this != &o) { if (Text) std::free(Text); Text = 0; if (o.Text) { Text = (char*)std::malloc(std::strlen(o.Text) + 1); std::strcpy(Text, o.Text); } } return *this; }
    const char* GetString() const { return Text ? Text : ""; }
    CCharString& operator+=(const char* text) { char* n = (char*)std::malloc(std::strlen(GetString()) + std::strlen(text) + 1); std::strcpy(n, GetString()); std::strcat(n, text); if (Text) std::free(Text); Text = n; return *this; }
    bool operator==(const char* text) const { return std::strcmp(GetString(), text) == 0; }
};

struct CCursorPos { bool InQuotes; long StringPos; long Line; };
// Parser stand-in over a flat text: SkipToItem finds the next `name` not inside a word, ReadAsSymbol returns one
// punctuation character or an identifier, ReadAsString an identifier.
struct CStringParser {
    const char* String;
    CCursorPos  CursorPos;
    CStringParser(const char* s) : String(s) { CursorPos.InQuotes = false; CursorPos.StringPos = 0; CursorPos.Line = 0; }
    CStringParser(const CStringParser& o) : String(o.String), CursorPos(o.CursorPos) { }
    void SkipSpace() { while (String[CursorPos.StringPos] == ' ' || String[CursorPos.StringPos] == '\r' || String[CursorPos.StringPos] == '\n') ++CursorPos.StringPos; }
    bool SkipToItem(const CCharString& name) {
        const char* hit = std::strstr(String + CursorPos.StringPos, name.GetString());
        if (!hit) return false;
        CursorPos.StringPos = (long)(hit - String) + (long)std::strlen(name.GetString());
        return true;
    }
    CCharString ReadAsSymbol() {
        SkipSpace();
        char c = String[CursorPos.StringPos];
        if (c == '[' || c == ']' || c == ';') { char b[2] = { c, 0 }; ++CursorPos.StringPos; return CCharString(b); }
        return ReadAsString();
    }
    CCharString ReadAsString() {
        SkipSpace();
        char b[64]; int n = 0;
        while (String[CursorPos.StringPos] && n < 63) {
            char c = String[CursorPos.StringPos];
            if (c == '[' || c == ']' || c == ';' || c == ' ' || c == '\r' || c == '\n') break;
            b[n++] = c; ++CursorPos.StringPos;
        }
        b[n] = 0;
        return CCharString(b);
    }
};

struct CPersistContext;
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);
    void TransferOut(CPersistContext* context, CCharString* out, T* value);
};
template <> struct CPersistTraits<bool> {
    void TransferIn(CPersistContext* context, CStringParser* parser, bool* value);
    void TransferOut(CPersistContext* context, CCharString* out, bool* value)
    {
        if (*value)
            *out += "TRUE";
        else
            *out += "FALSE";
    }
};
// the reconstruction's real-body stand-in (identical logic)
void CPersistTraits<bool>::TransferIn(CPersistContext* context, CStringParser* parser, bool* value)
{
    CCharString token = parser->ReadAsString();
    if (_stricmp(token.GetString(), CCharString("TRUE").GetString()) == 0 ||
        _stricmp(token.GetString(), CCharString("YES").GetString()) == 0)
        *value = true;
    else if (_stricmp(token.GetString(), CCharString("FALSE").GetString()) == 0 ||
             _stricmp(token.GetString(), CCharString("NO").GetString()) == 0)
        *value = false;
}
void CPersistTraits<CCharString>::TransferIn(CPersistContext*, CStringParser* parser, CCharString* value) { *value = parser->ReadAsString(); }
void CPersistTraits<CCharString>::TransferOut(CPersistContext*, CCharString* out, CCharString* value) { *out += value->GetString(); }

// std::map<CCharString, bool> stand-in: a sorted list walked like the node list (_M_header == end); find = map::find over _M_find
struct SMapNode { CCharString first; bool second; SMapNode* next; };
struct CMapValue { CCharString first; bool second; CMapValue(const CCharString& k, const bool& v) : first(k), second(v) { } };
struct CMapInsertResult { SMapNode* _M_node; bool second; CMapInsertResult() { } };
struct CStringBoolMap {
    SMapNode* _M_header;      // sentinel: end()
    SMapNode* head;
    long count;
    CStringBoolMap() { _M_header = new SMapNode; _M_header->next = 0; _M_header->second = false; head = 0; count = 0; }
    SMapNode* begin() { return head ? head : _M_header; }
    SMapNode* Next(SMapNode* n) { return n->next ? n->next : _M_header; }
    SMapNode* find(const CCharString& key) {
        for (SMapNode* n = head; n; n = n->next) if (std::strcmp(n->first.GetString(), key.GetString()) == 0) return n;
        return _M_header;
    }
    unsigned long erase(const CCharString& key) {
        SMapNode** pp = &head;
        while (*pp) { if (std::strcmp((*pp)->first.GetString(), key.GetString()) == 0) { SMapNode* d = *pp; *pp = d->next; delete d; --count; return 1; } pp = &(*pp)->next; }
        return 0;
    }
    CMapInsertResult insert(const CMapValue& v) {
        CMapInsertResult r;
        SMapNode* n = new SMapNode; n->first = v.first; n->second = v.second; n->next = 0;
        SMapNode** pp = &head; while (*pp && std::strcmp((*pp)->first.GetString(), v.first.GetString()) < 0) pp = &(*pp)->next;
        n->next = *pp; *pp = n; ++count; r._M_node = n; r.second = true; return r;
    }
    void clear() { while (head) { SMapNode* d = head; head = d->next; delete d; } count = 0; }
};

struct CPersistContext {
    CStringParser* PParser;
    bool           UseDefaults;
    bool           Finished;
    long           Mode;
    CCharString*   PStringToSaveTo;
    long           Entries;
    long           Errors;
    long           CleanedFrom, CleanedTo;
    void CheckCRC(const char*) { }
    void Error(const CStringParser&, const CCharString&) { ++Errors; }
    void EntryTransferred() { ++Entries; }
    void CleanText(long start, long end) { CleanedFrom = start; CleanedTo = end; }
    void TransferMap(const char* name, CStringBoolMap& map, const bool& defaultValue);
};

// ---- the reconstruction's TransferMap logic (binary modes 2/3 stubbed out: their helpers are separate retail bodies) ----
void CPersistContext::TransferMap(const char* name, CStringBoolMap& map, const bool& defaultValue)
{
    CheckCRC(name);
    CPersistTraits<CCharString> keyTraits;
    switch (Mode) {
    case 4:
        map.clear();
        return;
    case 3:
    case 2:
        return;
    case 1: {
        for (SMapNode* node = map.begin(); node != map._M_header; node = map.Next(node)) {
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
        long nameLength = (long)std::strlen(name);
        while (!Finished && parser.SkipToItem(name)) {
            long start = parser.CursorPos.StringPos - nameLength;
            if (parser.ReadAsSymbol() == "[") {
                CPersistTraits<CCharString> traits;
                CCharString key;
                traits.TransferIn(this, &parser, &key);
                if (parser.ReadAsSymbol() == "]") {
                    bool value;
                    SMapNode* old = map.find(key);
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

int main()
{
    bool ok = true;
    // text load (mode 0): two entries, one with a missing semicolon (error, still inserted), an unknown token keeps the default
    CStringParser parser("Flags[alpha] TRUE; Flags[beta] false; Flags[gamma] maybe; Flags[delta] YES");
    CPersistContext ctx; ctx.PParser = &parser; ctx.UseDefaults = true; ctx.Finished = false; ctx.Mode = 0; ctx.Entries = 0; ctx.Errors = 0; ctx.CleanedFrom = ctx.CleanedTo = -1;
    CStringBoolMap map;
    bool defaultValue = true;
    ctx.TransferMap("Flags", map, defaultValue);
    ok = ok && map.count == 4 && ctx.Entries == 4 && ctx.Errors == 1;
    SMapNode* a = map.find(CCharString("alpha")); SMapNode* b = map.find(CCharString("beta"));
    SMapNode* g = map.find(CCharString("gamma")); SMapNode* d = map.find(CCharString("delta"));
    ok = ok && a != map._M_header && a->second == true && b != map._M_header && b->second == false;
    ok = ok && g != map._M_header && g->second == true /* default kept */ && d != map._M_header && d->second == true;
    ok = ok && ctx.CleanedFrom == 38 && ctx.CleanedTo == 57;   // the last cleaned entry "Flags[gamma] maybe;" (delta has no semicolon)
    // text save (mode 1): one line per entry, sorted keys
    CCharString out;
    ctx.Mode = 1; ctx.PStringToSaveTo = &out;
    ctx.TransferMap("Flags", map, defaultValue);
    ok = ok && std::strcmp(out.GetString(), "Flags[alpha] TRUE;\r\nFlags[beta] FALSE;\r\nFlags[delta] TRUE;\r\nFlags[gamma] TRUE;\r\n") == 0;
    // clear (mode 4)
    ctx.Mode = 4;
    ctx.TransferMap("Flags", map, defaultValue);
    ok = ok && map.count == 0 && map.begin() == map._M_header;
    // text load with an existing entry: the old value is the default when the token is unknown
    CStringParser parser2("Flags[alpha] nonsense;");
    map.insert(CMapValue(CCharString("alpha"), false));
    ctx.PParser = &parser2; ctx.Mode = 0; ctx.UseDefaults = false;
    ctx.TransferMap("Flags", map, defaultValue);
    a = map.find(CCharString("alpha"));
    ok = ok && map.count == 1 && a != map._M_header && a->second == false;
    if (!ok) { std::printf("007ae3d0_TEST FAIL count=%ld entries=%ld errors=%ld cleaned=%ld..%ld out=[%s]\n", map.count, ctx.Entries, ctx.Errors, ctx.CleanedFrom, ctx.CleanedTo, out.GetString()); return 1; }
    std::printf("007ae3d0_TEST PASS\n");
    return 0;
}