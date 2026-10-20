// behaviour test for CThingObject::WorldUpdate (0x00704150): base result, die-off countdown (held by TC 0x47 or a shot TC), kill rules, own and shot updates
#include <cstdio>
struct CTCBase;
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; } };
struct CTCDieOffHold_47 { long HoldCount; };
static int g_shotUpdates = 0, g_ownUpdates = 0, g_kills = 0; static bool g_baseResult = false;
struct CTCShotDefinitionBase { void Update() { ++g_shotUpdates; } };
class CThingGameObject { public:
    unsigned long UsingComponents[9]; CInterfaceMap Components; float Health;
    void Kill(bool immediately) { if (immediately) ++g_kills; }
    bool WorldUpdate() { return g_baseResult; }
    bool HasTC(long type) const { if (UsingComponents[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template<class T> T* GetTC(long t) { long type = t; CInterfaceMap* map = &Components; CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || t < it->type) it = map->last; return (T*)it->component; }
    template<class T> bool GetInterface(const T** out, long t) const
    { if (UsingComponents[t >> 5] & (1UL << (t & 31))) { long type = t; const CInterfaceMap* map = &Components; CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || t < it->type) it = map->last; *out = (const T*)it->component; return true; } return false; } };
class CThingObject : public CThingGameObject { public:
    long DieOffTimer; bool DiesOff;
    void UpdateObject_007034d0() { ++g_ownUpdates; }
    bool IsDieOffHeld() const { const CTCDieOffHold_47* hold; return GetInterface(&hold, 0x47) && hold->HoldCount != 0; }
    bool WorldUpdate()
    {
        if (CThingGameObject::WorldUpdate()) return true;
        bool diesOff = DiesOff;
        if (diesOff) { if (!IsDieOffHeld() && !HasTC(0x77)) --DieOffTimer; }
        if (!(Health <= 0.0f) && (!diesOff || DieOffTimer > 0)) {
            UpdateObject_007034d0();
            if (HasTC(0x77)) GetTC<CTCShotDefinitionBase>(0x77)->Update();
            return false;
        }
        Kill(true);
        return true;
    } };
int main()
{
    CTCDieOffHold_47 hold; hold.HoldCount = 1; CTCShotDefinitionBase shot;
    CInterfaceMap::SEntry e[2] = { { 0x47, (CTCBase*)&hold }, { 0x77, (CTCBase*)&shot } };
    CThingObject o; for (int i = 0; i < 9; ++i) o.UsingComponents[i] = 0; o.Components.first = e; o.Components.last = e + 2;
    o.Health = 10.0f; o.DiesOff = true; o.DieOffTimer = 2;
    int bad = 0;
    if (o.WorldUpdate() || o.DieOffTimer != 1 || g_ownUpdates != 1) bad = 1;                    // counts down
    o.UsingComponents[2] = 1UL << 7;                                                          // held by TC 0x47
    if (o.WorldUpdate() || o.DieOffTimer != 1) bad = 1;
    hold.HoldCount = 0; if (!o.WorldUpdate() || o.DieOffTimer != 0 || g_kills != 1) bad = 1;  // time up: killed
    o.DieOffTimer = 5; o.UsingComponents[3] = 1UL << 23;                                      // shot TC: no countdown, shot updated
    if (o.WorldUpdate() || o.DieOffTimer != 5 || g_shotUpdates != 1) bad = 1;
    o.Health = 0.0f; if (!o.WorldUpdate() || g_kills != 2) bad = 1;                            // no health: killed
    g_baseResult = true; if (!o.WorldUpdate() || g_kills != 2) bad = 1;                       // base handled it
    if (bad) { std::printf("00704150_TEST FAIL\n"); return 1; }
    std::printf("00704150_TEST PASS\n");
    return 0;
}
