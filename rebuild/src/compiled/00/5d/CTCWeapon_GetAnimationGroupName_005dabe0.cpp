// CTCWeapon::GetAnimationGroupName() -- the weapon def's animation group name, or an empty name while carried by a creature that cannot use it
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DABE0, 103 bytes (+9 int3). __fastcall, returns const CCharString&.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0; CTCCarryable carrier getter 0x00705170
//          (catalog "CGuiControlListBox::GetCurrentSelectedEntryData" is a byte-twin label); CTCWeapon::IsUsableBy(CThing*)
//          0x005DAB10. Global empty CCharString 0x013BAF5C.
// Shape: interface 0x47 (CTCCarryable, bitset word 2 bit 7 -> `mov cl,[eax+28h] ; test cl,cl ; jns`); a live carrier
//        (Dead bit 0 of +0x91 clear) that fails IsUsableBy returns the global empty string, every other path returns the
//        def's CCharString at +0x34 read straight through the CDefPointer (no add-ref).
// Layout: CTCWeapon ParentThing +0x04, WeaponDef +0x0C; retail CThing bitset +0x20, interface map +0x44, Dead +0x91.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0 (another TU's copy is 0x0040F020)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
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
struct CCharString { void* PStringData; };
extern CCharString g_emptyCharString;            // 0x013BAF5C
struct CWeaponDef {
    unsigned char _pad_0x00[0x34];
    CCharString AnimationGroupName;              // +0x34
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CTCCarryable {
    CThing* GetCarrier() const;                  // 0x00705170
};
struct CTCWeapon {
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    unsigned char _pad_0x08[4];
    CDefPointer<CWeaponDef> WeaponDef;           // +0x0c
    bool IsUsableBy(CThing* thing) const;        // 0x005DAB10
    const CCharString& GetAnimationGroupName() const;
};
const CCharString& CTCWeapon::GetAnimationGroupName() const
{
    CTCCarryable* carryable;
    if (ParentThing->GetInterface(&carryable, 0x47)) {
        CThing* carrier = carryable->GetCarrier();
        if (carrier && !(carrier->Flags91 & 1) && !IsUsableBy(carrier))
            return g_emptyCharString;
    }
    return WeaponDef->AnimationGroupName;
}