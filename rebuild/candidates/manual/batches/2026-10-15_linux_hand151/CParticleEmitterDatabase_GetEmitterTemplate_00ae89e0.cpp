// CParticleEmitterDatabase::GetEmitterTemplate(long, long) -- with an emitter bank: ask the bank's emitter info for the template at `index` (null for handle 0); without one: look the handle up in the emitter map
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE89E0, length 145 bytes (padded to 160).
// Leaf: GetEmitterTemplate
// Callees: the data bank's emitter-info getter @ 0x00AF1280 (manifest label "CTCInventoryBase::GetCategoryWithIndex" is
// propagated), CParticleEmitterInfo::GetEmitterTemplate(CCountedPointer&, long) @ 0x00AF1480, the counted pointer's out-of-line
// destructor @ 0x00AE94A0, CVectorMap<long, CCountedPointer<CParticleEmitter>>::LowerBound(const long*) @ 0x00AE9520.
// Layout (as the landed ctor 0x00AE8BF0): Emitters +0x28 (16-byte CVectorMap, 12-byte records), EmitterBank +0x38.
// Shape: the info pointer is a NAMED local taken before the counted-pointer local is constructed (so the call precedes the
// zero stores and the index push; esi carries the zero across it); the template pointer is read before the local's
// destructor; the map path is the inline Find ternary over LowerBound(&handle) (the handle compared from its own argument
// slot, `end` re-read for the caller's test) returning the entry's object pointer.
namespace NParticleEngine { struct CParticleEmitter; }
typedef NParticleEngine::CParticleEmitter CEmitter;
struct SCountBlock;
struct CEmitterPointer {                             // CCountedPointer<CParticleEmitter>
    CEmitter*    Object;
    SCountBlock* Block;
    CEmitterPointer() : Object(0), Block(0) {}
    ~CEmitterPointer();                              // 0x00AE94A0
};
struct CParticleEmitterInfo {
    bool GetEmitterTemplate(CEmitterPointer& out, long index);   // 0x00AF1480
};
namespace NParticleEngine {
struct CParticleDataBank {
    CParticleEmitterInfo* GetEmitterInfo(long handle);           // 0x00AF1280
};
}
struct CBankPointer {                                // CCountedPointer<CParticleDataBank>
    NParticleEngine::CParticleDataBank* Object;
    SCountBlock* Block;
    NParticleEngine::CParticleDataBank* operator->() const { return Object; }
};
struct SEmitterEntry {                               // pair<long, CCountedPointer<CParticleEmitter>>
    long            first;
    CEmitterPointer second;
};
struct CEmitterVectorMap {                           // CVectorMap<long, CCountedPointer<CParticleEmitter>>
    SEmitterEntry* m_first;
    SEmitterEntry* m_last;
    SEmitterEntry* m_end;
    unsigned char  Compare;
    bool           Dirty;
    SEmitterEntry* LowerBound(const long* key);      // 0x00AE9520
    SEmitterEntry* end() { return m_last; }
    SEmitterEntry* Find(const long* key)
    {
        SEmitterEntry* it = LowerBound(key);
        return (it == m_last || *key < it->first) ? m_last : it;
    }
};
struct CSymbolMap { unsigned char m_data[0x24]; };
struct CParticleEmitterDatabase {
    CSymbolMap        SymbolMap;                     // +0x00
    bool              UsingSymbolMap;                // +0x24
    CEmitterVectorMap Emitters;                      // +0x28
    CBankPointer      EmitterBank;                   // +0x38
    CEmitter* GetEmitterTemplate(long handle, long index);
};
CEmitter* CParticleEmitterDatabase::GetEmitterTemplate(long handle, long index)
{
    if (EmitterBank.Object) {
        if (handle) {
            CParticleEmitterInfo* info = EmitterBank->GetEmitterInfo(handle);
            CEmitterPointer emitter;
            if (info->GetEmitterTemplate(emitter, index))
                return emitter.Object;
        }
        return 0;
    }
    SEmitterEntry* it = Emitters.Find(&handle);
    if (it == Emitters.end())
        return 0;
    return it->second.Object;
}
