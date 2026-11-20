// Behaviour test for 0x005CA68E CTCInventoryWeapons::SetupActiveWeaponsOnCarrier (links the REAL row object, including its inline
// GetTC / PeekTC expansions over the thing's component vector-map (+0x44, 8-byte {key, CTCBase*} pairs, the object's own LowerBound),
// its IS_THING_ALIVE<CThing*> / IS_THING_DEAD<CThing*> COMDATs and GetSubDef<CCarryableDef>). Fixtures: raw retail layouts (the
// component: parent thing +0x04, dying byte +0x88, PActiveMeleeWeapon +0x134, PActiveRangedWeapon +0x141; thing: name byte +0x00
// (test only), presence bitset words +0x20, components +0x44, base def +0x70, dead bit 0 of +0x91; hero: weapon use allowed +0x0C;
// multi-arrow: shots +0x10; def: refcount +0x04, slot 1 Delete, slot 14 GetPSubDefInfo, manager +0x1C; carryable def: PassiveCarrySlot
// +0x30). Every callee the object leaves undefined is a recording stub bound by /alternatename (the bitset test reads the real words).
// Each run checks the exact call sequence, the actions' arguments, and that the carried-things vector and the def handles are released.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#define ALT(a, b) comment(linker, "/alternatename:" a "=" b)
#pragma ALT("@CallRow@8", "?SetupActiveWeaponsOnCarrier@CTCInventoryWeapons@@AAEXXZ")
#pragma ALT("??0CCharString@@QAE@PBDJ@Z", "@StrCtor@16")
#pragma ALT("??1CCharString@@QAE@XZ", "@StrDtor@8")
#pragma ALT("??0CCreatureAction_UnsheatheItemFromInventory@@QAE@AAVCThingCreatureBase@@AAVCThing@@PAVCCreatureActionBase@@J@Z", "@ActCtor@24")
#pragma ALT("??1CCreatureAction_UnsheatheItemFromInventory@@UAE@XZ", "@ActDtor@8")
#pragma ALT("?PerformAction@CCreatureAction_UnsheatheItemFromInventory@@UAEXXZ", "@ActPerform@8")
#pragma ALT("?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z", "@SetAction@12")
#pragma ALT("?ApplyEffectOnWeapon@CTCMultiArrow@@QAEXPAVCThing@@@Z", "@MAApply@12")
#pragma ALT("?ApplyEffectOnWeapon@CTCMultiStrike@@QAEXPAVCThing@@@Z", "@MSApply@12")
#pragma ALT("?RemoveEffectOnWeapon@CTCMultiArrow@@QAEXPAVCThing@@@Z", "@MARemove@12")
#pragma ALT("?RemoveEffectOnWeapon@CTCMultiStrike@@QAEXPAVCThing@@@Z", "@MSRemove@12")
#pragma ALT("?GetAllCarriedThings@CTCCarrying@@QBEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z", "@GetAllCarried@12")
#pragma ALT("?GetPDefFromGlobalIndex@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@J@Z", "@PDefFromIndex@16")
#pragma ALT("?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ", "@IPGet@8")
#pragma ALT("?GetThingInCarrySlot@CTCCarrying@@QBEPAVCThing@@J@Z", "@InCarrySlot@12")
#pragma ALT("?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ", "@InPrimarySlot@8")
#pragma ALT("?IsMeleeWeapon@CTCWeapon@@QBE_NXZ", "@IsMelee@8")
#pragma ALT("?IsProjectile@CTCWeapon@@QBE_NXZ", "@IsProjectile@8")
#pragma ALT("?IsProjectileWeapon@CTCWeapon@@QBE_NXZ", "@IsProjectileWeapon@8")
#pragma ALT("?IsThingCarryingWeapon@CTCCarrying@@SI_NABVCThing@@@Z", "@CarryingWeapon@4")
#pragma ALT("?PutThingInCarrySlot@CTCCarrying@@QAEXAAVCThing@@J_N@Z", "@PutInSlot@20")
#pragma ALT("?RefreshSheathedWeapons@CTCInventoryWeapons@@QAEXXZ", "@Refresh@8")
#pragma ALT("?RemoveMode@CTCCreatureModeManager@@QAEXK@Z", "@RemoveMode@12")
#pragma ALT("?RemoveThing@CTCCarrying@@QAEXAAVCThing@@@Z", "@RemoveThing@12")
#pragma ALT("?ReturnToInventory@CTCInventoryItem@@QAEXXZ", "@ReturnToInv@8")
#pragma ALT("?test@?$bitset@$0BBC@@std@@QBE_NI@Z", "@BitTest@12")

