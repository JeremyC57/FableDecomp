#include <cstdio>
// Behaviour of CParticleEmitterDatabase::CParticleEmitterDatabase (0x00AE8BF0): symbol map and both file names
// constructed, the emitter map empty and clean, the bank pointer null, the handle counter at 1.
static int g_symbolMaps = 0; static int g_strings = 0;
struct CSymbolMap { unsigned char m_data[0x24]; CSymbolMap() { ++g_symbolMaps; } };
struct CCharString { void* PStringData; CCharString() : PStringData(0) { ++g_strings; } };
struct CParticleEmitter; struct CParticleDataBank; struct SCountBlock;
template <class T> struct CCountedPointer { T* Object; SCountBlock* Block; CCountedPointer() : Object(0), Block(0) {} };
struct SEmitterEntry { long first; CCountedPointer<CParticleEmitter> second; };
struct CEmitterVectorMap {
    SEmitterEntry* m_first; SEmitterEntry* m_last; SEmitterEntry* m_end; unsigned char Compare; bool Dirty;
    CEmitterVectorMap() : m_first(0), m_last(0), m_end(0), Dirty(false) {}
};
struct CParticleEmitterDatabase {
    CSymbolMap SymbolMap; bool UsingSymbolMap; CEmitterVectorMap Emitters;
    CCountedPointer<CParticleDataBank> EmitterBank; long CurrentHandle;
    CCharString ParFilename; CCharString HeaderFilename;
    CParticleEmitterDatabase() { CurrentHandle = 1; }
};
int main()
{
    CParticleEmitterDatabase d;
    if (g_symbolMaps != 1 || g_strings != 2 || d.CurrentHandle != 1 || d.Emitters.m_first != 0 || d.Emitters.m_last != 0 || d.Emitters.m_end != 0 || d.Emitters.Dirty || d.EmitterBank.Object != 0 || d.EmitterBank.Block != 0) { std::printf("00ae8bf0_TEST FAIL\n"); return 1; }
    std::printf("00ae8bf0_TEST PASS\n");
    return 0;
}
