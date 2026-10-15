#include <cstdio>
// Behaviour of CParticleEmitterDatabase::OpenBank (0x00AE8980): a fresh bank becomes EmitterBank, it is opened on
// the given file (mode 0x48) and fills the database's own symbol map.
struct CCharString { const char* m_text; };
struct CSymbolMap { unsigned char m_data[0x24]; };
static const CCharString* g_openName = 0; static long g_openMode = 0; static CSymbolMap* g_filled = 0; static int g_banks = 0;
struct CBankFile {
    virtual ~CBankFile() {}
    virtual void Open(const CCharString& fileName, long mode) { g_openName = &fileName; g_openMode = mode; }
    void CreateSymbolMap(CSymbolMap* symbolMap, bool) { g_filled = symbolMap; }
};
struct CParticleDataBank : CBankFile { unsigned char m_data[0x230]; CParticleDataBank() { ++g_banks; } };
struct SCountBlock;
template <class T> struct CCountedPointer {
    T* Object; SCountBlock* Block;
    void Reset(T* object) { delete Object; Object = object; }
    T* operator->() const { return Object; }
};
struct CParticleEmitterDatabase {
    CSymbolMap SymbolMap; bool UsingSymbolMap; unsigned char Emitters[0x10];
    CCountedPointer<CParticleDataBank> EmitterBank;
    void OpenBank(const CCharString& fileName)
    {
        EmitterBank.Reset(new CParticleDataBank);
        EmitterBank->Open(fileName, 0x48);
        EmitterBank->CreateSymbolMap(&SymbolMap, false);
    }
};
int main()
{
    CParticleEmitterDatabase d; d.EmitterBank.Object = 0; d.EmitterBank.Block = 0;
    CCharString name; name.m_text = "particles.big";
    d.OpenBank(name);
    if (g_banks != 1 || d.EmitterBank.Object == 0 || g_openName != &name || g_openMode != 0x48 || g_filled != &d.SymbolMap) { std::printf("00ae8980_TEST FAIL\n"); return 1; }
    d.EmitterBank.Reset(0);
    std::printf("00ae8980_TEST PASS\n");
    return 0;
}