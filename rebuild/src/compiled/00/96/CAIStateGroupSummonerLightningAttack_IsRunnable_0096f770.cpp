// CAIStateGroup_SummonerLightningAttack::IsRunnable() -- latch a live enemy as the lightning target when it is in line of sight and beyond the def's attack range, or (once the preemptive frame is reached) by a random percentage roll against the def's chances; otherwise schedule the next preemptive check
//   SMatch out dies at the call (retail packs it with the IP temporaries and the LOS distance word: frame 0x44); the world is a
//   named local before the camera-manager call (this straight into ecx); the from-vector reads PBrain->PCreature directly.
// Leaf: IsRunnable
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class C3DVector { public:
    float x, y, z;
    C3DVector() {}
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};
class C2DVector { public:
    float x, y;
    C2DVector(float ax, float ay) : x(ax), y(ay) {}
    float GetLengthSquared() const { return x * x + y * y; }
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceC(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCEnemySource {                         // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};
struct CTCLightningTarget {                     // interface 0x42
    struct SMatch { void* Object; void* Block; };
    bool CanBeTargeted(SMatch* out, CThing* const& attacker);   // 0x00970190
};
struct CTools {
    bool IsLineOfSightFiltered(const C3DVector* from, const C3DVector* to, C3DVector* hitPos, C3DVector* hitNormal,
        float* distance, CThing** hitThing, long test, bool flag);   // 0x00877C40
};
struct CCameraManager {
    unsigned char _pad[0x78];
    CTools Tools;                               // +0x78
};
class CGameCameraManager { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool GetCameraManager(CCameraManager** out);   // slot 10 (+0x28)
};
class CWorld { public:
    unsigned char _pad00[0x34];
    CGameCameraManager* PCameraManager;         // +0x34
    unsigned long* GetPRandomSeed();            // 0x0049D810
    static long GetWorldFrame();                // 0x0049D870
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
extern long g_FramesPerSecond;                  // 0x01375550
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // vptr 0x01238C6C
    virtual void v00();
    explicit __forceinline CIntelligentPointerCreature(CThing* thing) { SetPItem(thing); }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* thing);    // 0x004A96F0
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other);   // 0x004A9760
    CIntelligentPointerThing& operator=(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); return *this; }
};
class CSummonerDef { public:
    unsigned char _pad[0x28];
    float LightningRange;                       // +0x28
    float LightningChance;                      // +0x2c
    float LightningLOSChance;                   // +0x30
    float PreemptiveDelaySeconds;               // +0x34
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
};
struct CAIStateGroup_SummonerLightningAttack : CAIStateGroupBase {
    CIntelligentPointerThing PTarget;           // +0x14
    const CSummonerDef* PDef;                   // +0x1c CDefPointer<const CSummonerDef>
    long NextFrameForPreemptiveUse;             // +0x20
    CThing* GetCreature() const { return PBrain->PCreature; }
    __forceinline bool IsTargetable(CTCLightningTarget* target)
    {
        CTCLightningTarget::SMatch out;
        return target->CanBeTargeted(&out, GetCreature());
    }
    virtual bool IsRunnable();
};

bool CAIStateGroup_SummonerLightningAttack::IsRunnable()
{
    float range = PDef->LightningRange;
    CTCEnemySource* source;
    if (GetCreature()->GetInterfaceX(&source, 8)) {
        CThing* enemy = source->GetEnemy();
        if (enemy && !(enemy->m_flags91 & 1)) {
            CCameraManager* camera;
            CWorld* world = GetPWorld();
            if (world->PCameraManager->GetCameraManager(&camera)) {
                const C3DVector& e = enemy->GetPos();
                C3DVector to(e.x, e.y, e.z + 2.0f);
                const C3DVector& c = PBrain->PCreature->GetPos();
                C3DVector from(c.x, c.y, c.z + 2.0f);
                C3DVector hitPos, hitNormal;
                float distance;
                CThing* hitThing;
                if (!camera->Tools.IsLineOfSightFiltered(&from, &to, &hitPos, &hitNormal, &distance, &hitThing, 3, false))
                    return false;
            }
            {
                const C3DVector& from = GetCreature()->GetPos();
                const C3DVector& to = enemy->GetPos();
                float distanceSquared = C2DVector(to.x - from.x, to.y - from.y).GetLengthSquared();
                if (distanceSquared > range * range) {
                    PTarget = CIntelligentPointerCreature(enemy);
                    return true;
                }
                if (CWorld::GetWorldFrame() > NextFrameForPreemptiveUse) {
                    unsigned long* seed = GetPWorld()->GetPRandomSeed();
                    unsigned long next = *seed * 0x24A1U + 0x24DFU;
                    *seed = next;
                    unsigned long rotated = GFROR13(next);
                    *seed = rotated;
                    float percent = (long)(rotated % 1000) * 0.1f;
                    float losChance = PDef->LightningLOSChance;
                    if (percent < PDef->LightningChance) {
                        PTarget = enemy;
                        return true;
                    }
                    if (percent < losChance) {
                        CTCLightningTarget* target;
                        if (enemy->GetInterfaceC(&target, 0x42)) {
                            if (IsTargetable(target)) {
                                PTarget = enemy;
                                return true;
                            }
                        }
                    }
                    long fps = g_FramesPerSecond;
                    const CSummonerDef* def = PDef;
                    NextFrameForPreemptiveUse = (long)((float)CWorld::GetWorldFrame() + (float)fps * def->PreemptiveDelaySeconds);
                }
            }
        }
    }
    return false;
}