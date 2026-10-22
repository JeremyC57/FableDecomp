// Behaviour test for NParticleEngine::CParticleEmitterManager::DumpInfo (0x00AEAC90): every emitter in the list adds
// "Emitter <name>" and " Pos(x, y, z)\r\n" (six-digit doubles), then "\tSystem\r\n" plus "\t\tDead\r\n" or
// "\t\tAlive\r\n" per particle system; the whole text is saved once to the named file.
#include <cstdio>
#include <string>

struct CWideString { std::string s; };
struct CAFile { std::string path; std::string saved; int saves; };
struct CDiskFileWin32 : CAFile {
    CDiskFileWin32(const CWideString& pathname, int, unsigned int) { path = pathname.s; saves = 0; }
};
static CAFile g_lastFile;
struct CCharString {
    std::string s;
    CCharString() { }
    CCharString(const char* text) : s(text) { }
    CCharString& operator+=(const CCharString& other) { s += other.s; return *this; }
    CCharString& operator+=(const char* text) { s += text; return *this; }
    void SaveToFile(CAFile& file) const { file.saved = s; ++file.saves; g_lastFile = file; }
    static CCharString FormatDouble(int digits, double value)
    {
        char b[64];
        sprintf(b, "%.*f", digits, value);
        return CCharString(b);
    }
};
CCharString operator+(const CCharString& a, const CCharString& b) { CCharString r = a; r.s += b.s; return r; }
CCharString operator+(const CCharString& a, const char* b) { CCharString r = a; r.s += b; return r; }
CCharString operator+(const char* a, const CCharString& b) { CCharString r(a); r.s += b.s; return r; }
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
namespace NParticleEngine {
struct CParticleSystem {
    bool Initialised : 1;
    bool Enabled : 1;
    bool Dead : 1;
};
struct CParticleSystemPtr { CParticleSystem* Data; void* Info; };
struct CParticleEmitter {
    CParticleSystemPtr* ParticleSystems_start;
    int                 count;
    C3DVector           Position;
    CCharString         Name;
    C3DVector GetPosition() const { return Position; }
    int GetNoParticles() { return count; }
};
struct SEmitterNode { SEmitterNode* _M_next; SEmitterNode* _M_prev; CParticleEmitter* _M_data; };
struct CParticleEmitterManager {
    SEmitterNode* Emitters;
    void DumpInfo(const CWideString& fileName);
};

void CParticleEmitterManager::DumpInfo(const CWideString& fileName)
{
    CCharString info;
    for (SEmitterNode* it = Emitters->_M_next; it != Emitters; it = it->_M_next) {
        CParticleEmitter* emitter = it->_M_data;
        info += "Emitter " + emitter->Name;
        info += " Pos(" + CCharString::FormatDouble(6, emitter->GetPosition().X) + ", " + CCharString::FormatDouble(6, emitter->GetPosition().Y)
            + ", " + CCharString::FormatDouble(6, emitter->GetPosition().Z) + ")\r\n";
        int count = emitter->GetNoParticles();
        for (int i = 0; i < count; ++i) {
            CParticleSystem* system = emitter->ParticleSystems_start[i].Data;
            info += "\tSystem\r\n";
            if (system->Dead)
                info += "\t\tDead\r\n";
            else
                info += "\t\tAlive\r\n";
        }
    }
    CDiskFileWin32 file(fileName, 4, 2);
    info.SaveToFile(file);
}
}
using namespace NParticleEngine;

int main()
{
    int ok = 1;
    CParticleSystem s0, s1;
    s0.Initialised = true; s0.Enabled = true; s0.Dead = false;
    s1.Initialised = true; s1.Enabled = false; s1.Dead = true;
    CParticleSystemPtr systems[2];
    systems[0].Data = &s0; systems[0].Info = 0;
    systems[1].Data = &s1; systems[1].Info = 0;

    CParticleEmitter e0;
    e0.ParticleSystems_start = systems; e0.count = 2;
    e0.Position.X = 1.5f; e0.Position.Y = -2.0f; e0.Position.Z = 0.25f;
    e0.Name = CCharString("Fire");
    CParticleEmitter e1;
    e1.ParticleSystems_start = 0; e1.count = 0;
    e1.Name = CCharString("Smoke");

    SEmitterNode head, n0, n1;
    head._M_next = &n0; n0._M_next = &n1; n1._M_next = &head;
    head._M_prev = &n1; n1._M_prev = &n0; n0._M_prev = &head;
    n0._M_data = &e0; n1._M_data = &e1; head._M_data = 0;

    CParticleEmitterManager manager;
    manager.Emitters = &head;
    CWideString name; name.s = "dump.txt";
    manager.DumpInfo(name);
    ok &= g_lastFile.path == "dump.txt" && g_lastFile.saves == 1;
    ok &= g_lastFile.saved ==
        "Emitter Fire Pos(1.500000, -2.000000, 0.250000)\r\n\tSystem\r\n\t\tAlive\r\n\tSystem\r\n\t\tDead\r\n"
        "Emitter Smoke Pos(0.000000, 0.000000, 0.000000)\r\n";

    // Empty list: an empty file is still written.
    SEmitterNode empty;
    empty._M_next = &empty; empty._M_prev = &empty; empty._M_data = 0;
    manager.Emitters = &empty;
    manager.DumpInfo(name);
    ok &= g_lastFile.saves == 1 && g_lastFile.saved == "";

    printf("00aeac90_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}