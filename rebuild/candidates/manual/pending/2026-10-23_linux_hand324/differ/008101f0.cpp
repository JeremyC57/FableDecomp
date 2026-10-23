// CTCDExplosiveTrail::FrameUpdate() -- while the trail is alive, every spawn interval lay the next trap a step further along the trail (steering towards the target thing within a turn limit, wobbling randomly, snapped to the ground and stopping at impassable/too-high ground), make its trap immune to the immune things and trigger it, and remember it; once the trail has ended and its list is empty kill the trail; then tick the laid traps (kill finished ones, count down, reset when the timer reaches zero) and drop dead entries
// Grade: DIFFER(1608v1914) -- first draft: the steering block (acos/tanh, inline vector temporaries, x87 compare chains) is approximate; head (timers), trap creation/immunity/trigger and the trail tick follow retail
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008101F0, 1920 bytes, plain `ret`, no EH.
// Callees: __ftol2 0x00BFEA70, CThing::GetPosition 0x004C73D0, CThing physics vslot 70 (+0x118 GetVelocity), GetPItem 0x00A01B50,
// IS_THING_ALIVE 0x0040DCE0 / 0x004CC340, C3DVector::Normalise 0x00A14440 / Rotate 0x00A13C60, acos 0x00BFEBA2, tanh 0x00BFECE2,
// CTCBase::GetWorld 0x00686A70, CWorld::GetPRandomSeed 0x0049D810 / GetWorldMap (slot 12), GFFloatRandom 0x004978A0,
// CWorldMap::GetHeightZBelowPos 0x0063E480 / GetGroundSizeZAt 0x0063A300 / IsPosCameraPassable 0x004FCC40,
// CThingObject::Create 0x00703210, RandomiseThingOrientationAroundDirection 0x0080FFB0, LowerBound 0x0040F020,
// CTCTrapBase::SetThingAsImmune 0x0078839A / ManualTrigger 0x00788388 / IsFinished 0x00787C23 / IsArmed 0x00787C1B /
// ManualReset 0x00787C0C, make_pair 0x0080E200, pair conversion 0x0080DE90, list push_back 0x00810D40, CThing::Kill 0x004C9B80,
// CIntelligentPointer ctor 0x004A96F0 / dtor 0x00A01C10, free 0x00BFEA14.
// Shape note: first draft -- the steering block (0x0081039A..0x008106B5) is an x87 sequence with acos/tanh and inline
// vector temporaries that this draft only approximates.
#include <math.h>
extern "C" void __cdecl free(void* p);                       // 0x00BFEA14
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise();                                        // 0x00A14440
    C3DVector Rotate(const C3DVector& axis, float angle) const;   // 0x00A13C60
    float MagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};
extern long g_FramesPerSecond;                               // 0x01375550
extern const float g_Epsilon;                                // 0x0129BA3C (1e-4f)
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
struct CPhysics {
    virtual void GetVelocity(C3DVector* out);                // stand-in: slot 70 (+0x118)
};
struct CThing {
    unsigned char _pad00[0x2c];
    unsigned long m_presentBits3;                            // +0x2c (interface word 3)
    unsigned char _pad30[0x44 - 0x30];
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysics* Physics;                                       // +0x60
    unsigned char _pad64[0x90 - 0x64];
    signed char Region;                                      // +0x90
    unsigned char Flags91;                                   // +0x91 bit 0 Dead
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    void Kill(bool immediately);                             // 0x004C9B80
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CRightHandedSet;
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set,
                                     unsigned long flags, const CCharString& name);   // 0x00703210
