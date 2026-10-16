// CVillageTaskCarryCrate::IsCreatureValidForTask(CThing* creature) -- a creature suits the task when its carrying state is the opposite of the task's: empty-handed for a pickup, carrying something for a drop-off
// Grade: DIFFER(112v117) -- retail materialises `true` in bl at entry (`push ebx ; mov bl,1`) and compares hasItem and Pickup against it (`cmp al,bl ; cmp byte ptr [edi+18h],bl`, result `mov al,bl`); ours folds the constant; the uninitialised out pointer then sits one slot lower; tried `== true` compares, a `bool valid = true` local, optimize("s")
// Leaf: ?IsCreatureValidForTask@CVillageTaskCarryCrate@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008614B0, length 115 bytes (padded to 128), `ret 4`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (inline GetInterface for TC 0x46 = CTCCarrying,
// present bit `test byte ptr [thing+28h],40h`), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440.
// Layout: engine/CVillageTaskCarryCrate.h (Pickup +0x18).
// Shape: the out pointer is uninitialised (on a miss retail passes the stale argument-slot word); `carrying == true` style
// compares against a `bl = 1` constant that is also the true result.
struct CTCBase;
struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                    // +0x44
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
struct CBaseIntelligentPointer {                   // CIntelligentPointer<T>: vptr + item (8 bytes)
    virtual void v00();
    CThing* PItem;
    CThing* GetPItem() const;                      // 0x00A01B50
};
struct CTCCrateStack {
    long GetNumCrates() const;                     // 0x007B6660
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot() const;         // 0x006A1440
};
struct CVillageTaskCarryCrate {                    // engine/CVillageTaskCarryCrate.h layout (flattened pointers there)
    virtual void v00();
    unsigned char           _pad04[4];             // +0x04 (CVillageTaskBase)
    CBaseIntelligentPointer CreatureAssignedToTask;   // +0x08 CIntelligentPointer<const CThing>
    CBaseIntelligentPointer PCrateStack;           // +0x10 CIntelligentPointer<CThing>
    bool                    Pickup;                // +0x18
    long                    Priority;              // +0x1c
    bool IsValid();
    bool IsCreatureValidForTask(CThing* creature);
};
bool CVillageTaskCarryCrate::IsCreatureValidForTask(CThing* creature)
{
    CTCCarrying* carrying;
    creature->GetInterface(&carrying, 0x46);
    bool hasItem = carrying->GetThingInPrimarySlot() != 0;
    if (hasItem == true && Pickup == true)
        return false;
    if (!hasItem && !Pickup)
        return false;
    return true;
}
