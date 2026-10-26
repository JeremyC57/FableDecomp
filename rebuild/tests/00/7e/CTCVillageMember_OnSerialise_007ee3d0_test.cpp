// Behaviour test for 007ee3d0: src/007ee3d0.cpp copied verbatim; the persist context's Transfer, the thing manager's
// UID remap and the village pointer stubbed so the saving path (0 without a village, else the remapped village UID)
// and the loading path (transfer, then remap a non-zero UID) are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCVillageMember::OnSerialise(CPersistContext& context, bool) -- persist the member's village UID: saving takes it from the village intelligent pointer (0 when none) and remaps it for the save; loading remaps a non-zero saved UID back
// Retry 10b (chain-split sweep, FIXES B-16): the `GetPItem()` result is a named local `item` before it is dereferenced, which gives retail's register colouring/order (the chained call gives the parked difference).
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EE3D0, 242 bytes, `ret 8`. CTCVillageMember vtable 0x0124007C slot 1 (the manifest
//   label "CTCPreCalculatedNavigationRoute::OnSerialise" is propagated; the route's own OnSerialise is 0x00800A60).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CThing::IsLevelLess @ 0x004C7930,
//   CTCBase::GetThingManager @ 0x00686AE0, CThingManager::RemapUIDForSerialisation(bool, bool, long, unsigned __int64)
//   @ 0x0051E210, CPersistContext::Transfer(const char*, unsigned __int64&, const unsigned __int64&) @ 0x0044BB00 (manifest
//   "Transfer<signed_char>", a folded label); the name literal "VillageUID" @ 0x0127172C.
// Layout = Ego_r.pdb CTCVillageMember (32 bytes): CTCBase {vptr, ParentThing +4, flags +8}, PDef +0x0C,
//   PVillage (CIntelligentPointer<CThing>) +0x10, VillageUID +0x18; the village thing's UID at +0x18; CThing level +0x8E.
// Shape: same as the landed CTCActivationTrigger::OnSerialise (0x0077B0D0): the save arm calls GetPItem once for the null
//   test and once for the UID; the load arm tests the UID with `!= 0` (`or ecx,edx ; je`), and ebx (zero) also serves the
//   `false` push and the Transfer default.
#include "engine/CTCBase.h"
class CThing { public:
    unsigned char _pad00[0x18];
    unsigned __int64 UID;                       // +0x18
    unsigned char _pad20[0x8e - 0x20];
    short m_level;                              // +0x8e
    bool IsLevelLess();                         // 0x004C7930
};
class CThingManager { public:
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid);   // 0x0051E210
};
class CPersistContext { public:
    unsigned char _pad00[0x18];
    long Mode;                                  // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);   // 0x0044BB00
};
struct CBaseIntelligentPointer {
    void* m_pNode;
    void* m_pNext;
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CVillageMemberDef;
class CTCVillageMember : public CTCBase {
public:
    CVillageMemberDef*      PDef;               // +0x0c
    CBaseIntelligentPointer PVillage;           // +0x10
    unsigned __int64        VillageUID;         // +0x18
    CThingManager* GetThingManager();           // 0x00686AE0 (CTCBase)
    void OnSerialise(CPersistContext& context, bool);
};

void CTCVillageMember::OnSerialise(CPersistContext& context, bool)
{
    if (context.IsSaving()) {
        if (PVillage.GetPItem() == 0) {
            VillageUID = 0;
        } else {
            CThing* item = PVillage.GetPItem();
            VillageUID = item->UID;
            VillageUID = GetThingManager()->RemapUIDForSerialisation(true, ParentThing->IsLevelLess(), ParentThing->m_level, VillageUID);
        }
        context.Transfer("VillageUID", VillageUID, 0);
    } else {
        context.Transfer("VillageUID", VillageUID, 0);
        if (VillageUID != 0)
            VillageUID = GetThingManager()->RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, VillageUID);
    }
}
// ---- end source ----
static unsigned __int64 g_loaded = 0; static int g_transfers = 0; static char g_name[32];
void CPersistContext::Transfer(const char* name, unsigned __int64& value, const unsigned __int64& def)
{ ++g_transfers; strcpy(g_name, name); if (Mode == 2) value = g_loaded; }
static bool g_saving = false; static bool g_levelLess = true; static long g_lvl = 0;
unsigned __int64 CThingManager::RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid)
{ g_saving = saving; g_levelLess = levelLess; g_lvl = level; return uid + 1000; }
bool CThing::IsLevelLess() { return false; }
static CThingManager g_tm;
CThingManager* CTCVillageMember::GetThingManager() { return &g_tm; }
static CThing* g_village = 0;
CThing* CBaseIntelligentPointer::GetPItem() const { return g_village; }
int main()
{
    static CThing parent, village; memset(&parent, 0, sizeof(parent)); memset(&village, 0, sizeof(village));
    parent.m_level = 3; village.UID = 77;
    static unsigned char raw[sizeof(CTCVillageMember)]; memset(raw, 0, sizeof(raw));
    CTCVillageMember* m = (CTCVillageMember*)raw; *(CThing**)&m->ParentThing = &parent;
    CPersistContext ctx; memset(&ctx, 0, sizeof(ctx)); ctx.Mode = 1;
    m->VillageUID = 5; m->OnSerialise(ctx, false);
    CHECK(m->VillageUID == 0 && g_transfers == 1 && strcmp(g_name, "VillageUID") == 0);       // no village
    g_village = &village; m->OnSerialise(ctx, false);
    CHECK(m->VillageUID == 1077 && g_saving == true && g_levelLess == false && g_lvl == 3 && g_transfers == 2);
    ctx.Mode = 2; g_loaded = 40; m->OnSerialise(ctx, false);
    CHECK(m->VillageUID == 1040 && g_saving == false && g_transfers == 3);
    g_loaded = 0; m->VillageUID = 9; m->OnSerialise(ctx, false);
    CHECK(m->VillageUID == 0);
    if (fails == 0) printf("007ee3d0_TEST PASS\n"); else printf("007ee3d0_TEST FAIL\n");
    return fails;
}