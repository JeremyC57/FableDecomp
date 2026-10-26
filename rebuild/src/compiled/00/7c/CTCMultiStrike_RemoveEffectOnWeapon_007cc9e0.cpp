// CTCMultiStrike::RemoveEffectOnWeapon(CThing* weapon) -- when the weapon is alive, kill every live particle effect this multi-strike attached
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CC9E0, length 128 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three per element), CThing::Kill(bool) @ 0x004C9B80 (true).
// Layout: PDB CTCMultiStrike (0x5C): ParticleEffects +0x4C (vector<CIntelligentPointer<CThing>>, 8-byte elements; PDB offset).
// Shape: `for (long i = 0; i < (long)size(); ++i)` signed loop; IS_THING_ALIVE on the element reference, then the element re-indexed
// through STLport `*(begin() + i)` for the Kill (retail re-reads `_M_start`).
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
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCMultiStrike { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x4c - 0x08];
    CRetailVector<CBaseIntelligentPointer> ParticleEffects;   // +0x4c
    void RemoveEffectOnWeapon(CThing* weapon);
};

void CTCMultiStrike::RemoveEffectOnWeapon(CThing* weapon)
{
    if (weapon && !(weapon->m_flags91 & 1)) {
        for (long i = 0; i < (long)ParticleEffects.size(); ++i) {
            if (IS_THING_ALIVE(ParticleEffects[i]))
                ParticleEffects[i].GetPItem()->Kill(true);
        }
    }
}