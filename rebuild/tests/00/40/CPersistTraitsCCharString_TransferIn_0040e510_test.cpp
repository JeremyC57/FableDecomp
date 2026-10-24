// Behaviour test for 0x0040E510 CPersistTraits<CCharString>::TransferIn: opening quote, text up to the closing quote,
// closing quote; a missing opening quote reports an error and leaves the value alone, a missing closing quote reports
// an error after the value has been read. The parser is a scripted stand-in.
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

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);
};

template <>
void CPersistTraits<CCharString>::TransferIn(CPersistContext* context, CStringParser* parser, CCharString* value)
{
    if (static_cast<CStringParserReader*>(parser)->ReadAsSymbol() != "\"")
    {
        context->Error(*parser, "Unable to find closing quote");
        return;
    }
    *value = static_cast<CStringParserReader*>(parser)->ReadAsStringUntilString("\"");
    if (static_cast<CStringParserReader*>(parser)->ReadAsSymbol() != "\"")
        context->Error(*parser, "Unable to find closing quote");
}

int main()
{
    CPersistContext context;
    CPersistTraits<CCharString>* traits = (CPersistTraits<CCharString>*)new char[sizeof(CPersistTraits<CCharString>)];
    CCharString value("old");
    CStringParser ok = { { "\"", "Oakvale", "\"", "" }, 0 };
    traits->TransferIn(&context, &ok, &value);
    int pass = std::strcmp(value.Buf, "Oakvale") == 0 && ok.Next == 3 && g_errors == 0 && g_live == 1;
    CStringParser noOpen = { { "x", "Never", "\"", "" }, 0 };
    traits->TransferIn(&context, &noOpen, &value);
    pass = pass && std::strcmp(value.Buf, "Oakvale") == 0 && noOpen.Next == 1 && g_errors == 1 &&
           std::strcmp(g_message, "Unable to find closing quote") == 0 && g_live == 1;
    CStringParser noClose = { { "\"", "Bowerstone", ";", "" }, 0 };
    traits->TransferIn(&context, &noClose, &value);
    pass = pass && std::strcmp(value.Buf, "Bowerstone") == 0 && noClose.Next == 3 && g_errors == 2 && g_live == 1;
    delete[] (char*)traits;
    if (!pass) { std::printf("0040e510_TEST FAIL\n"); return 1; }
    std::printf("0040e510_TEST PASS\n");
    return 0;
}