void __fastcall RandomiseThingOrientationAroundDirection(CThing* thing, const C3DVector& direction);   // 0x0080FFB0
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                              // 0x00A01C10
    CThing* GetPItem() const;                                // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* item);                  // 0x004A96F0
};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);       // 0x0040DCE0
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer& p);   // 0x004CC340
struct STrailEntry {
    CIntelligentPointerThing first;
    long second;
};
struct SIntEntry { CIntelligentPointerThing first; int second; };
SIntEntry __cdecl make_pair(const CIntelligentPointerThing& a, const int& b);   // 0x0080E200
struct STrailConverted : STrailEntry {
    STrailConverted(const SIntEntry& other);                 // 0x0080DE90
};
struct STrailNode {
    STrailNode* Next;
    STrailNode* Prev;
    STrailEntry Value;
};
struct CTrailList {
    STrailNode* Head;
    void push_back(const STrailEntry& value);                // 0x00810D40
};
struct CTCTrapBase {
    void SetThingAsImmune(CThing* thing);                    // 0x0078839A
    void ManualTrigger();                                    // 0x00788388
    bool IsFinished() const;                                 // 0x00787C23
    bool IsArmed() const;                                    // 0x00787C1B
    void ManualReset();                                      // 0x00787C0C
};
struct CWorldMap {
    float GetHeightZBelowPos(const C3DVector& pos) const;    // 0x0063E480
    float GetGroundSizeZAt(const C3DVector& pos) const;      // 0x0063A300
    bool IsPosCameraPassable(const C3DVector& pos) const;    // 0x004FCC40
};
struct CWorld {
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11)
#undef VF
    virtual CWorldMap* GetWorldMap();                        // slot 12 (+0x30)
    unsigned long* GetPRandomSeed();                         // 0x0049D810
};
struct CExplosiveTrailDef {
    unsigned char _pad00[0x28];
    long  TrapDefIndex;                                      // +0x28
    float SpawnInterval;                                     // +0x2c
    float StepLength;                                        // +0x30
    float TrapResetTime;                                     // +0x34
    float Lifetime;                                          // +0x38
    float MaxStepHeight;                                     // +0x3c
    float MaxTurn;                                           // +0x40
    float Wobble;                                            // +0x44
    float Steering;                                          // +0x48
};
struct CImmuneVector {
    CBaseIntelligentPointer* _M_start;
    CBaseIntelligentPointer* _M_finish;
};
struct CTCDExplosiveTrail {
    virtual void v00();
    CThing* ParentThing;                                     // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CBaseIntelligentPointer Target;                          // +0x0c
    long FrameCount;                                         // +0x14
    bool Finished;                                           // +0x18
    CTrailList Trail;                                        // +0x1c
    CImmuneVector ImmuneThings;                              // +0x20
    unsigned char _pad28[0x2c - 0x28];
    const CExplosiveTrailDef* PDef;                          // +0x2c
    CWorld* GetWorld();                                      // 0x00686A70
    void FrameUpdate();
};

