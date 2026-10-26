// CParticleEmitterDatabase::CParticleEmitterDatabase() -- constructs the symbol map, the empty emitter map and bank pointer, the two file names, and starts the handle counter at 1
// Leaf: ??0CParticleEmitterDatabase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE8BF0, 55 bytes.
// Callees: CSymbolMap::CSymbolMap @ 0x009E5200 (labelled "CParticleUpdateParameters<unsigned long,unsigned long>",
// propagated), CCharString::CCharString() @ 0x0099E4B0 (twice).
// Shape: PDB member order at retail offsets: SymbolMap +0 (0x24 bytes), UsingSymbolMap +0x24, Emitters +0x28 (the
// 16-byte retail CVectorMap: first/last/end zeroed, Compare +0x34 left alone, Dirty +0x35 cleared), EmitterBank
// +0x38 (counted pointer: object and block zeroed), CurrentHandle +0x40, ParFilename +0x44, HeaderFilename +0x48.
// The member objects' inline constructors share one zero register; CurrentHandle = 1 is stored after the string
// constructors, so it is a body assignment (an initialiser would be scheduled before them).
class CSymbolMap { public:
    unsigned char m_data[0x24];
    CSymbolMap();                                    // 0x009E5200
};

class CCharString { public:
    void* PStringData;
    CCharString();                                   // 0x0099E4B0
};

namespace NParticleEngine { class CParticleEmitter; class CParticleDataBank; }

struct SCountBlock;
template <class T> class CCountedPointer { public:
    T*           Object;
    SCountBlock* Block;
    CCountedPointer() : Object(0), Block(0) {}
};

struct SEmitterEntry {                               // pair<long, CCountedPointer<CParticleEmitter>>
    long                                              first;
    CCountedPointer<NParticleEngine::CParticleEmitter> second;
};

struct CEmitterVectorMap {                           // CVectorMap<long, CCountedPointer<CParticleEmitter>>
    SEmitterEntry* m_first;
    SEmitterEntry* m_last;
    SEmitterEntry* m_end;
    unsigned char  Compare;
    bool           Dirty;
    CEmitterVectorMap() : m_first(0), m_last(0), m_end(0), Dirty(false) {}
};

struct CParticleEmitterDatabase {
    CSymbolMap                                         SymbolMap;        // +0x00
    bool                                               UsingSymbolMap;   // +0x24
    CEmitterVectorMap                                  Emitters;         // +0x28
    CCountedPointer<NParticleEngine::CParticleDataBank> EmitterBank;     // +0x38
    long                                               CurrentHandle;    // +0x40
    CCharString                                        ParFilename;      // +0x44
    CCharString                                        HeaderFilename;   // +0x48
    CParticleEmitterDatabase();
};

CParticleEmitterDatabase::CParticleEmitterDatabase()
{
    CurrentHandle = 1;
}