static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

struct Pair { long key; void* tc; };
struct Thing { unsigned char b[0xA0]; Pair comps[8]; int ncomps; };
struct IPtr { void* vt; void* item; };
struct Comp { unsigned char b[0x150]; };
struct Weapon { int melee, projectileWeapon, projectile; char name; };
struct Def { void** vt; long ref; unsigned char p08[0x1C - 8]; void* manager; unsigned char p20[0x30 - 0x20]; long passiveSlot; };
struct Vec { void** start; void** finish; void** eos; };
struct StrData { const char* text; unsigned long len; };

extern "C" void __fastcall CallRow(Comp* self, void*);
static char g_log[2048];
static void Log(const char* fmt, ...) { char b[128]; va_list ap; va_start(ap, fmt); vsprintf(b, fmt, ap); va_end(ap); strcat(g_log, b); }
static char Name(const void* t) { return t ? (char)((const Thing*)t)->b[0] : '0'; }

static Thing g_parent, g_things[8];               // things: 'H' hand, 'S' sheathed, 'M' active melee, 'R' active ranged, 'A'..'C' carried
static Weapon g_weapons[8]; static char g_hero[0x10], g_ms[0x20], g_ma[0x20], g_carrying[4], g_cmm[4], g_items[8][4];
static Thing* g_primary; static Thing* g_slotThing; static long g_expectSlot; static Thing* g_carried[8]; static int g_ncarried;
static int g_setActionResult, g_carryingWeapon, g_strLive, g_actLive, g_vecFrees;
static Def g_weaponDef, g_carryDef; static void* g_wdVt[16]; static void* g_cdVt[16]; static long g_subInfo[2] = { 63, 0 }; static char g_mgr[4]; static int g_deletes;

