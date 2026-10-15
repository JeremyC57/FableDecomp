// Behaviour test for CParticleEmitter::GetPParticleSystemFromName (0x00AEE180): the first system
// whose name matches (same block, or equal length and text) is returned, otherwise null.
#include <cstdio>
#include <cstring>

struct CCharStringData {
    const char*   m_pData;
    unsigned long m_length;
};

static long Compare_00411570(const char* a, const char* b) { return std::strcmp(a, b); }

struct CCharString {
    CCharStringData* m_pData;
};

inline bool operator==(const CCharString& left, const CCharString& right)
{
    if (left.m_pData == right.m_pData)
        return true;
    if (left.m_pData && right.m_pData && left.m_pData->m_length == right.m_pData->m_length)
        return Compare_00411570(left.m_pData->m_pData, right.m_pData->m_pData) == 0;
    return false;
}

struct CParticleSystem {
    unsigned char _state[0x6c];
    CCharString   Name;
    const CCharString& GetName() const { return Name; }
};

struct CParticleSystemPtr { CParticleSystem* m_pObject; void* m_pCount; };
struct CParticleSystemPtrVector { CParticleSystemPtr* m_first; CParticleSystemPtr* m_last; CParticleSystemPtr* m_end; };

struct CParticleEmitter {
    virtual void v00() {}
    unsigned char            _base4[4];
    CParticleSystemPtrVector ParticleSystems;
    CParticleSystem* GetPParticleSystemFromName(const CCharString& name);
};

CParticleSystem* CParticleEmitter::GetPParticleSystemFromName(const CCharString& name)
{
    for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it) {
        const CCharString* systemName = &it->m_pObject->GetName();
        if (*systemName == name)
            return it->m_pObject;
    }
    return 0;
}

int main()
{
    CCharStringData fire = { "Fire", 4 };
    CCharStringData smoke = { "Smoke", 5 };
    CCharStringData smoke2 = { "Smoke", 5 };
    CCharStringData smog = { "Smog!", 5 };
    CParticleSystem s[3];
    s[0].Name.m_pData = 0;
    s[1].Name.m_pData = &fire;
    s[2].Name.m_pData = &smoke;
    CParticleSystemPtr p[3];
    for (int i = 0; i < 3; ++i) { p[i].m_pObject = &s[i]; p[i].m_pCount = 0; }
    CParticleEmitter e;
    e.ParticleSystems.m_first = p; e.ParticleSystems.m_last = p + 3; e.ParticleSystems.m_end = p + 3;
    CCharString byText; byText.m_pData = &smoke2;
    CCharString sameBlock; sameBlock.m_pData = &fire;
    CCharString other; other.m_pData = &smog;
    CCharString empty; empty.m_pData = 0;
    bool ok = e.GetPParticleSystemFromName(byText) == &s[2] && e.GetPParticleSystemFromName(sameBlock) == &s[1]
        && e.GetPParticleSystemFromName(other) == 0 && e.GetPParticleSystemFromName(empty) == &s[0];
    e.ParticleSystems.m_last = p;
    ok = ok && e.GetPParticleSystemFromName(byText) == 0;
    if (!ok) {
        std::printf("00aee180_TEST FAIL\n");
        return 1;
    }
    std::printf("00aee180_TEST PASS\n");
    return 0;
}
