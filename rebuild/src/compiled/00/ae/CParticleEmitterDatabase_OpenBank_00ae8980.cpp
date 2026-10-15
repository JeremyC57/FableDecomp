// CParticleEmitterDatabase::OpenBank(const CCharString&) -- creates a new particle data bank as EmitterBank, opens the bank file and builds the database's symbol map from it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE8980, 72 bytes (`ret 4`).
// Callees: operator new @ 0x00BFEA1A (0x234 bytes), CParticleDataBank::CParticleDataBank @ 0x00AF1670,
// CCountedPointer<CParticleDataBank>::Reset(T*) @ 0x00AE93E0 (labelled with another instantiation's name),
// the bank's virtual slot 1 (Open(name, 0x48)), CBankFile::CreateSymbolMap(CSymbolMap*, bool) @ 0x009CC530.
// Shape: `new` with the null-tested constructor call, the counted pointer's address kept in esi (`lea esi,
// [edi+38h]`) for Reset and both `EmitterBank->` reads; the symbol map is the member at +0 (`push edi`).
struct CCharString {
    void* PStringData;
};

struct CSymbolMap {
    unsigned char m_data[0x24];
};

struct CBankFile {                                                      // polymorphic root: vptr at +0
    virtual void v00();
    virtual void Open(const CCharString& fileName, long mode);          // slot 1 (+0x04)
    void CreateSymbolMap(CSymbolMap* symbolMap, bool reportDuplicates);   // 0x009CC530
};

namespace NParticleEngine {
struct CParticleDataBank : CBankFile {                                  // 0x234 bytes
    unsigned char m_data[0x230];
    CParticleDataBank();                                                // 0x00AF1670
};
}

struct SCountBlock;
template <class T> struct CCountedPointer {
    T*           Object;
    SCountBlock* Block;
    void Reset(T* object);                                              // 0x00AE93E0
    T* operator->() const { return Object; }
};

struct CParticleEmitterDatabase {
    CSymbolMap                                          SymbolMap;      // +0x00
    bool                                                UsingSymbolMap; // +0x24
    unsigned char                                       Emitters[0x10]; // +0x28 CVectorMap
    CCountedPointer<NParticleEngine::CParticleDataBank> EmitterBank;    // +0x38
    void OpenBank(const CCharString& fileName);
};

void CParticleEmitterDatabase::OpenBank(const CCharString& fileName)
{
    EmitterBank.Reset(new NParticleEngine::CParticleDataBank);
    EmitterBank->Open(fileName, 0x48);
    EmitterBank->CreateSymbolMap(&SymbolMap, false);
}