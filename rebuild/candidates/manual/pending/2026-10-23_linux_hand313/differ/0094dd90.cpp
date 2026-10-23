// CAIStateGroup_RockTroll::Update() -- rock troll combat round: glance and stab when the enemy is close, otherwise throw rocks until the enemy closes in or goes out of range, then boast when the enemy was knocked down
// Grade: DIFFER(1754v1761) -- first pass; block placement (retail puts the throwing else-branch after the returns and jumps back into the idle/boast tail; a goto layout made it worse), duplicated inline exits, frame 0x14c v 0x150
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Length2D() const { return (float)sqrt(X * X + Y * Y); }
};
__forceinline C3DVector operator*(const C3DVector& v, const float& s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
__forceinline C3DVector Normalise(const C3DVector& v) { return v * FastInvSqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); }
__forceinline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetForward() const;       // slot 70 (+0x118)
};
struct CRockTrollDef {
    virtual void v00();
    virtual void DeleteThis();                  // slot 1 (+0x04)
    long RefCount;                              // +0x04
    unsigned char _pad08[0x50 - 0x08];
    float BoastMinDistance;                     // +0x50
    float BoastMaxFrames;                       // +0x54
};
template<class T> struct CDefPointer {
    T* P;
    CDefPointer() : P(0) { }
    ~CDefPointer() { T* p = P; if (p) { if (--p->RefCount == 0) p->DeleteThis(); } }
};
struct CDefClassBase {
    void GetSubDef(CDefPointer<CRockTrollDef>* out);   // 0x008C78F0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PPhysics;                   // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefClassBase* PDefClass;                   // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
};
struct CThingCreatureBase : CThing {
    bool IsPerformingAnyAction();               // 0x006622A0
    void SetLastActionHandedness(long hand);    // 0x006619F0
};
struct CTCCombo {                               // interface 0x30
    void Reset();                               // 0x00733210
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {
    CIntelligentPointerConstThing(const CIntelligentPointerCreature& o);   // 0x0091F990
};
struct CCombatActionBase {
    unsigned char _data[0x114];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_PlayCombatAnimation : CCombatActionBase {
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& anim, bool once);   // 0x008B7FF0
};
struct CCombatAction_StabThing : CCombatActionBase {
    CCombatAction_StabThing(CThingCreatureBase* creature, CThing* target);   // 0x0094E5F0
};
struct CWorld;
long __fastcall GetWorldFrame(CWorld* world);   // 0x0049D870
struct CAIBrain {
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad[0x18 - 4];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsStopping();                          // 0x008FD010
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float speed);   // 0x0097D6F0
    long AIAction_PerformAction(const CCombatActionBase& action);   // 0x0097D8B0
    long AIAction_PerformActionAndWait(const CCombatActionBase& action, bool b);   // 0x0097DB70
    long AIAction_DoNothing();                  // 0x0097E200
};
struct CAIStateGroup_TrollBase : CAIStateGroupBase {
    CIntelligentPointerCreature PEnemy;         // +0x14
    long EnemyKnockDownFrame;                   // +0x1c
    long ShotCounter;                           // +0x20
    float TROLL_MAX_THROWING_DISTANCE;          // +0x24
    float TROLL_MIN_THROWING_DISTANCE;          // +0x28
    float TROLL_STOP_POUNDING_DISTANCE;         // +0x2c
    float TROLL_STOP_THROWING_DISTANCE;         // +0x30
    C3DVector GetEnemyPredictedPos();           // 0x0094D680
    bool HasEnemyBeenKnockedDown();             // 0x0094D9C0
    void ForceRiseUp();                         // 0x0094DBC0
    long ThrowRocks();                          // 0x0094DC40
};
struct CAIStateGroup_RockTroll : CAIStateGroup_TrollBase {
    bool IsStillRunnable();                     // 0x0094D930
    virtual void Update();
};

