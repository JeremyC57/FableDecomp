// Behaviour test for NParticleEngine::CParticleEmitterDatabase::GenerateHeaderFile @ 0x00AE8680: the generated header text.
#include <cstdio>
#include <cstring>

static char g_file[512];
class CCharString { public:
    char text[96];
    CCharString(const char* t) { std::strcpy(text, t); }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); }
    CCharString& operator=(const CCharString& o) { std::strcpy(text, o.text); return *this; }
    CCharString ToUpper() const { CCharString r(*this); for (char* p = r.text; *p; ++p) if (*p >= 'a' && *p <= 'z') *p = (char)(*p - 32); return r; }
};
static CCharString operator+(const char* l, const CCharString& r) { CCharString s(l); std::strcat(s.text, r.text); return s; }
static CCharString operator+(const CCharString& l, const char* r) { CCharString s(l); std::strcat(s.text, r); return s; }
static CCharString operator+(const CCharString& l, const CCharString& r) { CCharString s(l); std::strcat(s.text, r.text); return s; }
static CCharString GFIntToCharString(long v) { char b[16]; std::sprintf(b, "%ld", v); return CCharString(b); }
class CAFile { public: int dummy; };
static CAFile& operator<<(CAFile& f, const CCharString& t) { std::strcat(g_file, t.text); return f; }
struct CParticleEmitter { CCharString Name; CParticleEmitter(const char* n) : Name(n) {} };
struct CEmitterEntry { long Handle; CParticleEmitter* Data; void* Info; };
struct CParticleEmitterDatabase {
    CEmitterEntry* m_entriesBegin; CEmitterEntry* m_entriesEnd;
    void GenerateHeaderFile();
};
void CParticleEmitterDatabase::GenerateHeaderFile()
{
    CAFile file;
    file << CCharString("enum EParticleEmitter\r\n{\r\n");
    long index = 1;
    for (CEmitterEntry* it = m_entriesBegin; it != m_entriesEnd; ++it) {
        CCharString name(it->Data->Name);
        name = name.ToUpper();
        CCharString line = "\t" + name + " = " + GFIntToCharString(index) + ",\r\n";
        file << line;
        ++index;
    }
    file << CCharString("};\r\n");
}
int main()
{
    CParticleEmitter a("fire_small"), b("Smoke");
    CEmitterEntry e[2] = { { 5, &a, 0 }, { 9, &b, 0 } };
    CParticleEmitterDatabase db; db.m_entriesBegin = e; db.m_entriesEnd = e + 2;
    db.GenerateHeaderFile();
    int ok = std::strcmp(g_file, "enum EParticleEmitter\r\n{\r\n\tFIRE_SMALL = 1,\r\n\tSMOKE = 2,\r\n};\r\n") == 0;
    if (!ok) { std::printf("00ae8680_TEST FAIL\n"); return 1; }
    std::printf("00ae8680_TEST PASS\n");
    return 0;
}