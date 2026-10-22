// Grade: DIFFER(569v571) -- only the sit-delay minimum add: ours folds `add eax,[0x13BB6FC]`, retail loads it first (`mov ecx,[0x13BB6FC] ; add eax,edi ; add eax,ecx`); named/const-ref/split/parenthesised sums and an inlined GFRandom 0x004CDAE0 tried
// CAIStateGroup_SitRoundCampfire::OnReplace() -- when replaced: free the seat's action point at the live campfire; when sitting (or the creature's current mode is type 10) schedule the next sit a random number of frames ahead, post a GetUpFromSitting combat action (when the creature is free to perform it), leave mode 10 and stop sitting; then pick a new idle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009397E0, length 571 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCampfire +0x14), CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface: 0x2D CTCUserActionPointBase, bit 13 of +0x24; 0x31 CTCCreatureModeManager, bit 17 of +0x24; 0xD3
// CTCIdleScheduler, bit 19 of +0x38), CTCUserActionPointBase::FreeActionPoint @ 0x007FEF40 (manifest
// "EditCyclePreviousKeyCamera" is propagated), the mode manager's current mode @ 0x0073A6E0 (manifest
// "CPixelShader::CountReferences" is propagated) and its virtual slot 1, GetPThingWorld @ 0x008FCFA0, GetPWorldSeed @
// 0x008FCFD0, GFROR13 @ 0x00497890, GetWorldFrame @ 0x0049D870, CBrainOutput ctor @ 0x00835620, CCombatActionBase ctor
// @ 0x00858030 / SetAnimationName @ 0x008574D0 / ~CCombatActionBase @ 0x0062E900 (the inline CCreatureAction_
// GetUpFromSitting ctor, vtable 0x01276C2C, landed 0x00860150), CThingCreatureBase::IsFreeToPerformAction @ 0x00662210,
// the action's virtual Clone (slot 11), CCountedPointer::Reset @ 0x00668D90, CAIBrain::SetBrainOutput @ 0x0088B8F0,
// CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, ~CBaseIntelligentPointer @ 0x00A01C10, operator delete @ 0x00BFE9BC,
// CTCIdleScheduler::ChooseNewIdle @ 0x00801A50.
// Data: the get-up animation name CCharString @ 0x013BB6F0, the sit delay range @ 0x013BB700 and minimum @ 0x013BB6FC.
// Layout: Ego_r.pdb CAIStateGroup_SitRoundCampfire: PCampfire +0x14, SeatPosIndex +0x1c, GotSeatPos +0x20, Sitting +0x21,
// FrameToNextSitOnSeat +0x24; retail CCombatActionBase is 0x114 bytes.
// Shape: the landed UsesSeat::OnReplace / AIAction_CombatPerformActionAndWait models (implicit ~CBrainOutput inline).
struct CTCBase;
struct CThing;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (STLport lower_bound over the 8-byte entries).
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
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CCreatureActionBase;
struct CThingCreatureBase : CThing {
    bool IsFreeToPerformAction(const CCreatureActionBase* action);   // 0x00662210
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCharString {
    void* PStringData;
};
extern CCharString g_GetUpFromSittingAnim;      // 0x013BB6F0 (descriptive)
extern unsigned long g_SitDelayRange;           // 0x013BB700 (descriptive)
extern unsigned long g_SitDelayMinimum;         // 0x013BB6FC (descriptive)
struct CCountBlock { long Count; void (__fastcall* Destroy)(void* object); void* Object; };
struct CCountedPointer {
    void* Object;
    CCountBlock* Block;
    void Reset(void* object);                   // 0x00668D90
    ~CCountedPointer()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
};
struct CBrainOutput {                           // 0x48 bytes
    unsigned long Flags;                        // +0x00
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                    // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;            // +0x40
    CBrainOutput();                             // 0x00835620
};
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void* Clone() const;                // slot 11 (+0x2c)
};
struct CCombatActionBase : CCreatureActionBase {   // retail sizeof 0x114
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    ~CCombatActionBase();                       // 0x0062E900
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};
struct CCreatureAction_GetUpFromSitting : CCombatActionBase {   // vtable 0x01276C2C
    long SittingMode;                           // +0x114
    bool RemoveOnPerformAction;                 // +0x118
    virtual void v00();
    __forceinline CCreatureAction_GetUpFromSitting(CThingCreatureBase* owner, const CCharString& animName, long sittingMode,
                                                   bool removeOnPerformAction)
        : CCombatActionBase(owner, 50, CSoundPair()), SittingMode(sittingMode), RemoveOnPerformAction(removeOnPerformAction)
    {
        SetAnimationName(animName);
    }
};
struct CMode {
    virtual void v00();
    virtual long GetType();                     // slot 1 (+0x04)
};
struct CTCCreatureModeManager {                 // interface 0x31
    CMode* GetCurrentMode();                    // 0x0073A6E0
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CTCUserActionPointBase {                 // interface 0x2d
    void FreeActionPoint(long index);           // 0x007FEF40
};
struct CTCIdleScheduler {                       // interface 0xd3
    void ChooseNewIdle();                       // 0x00801A50
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPThingWorld();                   // 0x008FCFA0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
};
struct CAIStateGroup_SitRoundCampfire : CAIStateGroupBase {
    CBaseIntelligentPointer PCampfire;          // +0x14
    long SeatPosIndex;                          // +0x1c
    bool GotSeatPos;                            // +0x20
    bool Sitting;                               // +0x21
    unsigned long FrameToNextSitOnSeat;         // +0x24
    virtual void OnReplace();
};

void CAIStateGroup_SitRoundCampfire::OnReplace()
{
    if (GotSeatPos && IS_THING_ALIVE(PCampfire)) {
        CTCUserActionPointBase* points;
        PCampfire.GetPItem()->GetInterface(&points, 0x2d);
        points->FreeActionPoint(SeatPosIndex);
    }
    CTCCreatureModeManager* modes;
    GetCreature()->GetInterface(&modes, 0x31);
    if (Sitting || modes->GetCurrentMode()->GetType() == 10) {
        CWorld* world = GetPThingWorld();
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        unsigned long range = g_SitDelayRange;
        *seed = rotated;
        unsigned long r;
        if (range != 0)
            r = rotated % range;
        else
            r = 0;
        FrameToNextSitOnSeat = world->GetWorldFrame() + r + g_SitDelayMinimum;
        CBrainOutput output;
        CCreatureAction_GetUpFromSitting action(GetCreature(), g_GetUpFromSittingAnim, 0, false);
        CCreatureActionBase* pAction = &action;
        if (GetCreature()->IsFreeToPerformAction(pAction)) {
            output.Flags |= 8;
            output.PAction.Reset(pAction->Clone());
            PBrain->SetBrainOutput(output);
        }
        modes->RemoveMode(10);
        Sitting = false;
    }
    CTCIdleScheduler* idle;
    if (GetCreature()->GetInterface(&idle, 0xd3))
        idle->ChooseNewIdle();
}
