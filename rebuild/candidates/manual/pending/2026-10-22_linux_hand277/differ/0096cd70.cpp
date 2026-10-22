// CAIStateGroup_EatAtTable::Update() -- claim a seat, refresh the eating duration, mark the seat occupied, walk to it, sit down, then eat each food item put in front of the seat while it keeps coming; terminate at the end
// Grade: DIFFER(578v578) -- frame packing (retail: flag byte at +0x13, key/table/string/counted pointer share +0x14,
//   frame 0xbc; ours 0xc0), the region def held in eax (retail ecx, same as GetSeat 0x0096CB40), the bit-24 flag test
//   compiled as a byte load (retail shr 0x18), occupant store register order.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096CD70, 592 bytes. ?Update@CAIStateGroup_EatAtTable@@MAEXXZ (Ego_r.pdb).
// Callees: GetSeat 0x0096CB40, Terminate 0x008FCF40, CInterfaceMap lower_bound 0x0040F020 / const 0x004365B0, region
//   accessor 0x007EE540, GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0, GetPItem 0x00A01B50, WalkTo 0x0096C880,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0, CThingCreatureBase::GetAnimFromName 0x00662A00, operator new 0x00BFEA1A,
//   NCreatureMode::CModeSitBase ctor 0x0094B530, CCountedPointer(T*) 0x0094B3A0 / dtor 0x008AB320,
//   CCreatureAction_SitDown 0x008AB9D0 / dtor 0x008ABAA0, AIAction_PerformAction 0x0097D8B0,
//   CTCTavernTable::SetSeatState 0x007B8530 / GetFood 0x007B85C0, Eat 0x0096C9A0, AIAction_DoNothing 0x0097E200.
//   Literal "SIT_AT_TABLE_INTO" @ 0x01293060.
// Layout: EatAtTable fields as GetSeat (Ego_r.pdb); tavern table Occupants _M_start +0x10 (retail operand).
#include <new>
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet {
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
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
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
float __fastcall GFFloatRandom(unsigned long* seed, float range);   // 0x004978A0
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CAnimationEntry;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    bool TestTC(long type) const { return ((m_presentBits[type >> 5] >> (type & 31)) & 1) != 0; }
    template<class T> __forceinline void GetInterfaceR(T** ppOut, long type, bool* has)
    {
        *has = TestTC(type);
        if (*has) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)(it == map->last || type < it->type ? map->last : it)->component;
        }
    }
    template<class T> __forceinline void GetInterfaceR(T** ppOut, long type, bool* has) const
    {
        *has = TestTC(type);
        if (*has) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)(it == map->last || type < it->type ? map->last : it)->component;
        }
    }
};
struct CThingCreatureBase : CThing {
    const CAnimationEntry& GetAnimFromName(const CCharString& name, long flags) const;   // 0x00662A00
};
struct CThingAICreature : CThingCreatureBase { };
struct CThingObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CEatDef {
    virtual void v00();
    virtual void DeleteThis();                  // slot 1
    long RefCount;                              // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DeleteThis(); }
    unsigned char _pad08[0x90 - 0x08];
    long EatingTime;                            // +0x90
};
struct CEatDefPointer {
    CEatDef* m_pDef;
    CEatDefPointer(CEatDef* def) : m_pDef(def) { if (m_pDef) m_pDef->AddRef(); }
    ~CEatDefPointer() { m_pDef->Release(); }
    CEatDef* operator->() const { return m_pDef; }
};
struct CRegionThing {
    unsigned char _pad00[0x94];
    CEatDef* PDef;                              // +0x94
};
struct CTCRegionLink {                          // interface 0x23
    CRegionThing* GetPRegion();                 // 0x007EE540
};
struct CTCTavernTable {                         // interface 0xB8
    unsigned char _pad00[0x10];
    CThingAICreature** Occupants;               // +0x10 vector _M_start
    CThingObject* GetFood(long seat);           // 0x007B85C0
    void SetSeatState(long seat, bool used);    // 0x007B8530
};
namespace NCreatureMode {
struct CModeSitBase {
    unsigned char _body[0x38];
    CModeSitBase(CThingCreatureBase* creature); // 0x0094B530
};
}
struct CCPPointerInfo { long Count; };
template<class T> struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data);                   // 0x0094B3A0
    CCountedPointer(const CCountedPointer& o)
    {
        CCPPointerInfo* info = o.Info;
        Data = o.Data;
        Info = info;
        if (info)
            ++info->Count;
    }
    ~CCountedPointer();                         // 0x008AB320
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa8 - 4];
};
struct CCreatureAction_SitDown : CCreatureActionBase {
    unsigned char _members[0xb0 - 0xa8];
    CCreatureAction_SitDown(CThingCreatureBase& creature, const CAnimationEntry& anim,
                            CCountedPointer<NCreatureMode::CModeSitBase> mode);   // 0x008AB9D0
    ~CCreatureAction_SitDown();                 // 0x008ABAA0
};
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThingAICreature* Creature;                 // +0x18
};
enum EAIActionReturn { AI_ACTION_DONE = 2, AI_ACTION_RUNNING = 4 };
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
    EAIActionReturn AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    EAIActionReturn AIAction_DoNothing();       // 0x0097E200
};
struct CAIStateGroup_EatAtTable : CAIStateGroupBase {
    CBaseIntelligentPointer Table;              // +0x14
    long SeatIndex;                             // +0x1c
    long SeatActionPointIndex;                  // +0x20
    C3DVector SeatPos;                          // +0x24
    CRightHandedSet SeatRHSet;                  // +0x30
    float Duration;                             // +0x48
    bool WalkTo(C3DVector pos, CRightHandedSet set);   // 0x0096C880
    bool Eat(CThing* food);                     // 0x0096C9A0
    bool GetSeat();                             // 0x0096CB40
    void Update();
};

void CAIStateGroup_EatAtTable::Update()
{
    if (!GetSeat()) {
        Terminate();
        return;
    }
    const CThingAICreature* creature = GetCreature();
    CTCRegionLink* link;
    bool has;
    creature->GetInterfaceR(&link, 0x23, &has);
    if (has && link->GetPRegion()) {
        {
            CEatDefPointer def(link->GetPRegion()->PDef);
            Duration = (float)def->EatingTime;
        }
        Duration += GFFloatRandom(GetPWorldSeed(), 45.0f);
    }
    CTCTavernTable* table;
    Table.GetPItem()->GetInterfaceR(&table, 0xb8, &has);
    table->Occupants[SeatIndex] = GetCreature();
    if (!WalkTo(SeatPos, SeatRHSet)) {
        Terminate();
        return;
    }
    const CAnimationEntry& anim = GetCreature()->GetAnimFromName("SIT_AT_TABLE_INTO", 0);
    CCountedPointer<NCreatureMode::CModeSitBase> mode(new NCreatureMode::CModeSitBase(GetCreature()));
    bool failed = AIAction_PerformAction(&CCreatureAction_SitDown(*GetCreature(), anim, mode)) != AI_ACTION_DONE;
    if (!failed) {
        table->SetSeatState(SeatIndex, true);
        for (;;) {
            CThingObject* food = table->GetFood(SeatIndex);
            if (food) {
                if (!Eat((CThing*)food))
                    break;
            } else if (AIAction_DoNothing() != AI_ACTION_RUNNING)
                break;
        }
    }
    Terminate();
}
