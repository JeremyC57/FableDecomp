// CTCCreatureGeneratorCreator::IsGeneratorAlive() -- true when the generator thing is alive and its creature-generator component (interface 0x3d) is not destroyed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C98A0, length 114 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0.
// Layout: Ego_r.pdb CTCCreatureGeneratorCreator: FrameToRegenerate +0xC, Generated +0x10, PGenerator +0x11 (an unaligned
// CIntelligentPointer<CThing>, hence `#pragma pack(1)` on the local model). Retail CThing: interface bitset +0x20, map +0x44,
// Dead bit +0x91. The generator component's Destroyed flag sits at +0x89 in retail (PDB CTCCreatureGenerator +0xA5, 0x1C lower).
// Shape: inline IsThingAlive-style test (GetPItem twice), then an inline accessor that returns an unset local on the absent path
// (`CTC* tc; if (GetInterface(0x3d,&tc)) ...; return tc;`): retail reads the dead key slot `[esp+4]` when the bit is clear.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template<class T> __forceinline bool GetInterface(long interfaceType, T** ppOut)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCCreatureGenerator {                   // interface 0x3d
    unsigned char _pad00[0x89];
    bool Destroyed;                             // +0x89 (PDB +0xA5)
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
};
class CTCCreatureGeneratorCreator : public CTCBase {
public:
    long FrameToRegenerate;                     // +0x0c
    bool Generated;                             // +0x10
    CBaseIntelligentPointer PGenerator;         // +0x11 CIntelligentPointer<CThing>
    unsigned char _pad19[0x20 - 0x19];
    unsigned long long GeneratorUID;            // +0x20
    bool IsGeneratorAlive();
};
#pragma pack(pop)

static __forceinline CTCCreatureGenerator* GetGenerator(CThing* thing)
{
    CTCCreatureGenerator* gen;
    thing->GetInterface(0x3d, &gen);
    return gen;
}

bool CTCCreatureGeneratorCreator::IsGeneratorAlive()
{
    if (PGenerator.GetPItem() && !PGenerator.GetPItem()->IsDead()) {
        if (!GetGenerator(PGenerator.GetPItem())->Destroyed)
            return true;
    }
    return false;
}