// CTCBoastingCrowdControl::GetCrowdGroup(CThing& hero, CTCCreatureGroupBoastingCrowd** out) const -- find the crowd thing recorded for the hero's UID and, when it is alive and has the boasting-crowd creature group component (interface 0x74), return that component
// Leaf: ?GetCrowdGroup@CTCBoastingCrowdControl@@QBE_NAAVCThing@@PAPAVCTCCreatureGroupBoastingCrowd@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00783730, length 142 bytes (+2 int3 padding). `ret 8`.
// Callees: map<unsigned __int64, CIntelligentPointer<CThing>>::find @ 0x00782E90 (node pointer in eax, end = the header word),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined
// GetInterface writing straight through `out`).
// Shape: nested ifs with one shared false return after them (an early `return false` opens its own exit before the
// callee-saved pushes); the null `out` test first; the hero's UID copied into a frame local whose address goes to find (a by-value getter);
// every failure joins one `xor al,al` exit, the success exit is `mov al,1`.
// Layout: PDB CTCBoastingCrowdControl: Active +0xC, HeroCrowdThings +0x10 (STLport map: header pointer, count), node value at
// +0x10 (key) / +0x18 (CIntelligentPointer<CThing>); retail CThing UniqueID +0x18, bitset +0x20 (0x74 = bit 20 of +0x2C),
// interface map +0x44, Dead = bit 0 of +0x91.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x18];
    unsigned __int64 UniqueID;                  // +0x18
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    unsigned __int64 GetUID() const { return UniqueID; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CHeroCrowdNode {                         // _Rb_tree_node<pair<const unsigned __int64, CIntelligentPointer<CThing>>>
    long _M_color;
    CHeroCrowdNode* _M_parent;
    CHeroCrowdNode* _M_left;
    CHeroCrowdNode* _M_right;
    unsigned __int64 first;                     // +0x10
    CBaseIntelligentPointer second;             // +0x18
};
struct CHeroCrowdMap {                          // STLport map<unsigned __int64, CIntelligentPointer<CThing>>
    CHeroCrowdNode* _M_header;
    unsigned long _M_node_count;
    CHeroCrowdNode* end() const { return _M_header; }
    CHeroCrowdNode* find(const unsigned __int64& key) const;   // 0x00782E90
};
struct CTCCreatureGroupBoastingCrowd;

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCBoastingCrowdControl : public CTCBase {
public:
    bool Active;                                // +0x0c
    CHeroCrowdMap HeroCrowdThings;              // +0x10
    bool CrowdSizesSuppressed;                  // +0x18
    bool GetCrowdGroup(CThing& hero, CTCCreatureGroupBoastingCrowd** out) const;
};

bool CTCBoastingCrowdControl::GetCrowdGroup(CThing& hero, CTCCreatureGroupBoastingCrowd** out) const
{
    if (out) {
        CHeroCrowdNode* it = HeroCrowdThings.find(hero.GetUID());
        if (it != HeroCrowdThings.end()) {
            CThing* crowd = it->second.GetPItem();
            if (crowd && !crowd->IsDead() && crowd->GetInterface(out, 0x74))
                return true;
        }
    }
    return false;
}