static void SetBit(Thing* t, int bit) { ((unsigned long*)&t->b[0x20])[bit >> 5] |= 1UL << (bit & 31); }
static void AddComp(Thing* t, long key, void* tc)   // keeps the vector-map sorted by key
{
    int i = t->ncomps++; while (i > 0 && t->comps[i - 1].key > key) { t->comps[i] = t->comps[i - 1]; --i; }
    t->comps[i].key = key; t->comps[i].tc = tc; SetBit(t, (int)key);
    *(Pair**)&t->b[0x44] = t->comps; *(Pair**)&t->b[0x48] = t->comps + t->ncomps; *(Pair**)&t->b[0x4C] = t->comps + 8;
}
extern "C" bool __fastcall BitTest(const unsigned long* words, void*, unsigned bit) { return ((words[bit >> 5] >> (bit & 31)) & 1) != 0; }
// The two template instances the object calls (??$ names do not bind through /alternatename): explicit specializations give the
// exact decorated names ??$?9VCThing@@@@YI_NPBVCBaseObject@@ABV?$CIntelligentPointer@VCThing@@@@@Z and ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@...
class CBaseObject; class CThing; template<class T> class CIntelligentPointer;
template<class T> bool __fastcall operator!=(const CBaseObject* a, const CIntelligentPointer<T>& b);
template<> bool __fastcall operator!=<CThing>(const CBaseObject* a, const CIntelligentPointer<CThing>& b) { return (const void*)a != ((const IPtr*)&b)->item; }
template<class T> bool __fastcall IS_THING_ALIVE(const T& thing);
template<> bool __fastcall IS_THING_ALIVE<CIntelligentPointer<CThing> >(const CIntelligentPointer<CThing>& p)
{ const IPtr* q = (const IPtr*)&p; return q->item != 0 && !(((Thing*)q->item)->b[0x91] & 1); }
extern "C" void* __fastcall IPGet(const IPtr* p, void*) { return p->item; }
extern "C" void* __fastcall StrCtor(StrData** self, void*, const char* text, long len)
{ StrData* d = (StrData*)malloc(sizeof(StrData)); d->text = text; d->len = (unsigned long)strlen(text); *self = d; ++g_strLive; return self; }
extern "C" void __fastcall StrDtor(StrData** self, void*) { free(*self); *self = 0; --g_strLive; }
extern "C" void* __fastcall ActCtor(void* self, void*, Thing* creature, Thing* thing, void* parent, long priority)
{ CHECK(creature == &g_parent && parent == 0 && priority == 150); Log("Unsheathe(%c) ", Name(thing)); ++g_actLive; return self; }
extern "C" void __fastcall ActDtor(void* self, void*) { --g_actLive; Log("~Act "); }
extern "C" void __fastcall ActPerform(void* self, void*) { Log("Perform "); }
extern "C" bool __fastcall SetAction(Thing* self, void*, const void* action) { CHECK(self == &g_parent); Log("SetAction "); return g_setActionResult != 0; }
extern "C" void __fastcall MSApply(void* self, void*, Thing* t) { CHECK(self == g_ms); Log("MSApply(%c) ", Name(t)); }
extern "C" void __fastcall MSRemove(void* self, void*, Thing* t) { CHECK(self == g_ms); Log("MSRemove(%c) ", Name(t)); }
extern "C" void __fastcall MAApply(void* self, void*, Thing* t) { CHECK(self == g_ma); Log("MAApply(%c) ", Name(t)); }
extern "C" void __fastcall MARemove(void* self, void*, Thing* t) { CHECK(self == g_ma); Log("MARemove(%c) ", Name(t)); }
extern "C" void __fastcall GetAllCarried(void* self, void*, Vec* v)
{
    CHECK(self == g_carrying && v->start == 0 && v->finish == 0 && v->eos == 0);
    v->start = (void**)malloc(sizeof(void*) * 8); for (int i = 0; i < g_ncarried; ++i) v->start[i] = g_carried[i];
    v->finish = v->start + g_ncarried; v->eos = v->start + 8; Log("AllCarried ");
}
extern "C" Def** __fastcall PDefFromIndex(void* self, void*, Def** ret, long index) { CHECK(self == g_mgr && index == 63); *ret = &g_carryDef; ++g_carryDef.ref; return ret; }
extern "C" Thing* __fastcall InCarrySlot(void* self, void*, long slot) { CHECK(self == g_carrying && slot == g_expectSlot); Log("InSlot(%ld) ", slot); return g_slotThing; }
extern "C" Thing* __fastcall InPrimarySlot(void* self, void*) { CHECK(self == g_carrying); return g_primary; }
static Weapon* W(const void* w) { return (Weapon*)w; }
extern "C" bool __fastcall IsMelee(const void* w, void*) { return W(w)->melee != 0; }
extern "C" bool __fastcall IsProjectile(const void* w, void*) { return W(w)->projectile != 0; }
extern "C" bool __fastcall IsProjectileWeapon(const void* w, void*) { return W(w)->projectileWeapon != 0; }
extern "C" bool __fastcall CarryingWeapon(const Thing* t) { CHECK(t == &g_parent); return g_carryingWeapon != 0; }
extern "C" void __fastcall PutInSlot(void* self, void*, Thing* t, long slot, bool b) { CHECK(self == g_carrying); Log("Put(%c,%ld,%d) ", Name(t), slot, (int)b); }
extern "C" void __fastcall Refresh(Comp* self, void*) { Log("Refresh "); }
extern "C" void __fastcall RemoveMode(void* self, void*, unsigned long mode) { CHECK(self == g_cmm); Log("RemoveMode(%lu) ", mode); }
extern "C" void __fastcall RemoveThing(void* self, void*, Thing* t) { CHECK(self == g_carrying); Log("Remove(%c) ", Name(t)); }
extern "C" void __fastcall ReturnToInv(char* item, void*) { int i = (int)((item - &g_items[0][0]) / 4); Log("Return(%c) ", Name(&g_things[i])); }
extern "C" void __fastcall DefDelete(Def* self, void*) { ++g_deletes; }
extern "C" long* __fastcall SubDefInfo(Def* self, void*, StrData* const* name)
{ CHECK(self == &g_weaponDef && strcmp((*name)->text, "CCarryableDef") == 0); return g_subInfo; }

