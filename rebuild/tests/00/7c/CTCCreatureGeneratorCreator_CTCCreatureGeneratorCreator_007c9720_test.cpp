#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static char g_seq[64] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
class CThing { public: int id; };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { note("I"); } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer() : CBaseIntelligentPointer() { } };
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { note("B"); }
};
struct CTCCreatureGeneratorCreator : CTCBase_Calls {
    long FrameToRegenerate; bool Generated; CIntelligentPointer PGenerator; u64 GeneratorUID;
    virtual void v00() { }
    CTCCreatureGeneratorCreator(CThing* thing) : CTCBase_Calls(thing), FrameToRegenerate(0), Generated(false), PGenerator(), GeneratorUID(0) { }
};
int main()
{
    CThing thing = { 1 };
    CTCCreatureGeneratorCreator* c = new CTCCreatureGeneratorCreator(&thing);
    if (std::strcmp(g_seq, "BI") || c->ParentThing != &thing || c->FrameToRegenerate != 0 || c->Generated || c->PGenerator.m_pItem != 0 || c->GeneratorUID != 0)
    { std::printf("007c9720_TEST FAIL\n"); return 1; }
    delete c;
    std::printf("007c9720_TEST PASS\n");
    return 0;
}