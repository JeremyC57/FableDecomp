// CAIStateGroup_CreatureTakeCover::Update() -- take cover from a live target: make the creature directly aware of it, find a hidden cover position (none: mark the failure and terminate); a distant cover is reached by crouching (with a live cover thing) and moving in (failure drops the cover thing); behind low cover duck and attack 1..3 times until the position drifts, behind high cover 1..3 times step out to a firing position (unless the target is visible), attack, move back in and reload; finally drop the cover thing from the list and refresh an empty list
// Grade: RELOCATION_MATCH -- three separate IsStillRunnable/AttackTargetThing exits, the firing-position pointer built by implicit conversion (lea-readdressed temporary), the reload name passed as const char*, the cover list size as an unsigned 8-byte-entry count, and the crouch action at its retail 0x124 size
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097A630, 1195 bytes, plain ret, 8-aligned ebp frame.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, interface LowerBound 0x0040F020 (type 0x34 perception),
//   CTCPerceiveThing::GetPerceivedThing 0x007446A0 / SetAsDirectlyAwareOfPerceivedThing 0x007449B0, the cover-base members
//   GetHiddenCoverPosAndSetUpCoverThing 0x00970CD0, SetFailedToFindCover 0x00970590, IsLowCover 0x00970760, MoveIntoCover
//   0x009711D0, RemoveThingFromCoverList 0x009708A0, ClearCoverThing 0x00970B60, DuckBehindCover 0x009713E0,
//   GetHiddenCoverPos 0x00970930, CanSeeTarget 0x00970790, RefreshCoverList 0x009710D0, IsStillRunnable 0x00979E90,
//   AttackTargetThing 0x00979F70, MoveFromCoverToFiringPosition 0x0097A1B0, CThing::GetPosition 0x004C73D0,
//   CCreatureAction_ProjectileCrouch 0x008B8310 / ~CCombatActionBase 0x0062E900, AIAction_PerformActionAndWait 0x0097DB70,
//   GetPWorld 0x008FCFC0 / CWorld::GetPRandomSeed 0x0049D810, GFROR13 0x00497890, IS_THING_ALIVE 0x006961F0 (__fastcall),
//   CIntelligentPointer<const CThing>(CThing*) 0x0056C860 / (const CIntelligentPointer<const CThing>&) 0x0091F990 / dtor
//   0x00A01C10, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CCreatureAction_PlayAnimation 0x008424F0 / ~ 0x00693EF0,
//   AIAction_PerformActionTargetFaceAndWait 0x0097DCB0, Terminate 0x008FCF40. Constant 1.8f @ 0x01298868.
// Layout: Ego_r.pdb CAIStateGroup_CreatureUseCoverBase (byte-packed): PTargetThing +0x14, PCoverThing +0x1f, cover list
//   begin/end +0x28/+0x2c, failure count +0x48 (engine/CAIStateGroup_CreatureTakeCover.h flattens the base; local model).
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct CTCBase;
struct C3DVector { float X, Y, Z; };
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
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(CThing* item);                         // 0x0056C860
    CIntelligentPointerConstThing(const CIntelligentPointerConstThing& other);   // 0x0091F990
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {        // CIntelligentPointer<CThing>
    virtual void v00();
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerConstThing* pointer);   // 0x006961F0
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
__forceinline bool IS_THING_ALIVE(CThing* thing)
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCPerceiveThing {
    CThing* GetPerceivedThing();                // 0x007446A0
    void SetAsDirectlyAwareOfPerceivedThing();  // 0x007449B0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCreatureActionBase {
    void* __vftable;
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char _combat[0x114 - 0xa8];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_ProjectileCrouch : CCombatActionBase {   // retail sizeof 0x124 (members up to +0x11d)
    unsigned char _crouch[0x124 - 0x114];
    CCreatureAction_ProjectileCrouch(CThing* creature, CThing* enemy, const C3DVector& coverPos);   // 0x008B8310
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThing* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);   // 0x008424F0
};
struct CWorld {
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
extern float g_CoverReachedDistance;            // 0x01298868 (1.8f)

struct SCoverEntry { CThing* thing; long data; };
#pragma pack(push, 1)
struct CAIStateGroup_CreatureTakeCover {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08
    CIntelligentPointerConstThing PTargetThing; // +0x14
    unsigned char _pad1c[3];                    // +0x1c
    CIntelligentPointerThing PCoverThing;       // +0x1f
    unsigned char _pad27;                       // +0x27
    SCoverEntry* CoverListStart;                // +0x28 std::vector of 8-byte entries
    SCoverEntry* CoverListFinish;               // +0x2c
    unsigned long CoverListSize() const { return (unsigned long)(CoverListFinish - CoverListStart); }
    unsigned char _pad30[0x48 - 0x30];
    long FailedCount;                           // +0x48
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_PerformActionTargetFaceAndWait(CCreatureActionBase* action, CIntelligentPointerConstThing target, bool wait);   // 0x0097DCB0
    bool GetHiddenCoverPosAndSetUpCoverThing(C3DVector& result);   // 0x00970CD0
    void SetFailedToFindCover();                // 0x00970590
    bool IsLowCover(CThing* cover);             // 0x00970760
    bool MoveIntoCover(C3DVector& pos);         // 0x009711D0
    void RemoveThingFromCoverList(CThing* thing);   // 0x009708A0
    void ClearCoverThing();                     // 0x00970B60
    bool DuckBehindCover(bool dive);            // 0x009713E0
    bool GetHiddenCoverPos(C3DVector& result, CThing* cover);   // 0x00970930
    bool CanSeeTarget();                        // 0x00970790
    void RefreshCoverList();                    // 0x009710D0
    bool IsStillRunnable();                     // 0x00979E90
    bool AttackTargetThing();                   // 0x00979F70
    bool MoveFromCoverToFiringPosition(C3DVector& pos, const CIntelligentPointerConstThing& cover);   // 0x0097A1B0
    __forceinline unsigned long Random()
    {
        unsigned long* seed = GetPWorld()->GetPRandomSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        return *seed = GFROR13(*seed);
    }
    __forceinline float FlatDistanceTo(const C3DVector& pos)
    {
        const C3DVector& here = GetCreature()->GetPosition();
        float dx = pos.X - here.X;
        float dy = pos.Y - here.Y;
        return (float)sqrt(dx * dx + dy * dy);
    }
    void Update();
};
#pragma pack(pop)

void CAIStateGroup_CreatureTakeCover::Update()
{
    if (!IS_THING_ALIVE(PTargetThing)) {
        Terminate();
        return;
    }
    CThing* me = GetCreature();
    if (me->HasTC(0x34)) {
        CTCPerceiveThing* perceive = (CTCPerceiveThing*)me->GetTC(0x34);
        CThing* target = PTargetThing.GetPItem();
        if (perceive->GetPerceivedThing() == target)
            perceive->SetAsDirectlyAwareOfPerceivedThing();
    }
    CThing* cover = 0;
    if (IS_THING_ALIVE(PCoverThing.GetPItem()))
        cover = PCoverThing.GetPItem();
    C3DVector coverPos;
    if (!GetHiddenCoverPosAndSetUpCoverThing(coverPos)) {
        if (FailedCount <= 0)
            SetFailedToFindCover();
        Terminate();
        return;
    }
    bool far = FlatDistanceTo(coverPos) > g_CoverReachedDistance;
    bool lowCover = IsLowCover(PCoverThing.GetPItem());
    if (far) {
        if (IS_THING_ALIVE(cover)) {
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            AIAction_PerformActionAndWait(&CCreatureAction_ProjectileCrouch(GetCreature(), PTargetThing.GetPItem(), coverPos), false);
        }
        if (!MoveIntoCover(coverPos)) {
            RemoveThingFromCoverList(PCoverThing.GetPItem());
            ClearCoverThing();
            goto refresh;
        }
    }
    if (lowCover) {
        unsigned long attacks = Random() % 3 + 1;
        bool drifted = false;
        for (unsigned long i = 0; i < attacks && !drifted; ++i) {
            DuckBehindCover(true);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            if (!AttackTargetThing()) {
                Terminate();
                return;
            }
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            GetHiddenCoverPos(coverPos, PCoverThing.GetPItem());
            if (FlatDistanceTo(coverPos) > g_CoverReachedDistance)
                drifted = true;
        }
        DuckBehindCover(true);
    } else {
        unsigned long attacks = Random() % 3 + 1;
        if (!IS_THING_ALIVE(&PTargetThing) || !IS_THING_ALIVE(PCoverThing.GetPItem())) {
            Terminate();
            return;
        }
        for (unsigned long i = 0; i < attacks; ++i) {
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            GetHiddenCoverPos(coverPos, PCoverThing.GetPItem());
            bool canFire = CanSeeTarget()
                || MoveFromCoverToFiringPosition(coverPos, PCoverThing.GetPItem());
            if (canFire) {
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                if (!AttackTargetThing()) {
                    Terminate();
                    return;
                }
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                GetHiddenCoverPos(coverPos, PCoverThing.GetPItem());
                bool inCover = MoveIntoCover(coverPos);
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                if (inCover) {
                    bool failed = AIAction_PerformActionTargetFaceAndWait(&CCreatureAction_PlayAnimation(GetCreature(),
                        "PROJECTILE_WEAPON_LOAD", CSoundPair(), false, true, false, 0, 0), PTargetThing, false) != 2;
                    if (failed) {
                        Terminate();
                        return;
                    }
                }
            }
        }
    }
    RemoveThingFromCoverList(PCoverThing.GetPItem());
refresh:
    if (FailedCount > 0 && CoverListSize() == 0)
        RefreshCoverList();
}
