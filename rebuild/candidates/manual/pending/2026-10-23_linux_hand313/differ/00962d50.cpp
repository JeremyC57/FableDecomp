// CAIStateGroup_VersusRockTroll::Update() -- away from the troll (30% of the time) close in, drain it when within reach and back off; otherwise circle it at twice the facing offset (turning the other way when the path is blocked), signalling combat to the brain while moving
// Grade: DIFFER(1611v1679) -- first pass: prologue/normalise match; x87 squared-term order, the drain-response call temporaries and the circling block layout diverge
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
void __cdecl operator delete(void* block);       // 0x00BFE9BC
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
extern const float g_three;                     // 0x0122DED4
extern const float g_half;                      // 0x0122F59C
extern const float g_closeRange;                // 0x01254DA8
extern const float g_reachSlack;                // 0x012305A0
extern const float g_circleOffset;              // 0x01296360
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    void RotateZ(float turns);                  // 0x00A14040
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CPhysicsInfo {
    unsigned char _pad[0x34];
    float Radius;                               // +0x34
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsInfo* PPhysics;                     // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const bool& found = HasTC(interfaceType);
        if (found) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return found;
    }
};
struct CThingCreatureBase : CThing {
    bool IsInCombat();                          // 0x006621A0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {     // vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct CTCCreatureModeManager {                 // interface 0x31
    bool HasMode(long mode);                    // 0x0073A8A0
    void RemoveMode(long mode);                 // 0x0073ABA0
    void AddMode(long mode);                    // 0x0073AE10
};
struct CTCCreatureNavigation {                  // interface 7
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float radius);   // 0x006CB620
};
struct CCreatureActionBase { };
struct CCombatAction_GenericResponseToDrain : CCreatureActionBase {
    unsigned char _data[0x114];
    CCombatAction_GenericResponseToDrain(CThingCreatureBase* creature, CThing* drainer);   // 0x008AE160
    ~CCombatAction_GenericResponseToDrain();    // 0x0062E900
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);
    CCreatureActionBase* m_pObject;
};
struct CCountedPointerAction {
    CCreatureActionBase* m_pObject;
    SCountBlock* m_pCount;
    __forceinline ~CCountedPointerAction()
    {
        if (m_pCount != 0) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct CBrainOutput {
    unsigned long Types;                        // +0x00
    long LookFlags;                             // +0x04
    unsigned char NavigationInput[0x18];        // +0x08
    CCountedPointerAction PAction;              // +0x20
    C3DVector MoveToPos;                        // +0x28
    C3DVector LookAtPos;                        // +0x34
    CBaseIntelligentPointer LookAtThing;        // +0x40
    CBrainOutput(void* source);                 // 0x006D6110
};
struct CRandomSeed { unsigned long Value; };
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
struct CWorldSeed { CRandomSeed* GetSeed(); };          // 0x0049D810
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
    unsigned char _pad1c[0x44 - 0x1c];
    unsigned char OutputSource[4];              // +0x44
    void AddOutput(CBrainOutput* output);       // 0x0088B8F0
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual bool IsStillRunnable();             // slot 14 (+0x38)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorldSeed* GetPWorldSeed();                // 0x008FCFC0
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool b);   // 0x0097DB70
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long a, bool b, bool c, bool d, bool e, long f);   // 0x0097EC00
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float prox, bool* reached, long a, bool b, bool c, bool d, long e, bool f, float g);   // 0x0097E890
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerThing thing, float prox, bool* reached, long a, bool b, bool c, bool d, long e, bool f);   // 0x0097E380
};
struct CAIStateGroup_VersusRockTroll : CAIStateGroupBase {
    CBaseIntelligentPointer PTroll;             // +0x14
    bool CircleClockwise;                       // +0x1C
    virtual void Update();
};

void CAIStateGroup_VersusRockTroll::Update()
{
    if (IsStillRunnable()) {
        CThingCreatureBase* creature = GetCreature();
        CTCCreatureModeManager* modes;
        creature->GetInterface(&modes, 0x31);
        CThing* troll = PTroll.GetPItem();
        C3DVector offset = troll->GetPosition() - creature->GetPosition();
        float xx = offset.X * offset.X;
        float yy = offset.Y * offset.Y;
        C3DVector direction;
        {
            float lengthSquared = offset.Z * offset.Z + yy + xx;
            unsigned long bits = *(unsigned long*)&lengthSquared;
            unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23)
                | (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
            float r = *(float*)&estimate;
            r = (g_three - r * r * lengthSquared) * r * g_half;
            direction.X = offset.X * r;
            direction.Y = offset.Y * r;
            direction.Z = offset.Z * r;
        }
        bool reached;
        if ((float)sqrt(yy + xx) > g_closeRange) {
            CRandomSeed* seed = GetPWorldSeed()->GetSeed();
            seed->Value = seed->Value * 0x24a1 + 0x24df;
            seed->Value = GFROR13(seed->Value);
            if (seed->Value % 100 < 30) {
                if (modes->HasMode(0x19))
                    modes->RemoveMode(0x19);
                float reach = PTroll.GetPItem()->PPhysics->Radius + 3.5f;
                C3DVector start = GetCreature()->GetPosition();
                AIAction_MoveToThingProximity(1, CIntelligentPointerThing(PTroll.GetPItem()), reach, &reached, 5, true, true, true, 0, false);
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                {
                    CThingCreatureBase* me = GetCreature();
                    C3DVector d = PTroll.GetPItem()->GetPosition() - me->GetPosition();
                    C3DVector flat = d;
                    if (reach + g_reachSlack > (float)sqrt(flat.X * flat.X + flat.Y * flat.Y)) {
                        CThingCreatureBase* c = GetCreature();
                        AIAction_PerformActionAndWait(CCombatAction_GenericResponseToDrain(c, PTroll.GetPItem()), false);
                    }
                }
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                AIAction_MoveToPosProximity(1, start, 2.0f, &reached, 5, true, true, true, 0, false, 0.0f);
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                if (!IsStillRunnable())
                    Terminate();
                return;
            }
        }
        if (GetCreature()->IsInCombat() && !modes->HasMode(0x19))
            modes->AddMode(0x19);
        C3DVector side = direction;
        side.RotateZ(CircleClockwise ? 0.25f : -0.25f);
        C3DVector twice(side.X + side.X, side.Y + side.Y, side.Z + side.Z);
        C3DVector back = direction * g_circleOffset;
        C3DVector target = PTroll.GetPItem()->GetPosition() - back + twice;
        CThing* me = GetCreature();
        CTCCreatureNavigation* nav;
        me->GetInterface(&nav, 7);
        if (!nav->IsAbleToNavigateFromPosToPos(me->GetPosition(), target, 2.0f)) {
            side.RotateZ(-0.5f);
            C3DVector twice2(side.X + side.X, side.Y + side.Y, side.Z + side.Z);
            target = PTroll.GetPItem()->GetPosition() - back + twice2;
        }
        long result;
        do {
            result = AIAction_MoveToPos(2, target, &reached, 3, !GetCreature()->IsInCombat(), true, false, false, 0);
            if (GetCreature()->IsInCombat() && IsStillRunnable()) {
                CBrainOutput output(PBrain->OutputSource);
                output.Types |= 4;
                output.LookAtThing.SetPItem(CIntelligentPointerThing(PTroll.GetPItem()).GetPItem());
                output.LookFlags = 0xa7;
                PBrain->AddOutput(&output);
            }
        } while (result == 4 && IsStillRunnable());
    }
    if (!IsStillRunnable())
        Terminate();
}
