// Behaviour test (0x006ba480): CPersistTraits<CFaction>::TransferIn reads Name, NumberOfAllies + Ally entries, NumberOfEnemies + Enemy entries, then EndFaction; a missing keyword stops the read
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#ifdef _MSC_VER
typedef __int64 i64;
#define STRICMP _stricmp
#else
typedef long long i64;
#include <strings.h>
#define STRICMP strcasecmp
#endif
// Token-stream model of CStringParser: every Read* call consumes the next pre-split token.
class CCharString { public:
    char Text[128];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { std::strcpy(Text, s); }
    bool operator==(const char* s) const { return std::strcmp(Text, s) == 0; }
};
class CStringParser { public:
    const char* const* Tokens; int Count; int Pos;
    CStringParser(const char* const* tokens, int count) : Tokens(tokens), Count(count), Pos(0) {}
    CCharString Next() { return Pos < Count ? CCharString(Tokens[Pos++]) : CCharString(""); }
    CCharString ReadAsString() { return Next(); }
    CCharString ReadAsSymbol() { return Next(); }
    i64 ReadInt64() { CCharString t = Next(); i64 v = 0; const char* p = t.Text; bool neg = *p == '-'; if (neg) ++p; while (*p) v = v * 10 + (*p++ - '0'); return neg ? -v : v; }
    float ReadAsFloat() { return (float)std::atof(Next().Text); }
    bool NextItemIs(const char* name) { if (Pos < Count && std::strcmp(Tokens[Pos], name) == 0) { ++Pos; return true; } return false; }
};
class CPersistContext { public:
    int Errors; char LastError[160];
    CPersistContext() : Errors(0) { LastError[0] = 0; }
    void Error(const CStringParser&, const char* message) { ++Errors; std::strcpy(LastError, message); }
};
static void TransferInLongValue(CPersistContext*, CStringParser* parser, int* value) { *value = std::atoi(parser->Next().Text); }   // 0x009E2590 stand-in
static int TransferInLong(CPersistContext* context, CStringParser* parser) { int v; TransferInLongValue(context, parser, &v); return v; }
static void TransferInBool(CPersistContext* context, CStringParser* parser, bool* value)                  // 0x00403FF0
{
    CCharString token = parser->ReadAsString();
    if (STRICMP(token.Text, "TRUE") == 0 || STRICMP(token.Text, "BTRUE") == 0) *value = true;
    else if (STRICMP(token.Text, "FALSE") == 0 || STRICMP(token.Text, "BFALSE") == 0) *value = false;
    else { char m[160]; std::sprintf(m, "Unknown boolean value entry %s", token.Text); context->Error(*parser, m); }
}
static void TransferInFloat(CPersistContext*, CStringParser* parser, float* value) { *value = parser->ReadAsFloat(); }
#define COUNTOF(a) ((int)(sizeof(a) / sizeof((a)[0])))
struct CStringSet {                                   // set<CCharString> stand-in (insert ignores duplicates)
    CCharString Items[8]; int Size;
    void insert(const CCharString& s) { for (int i = 0; i < Size; ++i) if (std::strcmp(Items[i].Text, s.Text) == 0) return; Items[Size++] = s; }
    bool Has(const char* s) const { for (int i = 0; i < Size; ++i) if (std::strcmp(Items[i].Text, s) == 0) return true; return false; }
};
class CFaction { public: CCharString Name; CStringSet AlliedFactions; CStringSet EnemyFactions; };
static void TransferInString(CPersistContext*, CStringParser* parser, CCharString* value) { *value = parser->ReadAsString(); }   // 0x0040E510 stand-in
static void TransferIn(CPersistContext& context, CStringParser& parser, CFaction& value)
{
    if (!parser.NextItemIs("Name")) return;
    TransferInString(&context, &parser, &value.Name);
    {
        if (!parser.NextItemIs("NumberOfAllies")) return;
        int numberOfAllies; TransferInLongValue(&context, &parser, &numberOfAllies);
        for (int i = 0; i < numberOfAllies; ++i) {
            if (!parser.NextItemIs("Ally")) return;
            CCharString ally; TransferInString(&context, &parser, &ally);
            value.AlliedFactions.insert(ally);
        }
    }
    {
        if (!parser.NextItemIs("NumberOfEnemies")) return;
        int numberOfEnemies; TransferInLongValue(&context, &parser, &numberOfEnemies);
        for (int i = 0; i < numberOfEnemies; ++i) {
            if (!parser.NextItemIs("Enemy")) return;
            CCharString enemy; TransferInString(&context, &parser, &enemy);
            value.EnemyFactions.insert(enemy);
        }
    }
    if (!parser.NextItemIs("EndFaction")) return;
}
int main()
{
    const char* t[] = { "Name", "FACTION_GUARDS", "NumberOfAllies", "2", "Ally", "FACTION_HERO", "Ally", "FACTION_VILLAGERS",
                        "NumberOfEnemies", "1", "Enemy", "FACTION_BANDITS", "EndFaction" };
    CStringParser p(t, COUNTOF(t)); CPersistContext c;
    CFaction f; f.AlliedFactions.Size = 0; f.EnemyFactions.Size = 0;
    TransferIn(c, p, f);
    if (!(f.Name == "FACTION_GUARDS") || f.AlliedFactions.Size != 2 || !f.AlliedFactions.Has("FACTION_VILLAGERS") || f.EnemyFactions.Size != 1
        || !f.EnemyFactions.Has("FACTION_BANDITS") || p.Pos != COUNTOF(t)) { std::printf("006ba480_TEST FAIL\n"); return 1; }
    const char* u[] = { "Name", "F2", "NumberOfAllies", "3", "Ally", "A1", "Enemy", "E1" };
    CStringParser q(u, COUNTOF(u));
    CFaction g; g.AlliedFactions.Size = 0; g.EnemyFactions.Size = 0;
    TransferIn(c, q, g);
    if (!(g.Name == "F2") || g.AlliedFactions.Size != 1 || g.EnemyFactions.Size != 0 || q.Pos != 6) { std::printf("006ba480_TEST FAIL\n"); return 1; }
    const char* v[] = { "Title", "X" };
    CStringParser r(v, COUNTOF(v));
    CFaction h; h.Name = CCharString("KEEP"); h.AlliedFactions.Size = 0; h.EnemyFactions.Size = 0;
    TransferIn(c, r, h);
    if (!(h.Name == "KEEP") || r.Pos != 0) { std::printf("006ba480_TEST FAIL\n"); return 1; }
    std::printf("006ba480_TEST PASS\n");
    return 0;
}