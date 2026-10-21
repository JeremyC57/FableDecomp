// Behaviour test for 005c99fd: src/005c99fd.cpp copied verbatim; the weapon pointers, IS_THING_ALIVE, the base
// confiscation and the refresh calls stubbed so the remembered def indices of active weapons that the base call took
// away (and only those) are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
struct CThing {
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                  // +0x8c
    unsigned short GetDefIndex() const { return DefIndex; }
};
struct CIntelligentPointer {
    unsigned char _body[8];
    CThing* GetPItem();                       // 0x00A01B50
};
bool __fastcall IS_THING_ALIVE(CIntelligentPointer* pointer);   // 0x004CC340
namespace NInventory { struct CItem; }
struct CItemVector;                           // std::vector<NInventory::CItem>
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    void ConfiscateItems(const CItemVector& items);   // 0x005BFB01
};
namespace NHeroInformationScreens {
struct CBase { virtual void v00(); };
}
struct CTCInventoryWeapons : CTCInventoryBase_Retail, NHeroInformationScreens::CBase {
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
// ---- end source ----
#pragma optimize("", on)
void CTCInventoryBase_Retail::v00() {}
void NHeroInformationScreens::CBase::v00() {}
CThing* CIntelligentPointer::GetPItem() { return *(CThing**)_body; }
bool __fastcall IS_THING_ALIVE(CIntelligentPointer* p) { return p->GetPItem() != 0; }
static CTCInventoryWeapons* g_w = 0; static bool g_takeMelee = false, g_takeRanged = false; static int g_refreshA = 0, g_refreshS = 0;
void CTCInventoryBase_Retail::ConfiscateItems(const CItemVector&)
{ if (g_takeMelee) *(CThing**)g_w->PActiveMeleeWeapon._body = 0; if (g_takeRanged) *(CThing**)g_w->PActiveRangedWeapon._body = 0; }
void CTCInventoryWeapons::RefreshActiveWeaponData() { ++g_refreshA; }
void CTCInventoryWeapons::RefreshSheathedWeapons() { ++g_refreshS; }
int main()
{
    static CThing melee, ranged; memset(&melee, 0, sizeof(melee)); memset(&ranged, 0, sizeof(ranged));
    melee.DefIndex = 3; ranged.DefIndex = 8;
    static unsigned char raw[sizeof(CTCInventoryWeapons)]; memset(raw, 0, sizeof(raw));
    CTCInventoryWeapons* w = (CTCInventoryWeapons*)raw; g_w = w;
    *(CThing**)w->PActiveMeleeWeapon._body = &melee; *(CThing**)w->PActiveRangedWeapon._body = &ranged;
    const CItemVector& items = *(const CItemVector*)raw;
    g_takeMelee = true;
    w->ConfiscateItems(items);
    CHECK(w->ConfiscatedActiveMeleeWeaponDefIndex == 3 && w->ConfiscatedActiveRangedWeaponDefIndex == 0 && g_refreshA == 1 && g_refreshS == 1);
    g_takeMelee = false; g_takeRanged = true;
    w->ConfiscateItems(items);
    CHECK(w->ConfiscatedActiveRangedWeaponDefIndex == 8 && w->ConfiscatedActiveMeleeWeaponDefIndex == 3 && g_refreshS == 2);
    if (fails == 0) printf("005c99fd_TEST PASS\n"); else printf("005c99fd_TEST FAIL\n");
    return fails;
}