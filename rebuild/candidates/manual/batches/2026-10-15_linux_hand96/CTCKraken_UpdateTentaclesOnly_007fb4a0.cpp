// CTCKraken::UpdateTentaclesOnly() -- pick the tentacle closest to the target into AttackingTentacleIterator and, when one was found, set that tentacle component's Tracking flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FB4A0, length 92 bytes.
// Callees: CTCKraken::GetClosestTentacleToTarget(list iterator*) @ 0x007FB270, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the interface-map search).
// Layout (local model, no engine header): AttackingTentacleIterator at +0x48 (PDB +0x58: the two retail STLport lists before it
// are 4 bytes each); the list node holds CIntelligentPointer<CThing> at +8; CTCKrakenTentacle::Tracking at +0x29 (PDB).
// Shape: the thing's inline GetInterface(type, &out) of the landed 0x007504E0 family with type 227 (CTCKrakenTentacle: word 7,
// bit 3 -> `test byte ptr [eax+3Ch],8`; key 0E3h in the `push ecx` frame slot); its bool is ignored, so the absent path
// stores through the uninitialised out word (`mov eax,[esp+4]`) and the Tracking store is duplicated into both paths.
// The template is `__forceinline` here: behind the GetPItem call VC7.1 otherwise emits it out of line.
struct CBaseObject;
struct CTCBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* Get() const { return (T*)GetPItem(); }
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44

    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCKrakenTentacle {                          // interface 227
    unsigned char _pad_0x00[0x29];
    bool          Tracking;                         // +0x29
};

struct STentacleNode {                              // STLport list node of CIntelligentPointer<CThing>
    STentacleNode*              _M_next;
    STentacleNode*              _M_prev;
    CIntelligentPointer<CThing> _M_data;            // +0x08
};

struct STentacleIterator {
    STentacleNode* _M_node;
    CIntelligentPointer<CThing>& operator*() const { return _M_node->_M_data; }
};

enum { TC_KRAKEN_TENTACLE = 227 };

struct CTCKraken {
    unsigned char     _pad_0x00[0x48];
    STentacleIterator AttackingTentacleIterator;    // +0x48
    bool GetClosestTentacleToTarget(STentacleIterator* closest);   // 0x007FB270
    void UpdateTentaclesOnly();
};

void CTCKraken::UpdateTentaclesOnly()
{
    if (GetClosestTentacleToTarget(&AttackingTentacleIterator)) {
        CTCKrakenTentacle* tentacle;
        (*AttackingTentacleIterator).Get()->GetInterface(TC_KRAKEN_TENTACLE, &tentacle);
        tentacle->Tracking = true;
    }
}
