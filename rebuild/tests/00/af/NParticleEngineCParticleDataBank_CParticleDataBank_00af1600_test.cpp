// Behaviour test for 0x00AF1600 NParticleEngine::CParticleDataBank::~CParticleDataBank (destruction order)
#include <cstdio>
#include <cstring>
static char g_order[16]; static int g_n = 0;
static void Note(char c) { g_order[g_n++] = c; g_order[g_n] = 0; }
struct CBankFile { virtual ~CBankFile() { Note('B'); } };
struct CSecondary { virtual ~CSecondary() { Note('S'); } };
struct CArray { ~CArray() { Note('A'); } };
struct CSymbolMap { char tag; ~CSymbolMap() { Note(tag); } };
struct CParticleDataBank : CBankFile, CSecondary {
    CArray ParticleEmitters; CSymbolMap GraphicSymbols; CSymbolMap MeshSymbols;
    CParticleDataBank() { GraphicSymbols.tag = 'G'; MeshSymbols.tag = 'M'; }
    virtual ~CParticleDataBank() {}
};
int main()
{
    { CParticleDataBank bank; }
    if (std::strcmp(g_order, "MGASB") != 0) { std::printf("00af1600_TEST FAIL\n"); return 1; }
    std::printf("00af1600_TEST PASS\n");
    return 0;
}