// CGameCameraManager::IsObservedThingInMelee() -- true when the observed thing is alive, has its combat component (interface type 0x36) and that component's melee target index (+0x34) is set (>= 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006994F0, length 108 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times, on PObservedThing), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x004365B0 (the inlined GetInterface for type 0x36 = bit 22 of the bitset word at +0x24).
// Layout: retail CGameCameraManager with PDB names: PObservedThing +0x100 (PDB +0x110). The thing: dead flag +0x91, interface bitset
// +0x20, interface map +0x44.
// Shape: the IS_THING_ALIVE idiom, then the out-parameter GetInterface template (out pointer left uninitialised: it shares the
// lookup key's frame word) as the condition; the success arm returns `mov al,1`, every failure joins the `xor al,al` exit.
struct CTCBase;

struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_statusFlags;            // +0x91 (bit 0: dead)
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

struct CTCCombat {                          // interface type 0x36
    unsigned char _pad[0x34];
    long          m_meleeTarget;            // +0x34
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CGameCameraManager {
    void*                   __vftable;      // +0x00
    unsigned char           _pad04[0xfc];   // +0x04
    CBaseIntelligentPointer PObservedThing; // +0x100
    bool IsObservedThingInMelee();
};

bool CGameCameraManager::IsObservedThingInMelee()
{
    if (PObservedThing.GetPItem() && !(PObservedThing.GetPItem()->m_statusFlags & 1)) {
        CTCCombat* combat;
        if (PObservedThing.GetPItem()->GetInterface(&combat, 0x36)) {
            if (combat->m_meleeTarget >= 0)
                return true;
        }
    }
    return false;
}
