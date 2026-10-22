// Grade: DIFFER(921v1006) -- no `and esp,-8` aligned frame in ours (some 8-aligned local in retail: likely an action member), the inline Random is not inlined (needs __forceinline), the building pointer's vtable constant is held in ebx in retail and reused for the reset temporary, and the loop/found-block placement differs
// CAIStateGroup_SitForMeal::GetFoodFromCookingPot() -- walk up to the cooking pot (2..7 units, while it is the village's cook time) until one of its action points is free, step onto it, look along it, take a bowl out of the pocket and play the fill animation; true when the bowl was filled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009456A0, length 1008 bytes (+ int3 padding; `and esp,-8` frame 0xf8).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThingBuilding> vtable 0x01292CFC, CIntelligentPointer<const CThing> vtable 0x01237F1C),
// CThingBuilding::IsPointInside @ 0x0082D000, CThing::GetPosition @ 0x004C73D0, GetPWorldSeed @ 0x008FCFD0, GFROR13
// @ 0x00497890, CVectorMap::LowerBound @ 0x0040F020 (0x2d action points) / const @ 0x004365B0 (0x23 village member),
// CTCUserActionPointBase::GetFreeActionPoint @ 0x007FEDE0, AIAction_MoveToThingProximity @ 0x0097E380, the village
// member's village getter @ 0x007EE540 (manifest "IS_THING_ALIVE<..>" is propagated), CTCVillage cook-time test
// @ 0x00658BB9, AIAction_MoveToPos @ 0x0097EC00, AIAction_LookAtPos @ 0x0097D580, GetPDefinitionManager @ 0x008FCFB0,
// CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410 (global name string @ 0x013BB72C),
// CCreatureAction_GetItemOutOfPocket ctor @ 0x00945410, CCreatureAction_PlayAnimation(name) ctor @ 0x008424F0, the action
// base dtor @ 0x00693EF0, AIAction_PerformActionAndWait @ 0x0097DB70, CCharString @ 0x0099EBF0 / 0x0099EAE0 ("ST_FILL_ITEM").
// Layout: CAIStateGroup_SitForMeal as the landed ReleaseControlOfPot 0x00945530 (PPot +0x34, ActionPointIndex +0x3c,
// HoldingBowl +0x40); the creature's building pointer at +0x168.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
private:
    void* PStringData;
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceWord(T** ppOut, long interfaceType)
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
    template<class T> __forceinline bool GetInterfaceWordC(T** ppOut, long interfaceType) const
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
};
struct CThingBuilding : CThing {
    bool IsPointInside(const C3DVector& pos);   // 0x0082D000
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {          // CIntelligentPointer<CThing>
    virtual void v00();
};
struct CIntelligentPointerBuilding : CBaseIntelligentPointer {       // CIntelligentPointer<CThingBuilding>, vtable 0x01292CFC
    virtual void v00();
    CIntelligentPointerBuilding(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerBuilding& operator=(const CIntelligentPointerBuilding& other) { SetPItem(other.GetPItem()); return *this; }
    CThingBuilding* GetPBuilding() const { return (CThingBuilding*)GetPItem(); }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CThingAICreature : CThing {
    unsigned char _pad94[0x168 - 0x94];
    CIntelligentPointerThing PBuilding;         // +0x168
};
struct CActionPointFrame {
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
struct CTCUserActionPointBase {                 // interface 0x2d
    bool GetFreeActionPoint(CThing* user, C3DVector* pos, CActionPointFrame* frame, long* index);   // 0x007FEDE0
};
struct CTCVillage {
    bool IsCookTime();                          // 0x00658BB9
};
struct CTCVillageMember {                       // interface 0x23
    CTCVillage* GetPVillage();                  // 0x007EE540
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
extern CCharString g_SitForMealBowlDefName;     // 0x013BB72C
struct CBlendTimes {
    long In, Out;
    CBlendTimes() : In(0), Out(0) { }
};
class CCreatureAction_GetItemOutOfPocket {
public:
    CCreatureAction_GetItemOutOfPocket(CThing* creature, long defIndex, long slot);   // 0x00945410
    ~CCreatureAction_GetItemOutOfPocket();      // 0x00693EF0 (the action base dtor)
private:
    unsigned char _body[0xac];
};
class CCreatureAction_PlayAnimation {
public:
    CCreatureAction_PlayAnimation(CThing* creature, const CCharString& name, const CBlendTimes& blend,
                                  long a, bool b, long c, long d, long e, long f);   // 0x008424F0
    ~CCreatureAction_PlayAnimation();           // 0x00693EF0 (the action base dtor)
private:
    unsigned char _body[0xb4];
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
    CDefinitionManager* GetPDefinitionManager();   // 0x008FCFB0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float proximity,
        bool* arrived, long movement, bool a, bool b, bool c, bool d, float e);    // 0x0097E380
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* arrived, long movement, bool a, bool b, bool c, bool d,
                            bool e);            // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long frames, float speed);       // 0x0097D580
    long AIAction_PerformActionAndWait(CCreatureAction_GetItemOutOfPocket* action);      // 0x0097DB70
    long AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation* action);           // 0x0097DB70
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};
struct CAIStateGroup_SitForMeal : CAIStateGroupBase {
    unsigned char _base2[0x20];                 // +0x14
    CIntelligentPointerThing PPot;              // +0x34
    long ActionPointIndex;                      // +0x3c
    bool HoldingBowl;                           // +0x40
    long DayLastAte;                            // +0x44
    bool GetFoodFromCookingPot();
};

bool CAIStateGroup_SitForMeal::GetFoodFromCookingPot()
{
    CIntelligentPointerBuilding building(GetCreature()->PBuilding.GetPItem());
    if (PPot.GetPItem() && !(PPot.GetPItem()->m_flags91 & 1) && building.GetPItem() && !(building.GetPItem()->m_flags91 & 1)) {
        CThingBuilding* house = building.GetPBuilding();
        if (!house->IsPointInside(PPot.GetPItem()->GetPosition()))
            building = CIntelligentPointerBuilding(0);
        ActionPointIndex = -1;
        PPot.GetPItem()->GetPosition();
        float distance = Random(100) * 0.05f + 2.0f;
        for (;;) {
            if (!PPot.GetPItem() || (PPot.GetPItem()->m_flags91 & 1))
                break;
            CTCUserActionPointBase* actionPoints;
            if (!PPot.GetPItem()->GetInterfaceWord(&actionPoints, 0x2d))
                break;
            C3DVector pos;
            CActionPointFrame frame;
            if (actionPoints->GetFreeActionPoint(GetCreature(), &pos, &frame, &ActionPointIndex)) {
                C3DVector target = pos + frame.Forward;
                bool reached;
                if (AIAction_MoveToPos(1, target, &reached, 3, true, true, false, true, false) != 2)
                    break;
                if (AIAction_LookAtPos(1, target, 0x11, 0.0027777778f) != 2)
                    break;
                if (AIAction_PerformActionAndWait(&CCreatureAction_GetItemOutOfPocket(GetCreature(),
                        GetPDefinitionManager()->GetDefGlobalIndexFromName(g_SitForMealBowlDefName), 0)) != 2)
                    break;
                HoldingBowl = true;
                CCharString fillAnim("ST_FILL_ITEM");
                long filled = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), fillAnim,
                                                                CBlendTimes(), 0, true, 0, 0, 0, 0));
                if (filled == 2)
                    return true;
                break;
            }
            bool arrived;
            long result = AIAction_MoveToThingProximity(2, PPot, distance, &arrived, 3, true, true, true, true, 0.0f);
            if (result != 2 && result != 4)
                break;
            CTCVillageMember* member;
            if (!GetCreature()->GetInterfaceWordC(&member, 0x23))
                break;
            if (!member->GetPVillage() || !member->GetPVillage()->IsCookTime())
                break;
        }
    }
    return false;
}
