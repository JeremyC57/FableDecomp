// CAIStateGroup_MinionKidGroupTag::TagTarget() -- pick the nearest other active member of the tag group, run up to it, face it (Terminate if the look is abandoned) and, still within reach and at least two seconds after the last tag, make it the tagger and play the TAG animation; Terminate once that finishes
// Grade: DIFFER(567v580) nd=73 (was 562v580 nd=81) -- levers: IsDead accessor pre-screen (567), the reach compare as !(lengthSquared <= reach*reach) (test ah,1 ; jne like retail); tried: target position by reference (555 nd=101), the delta built from components (549 nd=79); remaining: frame 0xc8 v 0xc4 (the reached byte and position copy 4 bytes off), the first reach test keeps delta.X on the FPU where retail spills it to the frame and re-reads it for the square
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009341C0, length 592 bytes (+ int3 padding), plain `ret`.
// Callees: CTCCreatureGroup::GetNearestOtherActiveMember @ 0x00812CA0, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem
// @ 0x00A01B90 (by-value CIntelligentPointer<const CThing>, vtable 0x01237F1C), AIAction_MoveToThingProximity @ 0x0097E380,
// CThingCreatureBase::GetPosition @ 0x006A4D00, AIAction_LookAtThing @ 0x0097D6F0, CWorld::GetWorldFrame @ 0x0049D870,
// CTCCreatureGroupTag::SetTagger @ 0x008137B0, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
// CCreatureAction_PlayAnimation ctor @ 0x008424F0 / ~CCreatureActionBase @ 0x00693EF0, AIAction_PerformActionAndWait
// @ 0x0097DB70, Terminate @ 0x008FCF40.
// Data: 0.5f @ 0x01292294, 2^32 @ 0x0122DCB4 (unsigned to float), g_framesPerSecond @ 0x01375550, "TAG" @ 0x012922A4;
// 0.5f move distance and 0.013888889f look speed (immediates).
// Layout: local model (PBrain +4; brain PCreature +0x18; PTCGroupTag +0x18, its last tag frame +0x30; retail CThing physics body
// +0x60, radius +0x34; CCreatureAction_PlayAnimation 0xb4 bytes).
// Shape: the reach test (planar distance against radius + 0.5) is one inline helper used twice.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector operator-(const C2DVector& other) const { return C2DVector(X - other.X, Y - other.Y); }
    float GetLengthSquared() const { return X * X + Y * Y; }
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CPhysicsBody {
    unsigned char _pad[0x34];
    float Radius;                               // +0x34
};
struct CThing {
    unsigned char _pad0[0x60];
    CPhysicsBody* PPhysics;                     // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }   // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CThingAICreature : CThingCreatureBase { };
struct CTCCreatureGroupTag {
    unsigned char _pad[0x30];
    unsigned long LastTagFrame;                 // +0x30
    CThingAICreature* GetNearestOtherActiveMember(CThingAICreature* creature);   // 0x00812CA0
    bool SetTagger(CThingAICreature* creature); // 0x008137B0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
struct CWorld {
    static unsigned long GetWorldFrame();       // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float distance, bool* reached,
        long movement, bool a, bool b, bool c, bool d, float speed);         // 0x0097E380
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
};
struct CAIStateGroup_MinionKidGroupTag : CAIStateGroupBase {
    bool InitiatedTag;                          // +0x14
    CTCCreatureGroupTag* PTCGroupTag;           // +0x18
    __forceinline bool IsOutOfReach(CThingAICreature* target)
    {
        C3DVector targetPos = target->GetPosition();
        const C3DVector& here = GetCreature()->GetPosition();
        C2DVector delta = C2DVector(targetPos) - C2DVector(here);
        float reach = GetCreature()->PPhysics->Radius + 0.5f;
        return !(delta.GetLengthSquared() <= reach * reach);
    }
    void TagTarget();
};

void CAIStateGroup_MinionKidGroupTag::TagTarget()
{
    CThingAICreature* target = PTCGroupTag->GetNearestOtherActiveMember(GetCreature());
    if (!target || target->IsDead())
        return;
    bool reached = false;
    if (AIAction_MoveToThingProximity(1, CIntelligentPointerConstThing(target), 0.5f, &reached, 6, true, false, true, true, 0.0f) != 2)
        return;
    if (IsOutOfReach(target))
        return;
    if (AIAction_LookAtThing(1, CIntelligentPointerConstThing(target), 1, 0.013888889f) == 1) {
        Terminate();
        return;
    }
    if (IsOutOfReach(target))
        return;
    long twoSeconds = g_framesPerSecond;
    unsigned long elapsed = CWorld::GetWorldFrame() - PTCGroupTag->LastTagFrame;
    if (!((float)elapsed > (float)twoSeconds * 2.0f))
        return;
    PTCGroupTag->SetTagger(target);
    bool done = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "TAG", CSoundPair(),
        false, true, false, 0, 0), false) == 1;
    if (done)
        Terminate();
}
