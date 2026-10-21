// Behaviour test for 0x0074B900 CTCCreatureGenerator::Persist: the generation parameters always transfer with their
// defaults; the live state only with the flag, Active defaulting to SelfTrigger and NumTriggersLeft to NumTriggers.
#include <cstdio>
#include <cstring>

struct CCharString { const char* text; CCharString() : text("") {} CCharString(const CCharString& o) : text(o.text) {} ~CCharString() {} };
inline CCharString EmptyName() { CCharString name; return name; }
struct CDefIndexArray { int n; };

struct CPersistContext {
    int count; char names[32][48]; long defs[32];
    CPersistContext() : count(0) {}
    void Note(const char* n, long d) { std::strcpy(names[count], n); defs[count] = d; ++count; }
    void Transfer(const char* n, CDefIndexArray&) { Note(n, 0); }
    void Transfer(const char* n, float&, const float& d) { Note(n, (long)d); }
    void Transfer(const char* n, bool&, const bool& d) { Note(n, d ? 1 : 0); }
    void Transfer(const char* n, long&, const long& d) { Note(n, d); }
    void Transfer(const char* n, CCharString&, const CCharString& d) { Note(n, (long)std::strlen(d.text)); }
};
struct TGenerationParameters {
    float GenerationRadius, SelfTriggerRadius; bool SelfTrigger; long SelfTriggerResetInterval; bool TriggerOnActivate;
    long ActiveCreatureLimit, TotalGenerationLimit, NumTriggers; CCharString ScriptNameOfAllGeneratedCreatures;
};
struct CTCCreatureGenerator {
    CDefIndexArray CreatureFamilies; TGenerationParameters GenParams;
    bool Active, ReadyToInitialise; long NumTriggersLeft; bool Destroyed;
    void Persist(CPersistContext* context, bool flag)
    {
        context->Transfer("CreatureFamilies", CreatureFamilies);
        context->Transfer("GenerationRadius", GenParams.GenerationRadius, 0.0f);
        context->Transfer("SelfTriggerRadius", GenParams.SelfTriggerRadius, 0.0f);
        context->Transfer("SelfTrigger", GenParams.SelfTrigger, false);
        context->Transfer("SelfTriggerResetInterval", GenParams.SelfTriggerResetInterval, 0L);
        context->Transfer("TriggerOnActivate", GenParams.TriggerOnActivate, false);
        context->Transfer("ActiveCreatureLimit", GenParams.ActiveCreatureLimit, -1L);
        context->Transfer("TotalGenerationLimit", GenParams.TotalGenerationLimit, -1L);
        context->Transfer("NumTriggers", GenParams.NumTriggers, -1L);
        {
            CCharString defaultName = EmptyName();
            context->Transfer("ScriptNameOfAllGeneratedCreatures", GenParams.ScriptNameOfAllGeneratedCreatures, defaultName);
        }
        if (flag) {
            context->Transfer("Active", Active, GenParams.SelfTrigger);
            context->Transfer("ReadyToInitialise", ReadyToInitialise, true);
            context->Transfer("NumTriggersLeft", NumTriggersLeft, GenParams.NumTriggers);
            context->Transfer("Destroyed", Destroyed, false);
        }
    }
};

int main()
{
    CTCCreatureGenerator g;
    g.GenParams.SelfTrigger = true; g.GenParams.NumTriggers = 7;
    CPersistContext a, b;
    g.Persist(&a, false);
    g.Persist(&b, true);
    bool ok = a.count == 10 && b.count == 14 && a.defs[6] == -1 && std::strcmp(a.names[9], "ScriptNameOfAllGeneratedCreatures") == 0
        && b.defs[10] == 1 && b.defs[11] == 1 && b.defs[12] == 7 && std::strcmp(b.names[13], "Destroyed") == 0 && b.defs[13] == 0;
    if (!ok) { std::printf("0074b900_TEST FAIL\n"); return 1; }
    std::printf("0074b900_TEST PASS\n");
    return 0;
}
