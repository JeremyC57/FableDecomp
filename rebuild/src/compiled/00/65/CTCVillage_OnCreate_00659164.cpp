// CTCVillage::OnCreate() -- takes the village def from the thing's def, copies its default stance to the hero, clears the guard respawn and curfew state, schedules the next guard respawn from the hero's renown ratio when the hero is alive, sets the light state from the time of day and resets the crier
// Leaf: ?OnCreate@CTCVillage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00659164, 305 bytes (plain `ret`, `leave`).
// Callees: CTCBase::GetDefinitionManager() const 0x006869D0; CDefinitionManager::GetDef<T>(long, CDefPointer<T>*) 0x0042AF3C;
//          CDefClassBase::GetSubDef<CVillageDef>(CDefPointer<const CVillageDef>*) 0x0065F440 (into PDef);
//          CWorld::GetCurrentPlayer() const 0x0049D9D0; CPlayer::GetCharacterThing() 0x00487DC0;
//          CDefClassBase::GetSubDef<CVillagePeopleDef> 0x00512FE0; IS_THING_ALIVE<const CThing*>(const CThing* const&) 0x0043DBD0
//          (`__fastcall`); std::bitset<>::test(size_t) const 0x00410DE0 (interface 0x14); CVectorMap<ETCInterfaceType,
//          CTCBase*>::LowerBound 0x0040F020; the interface-0x14 component's ratio getter 0x005CDBBE (manifest
//          "ETCInterface_CalculateRatio"); __ftol2 0x00BFEA70; CTCBase::PeekWorld() const 0x00686A80; CGameTimeManager::
//          IsDay() const 0x006BB2C0; CTCVillage::ResetCrier() 0x00658E73; the defs' slot 1 on release.
// Layout (retail CTCVillage: one-pointer lists, so Ego_r.pdb - 0x8c from Initialised on; engine/CTCVillage.h carries PDB
//          offsets, hence a local model): InCurfew +0x84, PDef +0x94, StanceToHero +0x98, NextGuardRespawn +0xa4,
//          LightActiveStateToChange +0xcc, MaxFrameToAssignTask +0xd0; CThing def index (ushort) +0x8c, bitset +0x20,
//          interface map +0x44; CVillageDef::DefaultStanceToHero +0x38; CVillagePeopleDef GuardRespawnMaxRenownBasedOffset
//          +0xc0; g_pMainGameComponent +0x24 the world.
// Shape: frame-pointer size TU; the thing def and the people def are out-locals whose lookups are this TU's / link-time
//        knowledge (stand-ins below), so VC keeps the thing def in ebx (reloaded from its slot once ebx holds the 0x14
//        key) and the people def is read back for the release; the hero stays in edi: IS_THING_ALIVE<const CThing*> takes a converted
//        temporary copy (its frame word then holds the lookup key).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
class CTCBase;

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CVillageDef : public CDefBase { public:
    unsigned char _pad_0x08[0x30];
    long          DefaultStanceToHero;          // +0x38 (EVillageStanceToHero)
};
class CVillagePeopleDef : public CDefBase { public:
    unsigned char _pad_0x08[0xb8];
    long          GuardRespawnMaxRenownBasedOffset;   // +0xc0
};

struct CThingDef : CDefBase {
    bool GetSubDef(CDefPointer<CVillageDef>* out);          // 0x0065F440
    bool GetSubDef(CDefPointer<CVillagePeopleDef>* out);    // 0x00512FE0
};
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CThingDef>* out) const;   // 0x0042AF3C
};
class CGameDefinitionManager : public CDefinitionManager
{
};
CThingDef* FindRef_CDefinitionManager_GetDef_0(long index);   // stand-in callees of the stand-in bodies below
CVillagePeopleDef* FindRef_CThingDef_GetSubDef_0(CThingDef* def);
// Stand-in bodies (the real ones are out of line: 0x0042AF3C elsewhere, 0x00512FE0 in another TU; retail's link-time
// knowledge): they only write the def through the out parameter, so VC7.1 knows the caller's address-taken pointer does
// not escape the call.
#pragma auto_inline(off)
bool CDefinitionManager::GetDef(long index, CDefPointer<CThingDef>* out) const
{
    CThingDef* def = FindRef_CDefinitionManager_GetDef_0(index);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
bool CThingDef::GetSubDef(CDefPointer<CVillagePeopleDef>* out)
{
    CVillagePeopleDef* def = FindRef_CThingDef_GetSubDef_0(this);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    CBitset        m_present;                   // +0x20
    unsigned char  _pad24[0x20];
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x40];
    unsigned short DefIndex;                    // +0x8c
    unsigned char  _pad8e[3];
    unsigned char  Flags91;                     // +0x91 bit 0: dead
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
// Stand-in body for IS_THING_ALIVE<const CThing*> 0x0043DBD0 (`__fastcall`, defined in another TU; retail's link-time
// knowledge that it only reads its argument lets the hero stay in edi across the call).
#pragma auto_inline(off)
__declspec(noinline) bool __fastcall IS_THING_ALIVE(const CThing* const& thing)
{
    if (thing == 0)
        return false;
    return (thing->Flags91 & 1) == 0;
}
#pragma auto_inline(on)

struct CTCRenownRatio {                         // the interface-0x14 component
    float CalculateRatio();                     // 0x005CDBBE
};
class CPlayer { public:
    CThing* GetCharacterThing();                // 0x00487DC0
};
class CWorld { public:
    unsigned char _pad_0x00[0x1c];
    class CGameTimeManager* PGameTimeManager;  // +0x1c
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
};
class CGameTimeManager { public:
    bool IsDay() const;                         // 0x006BB2C0
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

class CTCBase
{
public:
    virtual void v00();
    virtual void OnCreate();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
    const CWorld& PeekWorld() const;   // 0x00686A80
};

class CTCVillage : public CTCBase {
public:
    virtual void OnCreate();
 public:
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x7c];
    bool          InCurfew;                     // +0x84
    unsigned char _pad_0x85[0xf];
    CDefPointer<CVillageDef> PDef;              // +0x94
    long          StanceToHero;                 // +0x98
    long          TimeStopIndex;                // +0x9c
    long          AmountOfFineAlreadyPaid;      // +0xa0
    long          NextGuardRespawn;             // +0xa4
    unsigned char _pad_0xa8[0x24];
    bool          LightActiveStateToChange;     // +0xcc
    long          MaxFrameToAssignTask;         // +0xd0
    void ResetCrier();                          // 0x00658E73
};

void CTCVillage::OnCreate()
{
    CDefPointer<CThingDef> thingDef;
    GetDefinitionManager().GetDef(ParentThing->DefIndex, &thingDef);
    thingDef->GetSubDef(&PDef);
    StanceToHero = PDef->DefaultStanceToHero;
    NextGuardRespawn = 0;
    InCurfew = false;
    CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetCharacterThing();
    CDefPointer<CVillagePeopleDef> peopleDef;
    if (thingDef->GetSubDef(&peopleDef) && IS_THING_ALIVE(hero) && hero->HasTC(0x14))
        NextGuardRespawn = (long)(((CTCRenownRatio*)hero->GetTC(0x14))->CalculateRatio()
                                  * peopleDef->GuardRespawnMaxRenownBasedOffset);
    LightActiveStateToChange = !PeekWorld().PGameTimeManager->IsDay();
    MaxFrameToAssignTask = -1;
    ResetCrier();
}