void CTCDExplosiveTrail::FrameUpdate()
{
    const CExplosiveTrailDef* def = PDef;
    float interval = def->SpawnInterval;
    float stepLength = def->StepLength;
    float resetTime = def->TrapResetTime;
    float lifetime = def->Lifetime;
    float maxStepHeight = def->MaxStepHeight;
    long frame = ++FrameCount;
    float framesPerSecond = (float)g_FramesPerSecond;
    if ((float)frame < framesPerSecond * lifetime && !Finished) {
        long step = (long)(framesPerSecond * interval);
        if (frame % step == 0) {
            C3DVector last = ParentThing->GetPosition();
            C3DVector direction;
            ParentThing->Physics->GetVelocity(&direction);
            STrailNode* head = Trail.Head;
            long count = 0;
            for (STrailNode* n = head->Next; n != head; n = n->Next)
                ++count;
            if (count > 0) {
                CThing* newest = head->Prev->Value.first.GetPItem();
                if (newest && !(newest->Flags91 & 1))
                    last = newest->GetPosition();
                if (count > 1) {
                    CThing* previous = head->Prev->Prev->Value.first.GetPItem();
                    if (IS_THING_ALIVE(previous)) {
                        const C3DVector& p = previous->GetPosition();
                        direction = C3DVector(last.X - p.X, last.Y - p.Y, 0.0f);
                        if (direction.MagnitudeSquared() > g_Epsilon * g_Epsilon)
                            direction.Normalise();
                    }
                }
            }
            float maxTurn = def->MaxTurn;
            float wobble = def->Wobble;
            float steering = def->Steering;
            if (Target.GetPItem() && !(Target.GetPItem()->Flags91 & 1) && steering > 0.0f) {
                const C3DVector& target = Target.GetPItem()->GetPosition();
                C3DVector toTarget(target.X - last.X, target.Y - last.Y, 0.0f);
                if (toTarget.MagnitudeSquared() > g_Epsilon * g_Epsilon)
                    toTarget.Normalise();
                float dot = toTarget.X * direction.X + toTarget.Y * direction.Y;
                float side = toTarget.X * direction.Y - toTarget.Y * direction.X;
                float sign = side < 0.0f ? -1.0f : 1.0f;
                if (dot > 1.0f)
                    dot = 1.0f;
                float angle = (float)acos(dot) * sign;
                if (angle > maxTurn)
                    angle = maxTurn;
                direction = direction.Rotate(C3DVector(0.0f, 0.0f, 1.0f), -angle);
            }
            float turn = (float)tanh(wobble) * GFFloatRandom(wobble, GetWorld()->GetPRandomSeed());
            direction = direction.Rotate(C3DVector(0.0f, 0.0f, 1.0f), turn);
            C3DVector next(last.X + direction.X * stepLength, last.Y + direction.Y * stepLength, last.Z);
            CWorldMap* map = GetWorld()->GetWorldMap();
            C3DVector probe(next.X, next.Y, 0.0f);
            next.Z = map->GetHeightZBelowPos(probe);
            probe = next;
            float ground = map->GetGroundSizeZAt(probe);
            if (maxStepHeight - ground <= g_Epsilon || !map->IsPosCameraPassable(next))
                Finished = true;
            if (!Finished && def->TrapDefIndex > 0) {
                next.Z = ground;
                CThing* trap = CreateThingObject(def->TrapDefIndex, &next, ParentThing->Region, 0, 0, "");
                if (trap) {
                    RandomiseThingOrientationAroundDirection(trap, direction);
                    if (trap->m_presentBits3 & 0x20000000) {
                        CTCTrapBase* trapTC = trap->GetTC<CTCTrapBase>(0x7d);
                        for (CBaseIntelligentPointer* it = ImmuneThings._M_start; it != ImmuneThings._M_finish; ++it) {
                            if (IS_THING_ALIVE(*it))
                                trapTC->SetThingAsImmune(it->GetPItem());
                        }
                        trapTC->ManualTrigger();
                    }
                    Trail.push_back(STrailConverted(make_pair(CIntelligentPointerThing(trap), -1)));
                }
            }
        }
    } else if (Trail.Head->Next == Trail.Head) {
        ParentThing->Kill(true);
    }
    STrailNode* node = Trail.Head->Next;
    while (node != Trail.Head) {
        CThing* thing = node->Value.first.GetPItem();
        if (thing && !(thing->Flags91 & 1) && (thing->m_presentBits3 & 0x20000000)) {
            CTCTrapBase* trap = thing->GetTC<CTCTrapBase>(0x7d);
            if (!trap->IsFinished()) {
                if (trap->IsArmed()) {
                    if (node->Value.second < 0)
                        node->Value.second = (long)(g_FramesPerSecond * resetTime);
                    else if (node->Value.second == 0)
                        trap->ManualReset();
                    else
                        node->Value.second--;
                }
                node = node->Next;
                continue;
            }
            thing->Kill(true);
        }
        STrailNode* next = node->Next;
        node->Prev->Next = next;
        next->Prev = node->Prev;
        node->Value.first.~CIntelligentPointerThing();
        free(node);
        node = next;
    }
}
