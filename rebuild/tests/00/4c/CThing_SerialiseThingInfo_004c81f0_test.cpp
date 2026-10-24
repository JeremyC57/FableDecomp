// Behaviour test for 0x004C81F0 CThing::SerialiseThingInfo(context, info): three named transfers in order -- "Player"
// into the signed-char field (default 0), "UID" into the 64-bit UID (default 0), "DefinitionType" into the string (default
// empty) -- each writing through to the info record (a loading context fills the record from its source).
#include <cstdio>
#include <cstring>

struct Str { char text[32]; Str() { text[0] = 0; } Str(const Str& o) { std::strcpy(text, o.text); } };
static Str EmptyStr() { Str s; return s; }
struct Info { unsigned long long UID; Str DefinitionType; signed char Player; };
struct Context {
    int order; bool ok;
    void Transfer(const char* n, signed char& v, const signed char& d) { ok = ok && order++ == 0 && !std::strcmp(n, "Player") && d == 0; v = 3; }
    void Transfer(const char* n, unsigned long long& v, const unsigned long long& d) { ok = ok && order++ == 1 && !std::strcmp(n, "UID") && d == 0; v = 0x1122334455667788ULL; }
    void Transfer(const char* n, Str& v, const Str& d) { ok = ok && order++ == 2 && !std::strcmp(n, "DefinitionType") && d.text[0] == 0; std::strcpy(v.text, "CREATURE_HERO"); }
};

static void SerialiseThingInfo(Context& c, Info& i)
{
    c.Transfer("Player", i.Player, (signed char)0);
    c.Transfer("UID", i.UID, (unsigned long long)0);
    c.Transfer("DefinitionType", i.DefinitionType, EmptyStr());
}

int main()
{
    Context c; c.order = 0; c.ok = true;
    Info i; i.UID = 0; i.Player = -1;
    SerialiseThingInfo(c, i);
    if (!c.ok || c.order != 3 || i.Player != 3 || i.UID != 0x1122334455667788ULL || std::strcmp(i.DefinitionType.text, "CREATURE_HERO") != 0) {
        std::printf("004c81f0_TEST FAIL\n"); return 1;
    }
    std::printf("004c81f0_TEST PASS\n");
    return 0;
}