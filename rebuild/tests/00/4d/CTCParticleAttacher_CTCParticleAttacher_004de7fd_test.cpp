// Test for 004de7fd CTCParticleAttacher::CTCParticleAttacher: CTCBase(parent), empty def pointer and particle vector.
#include <cstdio>
class CThing { public: int id; };
static int g_baseCalls = 0;
class CTCBase { public:
    CThing* ParentThing; bool ToBeRemoved;
    CTCBase(CThing* parent) : ParentThing(parent), ToBeRemoved(false) { ++g_baseCalls; }
    virtual ~CTCBase() {}
    virtual int Kind() const { return 0; }
};
struct CDefPointer { void* Object; CDefPointer() : Object(0) {} };
struct CThingPointerVector { void* First; void* Last; void* End; CThingPointerVector() : First(0), Last(0), End(0) {} };
class CTCParticleAttacher : public CTCBase { public:
    CDefPointer PDef;
    CThingPointerVector ParticleList;
    CTCParticleAttacher(CThing* parent) : CTCBase(parent) {}
    virtual int Kind() const { return 1; }
};
int main()
{
    CThing t = { 5 };
    CTCParticleAttacher a(&t);
    const CTCBase& b = a;
    if (g_baseCalls != 1 || a.ParentThing != &t || a.PDef.Object != 0 || a.ParticleList.First != 0 || a.ParticleList.Last != 0
        || a.ParticleList.End != 0 || b.Kind() != 1) { std::printf("004de7fd_TEST FAIL\n"); return 1; }
    std::printf("004de7fd_TEST PASS\n");
    return 0;
}