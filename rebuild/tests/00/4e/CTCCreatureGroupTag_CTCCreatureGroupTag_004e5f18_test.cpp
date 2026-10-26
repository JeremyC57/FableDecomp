// Test for 004e5f18 CTCCreatureGroupTag::CTCCreatureGroupTag: base ctor runs, tagger UID and frame zeroed.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 UID64;
#else
typedef unsigned long long UID64;
#endif
class CThing { public: int id; };
static int g_baseCtorCalls = 0;
class CTCCreatureGroup { public:
    CThing* ParentThing;
    CTCCreatureGroup(CThing* owner) : ParentThing(owner) { ++g_baseCtorCalls; }
    virtual ~CTCCreatureGroup() {}
    virtual int Kind() const { return 1; }
};
class CTCCreatureGroupTag : public CTCCreatureGroup { public:
    UID64 TaggerUID;
    unsigned long TaggedFrame;
    CTCCreatureGroupTag(CThing* owner) : CTCCreatureGroup(owner), TaggerUID(0), TaggedFrame(0) {}
    virtual int Kind() const { return 2; }
};
int main()
{
    CThing owner = { 7 };
    CTCCreatureGroupTag* t = new CTCCreatureGroupTag(&owner);
    CTCCreatureGroup* b = t;
    if (g_baseCtorCalls != 1 || t->ParentThing != &owner || t->TaggerUID != 0 || t->TaggedFrame != 0 || b->Kind() != 2) {
        std::printf("004e5f18_TEST FAIL\n"); return 1;
    }
    delete t;
    std::printf("004e5f18_TEST PASS\n");
    return 0;
}