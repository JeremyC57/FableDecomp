// Behaviour test for 0x0090C900 CThingFilter_IsValidGossipee::operator(): the row's defines, includes and body verbatim over
// the shared headers, run on zeroed object images (no engine constructor runs) with doubles for every out-of-line callee:
// CVectorMap::LowerBound const (the header's own body), CTCVillageMember::GetPVillage, CTCVillage::IsSociableTime,
// CBaseIntelligentPointer::GetPItem, CThing::GetPos and CThingBuilding::IsPointInside.
#include <stdio.h>
#include <string.h>
// ---- row body ----
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_CURFEW
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CThingFilter_IsValidGossipee.h"
// FableWin.pdb ?GetHomeBuilding@CThingAICreature@@QBEPAVCThingBuilding@@XZ, inline in retail (declared in engine/CThingAICreature.h).
inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }

bool CThingFilter_IsValidGossipee::operator()(const CThingAICreature& thing) const
{
    const CTCVillageMember* member;
    if (PFaction) {
        const CTCEnemy* enemy;
        if (!thing.PeekTC(&enemy) || PFaction != enemy->GetPFaction())
            return false;
    } else if (thing.HasTC(TCI_ENEMY))
        goto fail;
    if (PTCVillage) {
        if (!thing.PeekTC(&member) || PTCVillage != member->GetPVillage()) {
fail:
            return false;
        }
    } else if (thing.PeekTC(&member) && member->GetPVillage())
        return false;
    if (PTCVillage && !PTCVillage->IsSociableTime()) {
        CThingBuilding* home = thing.GetHomeBuilding();
        if (home)
            return &thing != Thing && Thing->GetHomeBuilding() == home && home->IsPointInside(thing.GetPos());
        return &thing != Thing && !Thing->GetHomeBuilding();
    }
    return &thing != Thing;
}