enum { H, S, M, R, A, B, C, X };
static const char kNames[] = "HSMRABCX";
struct Setup {
    int heroAllowed, hasHero, dying, hasMS, hasMA, shots, inHand, slotThing, melee, ranged, setAction, carryingWeapon;
    int weapon[8];   // 0 none, 1 melee, 2 projectile weapon (IsProjectileWeapon and IsProjectile), 3 non-projectile, not melee
    int dead[8]; int item[8]; int carried[8]; int ncarried;
};
static void Run(const Setup& s, const char* want)
{
    ++g_run; memset(g_log, 0, sizeof g_log); g_deletes = g_strLive = g_actLive = 0;
    memset(&g_parent, 0, sizeof g_parent); g_parent.b[0] = 'P'; memset(g_things, 0, sizeof g_things);
    memset(g_hero, 0, sizeof g_hero); g_hero[0x0C] = (char)s.heroAllowed; *(long*)&g_ma[0x10] = s.shots;
    if (s.hasHero) AddComp(&g_parent, 0x29, g_hero);
    if (s.hasMS) AddComp(&g_parent, 0x87, g_ms);
    if (s.hasMA) AddComp(&g_parent, 0x88, g_ma);
    AddComp(&g_parent, 0x46, g_carrying); AddComp(&g_parent, 0x31, g_cmm); AddComp(&g_parent, 0x10, g_hero + 1);   // an unrelated low key
    for (int i = 0; i < 8; ++i) {
        Thing* t = &g_things[i]; t->b[0] = (unsigned char)kNames[i]; t->b[0x91] = (unsigned char)(s.dead[i] ? 1 : 0);
        *(Def**)&t->b[0x70] = &g_weaponDef;
        g_weapons[i].melee = s.weapon[i] == 1; g_weapons[i].projectileWeapon = s.weapon[i] == 2; g_weapons[i].projectile = s.weapon[i] == 2;
        if (s.weapon[i]) AddComp(t, 0x48, &g_weapons[i]);
        if (s.item[i]) AddComp(t, 0x1A, g_items[i]);
        AddComp(t, 0x05, g_hero + 2);
    }
    Comp comp; memset(&comp, 0, sizeof comp); *(Thing**)&comp.b[4] = &g_parent; comp.b[0x88] = (unsigned char)s.dying;
    IPtr* melee = (IPtr*)&comp.b[0x134]; IPtr* ranged = (IPtr*)&comp.b[0x141];
    melee->item = s.melee >= 0 ? &g_things[s.melee] : 0; ranged->item = s.ranged >= 0 ? &g_things[s.ranged] : 0;
    g_primary = s.inHand >= 0 ? &g_things[s.inHand] : 0; g_slotThing = s.slotThing >= 0 ? &g_things[s.slotThing] : 0; g_expectSlot = 3;
    g_ncarried = s.ncarried; for (int i = 0; i < s.ncarried; ++i) g_carried[i] = &g_things[s.carried[i]];
    g_setActionResult = s.setAction; g_carryingWeapon = s.carryingWeapon;
    memset(&g_weaponDef, 0, sizeof g_weaponDef); g_weaponDef.vt = g_wdVt; g_weaponDef.ref = 1; g_weaponDef.manager = g_mgr;
    g_wdVt[1] = (void*)DefDelete; g_wdVt[14] = (void*)SubDefInfo;
    memset(&g_carryDef, 0, sizeof g_carryDef); g_carryDef.vt = g_cdVt; g_carryDef.ref = 1; g_carryDef.passiveSlot = 3; g_cdVt[1] = (void*)DefDelete;
    CallRow(&comp, 0);
    if (strcmp(g_log, want) != 0) { g_fail = 1; printf("run %d\n  got  %s\n  want %s\n", g_run, g_log, want); }
    CHECK(g_weaponDef.ref == 1 && g_carryDef.ref == 1 && g_deletes == 0 && g_strLive == 0 && g_actLive == 0);
    CHECK(melee->item == (s.melee >= 0 ? &g_things[s.melee] : 0) && ranged->item == (s.ranged >= 0 ? &g_things[s.ranged] : 0));
}

