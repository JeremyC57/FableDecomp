// Behaviour test for 005c311a: src/005c311a.cpp copied verbatim; the weapon pointers, IS_THING_ALIVE and the base
// inventory search stubbed so the active melee weapon, then the active ranged weapon, then the base search answer.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryWeapons::GetItemOfTypeInInventory(long itemType) -- the active melee or ranged weapon thing when it is alive and of that type, else the base inventory's freshly created item
// Retry 10b (getsw sweep, FIXES B-15): the def index is read through an inline getter `GetDefIndex()`; the plain member read gives the parked register/scheduling difference.
// Leaf: GetItemOfTypeInInventory@CTCInventoryWeapons
// Leaf: GetItemOfTypeInInventory@CTCInventoryWeapons
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C311A, length 102 bytes (`ret 4`). Size TU (`#pragma optimize("s", on)`, as the
// landed neighbour RefreshActiveWeaponData 0x005C30D3).
// Callees: IS_THING_ALIVE<CIntelligentPointer> @ 0x004CC340 (`__fastcall`, pointer in ecx), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, CTCInventoryBase::GetItemOfTypeInInventory(long) @ 0x005BF3F1 (qualified base call).
// Shape: each weapon's address is formed once in esi (`lea esi,[edi+134h]` / `lea esi,[edi+141h]`); the two hit arms tail-merge
// into one `GetPItem` call on esi; the def index is CThing's unsigned short +0x8C.
// Layout: PActiveMeleeWeapon / PActiveRangedWeapon at retail +0x134 / +0x141 (PDB +0x14C / +0x159; unaligned in both).
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
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    CThing* GetItemOfTypeInInventory(long itemType);   // 0x005BF3F1
};
namespace NHeroInformationScreens {
struct CBase { virtual void v00(); };
}
struct CTCInventoryWeapons : CTCInventoryBase_Retail, NHeroInformationScreens::CBase {
    CIntelligentPointer PActiveMeleeWeapon;   // +0x134
    long ActiveMeleeWeaponDefIndex;           // +0x13C
    bool MeleeWeaponCarriedOnLoad;            // +0x140
    CIntelligentPointer PActiveRangedWeapon;  // +0x141
    CThing* GetItemOfTypeInInventory(long itemType);
};
CThing* CTCInventoryWeapons::GetItemOfTypeInInventory(long itemType)
{
    if (IS_THING_ALIVE(&PActiveMeleeWeapon) && PActiveMeleeWeapon.GetPItem()->GetDefIndex() == itemType)
        return PActiveMeleeWeapon.GetPItem();
    if (IS_THING_ALIVE(&PActiveRangedWeapon) && PActiveRangedWeapon.GetPItem()->GetDefIndex() == itemType)
        return PActiveRangedWeapon.GetPItem();
    return CTCInventoryBase_Retail::GetItemOfTypeInInventory(itemType);
}
// ---- end source ----
#pragma optimize("", on)
void CTCInventoryBase_Retail::v00() {}
void NHeroInformationScreens::CBase::v00() {}
CThing* CIntelligentPointer::GetPItem() { return *(CThing**)_body; }
bool __fastcall IS_THING_ALIVE(CIntelligentPointer* p) { return p->GetPItem() != 0; }
static CThing g_fromBase; static long g_baseAsked = -1;
CThing* CTCInventoryBase_Retail::GetItemOfTypeInInventory(long t) { g_baseAsked = t; return &g_fromBase; }
int main()
{
    static CThing melee, ranged; memset(&melee, 0, sizeof(melee)); memset(&ranged, 0, sizeof(ranged));
    melee.DefIndex = 3; ranged.DefIndex = 8;
    static unsigned char raw[sizeof(CTCInventoryWeapons)]; memset(raw, 0, sizeof(raw));
    CTCInventoryWeapons* w = (CTCInventoryWeapons*)raw;
    *(CThing**)w->PActiveMeleeWeapon._body = &melee; *(CThing**)w->PActiveRangedWeapon._body = &ranged;
    CHECK(w->GetItemOfTypeInInventory(3) == &melee && g_baseAsked == -1);
    CHECK(w->GetItemOfTypeInInventory(8) == &ranged && g_baseAsked == -1);
    CHECK(w->GetItemOfTypeInInventory(5) == &g_fromBase && g_baseAsked == 5);
    *(CThing**)w->PActiveMeleeWeapon._body = 0;
    CHECK(w->GetItemOfTypeInInventory(3) == &g_fromBase && g_baseAsked == 3);
    if (fails == 0) printf("005c311a_TEST PASS\n"); else printf("005c311a_TEST FAIL\n");
    return fails;
}