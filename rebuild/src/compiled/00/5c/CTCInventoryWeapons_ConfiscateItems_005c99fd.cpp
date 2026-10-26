// CTCInventoryWeapons::ConfiscateItems(const vector<NInventory::CItem>& items) -- confiscates the items through the base inventory, remembering the def index of each active weapon that the confiscation took away, then refreshes the sheathed weapons
// Retry 10b (getsw sweep, FIXES B-15): the def index is read through an inline getter `GetDefIndex()`; the plain member read gives the parked register/scheduling difference.
// Leaf: ConfiscateItems@CTCInventoryWeapons
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C99FD, length 152 bytes (`ret 4`). Size TU (`#pragma optimize("s", on)`).
// Ego_r.pdb: ?ConfiscateItems@CTCInventoryWeapons@@MAEXABV?$vector@VCItem@NInventory@@...@std@@@Z (virtual).
// Callees: CTCInventoryWeapons::RefreshActiveWeaponData() @ 0x005C30D3, IS_THING_ALIVE<CIntelligentPointer> @ 0x004CC340,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCInventoryBase::ConfiscateItems @ 0x005BFB01 (qualified base call),
// CTCInventoryWeapons::RefreshSheathedWeapons() @ 0x005C9962.
// Shape: the two def indices are locals preset to 0 (ebx / edi) and read from the live weapon things (CThing unsigned short +0x8C);
// after the base call a positive index whose weapon is no longer alive is stored.
// Layout: PActiveMeleeWeapon / PActiveRangedWeapon at retail +0x134 / +0x141; ConfiscatedActiveMeleeWeaponDefIndex /
// ConfiscatedActiveRangedWeaponDefIndex at retail +0x160 / +0x164 (PDB +0x17C / +0x180).
#pragma optimize("s", on)
class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                  // +0x8c
    unsigned short GetDefIndex() const { return DefIndex; }
};
struct CIntelligentPointer {
    unsigned char _body[8];
    CThing* GetPItem();                       // 0x00A01B50
};
bool __fastcall IS_THING_ALIVE(CIntelligentPointer* pointer);   // 0x004CC340
namespace NInventory { class CItem; }
struct CItemVector;                           // std::vector<NInventory::CItem>
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    void ConfiscateItems(const CItemVector& items);   // 0x005BFB01
};
namespace NHeroInformationScreens {
class CBase { public: virtual void v00(); };
}
class CTCInventoryWeapons : public CTCInventoryBase_Retail, public NHeroInformationScreens::CBase { public:
    CIntelligentPointer PActiveMeleeWeapon;   // +0x134
    long ActiveMeleeWeaponDefIndex;           // +0x13C
    bool MeleeWeaponCarriedOnLoad;            // +0x140
    CIntelligentPointer PActiveRangedWeapon;  // +0x141
    unsigned char _pad_0x149[0x160 - 0x149];  // ActiveRangedWeaponDefIndex (unaligned) .. the primitive handles
    long ConfiscatedActiveMeleeWeaponDefIndex;    // +0x160
    long ConfiscatedActiveRangedWeaponDefIndex;   // +0x164
    void RefreshActiveWeaponData();           // 0x005C30D3
    void RefreshSheathedWeapons();            // 0x005C9962
    void ConfiscateItems(const CItemVector& items);
};
void CTCInventoryWeapons::ConfiscateItems(const CItemVector& items)
{
    RefreshActiveWeaponData();
    long meleeDefIndex = 0;
    if (IS_THING_ALIVE(&PActiveMeleeWeapon))
        meleeDefIndex = PActiveMeleeWeapon.GetPItem()->GetDefIndex();
    long rangedDefIndex = 0;
    if (IS_THING_ALIVE(&PActiveRangedWeapon))
        rangedDefIndex = PActiveRangedWeapon.GetPItem()->GetDefIndex();
    CTCInventoryBase_Retail::ConfiscateItems(items);
    if (meleeDefIndex > 0 && !IS_THING_ALIVE(&PActiveMeleeWeapon))
        ConfiscatedActiveMeleeWeaponDefIndex = meleeDefIndex;
    if (rangedDefIndex > 0 && !IS_THING_ALIVE(&PActiveRangedWeapon))
        ConfiscatedActiveRangedWeaponDefIndex = rangedDefIndex;
    RefreshSheathedWeapons();
}