// ---- doubles ----
static const void* g_memberOf[4]; static CTCVillage* g_villageOf[4]; static int g_members = 0;
CTCVillage* CTCVillageMember::GetPVillage() const
{
    for (int i = 0; i < g_members; ++i)
        if (g_memberOf[i] == (const void*)this)
            return g_villageOf[i];
    return 0;
}
static bool g_sociable = false; static int g_sociableCalls = 0;
bool CTCVillage::IsSociableTime() const { ++g_sociableCalls; return g_sociable; }
// the pointer images keep the building itself in PData
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
static C3DVector* g_pos; static const CThing* g_posOf = 0;
const C3DVector& CThing::GetPos() const { g_posOf = this; return *g_pos; }
static bool g_inside = true; static const CThingBuilding* g_insideOf = 0; static const C3DVector* g_insidePoint = 0;
bool CThingBuilding::IsPointInside(const C3DVector& point) const { g_insideOf = this; g_insidePoint = &point; return g_inside; }

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
struct SEntry { long type; void* component; };          // std::pair<ETCInterfaceType,CTCBase*>
struct SCreature {                                      // one zeroed CThingAICreature image and its component table
    unsigned char image[0x400];
    SEntry entries[2]; int count;
    unsigned char enemy[0x40], member[0x40];
    void Clear() { memset(this, 0, sizeof(*this)); Table(); }
    void Table() { *(SEntry**)(image + 0x44) = entries; *(SEntry**)(image + 0x48) = entries + count; }
    void Add(long type, void* tc)
    {
        // keep the table sorted by interface type (TCI_VILLAGE_MEMBER 0x23 before TCI_ENEMY 0x49)
        int at = count;
        while (at > 0 && entries[at - 1].type > type) { entries[at] = entries[at - 1]; --at; }
        entries[at].type = type; entries[at].component = tc; ++count;
        *(unsigned long*)(image + 0x20 + 4 * (type >> 5)) |= 1UL << (type & 31);
        Table();
    }
    void SetFaction(const void* faction) { *(const void**)(enemy + 0x18) = faction; Add(0x49, enemy); }
    void SetVillage(CTCVillage* village) { g_memberOf[g_members] = member; g_villageOf[g_members] = village; ++g_members; Add(0x23, member); }
    void SetHome(void* building) { *(void**)(image + 0x168 + 4) = building; }
    const CThingAICreature& Thing() const { return *(const CThingAICreature*)image; }
};
static SCreature g_self, g_other;
static unsigned char g_filterImage[12];
static bool Run(const void* faction, void* village)
{
    *(const void**)(g_filterImage + 0) = g_self.image;
    *(const void**)(g_filterImage + 4) = faction;
    *(void**)(g_filterImage + 8) = village;
    return (*(const CThingFilter_IsValidGossipee*)g_filterImage)(g_other.Thing());
}
int main()
{
    static unsigned char factionA[4], factionB[4], villageA[4], villageB[4], home[4], home2[4];
    static float pos[3];
    g_pos = (C3DVector*)pos;
    CTCVillage* va = (CTCVillage*)villageA; CTCVillage* vb = (CTCVillage*)villageB;
    check(sizeof(CThingFilter_IsValidGossipee) == 12, "filter layout");

    // no faction, no village required
    g_self.Clear(); g_other.Clear(); g_members = 0;
    check(Run(0, 0) == true, "plain creature accepted");
    check((*(const CThingFilter_IsValidGossipee*)g_filterImage)(g_self.Thing()) == false, "the gossiper itself rejected");
    g_other.SetFaction(factionA);
    check(Run(0, 0) == false, "factionless filter rejects a creature with an enemy component");
    g_other.Clear(); g_other.SetVillage(va);
    check(Run(0, 0) == false, "village-less filter rejects a villager");
    g_other.Clear(); g_members = 0; g_other.SetVillage(0);
    check(Run(0, 0) == true, "village member without a village accepted");

    // faction required
    g_other.Clear(); g_members = 0;
    check(Run(factionA, 0) == false, "required faction: no enemy component");
    g_other.SetFaction(factionB);
    check(Run(factionA, 0) == false, "required faction: other faction");
    g_other.Clear(); g_other.SetFaction(factionA);
    check(Run(factionA, 0) == true, "required faction: same faction");

    // village required, sociable hours: only identity matters after the village test
    g_sociable = true; g_sociableCalls = 0;
    g_other.Clear(); g_members = 0;
    check(Run(0, va) == false && g_sociableCalls == 0, "required village: not a villager");
    g_other.SetVillage(vb);
    check(Run(0, va) == false && g_sociableCalls == 0, "required village: other village");
    g_other.Clear(); g_members = 0; g_other.SetVillage(va);
    check(Run(0, va) == true && g_sociableCalls == 1, "required village: same village, sociable hours");
    g_other.SetFaction(factionA);
    check(Run(factionA, va) == true, "faction and village both matched");
    check(Run(factionB, va) == false, "village matched, faction not");

    // outside the sociable hours the home buildings decide
    g_sociable = false; g_other.Clear(); g_members = 0; g_other.SetVillage(va);
    check(Run(0, va) == true, "both homeless");
    g_self.SetHome(home);
    check(Run(0, va) == false, "gossiper has a home, candidate none");
    g_other.SetHome(home2);
    check(Run(0, va) == false, "different homes");
    g_other.SetHome(home); g_inside = true; g_insideOf = 0; g_posOf = 0;
    check(Run(0, va) == true, "same home, candidate inside");
    check((const void*)g_insideOf == (const void*)home && g_insidePoint == g_pos && (const void*)g_posOf == (const void*)g_other.image, "IsPointInside(home, candidate position)");
    g_inside = false;
    check(Run(0, va) == false, "same home, candidate outside");
    g_self.SetHome(0);
    check(Run(0, va) == false, "candidate has a home, gossiper none");

    if (!g_fail) printf("0090c900_TEST PASS\n");
    return g_fail;
}