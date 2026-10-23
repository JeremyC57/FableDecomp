// CAIStateGroup_VersusScorpionKing::Update() -- dodge a charge; otherwise (30% of the time) close in on the target, drain it within reach and back off, or circle the king at twice the facing offset, signalling combat to the brain while moving
// Grade: DIFFER(1635v1695) -- first pass: overall structure follows retail; the circling branch block layout, the second random roll placement and x87 copy scheduling diverge
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
void __cdecl operator delete(void* block);       // 0x00BFE9BC
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
extern const float g_three;                     // 0x0122DED4
extern const float g_half;                      // 0x0122F59C
extern const float g_closeRange;                // 0x01254DA8
extern const float g_reachSlack;                // 0x012305A0
extern const float g_circleOffset;              // 0x01296370
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
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91
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
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& other);   // 0x0091F990
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
struct CBrainOutput {
    unsigned char _data[0x48];
    CBrainOutput(void* source);                 // 0x006D6110
    ~CBrainOutput();                            // 0x006D60C0
    void SetAsLookAtThing(CThing* thing, long flags);   // 0x0093E760
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
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing thing, float prox, bool* reached, long a, bool b, bool c, bool d, long e, bool f);   // 0x0097E380
};
#pragma pack(push, 1)
struct CAIStateGroup_VersusScorpionKing : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14
    bool CircleClockwise;                       // +0x1C
    CBaseIntelligentPointer PKing;              // +0x1D
    bool IsScorpionChargingAtMe();              // 0x00963880
    void GetChargeDodgePos(C3DVector* out);     // 0x009633E0
    virtual void Update();
};
#pragma pack(pop)

void CAIStateGroup_VersusScorpionKing::Update()
{
    long result = 2;
    bool charging = IsScorpionChargingAtMe();
    CTCCreatureModeManager* modes;
    GetCreature()->GetInterface(&modes, 0x31);
    if (IsStillRunnable() && PKing.GetPItem() && !(PKing.GetPItem()->m_flags91 & 1)) {
        bool reached;
        if (charging) {
            if (modes->HasMode(0x19))
                modes->RemoveMode(0x19);
            C3DVector dodge;
            GetChargeDodgePos(&dodge);
            do {
                if (AIAction_MoveToPos(2, dodge, &reached, 5, false, true, false, false, 0) != 4)
                    break;
            } while (IsStillRunnable());
        } else {
            CRandomSeed* seed = GetPWorldSeed()->GetSeed();
            seed->Value = seed->Value * 0x24a1 + 0x24df;
            seed->Value = GFROR13(seed->Value);
            if (seed->Value % 100 < 30) {
                if (modes->HasMode(0x19))
                    modes->RemoveMode(0x19);
                float reach = PTarget.GetPItem()->PPhysics->Radius + 3.5f;
                C3DVector start = GetCreature()->GetPosition();
                AIAction_MoveToThingProximity(1, CIntelligentPointerConstThing(PTarget), reach, &reached, 5, true, true, true, 0, false);
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                {
                    CThingCreatureBase* me = GetCreature();
                    CThing* target = PTarget.GetPItem();
                    const C3DVector& a = target->GetPosition();
                    const C3DVector& b = me->GetPosition();
                    float dy = a.Y - b.Y;
                    float dx = a.X - b.X;
                    float lengthSquared = dx * dx + dy * dy;
                    if (reach + g_reachSlack > (float)sqrt(lengthSquared)) {
                        CThingCreatureBase* c = GetCreature();
                        AIAction_PerformActionAndWait(CCombatAction_GenericResponseToDrain(c, PTarget.GetPItem()), false);
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
            } else {
                CThingCreatureBase* creature = GetCreature();
                CThing* king = PKing.GetPItem();
                C3DVector offset = king->GetPosition() - creature->GetPosition();
                C3DVector direction;
                {
                    float lengthSquared = offset.X * offset.X + offset.Y * offset.Y + offset.Z * offset.Z;
                    unsigned long bits = *(unsigned long*)&lengthSquared;
                    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23)
                        | (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
                    float r = *(float*)&estimate;
                    r = (g_three - r * r * lengthSquared) * r * g_half;
                    direction.X = offset.X * r;
                    direction.Y = offset.Y * r;
                    direction.Z = offset.Z * r;
                }
                C3DVector side = direction;
                side.RotateZ(CircleClockwise ? 0.25f : -0.25f);
                C3DVector twice(side.X + side.X, side.Y + side.Y, side.Z + side.Z);
                C3DVector back = direction * g_circleOffset;
                C3DVector target = PKing.GetPItem()->GetPosition() - back + twice;
                CThing* me = GetCreature();
                CTCCreatureNavigation* nav;
                me->GetInterface(&nav, 7);
                if (!nav->IsAbleToNavigateFromPosToPos(me->GetPosition(), target, 2.0f)) {
                    side.RotateZ(-0.5f);
                    C3DVector twice2(side.X + side.X, side.Y + side.Y, side.Z + side.Z);
                    target = PTarget.GetPItem()->GetPosition() - back + twice2;
                }
                CRandomSeed* seed2 = GetPWorldSeed()->GetSeed();
                seed2->Value = seed2->Value * 0x24a1 + 0x24df;
                seed2->Value = GFROR13(seed2->Value);
                long roll = seed2->Value % 100;
                do {
                    charging = IsScorpionChargingAtMe();
                    if (!charging) {
                        if (GetCreature()->IsInCombat() && !modes->HasMode(0x19))
                            modes->AddMode(0x19);
                        result = AIAction_MoveToPos(2, target, &reached, 5, !GetCreature()->IsInCombat(), true, false, false, 0);
                        if (GetCreature()->IsInCombat() && IsStillRunnable()) {
                            CBrainOutput output(PBrain->OutputSource);
                            output.SetAsLookAtThing(PTarget.GetPItem(), 0xa7);
                            PBrain->AddOutput(&output);
                        }
                    }
                } while (result == 4 && IsStillRunnable() && !charging);
            }
        }
    }
    if (!IsStillRunnable())
        Terminate();
}
