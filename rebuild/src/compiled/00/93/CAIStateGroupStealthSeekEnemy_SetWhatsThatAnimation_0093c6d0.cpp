// CAIStateGroup_StealthSeekEnemy::SetWhatsThatAnimation(CTCPerceiveThing*) -- play a random WHATS_THAT animation; the
// Leaf: SetWhatsThatAnimation
// update finishes unless the animation completed, the perceived thing is still worth seeking and it has not moved
// from the last known position (squared 3D distance within 1e-4 squared).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093C6D0, length 206 bytes (+ int3 padding), `ret 4`.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("WHATS_THAT" @ 0x01273694), CThingCreatureBase::
// GetRandomAnimFromName @ 0x00662D80, CCreatureAction_PlayAnimation(creature, CAnimationEntry*, sound, ...) @
// 0x00842570, CAIStateGroupBase::AIAction_PerformAction @ 0x0097D8B0, ~CCreatureActionBase @ 0x00693EF0, ~CCharString
// @ 0x0099EAE0, IsStillRunnable(CTCPerceiveThing*) @ 0x0093C4E0, the finish call @ 0x008FCF40; epsilon float @ 0x0129BA3C.
// Shape: the creature through an inline const GetCreature() accessor (PBrain loaded early, as retail); an IMPLICIT CCharString conversion temporary for the name, the action result as a named long compared after
// the action dtor, `diff.GetMagnitudeSquared() > EPS * EPS` with the epsilon read from its global twice.
// No engine header for this class; LastKnownEnemyPos (+0x1c) named descriptively.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetMagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};

struct CAnimationEntry;
struct CThingCreatureBase;
struct CThingCreatureAnim {
    CAnimationEntry* GetRandomAnimFromName(const CCharString& name, long index);   // 0x00662D80
};

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];                  // +0x04..+0xa7
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];                  // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThing* creature, CAnimationEntry* animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x00842570
};

struct CTCPerceiveThing {
    unsigned char _pad[0x20];
    float PosX, PosY, PosZ;                     // +0x20
};

extern float g_Epsilon;                         // 0x0129BA3C (1e-4)

struct CAIStateGroup_StealthSeekEnemy : CAIStateGroupBase {
    unsigned char _pad_0x14[0x8];               // +0x14 PEnemy
    float LastKnownEnemyPosX, LastKnownEnemyPosY, LastKnownEnemyPosZ;   // +0x1c
    CThing* GetCreature() const { return PBrain->PCreature; }
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    bool IsStillRunnable(CTCPerceiveThing* perceive);   // 0x0093C4E0
    void FinishUpdate();                        // 0x008FCF40
    void SetWhatsThatAnimation(CTCPerceiveThing* perceive);
};

void CAIStateGroup_StealthSeekEnemy::SetWhatsThatAnimation(CTCPerceiveThing* perceive)
{
    CAnimationEntry* animation = ((CThingCreatureAnim*)GetCreature())->GetRandomAnimFromName("WHATS_THAT", -1);
    long result = AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(),
        false, true, false, 0, 0));
    if (result == 2 && IsStillRunnable(perceive)) {
        C3DVector diff(perceive->PosX - LastKnownEnemyPosX, perceive->PosY - LastKnownEnemyPosY,
            perceive->PosZ - LastKnownEnemyPosZ);
        if (!(diff.GetMagnitudeSquared() > g_Epsilon * g_Epsilon))
            return;
    }
    FinishUpdate();
}