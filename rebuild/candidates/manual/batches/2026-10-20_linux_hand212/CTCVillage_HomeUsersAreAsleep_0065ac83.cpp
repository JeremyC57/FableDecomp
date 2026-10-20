// CTCVillage::HomeUsersAreAsleep(CThingBuilding* home) -- true when the building has at least one living villager calling it home and every such villager is in creature mode 13 (asleep)
// Leaf: HomeUsersAreAsleep
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065AC83, 180 bytes (`ret 4`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`. Ego_r.pdb `?HomeUsersAreAsleep@CTCVillage@@AAE_NPAVCThingBuilding@@@Z`.
// Callees: IS_THING_ALIVE 0x004AB8F0 (`__fastcall`, the landed spelling; Ego_r.pdb instantiates it as
//          IS_THING_ALIVE<CIntelligentPointer<const CThingPhysical> >); CBaseIntelligentPointer::GetPItem() const 0x00A01B50;
//          std::bitset<>::test(size_t) const 0x00410DE0 (interface 0x31, the creature mode manager); CVectorMap<
//          ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020; CTCCreatureModeManager::GetCurrentMode() const 0x0073A6E0
//          (Ego_r.pdb returns CTCCreatureModeManager::CModeBase&); the mode's GetModeType (vtable slot 1).
// Layout: retail CTCVillage VillagePeople (list<CIntelligentPointer<CThingAICreature> >: the head node pointer only) +0x2c
//         (PDB +0x34); list node: next +0, prev +4, the intelligent pointer +8; retail CThingAICreature HomeBuilding
//         +0x168 (PDB +0x190).
// Shape: the home building is read through an inline accessor (`lea ecx,[eax+168h]`; the direct member form adds the
//        offset with `mov ecx,eax ; add ecx,168h`); the result is a named bool in a frame byte set when a home user is found; the walk re-reads end() through the
//        spilled `this`; the mode manager comes from the unchecked out-parameter GetInterface (the absent arm reads the
//        lookup key's frame word); an awake home user returns false straight away (`xor al,al` after the epilogue).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
struct CBaseObject;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
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
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
};
struct CThingBuilding : CThing { };

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    void* __vftable;
    CBaseObject* PData;
    CThing* GetPItem() const;                   // 0x00A01B50
};
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer& thing);   // 0x004AB8F0

struct CThingAICreature : CThing {
    unsigned char _pad_0x4c[0x168 - 0x4c];
    CBaseIntelligentPointer HomeBuilding;       // +0x168 CIntelligentPointer<CThingBuilding>
    CThingBuilding* GetHomeBuilding() const { return (CThingBuilding*)HomeBuilding.GetPItem(); }
};

struct CTCCreatureModeManager {
    struct CModeBase {
        virtual void v00();
        virtual long GetModeType() const;       // slot 1
    };
    CModeBase& GetCurrentMode() const;          // 0x0073A6E0
};

struct SPeopleNode {                            // STLport list node of CIntelligentPointer<CThingAICreature>
    SPeopleNode*            _M_next;
    SPeopleNode*            _M_prev;
    CBaseIntelligentPointer _M_data;            // +0x08
};
struct CPeopleList {                            // retail STLport list: the head node pointer
    SPeopleNode* _M_node;
    SPeopleNode* begin() const { return _M_node->_M_next; }
    SPeopleNode* end() const { return _M_node; }
};

struct CTCVillage {
    unsigned char _pad_0x00[0x2c];
    CPeopleList   VillagePeople;                // +0x2c
    bool HomeUsersAreAsleep(CThingBuilding* home);
};

bool CTCVillage::HomeUsersAreAsleep(CThingBuilding* home)
{
    bool usersFound = false;
    for (SPeopleNode* it = VillagePeople.begin(); it != VillagePeople.end(); it = it->_M_next) {
        CBaseIntelligentPointer& person = it->_M_data;
        if (IS_THING_ALIVE(person)) {
            if (((CThingAICreature*)person.GetPItem())->GetHomeBuilding() == home) {
                usersFound = true;
                CTCCreatureModeManager* modes;
                person.GetPItem()->GetInterface(&modes, 0x31);
                if (modes->GetCurrentMode().GetModeType() != 0xd)
                    return false;
            }
        }
    }
    return usersFound;
}
