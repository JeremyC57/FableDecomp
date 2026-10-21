// CTCVillage::EnableGuards(bool enable) -- enable or disable every live guard dummy (disabling kills its live guard, enabling re-creates a missing one at the village owner), then clear the village's crimes and have every live villager decriminalise all known crimes
// Leaf: EnableGuards
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065C45A, length 351 bytes (`leave`, `ret 4`): frame-pointer size TU
//   (PDB `?EnableGuards@CTCVillage@@QAEX_N@Z`), as the landed CTCVillage siblings.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; IS_THING_ALIVE(CBaseObject* const&) 0x0040DCE0, the villager's
//   IS_THING_ALIVE 0x004A88F0 and IS_THING_ALIVE(const CIntelligentPointer<CThingAICreature>&) 0x004AB8F0 (all
//   `__fastcall`); std::bitset<>::test 0x00410DE0 (interfaces 0xD6 and 0x4F); CVectorMap LowerBound 0x0040F020 (inlined
//   GetTC); the dummy villager's enable switch 0x0072F0E0 (manifest "EnableNavigator"); CThing::Kill(bool) 0x004C9B80;
//   CThing::GetPosition 0x004C73D0; CTCDummyVillager::CreateVillager 0x0072F750; the village crime reset 0x0065C2D0
//   (landed spellings ProcessCrimeDeeds / OnAllFinesPaid; most likely the PDB's `?ClearCrimes@CTCVillage@@QAEXXZ`);
//   CTCCreatureOpinionOfHero::DecriminaliseAllKnownCrimes 0x00684050 (interface 0x4F).
// Layout: CTCBase ParentThing +0x04; retail CTCVillage VillagePeople +0x2c, VillageDummies +0x74 (4-byte STLport lists,
//   landed siblings); CTCDummyVillager Creature +0x30, Guard +0x49 (PDB +0x30 / +0x49).
// Shape: the EnableVillagerDefTypes 0x0065B142 model (same-TU LowerBound and IS_THING_ALIVE bodies) with the guard flag
//   filter and the owner's position passed straight to CreateVillager; the people walk keys GetTC through the dead
//   `enable` slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
struct C3DVector { float X, Y, Z; };

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


template <class T> struct CIntelligentPointer {
    void* _vfptr;
    T*    m_pItem;
    T* GetPItem() const;                        // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};
struct CTCVillage;
struct CTCDummyVillager {                       // interface 0xD6
    unsigned char _pad_0x00[0x30];
    CIntelligentPointer<CThingAICreature> PVillager;   // +0x30 Creature
    unsigned char _pad_0x38[0x11];
    bool          Guard;                        // +0x49
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
struct SPersonNode {
    SPersonNode* _M_next;
    SPersonNode* _M_prev;
    CIntelligentPointer<CThingAICreature> _M_data;   // +0x08
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThingAICreature>& thing);   // 0x004AB8F0
struct CTCCreatureOpinionOfHero {              // interface 0x4F
    void DecriminaliseAllKnownCrimes();         // 0x00684050
};

struct CTCVillage {
    void*         __vftable;                    // +0x00
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad_0x08[0x24];
    SPersonNode*  VillagePeople;                // +0x2c list<CIntelligentPointer<CThingAICreature> > head node
    unsigned char _pad_0x30[0x44];
    SThingNode*   VillageDummies;               // +0x74 list<CIntelligentPointer<CThing> > head node
    void ClearCrimes();                         // 0x0065C2D0
    void EnableGuards(bool enable);
};

void CTCVillage::EnableGuards(bool enable)
{
    for (SThingNode* it = VillageDummies->_M_next; it != VillageDummies; it = it->_M_next) {
        CThing* dummy = it->_M_data.GetPItem();
        if (IS_THING_ALIVE(dummy) && dummy->HasTC(0xd6)) {
            CTCDummyVillager* villager = (CTCDummyVillager*)dummy->GetTC(0xd6);
            if (villager->Guard) {
                villager->SetEnabled(enable);
                if (!enable) {
                    if (IS_THING_ALIVE(villager->PVillager.GetPItem()))
                        villager->PVillager.GetPItem()->Kill(false);
                } else if (!IS_THING_ALIVE(villager->PVillager.GetPItem()))
                    villager->CreateVillager(this, ParentThing->GetPosition(), false);
            }
        }
    }
    ClearCrimes();
    for (SPersonNode* node = VillagePeople->_M_next; node != VillagePeople; node = node->_M_next) {
        if (IS_THING_ALIVE(node->_M_data)) {
            CThingAICreature* person = node->_M_data.GetPItem();
            if (person->HasTC(0x4f))
                ((CTCCreatureOpinionOfHero*)person->GetTC(0x4f))->DecriminaliseAllKnownCrimes();
        }
    }
}