int main()
{
    Setup s;
    // 1: the hero with weapon use disallowed: nothing happens
    memset(&s, 0, sizeof s); s.hasHero = 1; s.heroAllowed = 0; s.inHand = s.slotThing = s.melee = s.ranged = -1;
    Run(s, "");
    // 2: weapon use allowed but the inventory is dying: nothing happens
    s.heroAllowed = 1; s.dying = 1; Run(s, "");
    // 3: a melee weapon in hand, another active melee weapon: unsheathe it (the action is refused, so performed directly), drop the
    //    hand weapon's multi-strike; then put away every carried non-projectile weapon that is not active; re-apply multi-strike to M
    memset(&s, 0, sizeof s); s.hasHero = 1; s.heroAllowed = 1; s.hasMS = 1; s.inHand = H; s.slotThing = -1; s.melee = M; s.ranged = R;
    s.weapon[H] = 1; s.weapon[M] = 1; s.weapon[R] = 2; s.weapon[A] = 3; s.weapon[B] = 2; s.item[H] = 1; s.item[A] = 0;
    s.carried[0] = H; s.carried[1] = A; s.carried[2] = B; s.carried[3] = C; s.carried[4] = M; s.carried[5] = R; s.ncarried = 6;
    s.setAction = 0; s.carryingWeapon = 0;
    Run(s, "Refresh Unsheathe(M) SetAction Perform MSRemove(H) ~Act AllCarried Remove(H) RemoveMode(4) Return(H) Remove(A) RemoveMode(4) MSApply(M) ");
    // 4: the same with the parent still carrying a weapon after each removal: no mode change; no multi-strike component: no effects
    s.hasMS = 0; s.carryingWeapon = 1; s.setAction = 1;
    Run(s, "Refresh Unsheathe(M) SetAction ~Act AllCarried Remove(H) Return(H) Remove(A) ");
    // 4b: the active ranged weapon is carried with a non-projectile weapon: kept (it is active); the last carried thing A is put away
    memset(&s, 0, sizeof s); s.hasMS = 1; s.inHand = H; s.slotThing = -1; s.melee = M; s.ranged = R; s.weapon[H] = 1; s.weapon[M] = 1;
    s.weapon[R] = 3; s.weapon[A] = 3; s.carried[0] = R; s.carried[1] = M; s.carried[2] = A; s.ncarried = 3; s.carryingWeapon = 1; s.setAction = 1;
    Run(s, "Refresh Unsheathe(M) SetAction MSRemove(H) ~Act AllCarried Remove(A) MSApply(M) ");
    // 5: a projectile weapon in hand: unsheathe the active ranged weapon (accepted), drop the hand weapon's multi-arrow; re-apply it to R
    memset(&s, 0, sizeof s); s.hasMA = 1; s.shots = 2; s.inHand = H; s.slotThing = -1; s.melee = M; s.ranged = R;
    s.weapon[H] = 2; s.weapon[M] = 1; s.weapon[R] = 2; s.setAction = 1; s.ncarried = 0;
    Run(s, "Refresh Unsheathe(R) SetAction MARemove(H) ~Act AllCarried MAApply(R) ");
    // 6: as 5 with no shots left: unsheathe as before, but multi-arrow is not re-applied
    s.shots = 0; Run(s, "Refresh Unsheathe(R) SetAction MARemove(H) ~Act AllCarried ");
    // 6b: shots left but the active ranged weapon dead: no unsheathe, no re-apply
    s.shots = 3; s.dead[R] = 1; Run(s, "Refresh AllCarried ");
    // 7: the hand weapon IS the active melee weapon: nothing to unsheathe; multi-strike is not re-applied (it is in hand)
    memset(&s, 0, sizeof s); s.hasMS = 1; s.inHand = M; s.slotThing = -1; s.melee = M; s.ranged = -1; s.weapon[M] = 1;
    Run(s, "Refresh AllCarried ");
    // 8: nothing in hand, the passive slot (3) holds another melee weapon S: drop its multi-strike, return it, sheathe M there
    memset(&s, 0, sizeof s); s.hasMS = 1; s.hasMA = 1; s.shots = 1; s.inHand = -1; s.slotThing = S; s.melee = M; s.ranged = R;
    s.weapon[S] = 1; s.weapon[M] = 1; s.weapon[R] = 2; s.item[S] = 1;
    Run(s, "Refresh InSlot(3) MSRemove(S) Remove(S) Return(S) Put(M,3,1) AllCarried MSApply(M) MAApply(R) ");
    // 9: nothing in hand, empty passive slot: sheathe M there
    s.slotThing = -1; s.hasMA = 0; Run(s, "Refresh InSlot(3) Put(M,3,1) AllCarried MSApply(M) ");
    // 10: the slot holds a dead thing: sheathe M there
    s.slotThing = S; s.dead[S] = 1; Run(s, "Refresh InSlot(3) Put(M,3,1) AllCarried MSApply(M) ");
    // 11: the slot holds a projectile weapon: drop its multi-arrow (the active ranged weapon is another), leave it there
    memset(&s, 0, sizeof s); s.hasMS = 1; s.hasMA = 1; s.shots = 1; s.inHand = -1; s.slotThing = S; s.melee = M; s.ranged = R;
    s.weapon[S] = 2; s.weapon[M] = 1; s.weapon[R] = 2;
    Run(s, "Refresh InSlot(3) MARemove(S) AllCarried MSApply(M) MAApply(R) ");
    // 12: the slot holds a non-weapon thing with an inventory item: it goes back to the inventory and M is sheathed
    s.weapon[S] = 0; s.item[S] = 1; Run(s, "Refresh InSlot(3) Remove(S) Return(S) Put(M,3,1) AllCarried MSApply(M) MAApply(R) ");
    // 13: the slot already holds M: nothing to do there
    s.slotThing = M; Run(s, "Refresh InSlot(3) AllCarried MSApply(M) MAApply(R) ");
    // 14: nothing in hand and no live active melee weapon: no slot work at all
    s.dead[M] = 1; Run(s, "Refresh AllCarried MAApply(R) ");
    if (!g_fail) printf("005ca68e_TEST PASS\n");
    return g_fail;
}