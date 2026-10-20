// behaviour test for 007b8710 CTCTavernTable::CTCTavernTable: base ctor with the owner, empty containers, cleared flags,
// available for use, and the five prefix lengths taken from temporary strings (each destroyed)
#include <cstdio>
#include <cstring>
class CThing;
static int g_live = 0;
static CThing* g_baseOwner = 0;
struct CCharStringData { char* Text; long Length; };
struct CCharString {
    CCharStringData* m_data;
    CCharStringData  m_storage;
    CCharString(const char* text, long length = -1) { (void)length; ++g_live; m_storage.Text = 0; m_storage.Length = (long)std::strlen(text); m_data = m_storage.Length ? &m_storage : 0; }
    ~CCharString() { --g_live; }
    long GetLength() const { return m_data == 0 ? 0 : m_data->Length; }
};
struct CTCBase {
    virtual void v00() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing* owner) : ParentThing(owner) { g_baseOwner = owner; }
};
template<class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CIntelligentPointerSlot { void* _vfptr; void* m_pItem; };
struct CThingAICreature;
struct CTCTavernTable : CTCBase {
    bool  BeingServed;
    CRetailVector<CThingAICreature*>        Occupants;
    CRetailVector<CIntelligentPointerSlot>  Tankards;
    CRetailVector<CIntelligentPointerSlot>  Food;
    CRetailVector<unsigned long>            SeatsCurrentlyUsed;
    long  NumPeopleSeated;
    bool  SomeoneTalking;
    bool  SomeoneLaughing;
    long  ActionPointSitStrlen;
    long  ActionPointGameMasterStrlen;
    long  ActionPointHeroStrlen;
    long  ActionPointTankardStrlen;
    long  ActionPointMaidStrlen;
    bool  AvailableForUse;
    virtual void v00() {}
    CTCTavernTable(CThing* owner);
};
CTCTavernTable::CTCTavernTable(CThing* owner)
    : CTCBase(owner), BeingServed(false), NumPeopleSeated(0), SomeoneTalking(false), SomeoneLaughing(false),
      AvailableForUse(true)
{
    ActionPointSitStrlen = CCharString("ACTION_POINT_SIT_").GetLength();
    ActionPointGameMasterStrlen = CCharString("ACTION_POINT_GAME_MASTER_").GetLength();
    ActionPointHeroStrlen = CCharString("ACTION_POINT_HERO_").GetLength();
    ActionPointTankardStrlen = CCharString("DUMMY_POINT_TANKARD_").GetLength();
    ActionPointMaidStrlen = CCharString("ACTION_POINT_MAID_").GetLength();
}
int main()
{
    CThing* owner = (CThing*)&g_live;
    CTCTavernTable table(owner);
    bool ok = g_baseOwner == owner && table.ParentThing == owner && !table.BeingServed && table.Occupants._M_start == 0 &&
              table.Tankards._M_finish == 0 && table.Food._M_end_of_storage == 0 && table.SeatsCurrentlyUsed._M_start == 0 &&
              table.NumPeopleSeated == 0 && !table.SomeoneTalking && !table.SomeoneLaughing && table.AvailableForUse &&
              table.ActionPointSitStrlen == 17 && table.ActionPointGameMasterStrlen == 25 && table.ActionPointHeroStrlen == 18 &&
              table.ActionPointTankardStrlen == 20 && table.ActionPointMaidStrlen == 18 && g_live == 0;
    if (!ok) { std::printf("007b8710_TEST FAIL\n"); return 1; }
    std::printf("007b8710_TEST PASS\n");
    return 0;
}
