// Grade: DIFFER(386v386) -- one scheduling difference: retail loads this->ParentThing (mov eax,[ebp-4]; mov ecx,[eax+4]) right after fld 50.0 before the pushes; ours loads it after push eax. Tried named parent locals (branch/top/after nav), nav local, centre ref local, radius local, enable as arg, default args, pointer centre param, same-TU GetPosition (pure and impure), GetNearbyWanderPosition and bitset::test bodies.
// CTCVillage::EnableVillagerDefTypes(bool enable, const CCharString& defName) -- enable or disable every live village dummy standing for villagers of that def: disabling kills its live villager; enabling re-creates a missing villager near the dummy (a wander point within 50 of the village owner when it can navigate, else at the owner)
// Leaf: EnableVillagerDefTypes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065B142, length 384 bytes (`leave`, `ret 8`): frame-pointer size TU,
//   `#pragma optimize("y", off)` + `#pragma optimize("s", on)` as the landed sibling TryToRespawnDefNamed 0x0065B349
//   (PDB `?EnableVillagerDefTypes@CTCVillage@@QAEX_NABVCCharString@@@Z`).
// Callees: CGameDefinitionManager::Get() 0x0044C6B0; CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410;
//   CBaseIntelligentPointer::GetPItem 0x00A01B50; IS_THING_ALIVE(CBaseObject* const&) 0x0040DCE0 and the villager's
//   IS_THING_ALIVE instantiation 0x004A88F0 (`__fastcall`); std::bitset<>::test 0x00410DE0 (interfaces 0xD6 and 7);
//   CVectorMap LowerBound 0x0040F020 (inlined GetTC); the dummy villager's enable switch 0x0072F0E0 (manifest
//   "EnableNavigator"); CThing::Kill(bool) 0x004C9B80; CThing::GetPosition 0x004C73D0; CTCCreatureNavigation::
//   GetNearbyWanderPosition(const C3DVector&, float, C3DVector*, bool) 0x006CB6E0; CTCDummyVillager::CreateVillager
//   (CTCVillage*, const C3DVector&, bool) 0x0072F750. Data: 50.0f @ 0x0124E7F8.
// Layout: CTCBase ParentThing +0x04; retail CTCVillage VillageDummies +0x74 (as the landed siblings); CTCDummyVillager
//   villager def index +0x0c, PVillager +0x30 (as TryToRespawnDefNamed).
// Shape: same-TU bodies of LowerBound, both IS_THING_ALIVE instantiations and a CreateVillager stand-in (reads its
//   position through the reference) as the sibling, so the keys and temporaries pack into the dead argument slots.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CCharString { void* m_data; };

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CBaseObject {
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CThing : CBaseObject {
    const C3DVector& GetPosition() const;       // 0x004C73D0
    void Kill(bool silent);                     // 0x004C9B80
};
struct CThingAICreature : CThing { };
bool ThingAliveBit_s8(const CBaseObject* thing);   // stand-in: the dead-flag test (outside the bytes graded here)
bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing);   // 0x0040DCE0
#pragma auto_inline(off)
__declspec(noinline) bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing)
{
    if (thing == 0)
        return false;
    return ThingAliveBit_s8(thing);
}
#pragma auto_inline(on)
bool __fastcall IS_THING_ALIVE(CThingAICreature* const& thing);   // 0x004A88F0
#pragma auto_inline(off)
__declspec(noinline) bool __fastcall IS_THING_ALIVE(CThingAICreature* const& thing)
{
    if (thing == 0)
        return false;
    return ThingAliveBit_s8(thing);
}
#pragma auto_inline(on)

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);  // 0x009AD410
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();          // 0x0044C6B0
};

template <class T> struct CIntelligentPointer {
    void* _vfptr;
    T*    m_pItem;
    T* GetPItem() const;                        // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};
struct CTCCreatureNavigation {                  // interface 7
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool onMap);   // 0x006CB6E0
};
struct CTCVillage;
struct CTCDummyVillager {                       // interface 0xD6
    unsigned char _pad_0x00[0x0c];
    long          VillagerDefIndex;             // +0x0c
    unsigned char _pad_0x10[0x20];
    CIntelligentPointer<CThingAICreature> PVillager;   // +0x30
    void SetEnabled(bool enable);               // 0x0072F0E0
    CThingAICreature* CreateVillager(CTCVillage* village, const C3DVector& position, bool flag);   // 0x0072F750
};
CThingAICreature* CreateVillagerSink_s8(CTCDummyVillager* dummy, CTCVillage* village, float x, float y, float z, bool flag);
#pragma auto_inline(off)
__declspec(noinline) CThingAICreature* CTCDummyVillager::CreateVillager(CTCVillage* village, const C3DVector& position, bool flag)
{
    return CreateVillagerSink_s8(this, village, position.X, position.Y, position.Z, flag);
}
#pragma auto_inline(on)
struct SThingNode {                             // STLport list node
    SThingNode* _M_next;
    SThingNode* _M_prev;
    CIntelligentPointer<CThing> _M_data;        // +0x08
};

struct CTCVillage {
    void*         __vftable;                    // +0x00
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad_0x08[0x6c];
    SThingNode*   VillageDummies;               // +0x74 list<CIntelligentPointer<CThing> > head node
    void EnableVillagerDefTypes(bool enable, const CCharString& defName);
};

void CTCVillage::EnableVillagerDefTypes(bool enable, const CCharString& defName)
{
    long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(defName);
    for (SThingNode* it = VillageDummies->_M_next; it != VillageDummies; it = it->_M_next) {
        CThing* dummy = it->_M_data.GetPItem();
        if (IS_THING_ALIVE(dummy) && dummy->HasTC(0xd6)) {
            CTCDummyVillager* villager = (CTCDummyVillager*)dummy->GetTC(0xd6);
            if (villager->VillagerDefIndex == defIndex) {
                villager->SetEnabled(enable);
                if (!enable) {
                    if (IS_THING_ALIVE(villager->PVillager.GetPItem()))
                        villager->PVillager.GetPItem()->Kill(false);
                } else if (!IS_THING_ALIVE(villager->PVillager.GetPItem())) {
                    C3DVector position;
                    if (dummy->HasTC(7))
                        ((CTCCreatureNavigation*)dummy->GetTC(7))->GetNearbyWanderPosition(ParentThing->GetPosition(), 50.0f, &position, true);
                    else
                        position = ParentThing->GetPosition();
                    villager->CreateVillager(this, position, false);
                }
            }
        }
    }
}
