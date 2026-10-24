// Behaviour test for 0x007E3820 CPersistTraits<CDefString>::TransferIn: opening quote, def name up to the closing
// quote (handed to CDefString::SetString), closing quote; a missing opening quote reports "Unable to find quoted
// string" and leaves the value alone, a missing closing quote reports "Unable to find closing quote". The parser is a scripted stand-in.
#include <cstdio>
#include <cstring>

static int g_live = 0;
struct CCharString {
    char Buf[64];
    CCharString(const char* text, long length = -1) { std::strcpy(Buf, text); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(Buf, o.Buf); ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& other) { std::strcpy(Buf, other.Buf); return *this; }
    bool operator!=(const char* text) const { return std::strcmp(Buf, text) != 0; }
};

struct CStringParser {
    const char* Tokens[4];
    int         Next;
};
struct CStringParserReader : CStringParser {
    CCharString ReadAsSymbol() { return CCharString(Tokens[Next++]); }
    CCharString ReadAsStringUntilString(const CCharString& end) { return CCharString(Tokens[Next++]); }
};

static int g_errors = 0; static char g_message[64];
struct CPersistContext {
    void Error(const CStringParser& parser, const CCharString& message) { ++g_errors; std::strcpy(g_message, message.Buf); }
};

static char g_setName[64];
class CDefString {
public:
    long Index;
    void SetString(const CCharString& name) { std::strcpy(g_setName, name.Buf); Index = (long)std::strlen(name.Buf); }
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);
};

template <>
void CPersistTraits<CDefString>::TransferIn(CPersistContext* context, CStringParser* parser, CDefString* value)
{
    if (static_cast<CStringParserReader*>(parser)->ReadAsSymbol() != "\"")
    {
        context->Error(*parser, "Unable to find quoted string");
        return;
    }
    value->SetString(static_cast<CStringParserReader*>(parser)->ReadAsStringUntilString("\""));
    if (static_cast<CStringParserReader*>(parser)->ReadAsSymbol() != "\"")
        context->Error(*parser, "Unable to find closing quote");
}

int main()
{
    CPersistContext context;
    CPersistTraits<CDefString>* traits = (CPersistTraits<CDefString>*)new char[sizeof(CPersistTraits<CDefString>)];
    CDefString value; value.Index = -1;
    CStringParser ok = { { "\"", "Oakvale", "\"", "" }, 0 };
    traits->TransferIn(&context, &ok, &value);
    int pass = std::strcmp(g_setName, "Oakvale") == 0 && value.Index == 7 && ok.Next == 3 && g_errors == 0 && g_live == 0;
    CStringParser noOpen = { { "x", "Never", "\"", "" }, 0 };
    traits->TransferIn(&context, &noOpen, &value);
    pass = pass && value.Index == 7 && noOpen.Next == 1 && g_errors == 1 &&
           std::strcmp(g_message, "Unable to find quoted string") == 0 && g_live == 0;
    CStringParser noClose = { { "\"", "Bowerstone", ";", "" }, 0 };
    traits->TransferIn(&context, &noClose, &value);
    pass = pass && std::strcmp(g_setName, "Bowerstone") == 0 && value.Index == 10 && noClose.Next == 3 && g_errors == 2 &&
           std::strcmp(g_message, "Unable to find closing quote") == 0 && g_live == 0;
    delete[] (char*)traits;
    if (!pass) { std::printf("007e3820_TEST FAIL\n"); return 1; }
    std::printf("007e3820_TEST PASS\n");
    return 0;
}