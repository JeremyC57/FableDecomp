// CAIStateGroup_WanderAroundInitialPos::Update() -- walk once to a wander position around the creature's initial position (radius = the native scratchpad's WanderMaxDistance), then, if the creature is alive, clear its targeting list and reset its perception, and terminate
// Grade: DIFFER(ours v retail) -- 391v391, esi/edi swapped in the first half only (retail: creature esi, interface map edi; ours the reverse); second half byte-identical
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00911A90, length 400 bytes (same TU as the landed IsRunnable @ 0x00911A00).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (four inlined interface lookups: 7 = CTCCreatureNavigation, 170 =
//          CTCAIScratchpadNative, 8 = the targeting component, 52 = CTCPerceiveThing), CTCCreatureNavigation::
//          GetNearbyWanderPosition @ 0x006CB6E0, CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00, the type-8
//          component's list clear @ 0x007F6160 (a thunk `add ecx,48h ; jmp 0x4B8050`), CTCPerceiveThing::
//          ResetPerceptionVariables @ 0x00744DB0, CAIStateGroupBase::Terminate @ 0x008FCF40 (manifest label
//          "CXMVPlaybackInfo" is propagated; 47 landed callers name it Terminate).
// Shape: the first two lookups spill the presence bit as a frame byte (the `bool has = test; if (has)` template form,
//          as in the landed ChickenWander/Wander Update bodies) and the result pointer shares its home with the key; the
//          wander centre is the planar initial position at +0xE8 (same descriptive member as the landed IsRunnable), the
//          radius the scratchpad's WanderMaxDistance (+0x24); the MoveToPos result is dropped. The alive test is the
//          inline `(unsigned char)~flags & 1` (`not cl ; test cl,1`, landed 0x00723AC0 idiom); the last two lookups use the masked-dword HasTC form
//          (`test ch,1` / `test dword ptr`) with the component straight into ecx, each re-reading the brain's creature.
// Layout: CAIStateGroupBase per engine/CAIStateGroupBase.h (PBrain +4); creature bitset +0x20, interface map +0x44,
//          dead flag +0x91, initial position +0xE8 (retail CThingAICreature; PDB names differ, see the landed IsRunnable).
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsAlive() const { return ((unsigned char)~m_flags91 & 1) != 0; }
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    // Spilled-bit form (the presence test stored as a frame byte before the lookup).
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
    // Masked-dword form (`test ch,1` / `test dword ptr [eax+24h],100000h`).
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

struct CThingAICreature : CThing {
    unsigned char _pad2[0xe8 - sizeof(CThing)];
    C3DVector InitialPos;                       // +0xe8 (descriptive: the wander centre)
};

struct CTCCreatureNavigation {                  // interface 7
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool onMap);   // 0x006CB6E0
};
struct CTCAIScratchpadNative {                  // interface 170
    unsigned char _pad[0x24];                   // CTCBase + ThankingPhrase, IgnoringPhrase, WanderCentrePoint, WanderMinDistance
    float WanderMaxDistance;                    // +0x24
};
struct CTCTargetingBase {                       // interface 8 (descriptive)
    void ClearTargets();                        // 0x007F6160 (thunk: list at +0x48 -> clear @ 0x004B8050)
};
struct CTCPerceiveThing {                       // interface 52
    void ResetPerceptionVariables();            // 0x00744DB0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* avoid, bool c, float speed);    // 0x0097EC00
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_WanderAroundInitialPos : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_WanderAroundInitialPos::Update()
{
    CThingAICreature* creature = PBrain->PCreature;
    CTCCreatureNavigation* navigation;
    creature->GetInterfaceT(&navigation, 7);
    CTCAIScratchpadNative* scratchpad;
    creature->GetInterfaceT(&scratchpad, 170);
    C3DVector target;
    if (navigation->GetNearbyWanderPosition(creature->InitialPos, scratchpad->WanderMaxDistance, &target, true)) {
        bool reached = false;
        AIAction_MoveToPos(1, target, &reached, 5, true, true, 0, true, 0.0f);
    }
    if (PBrain->PCreature->IsAlive()) {
        CTCTargetingBase* targeting;
        PBrain->PCreature->GetInterface(&targeting, 8);
        targeting->ClearTargets();
        CTCPerceiveThing* perceive;
        PBrain->PCreature->GetInterface(&perceive, 52);
        perceive->ResetPerceptionVariables();
    }
    Terminate();
}
