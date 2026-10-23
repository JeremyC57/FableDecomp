// CAIStateGroup_MinionGuardReactToBribe::Update() -- react to the hero's bribe: face the hero, then either wait out the bribe window and thank them, or chase and warn the hero, reporting deeds when they run off
// Grade: DIFFER(1357v1347) -- structure and block order match; register roles differ (retail edi=&PHero, ebx/ebp hold the bribe window, lastReact in the frame; ours ebp=&PHero) and one extra frame slot
// Layout: local model (the engine header flattens the base); PHero +0x14, FrameLastReactedToReceiveing +0x1c, HasRecievedBribe +0x20.
struct CTCBase;
struct CVillage;
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& o) { SetPItem(o.GetPItem()); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) { SetPItem(item); }
};
struct CCharString {
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CTCVillageMember;
CVillage* __fastcall GetVillage(CTCVillageMember* member);   // 0x007EE540
bool __fastcall IsThingRunningAwayFromThing(const CThing* runner, const CThing* from, const C3DVector* oldPos);   // 0x00864170
struct CTCGuard {                               // interface 0xB4
    unsigned char _pad[0x10];
    long BribeState;                            // +0x10
    unsigned char _pad14[0x3c - 0x14];
    long BribeWarnFrame;                        // +0x3c
    long BribeFrame;                            // +0x40
    long GetBribeWarnSeconds();                 // 0x007B2990
    void ProcessDeedReports(CVillage* village); // 0x007B2E20
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
extern long g_FramesPerSecond;                  // 0x01375550
long __fastcall GFGetCurrentFrame();            // 0x0049D870
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float speed);   // 0x0097D6F0
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing thing, float proximity, bool* arrived,
                                       long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    long AIAction_SpeakAISpeechToTarget(long repeat, CIntelligentPointerCreature speaker,
        CIntelligentPointerConstThing target, const CCharString& speech, bool interrupt, long priority);  // 0x00981EA0
};
struct CAIStateGroup_MinionGuardReactToBribe : CAIStateGroupBase {
    CBaseIntelligentPointer PHero;              // +0x14
    long FrameLastReactedToReceiveing;          // +0x1c
    bool HasRecievedBribe;                      // +0x20
    void Update();
};

void CAIStateGroup_MinionGuardReactToBribe::Update()
{
    CTCGuard* guard = 0;
    bool hasGuard = PBrain->PCreature->GetInterfaceT(&guard, 0xb4);
    if (AIAction_LookAtThing(1, PHero, 0x13, 0.0027777778f) == 1) {
        Terminate();
        return;
    }
    if (HasRecievedBribe) {
        long fps = g_FramesPerSecond;
        long lastReact = FrameLastReactedToReceiveing + fps * 7;
        for (;;) {
            long earliest = guard->BribeFrame + fps * 2;
            long latest = guard->BribeFrame + fps * 4;
            long now = GFGetCurrentFrame();
            if (now > latest || guard->BribeState < 0 || lastReact > latest)
                break;
            if (now >= earliest && now >= lastReact) {
                CCharString speech("GUARD_THANKS_FOR_BRIBE");
                AIAction_SpeakAISpeechToTarget(1, PBrain->PCreature, PHero, speech, false, 0x93);
                FrameLastReactedToReceiveing = GFGetCurrentFrame();
                Terminate();
                return;
            }
            if (AIAction_LookAtThing(1, PHero, 0x13, 0.0027777778f) == 1)
                break;
            fps = g_FramesPerSecond;
        }
        Terminate();
        return;
    }
    long result;
    {
        CCharString speech("GUARD_ASK_FOR_BRIBE");
        result = AIAction_SpeakAISpeechToTarget(1, PBrain->PCreature, PHero, speech, false, 0x93);
    }
    if (result == 1) {
        Terminate();
        return;
    }
    long warnStart = guard->BribeWarnFrame - 5;
    long endFrame = g_FramesPerSecond * guard->GetBribeWarnSeconds() + warnStart;
    long midFrame = (long)((float)(g_FramesPerSecond * guard->GetBribeWarnSeconds()) * 0.5f + warnStart);
    C3DVector heroPos = PHero.GetPItem()->GetPosition();
    bool said = false;
    do {
        if (guard->BribeState >= 0) {
            CCharString speech("GUARD_BRIBE_PAID");
            result = AIAction_SpeakAISpeechToTarget(1, PBrain->PCreature, PHero, speech, false, 0x93);
            FrameLastReactedToReceiveing = GFGetCurrentFrame();
            goto done;
        }
        CThing* creature = PBrain->PCreature;
        if (IsThingRunningAwayFromThing(PHero.GetPItem(), creature, &heroPos)) {
            CCharString speech("GUARD_BRIBE_RAN");
            result = AIAction_SpeakAISpeechToTarget(1, PBrain->PCreature, PHero, speech, false, 0x93);
            CTCVillageMember* member = 0;
            PBrain->PCreature->GetInterfaceV(&member, 0x23);
            guard->ProcessDeedReports(GetVillage(member));
            goto done;
        }
        bool arrived;
        result = AIAction_MoveToThingProximity(2, PHero, 3.0f, &arrived, 3, true, false, true, true, 0.0f);
        if (result == 2)
            result = AIAction_LookAtThing(2, PHero, 0x13, 0.0027777778f);
        if (!said && result == 2 && GFGetCurrentFrame() >= midFrame) {
            CCharString speech("GUARD_BRIBE_REMIND");
            result = AIAction_SpeakAISpeechToTarget(1, PBrain->PCreature, PHero, speech, false, 0x93);
            said = true;
        }
        if (GFGetCurrentFrame() >= endFrame)
            goto done;
    } while (result != 1);
    Terminate();
    return;
done:
    if (result != 1 && GFGetCurrentFrame() >= endFrame) {
        CCharString speech("GUARD_BRIBE_TIMEOUT");
        AIAction_SpeakAISpeechToTarget(1, PBrain->PCreature, PHero, speech, false, 0x93);
    }
    Terminate();
}
