// behaviour test for CThingFilter_IsMySleepPoint::operator() (0x006A8930): occupiable type 2, unowned or owned by the sleeper
#include <cstdio>
struct CTCBase;
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; } };
class CThing;
enum EOccupyType { OCCUPY_TYPE_SLEEP_2 = 2 };
struct CTCSinglePersonOccupiable { long m_type; bool IsOfType(EOccupyType t) { return m_type == t; } };
struct CBaseIntelligentPointer { CThing* m_p; CThing* GetPItem() const { return m_p; } };
struct CTCOwnedEntity { CBaseIntelligentPointer POwner; };
class CThing { public:
    unsigned long UsingComponents[9]; CInterfaceMap Components;
    bool HasTC(long type) const { if (UsingComponents[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template<class T> T* GetTC(long interfaceType) const
    { long type = interfaceType; const CInterfaceMap* map = &Components; CInterfaceMap::SEntry* it = map->LowerBound(&type);
      if (it == map->last || interfaceType < it->type) it = map->last; return (T*)it->component; } };
struct CThingFilter_IsMySleepPoint {
    CThing* Sleeper;
    bool operator()(CThing* thing) const
    {
        if (thing->HasTC(0xac)) {
            if (thing->GetTC<CTCSinglePersonOccupiable>(0xac)->IsOfType(OCCUPY_TYPE_SLEEP_2)) {
                if (thing->HasTC(0x33)) { CThing* owner = thing->GetTC<CTCOwnedEntity>(0x33)->POwner.GetPItem(); return owner == Sleeper; }
                return true;
            }
        }
        return false;
    } };
int main()
{
    CThing me, other, bed; for (int i = 0; i < 9; ++i) bed.UsingComponents[i] = 0;
    CTCOwnedEntity owned; owned.POwner.m_p = &other;
    CTCSinglePersonOccupiable occ; occ.m_type = 2;
    CInterfaceMap::SEntry e[2] = { { 0x33, (CTCBase*)&owned }, { 0xac, (CTCBase*)&occ } };
    bed.Components.first = e; bed.Components.last = e + 2;
    CThingFilter_IsMySleepPoint f; f.Sleeper = &me;
    int bad = 0;
    if (f(&bed)) bad = 1;                                  // not occupiable
    bed.UsingComponents[5] = 1UL << 12;
    if (!f(&bed)) bad = 1;                                 // occupiable, unowned
    occ.m_type = 1; if (f(&bed)) bad = 1; occ.m_type = 2;  // wrong occupy type
    bed.UsingComponents[1] = 1UL << 19;
    if (f(&bed)) bad = 1;                                  // owned by someone else
    owned.POwner.m_p = &me; if (!f(&bed)) bad = 1;         // owned by the sleeper
    if (bad) { std::printf("006a8930_TEST FAIL\n"); return 1; }
    std::printf("006a8930_TEST PASS\n");
    return 0;
}
