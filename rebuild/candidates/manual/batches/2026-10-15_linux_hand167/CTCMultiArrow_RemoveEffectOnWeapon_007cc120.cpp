// CTCMultiArrow::RemoveEffectOnWeapon(CThing* weapon) -- once: if the weapon is alive and has a graphic appearance (interface 0x5B), clear this ability's mesh effect and tint from it; then mark the effect removed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CC120, length 102 bytes (`ret 4`).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x5B = bit 27 of CThing+0x28),
// CTCGraphicAppearance::ClearMeshEffect(void* key) @ 0x004BFFC0 and RemoveTintColour(void* owner) @ 0x004BFAE0 (key = this).
// Layout: PDB CTCMultiArrow (0x4C; retail 0x48 with a 12-byte vector): RemovedEffect +0x20 (PDB offset).
// Shape: IS_ALIVE `&&` the out-pointer GetInterface; RemovedEffect set on every path past the first test.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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
    void Kill(bool immediately);                // 0x004C9B80
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
template<class T> struct CRetailVector {        // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CTCGraphicAppearance {
    void ClearMeshEffect(void* key);            // 0x004BFFC0
    void RemoveTintColour(void* owner);         // 0x004BFAE0
};
struct CTCMultiArrow {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    void* PDef;                                 // +0x0c CDefPointer<CSpecialAbilitiesMultiArrowDef const>
    long NumberOfShots;                         // +0x10
    long NumberInVolley;                        // +0x14
    long NumOfVolleys;                          // +0x18
    long SpellLevel;                            // +0x1c
    bool RemovedEffect;                         // +0x20
    void RemoveEffectOnWeapon(CThing* weapon);
};

void CTCMultiArrow::RemoveEffectOnWeapon(CThing* weapon)
{
    if (!RemovedEffect) {
        CTCGraphicAppearance* appearance;
        if (weapon && !(weapon->m_flags91 & 1) && weapon->GetInterface(&appearance, 0x5b)) {
            appearance->ClearMeshEffect(this);
            appearance->RemoveTintColour(this);
        }
        RemovedEffect = true;
    }
}
