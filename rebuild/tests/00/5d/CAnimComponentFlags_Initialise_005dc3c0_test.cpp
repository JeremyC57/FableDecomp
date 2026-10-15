// Behaviour test for CAnimComponentFlags::Initialise (0x005DC3C0).
#include <cstdio>
#include <cstdlib>

struct CCharString { const char* text; CCharString(const char* t, long) : text(t) {} ~CCharString() {} };

struct CStringParser {
    const char* text;
    CStringParser(const CCharString& t, void (*)(CCharString*), const CCharString&, long) : text(t.text) {}
};

struct CPersistContext { int calls; };

static void TransferIn(CPersistContext* context, CStringParser* parser, long* value)
{
    ++context->calls;
    *value = std::strtol(parser->text, 0, 16);
}

inline long ParseLong(CPersistContext& context, CStringParser& parser)
{
    long value;
    TransferIn(&context, &parser, &value);
    return value;
}

struct CAnimComponentFlags {
    long Flags; int resets;
    void Reset() { ++resets; }
    bool Initialise(CPersistContext& context, const CCharString& text)
    {
        Reset();
        CStringParser parser(text, 0, CCharString("", -1), 0);
        Flags |= ParseLong(context, parser);
        return true;
    }
};

int main()
{
    CPersistContext ctx; ctx.calls = 0;
    CAnimComponentFlags c; c.Flags = 0x100; c.resets = 0;
    CCharString t("0x0011", -1);
    if (!c.Initialise(ctx, t) || c.Flags != 0x111 || ctx.calls != 1 || c.resets != 1) {
        std::printf("005dc3c0_TEST FAIL\n");
        return 1;
    }
    std::printf("005dc3c0_TEST PASS\n");
    return 0;
}