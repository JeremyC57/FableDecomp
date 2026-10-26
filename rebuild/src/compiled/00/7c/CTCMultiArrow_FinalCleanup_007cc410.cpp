// CTCMultiArrow::FinalCleanup() -- removes the multi-arrow effect from the weapon held by the parent's type-0x13 component
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CC410, length 86 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (this TU's copy), CBaseIntelligentPointer::GetPItem()
// const @ 0x00A01B50 (on the component's pointer at +0x141), CTCMultiArrow::RemoveEffectOnWeapon(CThing*) @ 0x007CC120.
// Shape: the thing's inline out-parameter GetInterface(&component, 0x13) (bit 19 of the bitset word at +0x20, the sorted map at
// +0x44) whose result is NOT tested: the absent path reads the uninitialised out pointer from the shared `push ecx` frame word
// (the same word that holds the lookup key), exactly as retail.
class CThing;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
};

#pragma pack(push, 1)
struct CTCWeaponHolder_007cc410 {                   // the parent's type-0x13 component
    unsigned char           _pad000[0x141];
    CBaseIntelligentPointer m_weapon;               // +0x141 CIntelligentPointer<CThing>
};
#pragma pack(pop)

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
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

struct CTCBase_007cc410 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
};

class CTCMultiArrow : public CTCBase_007cc410 { public:
    void RemoveEffectOnWeapon(CThing* weapon);      // 0x007CC120
    void FinalCleanup();
};

void CTCMultiArrow::FinalCleanup()
{
    CTCWeaponHolder_007cc410* holder;
    ParentThing->GetInterface(&holder, 0x13);
    RemoveEffectOnWeapon(holder->m_weapon.GetPItem());
}