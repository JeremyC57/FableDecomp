// Grade: DIFFER(922v922) -- control flow and calls match; x87 shapes of the two planar distances differ (retail spills the tagger delta X to memory and squares Y first; computes the start offset X before Y) and the frame is 0xc smaller (retail keeps more vector temporaries)
// CAIStateGroup_MinionKidGroupTag::EvadeTarget() -- when this creature is not the tagger: closer than 3 units to the tagger, face it and run (to a flee position 3.5 units from it, or back to the initial tag position once 15 units away) until tagged; farther than 4.5 units, face it and creep to within 2.5 units until tagged; in between, face it until tagged, then pull faces
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00934410, length 928 bytes (+ int3 padding), plain `ret`.
// Callees: CTCCreatureGroupTag::GetTagger @ 0x008138E0, CThingCreatureBase::GetPosition @ 0x006A4D00, CVectorMap::LowerBound
// @ 0x0040F020 (GetInterface 7 = CTCCreatureNavigation, bit 7 of +0x20, unchecked), CThing::GetPosition @ 0x004C73D0,
// CTCCreatureNavigation::GetNearestFleePosition @ 0x006CB840, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90
// (by-value CIntelligentPointer<const CThing>, vtable 0x01237F1C), AIAction_LookAtThing @ 0x0097D6F0, AIAction_MoveToPosProximity
// @ 0x0097E890, AIAction_MoveToThingProximity @ 0x0097E380, AIAction_LookAtPos @ 0x0097D580, CCharString(const char*, long)
// @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CCreatureAction_PlayAnimation ctor @ 0x008424F0 / ~CCreatureActionBase @ 0x00693EF0,
// AIAction_PerformAction @ 0x0097D8B0, Terminate @ 0x008FCF40.
// Data: 9.0f @ 0x0125EF98, 225.0f @ 0x012922C0, 20.25f @ 0x0128FFF4, "BOY_PULLING_FACES_01" @ 0x012922A8; 3.5f / 1.0f / 2.5f and the
// 0.0138889f look speed (immediates).
// Layout: local model (PBrain +4; brain PCreature +0x18; PTCGroupTag +0x18, InitialTagPos +0x1c; presence bits +0x20, interface
// map +0x44; the frame fits a 0xa8-byte CCreatureAction_PlayAnimation).
// Shape: the tagger distance is C2DVector(tagger) - C2DVector(creature) (TagTarget's form); the navigation lookup is the
// word-local HasTC bound to `const bool&`.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    __forceinline C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
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
    bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CThingAICreature : CThingCreatureBase { };
struct CTCCreatureNavigation {                  // interface 7
    bool GetNearestFleePosition(const C3DVector& centre, float radius, C3DVector* result);   // 0x006CB840
};
struct CTCCreatureGroupTag {
    CThingAICreature* GetTagger();              // 0x008138E0
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
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float turnSpeed);   // 0x0097D580
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float distance, bool* reached,
        long movement, bool a, bool b, bool c, bool d, float speed);         // 0x0097E380
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float distance, bool* reached, long movement,
        bool a, bool b, bool c, CThing* avoid, bool d, float speed);        // 0x0097E890
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
};
struct CAIStateGroup_MinionKidGroupTag : CAIStateGroupBase {
    bool InitiatedTag;                          // +0x14
    CTCCreatureGroupTag* PTCGroupTag;           // +0x18
    C3DVector InitialTagPos;                    // +0x1c
    __forceinline float TaggerDistanceSquared()
    {
        C3DVector taggerPos = PTCGroupTag->GetTagger()->GetPosition();
        const C3DVector& here = GetCreature()->GetPosition();
        C2DVector delta = C2DVector(taggerPos) - C2DVector(here);
        return delta.GetLengthSquared();
    }
    void EvadeTarget();
};

void CAIStateGroup_MinionKidGroupTag::EvadeTarget()
{
    if (PTCGroupTag->GetTagger() == GetCreature())
        return;
    float distanceSquared = TaggerDistanceSquared();
    if (distanceSquared < 9.0f) {
        CThingAICreature* creature = GetCreature();
        CTCCreatureNavigation* navigation;
        creature->GetInterface(&navigation, 7);
        C3DVector fleePos;
        const C3DVector& position = ((const CThing*)creature)->GetPosition();
        C3DVector fromStart = position - InitialTagPos;
        if (!(fromStart.X * fromStart.X + fromStart.Y * fromStart.Y < 225.0f))
            fleePos = InitialTagPos;
        else if (!navigation->GetNearestFleePosition(PTCGroupTag->GetTagger()->GetPosition(), 3.5f, &fleePos))
            fleePos = GetCreature()->GetPosition();
        bool reached = false;
        if (AIAction_LookAtThing(2, CIntelligentPointerConstThing(PTCGroupTag->GetTagger()), 0x22, 0.013888889f) == 1) {
            Terminate();
            return;
        }
        do {
            if (AIAction_MoveToPosProximity(2, fleePos, 1.0f, &reached, 6, true, false, true, 0, true, 0.0f) != 4)
                return;
        } while (PTCGroupTag->GetTagger() != GetCreature());
    } else if (distanceSquared > 20.25f) {
        bool reached = false;
        if (AIAction_LookAtThing(2, CIntelligentPointerConstThing(PTCGroupTag->GetTagger()), 0x22, 0.013888889f) == 1) {
            Terminate();
            return;
        }
        do {
            if (AIAction_MoveToThingProximity(2, CIntelligentPointerConstThing(PTCGroupTag->GetTagger()), 2.5f, &reached, 6,
                    true, false, true, true, 0.0f) != 4)
                return;
        } while (PTCGroupTag->GetTagger() != GetCreature());
    } else {
        if (AIAction_LookAtThing(2, CIntelligentPointerConstThing(PTCGroupTag->GetTagger()), 0x22, 0.013888889f) == 1) {
            Terminate();
            return;
        }
        while (PTCGroupTag->GetTagger() != GetCreature()) {
            if (AIAction_LookAtPos(2, ((const CThing*)PTCGroupTag->GetTagger())->GetPosition(), 1, 0.013888889f) != 4)
                break;
        }
        AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), "BOY_PULLING_FACES_01", CSoundPair(),
            false, true, false, 0, 0));
    }
}
