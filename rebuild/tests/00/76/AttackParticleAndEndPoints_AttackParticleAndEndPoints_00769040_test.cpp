// behaviour test for 00769040: AttackParticleAndEndPoints' copy constructor copies both dummy names
// (deep string copies), the two longs and re-points the particle-effect pointer at the same thing.
#include <cstdio>
#include <cstring>
#include <cstdlib>

class CThing { public: long m_id; };
class CBaseObject;

class CCharString {
public:
    char* PStringData;
    explicit CCharString(const char* text) { PStringData = (char*)std::malloc(std::strlen(text) + 1); std::strcpy(PStringData, text); }
    CCharString(const CCharString& other) { PStringData = (char*)std::malloc(std::strlen(other.PStringData) + 1); std::strcpy(PStringData, other.PStringData); }
    ~CCharString() { std::free(PStringData); }
};

struct CBaseIntelligentPointer {
    virtual long Kind() const { return 1; }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual long Kind() const { return 2; }
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
};

struct AttackParticleAndEndPoints {
    CCharString StartDummy;
    CCharString EndDummy;
    long        ParticleType;
    long        PeriodicFrames;
    CIntelligentPointer<CThing> ParticleEffect;
    AttackParticleAndEndPoints(const char* a, const char* b) : StartDummy(a), EndDummy(b), ParticleType(0), PeriodicFrames(0) { }
    AttackParticleAndEndPoints(const AttackParticleAndEndPoints& other)
        : StartDummy(other.StartDummy), EndDummy(other.EndDummy), ParticleType(other.ParticleType),
          PeriodicFrames(other.PeriodicFrames), ParticleEffect(other.ParticleEffect)
    {
    }
};

int main()
{
    CThing effect;
    int ok = 1;
    AttackParticleAndEndPoints source("hand_l", "target");
    effect.m_id = 9;
    source.ParticleType = 4;
    source.PeriodicFrames = 30;
    source.ParticleEffect.SetPItem((CBaseObject*)&effect);
    AttackParticleAndEndPoints copy(source);
    if (std::strcmp(copy.StartDummy.PStringData, "hand_l") != 0 || std::strcmp(copy.EndDummy.PStringData, "target") != 0)
        ok = 0;
    if (copy.StartDummy.PStringData == source.StartDummy.PStringData)
        ok = 0;
    if (copy.ParticleType != 4 || copy.PeriodicFrames != 30)
        ok = 0;
    if (copy.ParticleEffect.GetPItem() != &effect || copy.ParticleEffect.Kind() != 2)
        ok = 0;
    if (!ok) {
        std::printf("00769040_TEST FAIL\n");
        return 1;
    }
    std::printf("00769040_TEST PASS\n");
    return 0;
}