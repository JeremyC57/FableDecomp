// CTCVillage::SetVillageLimbo(bool limbo) -- records the village limbo state; entering limbo also puts every living villager that is not a scripted quest thing into limbo (kept on level unload) and hands every living building's door locking to the limbo flag, then refreshes the mirrored crime data
// Leaf: SetVillageLimbo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065C36E, 236 bytes (`ret 4`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (`or byte ptr [eax+92h],40h`). Ego_r.pdb
// `?SetVillageLimbo@CTCVillage@@QAEX_N@Z`.
// Callees: list<CIntelligentPointer<CThingAICreature> > copy constructor 0x00519420 and destructor 0x00512890 (a local copy
//          of VillagePeople); IS_THING_ALIVE 0x004AB8F0 (people) and the CIntelligentPointer<CThingBuilding> instantiation
//          0x0065F8CA (`__fastcall`); CBaseIntelligentPointer::GetPItem() const 0x00A01B50; GFHasScriptThing(CThing*)
//          0x004C7E70 (`__fastcall`, landed spelling; Ego_r.pdb CThing::HasScriptThing() const); std::bitset<>::test(size_t)
//          const 0x00410DE0 (interface 0x1F); CThing::SetInLimbo(bool) 0x004C8CF0; CThingBuilding::LockAllDoors(bool,
//          bool) 0x0082DB00; the village's crime refresh 0x0065C2D0 (landed spellings ProcessCrimeDeeds / OnAllFinesPaid;
//          Ego_r.pdb's source order suggests UpdateMirroredCrimeData).
// Layout: retail CTCVillage VillageBuildings +0x28, VillagePeople +0x2c (bare list head pointers), Limbo +0xb2, LaggyLimbo
//         +0xb3; CThing flag byte +0x92 bit 6 PersistOnUnloadWhenInLimbo (PDB +0xa2); retail CThingBuilding
//         OverrideAutomaticLocking +0xd4 bit 0 (PDB +0xec).
// Shape: the people walk runs over a local copy of the list (re-reading its head each pass) and the building walk over
//        the member (re-reading it through `this`); the scripted-thing test is `!GFHasScriptThing(t) || !t->HasTC(0x1F)`;
//        the persist flag goes through an inline setter (`or byte ptr [eax+92h],40h`; the direct bit-field store forms
//        the address first with `add eax,92h`), the locking flag is the `xor ; and 1 ; xor` merge of the bool argument.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CBaseObject;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
class CThing { public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad21[0x71];
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;   // +0x92 bit 0
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;      // bit 6
    unsigned char PausedDuringCutscenes : 1;
    bool HasTC(long type) const { return m_present.test(type); }
    void SetInLimbo(bool inLimbo);              // 0x004C8CF0
    void SetPersistOnUnloadWhenInLimbo(bool persist) { PersistOnUnloadWhenInLimbo = persist; }
};
bool __fastcall GFHasScriptThing(CThing* thing);   // 0x004C7E70

class CThingBuilding : public CThing { public:
    unsigned char _pad_0x93[0xd4 - 0x93];
    bool OverrideAutomaticLocking : 1;          // +0xd4 bit 0
    void LockAllDoors(bool lock, bool silent);  // 0x0082DB00
};

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    void* __vftable;
    CBaseObject* PData;
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerBuilding : CBaseIntelligentPointer { };   // CIntelligentPointer<CThingBuilding>
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer& thing);   // 0x004AB8F0
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);     // 0x0065F8CA (T = CIntelligentPointer<CThingBuilding>)

template <class T> struct SListNode {           // STLport list node
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;                         // +0x08
};
struct CPeopleList {                            // list<CIntelligentPointer<CThingAICreature> >: the head node pointer
    SListNode<CBaseIntelligentPointer>* _M_node;
    CPeopleList() { }
    CPeopleList(const CPeopleList& other);      // 0x00519420
    ~CPeopleList();                             // 0x00512890
};
struct CBuildingList {                          // list<CIntelligentPointer<CThingBuilding> >
    SListNode<CIntelligentPointerBuilding>* _M_node;
};

class CTCVillage { public:
    unsigned char _pad_0x00[0x28];
    CBuildingList VillageBuildings;             // +0x28
    CPeopleList   VillagePeople;                // +0x2c
    unsigned char _pad_0x30[0x82];
    bool          Limbo;                        // +0xb2
    bool          LaggyLimbo;                   // +0xb3
    void ProcessCrimeDeeds();                   // 0x0065C2D0
    void SetVillageLimbo(bool limbo);
};

void CTCVillage::SetVillageLimbo(bool limbo)
{
    Limbo = limbo;
    if (limbo) {
        LaggyLimbo = true;
        CPeopleList people(VillagePeople);
        for (SListNode<CBaseIntelligentPointer>* it = people._M_node->_M_next; it != people._M_node; it = it->_M_next) {
            CBaseIntelligentPointer& person = it->_M_data;
            if (IS_THING_ALIVE(person)) {
                if (!GFHasScriptThing(((CThing*)person.GetPItem())) || !((CThing*)person.GetPItem())->HasTC(0x1f)) {
                    ((CThing*)person.GetPItem())->SetInLimbo(true);
                    ((CThing*)person.GetPItem())->SetPersistOnUnloadWhenInLimbo(true);
                }
            }
        }
        for (SListNode<CIntelligentPointerBuilding>* it = VillageBuildings._M_node->_M_next; it != VillageBuildings._M_node; it = it->_M_next) {
            CIntelligentPointerBuilding& building = it->_M_data;
            if (IS_THING_ALIVE(building)) {
                ((CThingBuilding*)building.GetPItem())->OverrideAutomaticLocking = limbo;
                ((CThingBuilding*)building.GetPItem())->LockAllDoors(true, false);
            }
        }
        ProcessCrimeDeeds();
    }
}