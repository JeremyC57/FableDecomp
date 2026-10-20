// CTCCombat::SetBlocking() -- enter the blocking state: a ranged-attacks user stops aiming and resets its projectile weapon mode (and its player drops mode 0x22); the creature mode manager is put into mode 0xc unless already there; Blocking is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007337E0, 182 bytes (plain `ret`).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (two inlined lookups), CTCHeroRangedAttacks::
// CancelAimingWeapon @ 0x00789FF0 and ::ResetProjectileWeaponMode @ 0x00789B30, CThing::GetPlayer @ 0x004C7A30, CPlayer::RemoveMode
// @ 0x00634190, CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 (manifest "CPixelShader::CountReferences" is propagated) and
// ::AddMode @ 0x0073AE10, the mode's virtual slot 1 GetModeType.
// Layout: local model -- engine/CTCCombat.h carries Ego_r.pdb offsets (Blocking +0x6f) while retail's three 12-byte vectors put
// Blocking at +0x63, and the header's +0x04 word is the CTCBase ParentThing. CThing bitset +0x20 (ranged attacks = 0x81, bit 1 of
// the +0x30 word; mode manager = 0x31, bit 17 of the +0x24 word), map +0x44.
// Shape: HasTC + GetTC with the ranged-attacks component in a named local (esi); the mode manager is the unchecked out-parameter
// GetInterface (the absent arm reads the lookup key's frame word); RemoveMode's argument is pushed before GetPlayer runs.
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CPlayer {
    void RemoveMode(long mode);                         // 0x00634190
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
    CPlayer* GetPlayer();                               // 0x004C7A30
};
struct CTCHeroRangedAttacks {
    void CancelAimingWeapon();                          // 0x00789FF0
    void ResetProjectileWeaponMode();                   // 0x00789B30
};
struct CMode {
    virtual void v00();
    virtual long GetModeType();                         // slot 1
};
struct CTCCreatureModeManager {
    CMode* GetCurrentMode();                            // 0x0073A6E0
    bool AddMode(unsigned long mode);                   // 0x0073AE10
};
struct CTCCombat {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x63 - 0x08];
    bool Blocking;                                      // +0x63 (PDB +0x6f)
    void SetBlocking();
};

void CTCCombat::SetBlocking()
{
    if (ParentThing->HasTC(0x81)) {
        CTCHeroRangedAttacks* ranged = (CTCHeroRangedAttacks*)ParentThing->GetTC(0x81);
        ranged->CancelAimingWeapon();
        ranged->ResetProjectileWeaponMode();
        ParentThing->GetPlayer()->RemoveMode(0x22);
    }
    CTCCreatureModeManager* modes;
    ParentThing->GetInterface(&modes, 0x31);
    if (modes->GetCurrentMode()->GetModeType() != 0xc)
        modes->AddMode(0xc);
    Blocking = true;
}