void CAIStateGroup_RockTroll::Update()
{
    ForceRiseUp();
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CDefPointer<CRockTrollDef> def;
    GetCreature()->PDefClass->GetSubDef(&def);
    bool knockedDown = false;
    if (!PEnemy.GetPItem() || (PEnemy.GetPItem()->m_flags91 & 1)) {
        Terminate();
    } else {
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }
        const C3DVector& myPos = GetCreature()->GetPosition();
        C3DVector offset = GetEnemyPredictedPos() - myPos;
        float dist = offset.Length2D();
        if (dist < TROLL_MIN_THROWING_DISTANCE) {
            C3DVector dir = Normalise(offset);
            float facing = DotProduct(GetCreature()->PPhysics->GetForward(), dir);
            if (facing < 0.5f) {
                CCharString anim("LOOK_BACKWARD");
                AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), anim, true), false);
            } else {
                CCharString anim("LOOK_FORWARD");
                AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), anim, true), false);
            }
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            const C3DVector& myPos2 = GetCreature()->GetPosition();
            offset = GetEnemyPredictedPos() - myPos2;
            dist = offset.Length2D();
            if (dist < TROLL_MIN_THROWING_DISTANCE) {
                AIAction_PerformAction(CCombatAction_StabThing(GetCreature(), PEnemy.GetPItem()));
                while (GetCreature()->IsPerformingAnyAction()) {
                    if (!IsStillRunnable()) {
                        Terminate();
                        return;
                    }
                    if (HasEnemyBeenKnockedDown())
                        knockedDown = true;
                    PBrain->Yield();
                }
            }
        } else {
            if (!(dist <= TROLL_MAX_THROWING_DISTANCE)) {
                Terminate();
                return;
            }
            AIAction_LookAtThing(1, PEnemy, 0x11, 0.2f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            ShotCounter = 0;
            long result;
            for (;;) {
                if (ShotCounter == 0) {
                    EnemyKnockDownFrame = GetWorldFrame(GetPWorld());
                    CTCCombo* combo;
                    GetCreature()->GetInterface(&combo, 0x30);
                    combo->Reset();
                    GetCreature()->SetLastActionHandedness(0);
                }
                result = ThrowRocks();
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                if (HasEnemyBeenKnockedDown())
                    knockedDown = true;
                if (ShotCounter == 4 && result == 2) {
                    CCharString anim("SPECIAL_IDLE");
                    CCreatureAction_PlayCombatAnimation idle(GetCreature(), anim, true);
                    while (GetCreature()->IsPerformingAnyAction()) {
                        if (!IsStillRunnable()) {
                            Terminate();
                            return;
                        }
                        if (HasEnemyBeenKnockedDown())
                            knockedDown = true;
                        PBrain->Yield();
                    }
                    ShotCounter = 0;
                    if (!knockedDown)
                        result = AIAction_PerformActionAndWait(idle, false);
                }
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                CThingCreatureBase* creature = GetCreature();
                CThing* enemy = PEnemy.GetPItem();
                const C3DVector& cp = creature->GetPosition();
                offset = enemy->GetPosition() - cp;
                dist = offset.Length2D();
                if (HasEnemyBeenKnockedDown())
                    knockedDown = true;
                if ((result == 4 || result == 2) && dist >= TROLL_MIN_THROWING_DISTANCE && dist <= TROLL_MAX_THROWING_DISTANCE && !knockedDown)
                    continue;
                break;
            }
            if (dist > TROLL_MAX_THROWING_DISTANCE) {
                Terminate();
                return;
            }
        }
        while (GetCreature()->IsPerformingAnyAction()) {
            AIAction_DoNothing();
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
        }
        long elapsed = GetWorldFrame(GetPWorld()) - EnemyKnockDownFrame;
        if (!IsStopping() && dist >= def.P->BoastMinDistance && knockedDown && elapsed < def.P->BoastMaxFrames) {
            CCharString anim("SPECIAL_BOAST");
            AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), anim, true), false);
            EnemyKnockDownFrame = GetWorldFrame(GetPWorld());
        }
    }
    if (IsStopping())
        Terminate();
}
