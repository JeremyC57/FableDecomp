// CAIStateGroup_SeekPerceivedThing::RunToPerceivedThingPosition() -- while alert, run to the last perceived position and play WHATS_THAT when the thing is not there
// Leaf: RunToPerceivedThingPosition
// Grade: DIFFER(472v455) -- one attempt (out of budget for the band): 8-aligned frame, perception component held in ebp across two nested move loops, WHATS_THAT animation through PlayAnimation(CAnimationEntry*); register roles and the inner-loop exit shape differ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091B3A0; best of the probes graded under VC7.1 RTM (probe/0091b3a0.cpp).
// Note: GetPerceivedThing @ 0x007446A0 takes NO argument (0x0091B570 calls it with none); the 3.5f passed below belongs to IsInRange -- fix before re-grading.
struct CTCBase;
struct C3DVector {
    float x, y, z;
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
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
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        bool has = (unsigned char)(m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1;
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CAnimationEntry;
struct CThingCreatureBase;
struct CThingCreature : CThing {
    bool IsInRange(const C3DVector& pos, float range);              // 0x006A4DF0
    CAnimationEntry* GetRandomAnimFromName(const CCharString& name, long index);   // 0x00662D80
};
struct CTCPerception {                          // interface 0x34
    unsigned char _pad[0x18];
    float Alertness;                            // +0x18
    unsigned char _pad1c[4];
    C3DVector LastPerceivedPos;                 // +0x20
    unsigned char _pad2c;
    bool Seen;                                  // +0x2d
    CThing* GetPerceivedThing(float range);     // 0x007446A0
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, CAnimationEntry* anim, const CSoundPair& sound,
        bool a, bool b, bool c, long d, long e);    // 0x00842570
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                  // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    bool IsStopping();                          // 0x008FD010
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float proximity, bool* reached, long movement,
        bool a, bool b, bool c, CThing* thing, bool d, float speed);   // 0x0097E890
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b);   // 0x0097DB70
};
struct CAIStateGroup_SeekPerceivedThing : CAIStateGroupBase {
    long FrameLastCaptured;                     // +0x14
    void RunToPerceivedThingPosition();
};

void CAIStateGroup_SeekPerceivedThing::RunToPerceivedThingPosition()
{
    CTCPerception* perception;
    PBrain->PCreature->GetInterface(&perception, 0x34);
    while (perception->Alertness != 0.0f && !IsStopping()) {
        long result;
        do {
            if (perception->Alertness == 0.0f)
                break;
            C3DVector target = perception->LastPerceivedPos;
            bool reached;
            result = AIAction_MoveToPosProximity(2, target, 0.5f, &reached, 5, true, false, true, 0, true, 0.0f);
            if (PBrain->PCreature->IsInRange(target, 3.5f)
                && !PBrain->PCreature->IsInRange(perception->GetPerceivedThing(3.5f)->GetPosition(), 3.5f)
                && !perception->Seen
                && PBrain->PCreature->IsInRange(perception->LastPerceivedPos, 3.5f)) {
                CAnimationEntry* anim = PBrain->PCreature->GetRandomAnimFromName(CCharString("WHATS_THAT"), -1);
                CCreatureAction_PlayAnimation action((CThingCreatureBase*)PBrain->PCreature, anim, CSoundPair(),
                    false, true, false, 0, 0);
                AIAction_PerformActionAndWait(&action, false);
            }
        } while (result == 4);
    }
}
