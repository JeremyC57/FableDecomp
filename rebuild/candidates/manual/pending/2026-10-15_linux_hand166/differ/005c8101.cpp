// CTCInventoryWeapons::PutWeaponsInSheathes() -- takes the weapons out of the hands, then puts each live active weapon (melee, then ranged) into its carry slot on the owner
// Grade: DIFFER(117v117) -- (1) the discarded presence test forms its this as `mov eax,[esi+4] ; lea ecx,[eax+20h]`, ours `mov ecx,[esi+4] ; add ecx,20h`; (2) retail pushes ebx only inside the melee block (owner in ebx, weapon in edi), ours at entry with the registers rotated (tried: direct call arguments, inline PutWeaponInSheathe helper reading the owner first (fixed the owner-before-GetPItem order), a dead GetInterface, a presence-bits pointer local) -- lane gap: callee-saved push placement
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C8101, length 117 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Ego_r.pdb: ?PutWeaponsInSheathes@CTCInventoryWeapons@@QAEXXZ.
// Callees: CTCInventoryWeapons::RemoveWeaponsFromSheathes() @ 0x005C552C (manifest "CTCCarryable_PrepareWeaponSheathing"),
// std::bitset::test @ 0x00410DE0 (a presence test of interface 0x46 whose result is discarded), IS_THING_ALIVE<CIntelligentPointer>
// @ 0x004CC340, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the file-static PutSheatheObjectInCarrySlot(CThing&, CThing&,
// float&) @ 0x005C36C2 (PDB ?PutSheatheObjectInCarrySlot@?A0x0a34196c@@YIXAAVCThing@@0AAM@Z, `__fastcall`).
// Shape: the shared float out value is a zeroed local ([ebp-4], `fldz ; fstp`) passed by reference; the owner is read into a
// register before the GetPItem call (`__fastcall` arguments: the pushed float& first, then the weapon, then the owner in ecx).
// Layout: CTCBase ParentThing +0x04; PActiveMeleeWeapon / PActiveRangedWeapon at retail +0x134 / +0x141.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CPresenceBits {
    bool test(long bit);                      // 0x00410DE0 (std::bitset::test)
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                  // +0x20
    bool HasTC(long type) { return m_present.test(type); }
};
struct CIntelligentPointer {
    unsigned char _body[8];
    CThing* GetPItem();                       // 0x00A01B50
};
bool __fastcall IS_THING_ALIVE(CIntelligentPointer* pointer);   // 0x004CC340
void __fastcall PutSheatheObjectInCarrySlot(CThing& owner, CThing& weapon, float& time);   // 0x005C36C2 (file static)

struct CTCInventoryWeapons {
    void*   __vftable;
    CThing* ParentThing;                      // +0x04
    unsigned char _pad08[0x134 - 0x08];
    CIntelligentPointer PActiveMeleeWeapon;   // +0x134
    long ActiveMeleeWeaponDefIndex;           // +0x13C
    bool MeleeWeaponCarriedOnLoad;            // +0x140
    CIntelligentPointer PActiveRangedWeapon;  // +0x141
    void RemoveWeaponsFromSheathes();         // 0x005C552C
    void PutWeaponsInSheathes();
    __forceinline void PutWeaponInSheathe(CIntelligentPointer& weapon, float& time)
    {
        CThing& owner = *ParentThing;
        PutSheatheObjectInCarrySlot(owner, *weapon.GetPItem(), time);
    }
};

void CTCInventoryWeapons::PutWeaponsInSheathes()
{
    RemoveWeaponsFromSheathes();
    ParentThing->HasTC(0x46);
    float time = 0.0f;
    if (IS_THING_ALIVE(&PActiveMeleeWeapon))
        PutWeaponInSheathe(PActiveMeleeWeapon, time);
    if (IS_THING_ALIVE(&PActiveRangedWeapon))
        PutWeaponInSheathe(PActiveRangedWeapon, time);
}
