// Grade: DIFFER(834v833) -- shape aligned (loop, frame, wrap, normalise); remaining: Pillars[i] address formed with add instead of lea, the squared-length add order, the dead Z scale, and the SetPosition vcall register (6 tries)
// CAIStateGroup_JackOfBladesBase::SummonPillars() -- play the SUMMON_LOOP combat animation repeatedly; every def-interval of elapsed time raise the next pillar: enable its jack-pillar component, turn it to face the arena centre and drop it onto the ground just above the landscape; stop after the last pillar or when the action reports stopping (1)
// Layout: retail STLport vectors are 12 bytes, so Pillars is +0x40 (PDB header +0x44); PArenaCentre +0x1c.
#include <math.h>
#pragma intrinsic(atan2)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
extern long g_FramesPerSecond;                      // 0x01375550
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline void Normalise()
    {
        float lengthSquared = Z * Z + Y * Y + X * X;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
struct CCharString {
    void* PData;
    CCharString(const char* text, long length);  // 0x0099EBF0
    ~CCharString();                              // 0x0099EAE0
};
struct CJackDef {
    virtual void v00();
    virtual void DeleteThis();
    long RefCount;                              // +0x04
    unsigned char _pad08[0x44];
    float PillarInterval;                       // +0x4c
};
struct CDefClassBase {
    void GetSubDef(CJackDef** out);             // 0x008C33E0
};
#pragma auto_inline(off)
void CDefClassBase::GetSubDef(CJackDef** out) { *out = 0; }
#pragma auto_inline(on)
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCJackPillar {
    void Enable(bool enable);                   // 0x007317D0
};
struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30)
    virtual void SetPosition(const C3DVector& pos);   // slot 31 (+0x7c)
    VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF10(4) VF10(5) VF(60) VF(61) VF(62) VF(63) VF(64) VF(65)
    virtual void SetAngleXY(float angle);       // slot 66 (+0x108)
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* PhysicsBody;                  // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefClassBase* PDef;                        // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                      // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
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
struct CThingCreatureBase : CThing { };
struct CWorldMap {
    float GetHeightZBelowPos(const C3DVector& pos) const;   // 0x0063E480
};
struct CWorld {
    VF10(0) VF(10) VF(11) VF(12)
    virtual CWorldMap* GetWorldMap();           // slot 13 (+0x34)
    static long GetWorldFrame();                // 0x0049D870
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
template <class T> struct CVec {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long i) { return _M_start[i]; }
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x114];
};
struct CCombatActionBase : CCreatureActionBase {
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_PlayCombatAnimation : CCombatActionBase {
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& anim, bool loop);   // 0x008B7FF0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
};
struct CAIStateGroup_JackOfBladesBase : CAIStateGroupBase {
    CBaseIntelligentPointer PSwordEmitter;      // +0x14
    CBaseIntelligentPointer PArenaCentre;       // +0x1c
    CBaseIntelligentPointer PTargetThing;       // +0x24
    long EnemyKnockDownFrame;                   // +0x2c
    long SpeechCounter;                         // +0x30
    CVec<CBaseIntelligentPointer> PillarMarkers;   // +0x34
    CVec<CBaseIntelligentPointer> Pillars;      // +0x40
    void SummonPillars();
};

__forceinline float WrapUnit(float value)
{
    if (value < 0.0f || value >= 1.0f) {
        value = (float)fmod((double)value, 1.0);
        if (value < 0.0f)
            value += 1.0f;
    }
    return value;
}

void CAIStateGroup_JackOfBladesBase::SummonPillars()
{
    CJackDef* def = 0;
    GetCreature()->PDef->GetSubDef(&def);
    long count = Pillars.size();
    long index = 0;
    if (count == 0) {
        if (def && --def->RefCount == 0)
            def->DeleteThis();
        return;
    }
    float interval = def->PillarInterval;
    long lastFrame = 0;
    long result;
    do {
        long frame = GetPWorld()->GetWorldFrame();
        if ((float)((frame - lastFrame) * g_FramesPerSecond) > interval) {
            lastFrame = frame;
            CBaseIntelligentPointer& slot = Pillars[index];
            if (slot.GetPItem() && !(slot.GetPItem()->Flags91 & 1)) {
                CThing* pillar = Pillars[index].GetPItem();
                CTCJackPillar* jack;
                if (pillar->GetInterface(&jack, 0x25))
                    jack->Enable(true);
                if (PArenaCentre.GetPItem() && !(PArenaCentre.GetPItem()->Flags91 & 1)) {
                    CBaseIntelligentPointer& target = Pillars[index];
                    CThing* centre = PArenaCentre.GetPItem();
                    C3DVector facing = centre->GetPosition() - target.GetPItem()->GetPosition();
                    facing.Normalise();
                    CPhysicsBody* body = Pillars[index].GetPItem()->PhysicsBody;
                    body->SetAngleXY(WrapUnit((float)atan2(facing.X, facing.Y) * 0.15915494f));
                    C3DVector pos = Pillars[index].GetPItem()->GetPosition();
                    pos.Y = GetPWorld()->GetWorldMap()->GetHeightZBelowPos(pos) + 0.1f;
                    Pillars[index].GetPItem()->PhysicsBody->SetPosition(pos);
                }
                ++index;
            }
        }
        CCharString anim("SUMMON_LOOP", -1);
        result = AIAction_PerformAction(&CCreatureAction_PlayCombatAnimation(GetCreature(), anim, true));
    } while (index < count && result != 1);
    if (--def->RefCount == 0)
        def->DeleteThis();
}
