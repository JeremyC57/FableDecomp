// CParticleEmitterDatabase::CreateSymbolMap() -- rebuild the symbol map from the emitter map: clear it, add each emitter's upper-cased name with its handle, then finalise the map
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE85F0, length 134 bytes (padded to 144).
// Leaf: CreateSymbolMap
// Callees: CSymbolMap::Clear @ 0x009E5250 (manifest label "allocator::construct" is propagated), CCharString copy ctor
// @ 0x0099EC30, CCharString(const char*, long) @ 0x0099EBF0 (the empty literal 0x0122D70E), CCharString::ToUpper() const
// @ 0x0099EE20 (hidden result), CSymbolMap::AddSymbolDirty(prefix, name, value) @ 0x009E54E0, ~CCharString @ 0x0099EAE0 (x3),
// the map's finalise/sort @ 0x009E5C30 (manifest label "CTCInventoryQuests::ProcessButtonBReleased" is propagated).
// Layout (as the landed ctor 0x00AE8BF0): SymbolMap +0, Emitters +0x28 (16-byte CVectorMap of pair<long,
// CCountedPointer<CParticleEmitter>>, 12-byte records); the emitter's Name is at +0x140 (PDB +0x14c minus 12).
// Shape: per entry a NAMED copy of the emitter name, then a named empty prefix string, then the call: arguments right to
// left (the handle pushed first, the upper-cased temporary built in the third frame word); destruction temporary, prefix,
// name; the map end re-read each pass.
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    CCharString(const char* text, long length);      // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
    CCharString ToUpper() const;                     // 0x0099EE20
};
struct CSymbolMap {
    unsigned char m_data[0x24];
    void Clear();                                                                   // 0x009E5250
    void AddSymbolDirty(const CCharString& prefix, const CCharString& name, long value);   // 0x009E54E0
    void Finalise();                                                                // 0x009E5C30
};
namespace NParticleEngine {
struct CParticleEmitter {
    unsigned char m_body[0x140];
    CCharString   Name;                              // +0x140
};
}
struct SCountBlock;
template <class T> struct CCountedPointer {
    T*           Object;
    SCountBlock* Block;
    T* operator->() const { return Object; }
};
struct SEmitterEntry {                               // pair<long, CCountedPointer<CParticleEmitter>>
    long                                               first;
    CCountedPointer<NParticleEngine::CParticleEmitter> second;
};
struct CEmitterVectorMap {                           // CVectorMap<long, CCountedPointer<CParticleEmitter>>
    SEmitterEntry* m_first;
    SEmitterEntry* m_last;
    SEmitterEntry* m_end;
    unsigned char  Compare;
    bool           Dirty;
};
struct CParticleEmitterDatabase {
    CSymbolMap        SymbolMap;                     // +0x00
    bool              UsingSymbolMap;                // +0x24
    CEmitterVectorMap Emitters;                      // +0x28
    void CreateSymbolMap();
};
void CParticleEmitterDatabase::CreateSymbolMap()
{
    SymbolMap.Clear();
    for (SEmitterEntry* it = Emitters.m_first; it != Emitters.m_last; ++it) {
        CCharString name(it->second->Name);
        CCharString prefix("", -1);
        SymbolMap.AddSymbolDirty(prefix, name.ToUpper(), it->first);
    }
    SymbolMap.Finalise();
}