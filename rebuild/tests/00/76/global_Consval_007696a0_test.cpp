// behaviour test for 007696a0: placement-copy-construct a ParticleAndEndPoints record into raw storage:
// both dummy names deep-copied, the three longs copied.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>

class CCharString {
public:
    char* PStringData;
    explicit CCharString(const char* text) { PStringData = (char*)std::malloc(std::strlen(text) + 1); std::strcpy(PStringData, text); }
    CCharString(const CCharString& other) { PStringData = (char*)std::malloc(std::strlen(other.PStringData) + 1); std::strcpy(PStringData, other.PStringData); }
    ~CCharString() { std::free(PStringData); }
};

struct ParticleAndEndPoints {
    CCharString StartDummy;
    CCharString EndDummy;
    long        ParticleType;
    long        m_secondaryValue;
    long        PeriodicFrames;
    ParticleAndEndPoints(const char* a, const char* b) : StartDummy(a), EndDummy(b), ParticleType(0), m_secondaryValue(0), PeriodicFrames(0) { }
};

static void Cons_val(ParticleAndEndPoints* ptr, const ParticleAndEndPoints& value)
{
    new ((void*)ptr) ParticleAndEndPoints(value);
}

int main()
{
    int ok = 1;
    ParticleAndEndPoints source("start", "end");
    void* raw = ::operator new(sizeof(ParticleAndEndPoints));
    ParticleAndEndPoints* dest = (ParticleAndEndPoints*)raw;
    source.ParticleType = 2;
    source.m_secondaryValue = -6;
    source.PeriodicFrames = 45;
    Cons_val(dest, source);
    if (std::strcmp(dest->StartDummy.PStringData, "start") != 0 || std::strcmp(dest->EndDummy.PStringData, "end") != 0)
        ok = 0;
    if (dest->EndDummy.PStringData == source.EndDummy.PStringData)
        ok = 0;
    if (dest->ParticleType != 2 || dest->m_secondaryValue != -6 || dest->PeriodicFrames != 45)
        ok = 0;
    dest->~ParticleAndEndPoints();
    ::operator delete(raw);
    if (!ok) {
        std::printf("007696a0_TEST FAIL\n");
        return 1;
    }
    std::printf("007696a0_TEST PASS\n");
    return 0;
}