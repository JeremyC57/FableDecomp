// behaviour test for CThingFilter_IsInSameVolumeContextAsThing::operator() (0x006A8C60): override, volume choice (0x75 then 0x33), alive agreement
#include <cstdio>
struct CTCBase;
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; } };
class CThing;
struct CBaseIntelligentPointer { CThing* m_p; CThing* GetPItem() const { return m_p; } };
struct CTCInsideBuilding { CBaseIntelligentPointer PBuilding; };
struct CTCOwnedEntity { CBaseIntelligentPointer POwner; };
class CThing { public:
    unsigned long UsingComponents[9]; CInterfaceMap Components; unsigned char Dead : 1;
    bool HasTC(long type) const { if (UsingComponents[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template<class T> T* GetTC(long interfaceType) const
    { long type = interfaceType; const CInterfaceMap* map = &Components; CInterfaceMap::SEntry* it = map->LowerBound(&type);
      if (it == map->last || interfaceType < it->type) it = map->last; return (T*)it->component; } };
inline bool IS_THING_ALIVE(const CThing* t) { return t && !t->Dead; }
struct CThingFilter_IsInSameVolumeContextAsThing {
    CThing* PThingInside; bool Override;
    bool operator()(CThing* thing) const
    {
        if (Override) return true;
        CThing* volume = 0;
        if (thing->HasTC(0x75)) volume = thing->GetTC<CTCInsideBuilding>(0x75)->PBuilding.GetPItem();
        else if (thing->HasTC(0x33)) volume = thing->GetTC<CTCOwnedEntity>(0x33)->POwner.GetPItem();
        bool thingInVolume = IS_THING_ALIVE(volume);
        bool referenceInVolume = IS_THING_ALIVE(PThingInside);
        if ((!thingInVolume && !referenceInVolume) || (thingInVolume && referenceInVolume)) return true;
        return false;
    } };
static void Init(CThing& t) { for (int i = 0; i < 9; ++i) t.UsingComponents[i] = 0; t.Components.first = t.Components.last = 0; t.Dead = 0; }
int main()
{
    CThing building, owner, deadOwner, t; Init(building); Init(owner); Init(deadOwner); Init(t); deadOwner.Dead = 1;
    CTCOwnedEntity owned; owned.POwner.m_p = &owner;
    CTCInsideBuilding inside; inside.PBuilding.m_p = &building;
    CInterfaceMap::SEntry e[2] = { { 0x33, (CTCBase*)&owned }, { 0x75, (CTCBase*)&inside } };
    t.Components.first = e; t.Components.last = e + 2;
    CThingFilter_IsInSameVolumeContextAsThing f; f.Override = false; f.PThingInside = 0;
    int bad = 0;
    if (!f(&t)) bad = 1;                                   // neither in a volume
    t.UsingComponents[1] = 1UL << 19;                      // owned entity only: thing in, reference not
    if (f(&t)) bad = 1;
    f.PThingInside = &building; if (!f(&t)) bad = 1;       // both in
    owned.POwner.m_p = &deadOwner; if (f(&t)) bad = 1;     // dead owner: thing not in
    t.UsingComponents[3] = 1UL << 21;                      // inside-building wins over owned
    if (!f(&t)) bad = 1;
    f.PThingInside = 0; f.Override = true; if (!f(&t)) bad = 1;
    if (bad) { std::printf("006a8c60_TEST FAIL\n"); return 1; }
    std::printf("006a8c60_TEST PASS\n");
    return 0;
}