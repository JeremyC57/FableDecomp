// Behaviour test for 00800a60: src/00800a60.cpp copied verbatim; the persist context, the UID remap, the target
// pointer and the route save/load stubbed so the version transfer, the saving path (target UID remapped, route saved)
// and the loading path (UID remapped when non-zero, route loaded with the transferred version) are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCPreCalculatedNavigationRoute::OnSerialise(CPersistContext& context, bool) -- persist the route version (2) and the target thing's UID (taken from the intelligent pointer and remapped when saving, remapped back when loading), then save or load the route itself
// Retry 10b (chain-split sweep, FIXES B-16): the `GetPItem()` result is a named local `item` before it is dereferenced, which gives retail's register colouring/order (the chained call gives the parked difference).
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00800A60, 300 bytes, `ret 8`. CTCPreCalculatedNavigationRoute vtable 0x01242444 slot 1.
// Callees: CPersistContext::Transfer(const char*, long&, const long&) @ 0x00410BE0, CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50 (twice), CThing::IsLevelLess @ 0x004C7930, CTCBase::GetThingManager @ 0x00686AE0,
//   CThingManager::RemapUIDForSerialisation @ 0x0051E210, CPersistContext::Transfer(const char*, unsigned __int64&,
//   const unsigned __int64&) @ 0x0044BB00, SaveRoute(CPersistContext&) @ 0x008006D0, LoadRoute(CPersistContext&, long)
//   @ 0x008008B0. Names "PrecCalculatedNavigationRouteVersion" 0x012722AC, "ThingToCalculateRouteToUID" 0x01272290.
// Layout = Ego_r.pdb CTCPreCalculatedNavigationRoute (48 bytes): CTCBase 12 bytes, PThingToCalculateRouteTo
//   (CIntelligentPointer<const CThing>) +0x0C, ThingToCalculateRouteToUID +0x18, Route +0x20.
// Shape: the CTCVillageMember::OnSerialise UID block (0x007EE3D0) behind a version Transfer; the zero register (ebp) is the
//   long default, the null UID, the 64-bit default and the `false` push; the version local is passed on to LoadRoute.
#include "engine/CTCBase.h"
struct CThing {
    unsigned char _pad00[0x18];
    unsigned __int64 UID;                       // +0x18
    unsigned char _pad20[0x8e - 0x20];
    short m_level;                              // +0x8e
    bool IsLevelLess();                         // 0x004C7930
};
struct CThingManager {
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid);   // 0x0051E210
};
struct CPersistContext {
    unsigned char _pad00[0x18];
    long Mode;                                  // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, long& value, const long& defaultValue);                         // 0x00410BE0
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue); // 0x0044BB00
};
struct CBaseIntelligentPointer {
    void* m_pNode;
    void* m_pNext;
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CTCPreCalculatedNavigationRoute : public CTCBase {
public:
    CBaseIntelligentPointer PThingToCalculateRouteTo;   // +0x0c
    unsigned char           _pad14[0x4];
    unsigned __int64        ThingToCalculateRouteToUID; // +0x18
    unsigned char           Route[0xc];                 // +0x20 vector<pair<C2DVector, unsigned char> >
    CThingManager* GetThingManager();           // 0x00686AE0 (CTCBase)
    void SaveRoute(CPersistContext& context);   // 0x008006D0
    void LoadRoute(CPersistContext& context, long version);   // 0x008008B0
    void OnSerialise(CPersistContext& context, bool);
};

void CTCPreCalculatedNavigationRoute::OnSerialise(CPersistContext& context, bool)
{
    long version = 2;
    context.Transfer("PrecCalculatedNavigationRouteVersion", version, 0);
    if (context.IsSaving()) {
        if (PThingToCalculateRouteTo.GetPItem() == 0) {
            ThingToCalculateRouteToUID = 0;
        } else {
            CThing* item = PThingToCalculateRouteTo.GetPItem();
            ThingToCalculateRouteToUID = item->UID;
            ThingToCalculateRouteToUID = GetThingManager()->RemapUIDForSerialisation(true, ParentThing->IsLevelLess(), ParentThing->m_level, ThingToCalculateRouteToUID);
        }
        context.Transfer("ThingToCalculateRouteToUID", ThingToCalculateRouteToUID, 0);
        SaveRoute(context);
    } else {
        context.Transfer("ThingToCalculateRouteToUID", ThingToCalculateRouteToUID, 0);
        if (ThingToCalculateRouteToUID != 0)
            ThingToCalculateRouteToUID = GetThingManager()->RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, ThingToCalculateRouteToUID);
        LoadRoute(context, version);
    }
}
// ---- end source ----
static unsigned __int64 g_loaded = 0; static long g_loadedVersion = 2; static int g_transfers = 0;
void CPersistContext::Transfer(const char*, long& v, const long&) { ++g_transfers; if (Mode == 2) v = g_loadedVersion; }
void CPersistContext::Transfer(const char*, unsigned __int64& v, const unsigned __int64&) { ++g_transfers; if (Mode == 2) v = g_loaded; }
static bool g_saving = false; static long g_lvl = -1;
unsigned __int64 CThingManager::RemapUIDForSerialisation(bool saving, bool, long level, unsigned __int64 uid) { g_saving = saving; g_lvl = level; return uid + 1000; }
bool CThing::IsLevelLess() { return false; }
static CThingManager g_tm;
CThingManager* CTCPreCalculatedNavigationRoute::GetThingManager() { return &g_tm; }
static CThing* g_target = 0;
CThing* CBaseIntelligentPointer::GetPItem() const { return g_target; }
static int g_saves = 0, g_loads = 0; static long g_loadVersion = -1;
void CTCPreCalculatedNavigationRoute::SaveRoute(CPersistContext&) { ++g_saves; }
void CTCPreCalculatedNavigationRoute::LoadRoute(CPersistContext&, long version) { ++g_loads; g_loadVersion = version; }
int main()
{
    static CThing parent, target; memset(&parent, 0, sizeof(parent)); memset(&target, 0, sizeof(target));
    parent.m_level = 4; target.UID = 12;
    static unsigned char raw[sizeof(CTCPreCalculatedNavigationRoute)]; memset(raw, 0, sizeof(raw));
    CTCPreCalculatedNavigationRoute* r = (CTCPreCalculatedNavigationRoute*)raw; *(CThing**)&r->ParentThing = &parent;
    CPersistContext ctx; memset(&ctx, 0, sizeof(ctx)); ctx.Mode = 3;
    r->ThingToCalculateRouteToUID = 9; r->OnSerialise(ctx, false);
    CHECK(r->ThingToCalculateRouteToUID == 0 && g_saves == 1 && g_transfers == 2);
    g_target = &target; r->OnSerialise(ctx, false);
    CHECK(r->ThingToCalculateRouteToUID == 1012 && g_saving && g_lvl == 4 && g_saves == 2);
    ctx.Mode = 2; g_loaded = 30; g_loadedVersion = 1; r->OnSerialise(ctx, false);
    CHECK(r->ThingToCalculateRouteToUID == 1030 && !g_saving && g_loads == 1 && g_loadVersion == 1);
    g_loaded = 0; r->OnSerialise(ctx, false); CHECK(r->ThingToCalculateRouteToUID == 0 && g_loads == 2);
    if (fails == 0) printf("00800a60_TEST PASS\n"); else printf("00800a60_TEST FAIL\n");
    return fails;
}