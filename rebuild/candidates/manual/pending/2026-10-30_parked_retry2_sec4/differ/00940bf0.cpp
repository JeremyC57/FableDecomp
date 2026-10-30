// CAIStateGroup_KidWatchHopscotch::Update() -- for up to 30 seconds: walk back to the hopscotch board when more than two radii away; otherwise step out of the board's inner ring, wander around it every 2..10 seconds, and watch the creature currently playing the board's hopscotch event (else turn to look at the board); terminate on any failed action
// Grade: DIFFER(1520v1563) nd=358 (was nd=406) -- C2DVector::Length with a named length-squared (fsqrt after the temp pops, y-first ctor order kept); residue: loop-top register roles (&board edi / &centre ebx), frame slot +4, directed-move branch scheduling, event-search compares
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00940BF0, length 1563 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPThingWorld @ 0x008FCFA0, CWorld::GetWorldFrame @ 0x0049D870, GetPWorldSeed @ 0x008FCFD0,
//          __ftol2 @ 0x00BFEA70, GFROR13 @ 0x00497890, GFFloatRandom @ 0x004978A0, CBaseIntelligentPointer::GetPItem
//          @ 0x00A01B50 / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 (CIntelligentPointer<const CThing> vtable 0x01237F1C),
//          CThing::GetPosition @ 0x004C73D0, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 7 =
//          CTCCreatureNavigation), CTCCreatureNavigation::GetNearbyDirectedPosition @ 0x006CB780 / GetNearbyWanderPosition
//          @ 0x006CB6E0, AIAction_MoveToPosProximity @ 0x0097E890, AIAction_MoveToPos @ 0x0097EC00, AIAction_LookAtThing
//          @ 0x0097D6F0, AIAction_LookAtPos @ 0x0097D580, AIAction_DoNothing @ 0x0097E200, CThingCreatureBase::CanSeeTarget
//          @ 0x006650E0, the creature's vtable slot 92 (+0x170, field of view), Terminate @ 0x008FCF40.
// Globals: frames per second @ 0x01375550, the fast inverse-sqrt mantissa table @ 0x013CE558, 8.0f @ 0x01292724,
//          1.2f @ 0x0129272C, 0.25f @ 0x01292728, 0.8f @ 0x01292734, 1/360 @ 0x01238E00, 30.0f @ 0x0129271C,
//          3.0f @ 0x0122DED4, 0.5f @ 0x0122F59C, 2^32 @ 0x0122DCB4.
// Layout: local model from Ego_r.pdb CAIStateGroup_KidWatchHopscotch (no engine header): PHopscotchBoard +0x14,
//          HopscotchCentre +0x1c, HopscotchRadius +0x28, StartFrame +0x30.
struct CTCBase;
struct CThing;
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float MagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline C3DVector operator*(const C3DVector& v, const float& s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
__forceinline C3DVector Normalise(const C3DVector& v) { return v * FastInvSqrt(v.MagnitudeSquared()); }
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float LengthSquared() const { return X * X + Y * Y; }
    float Length() const { float lengthSquared = LengthSquared(); return (float)sqrt(lengthSquared); }
};
__forceinline float Distance2D(const C3DVector& a, const C3DVector& b) { return C2DVector(a.X - b.X, a.Y - b.Y).Length(); }

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
enum EThingType { THING_TYPE_CREATURE = 31 };
struct CThing {
    virtual void v00();
    unsigned char _pad04[0x18 - 0x04];
    long UID[2];                                // +0x18 the thing's unique id (descriptive)
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    EThingType TypeBits : 5;                    // +0x6c thing type (31: creature)
    unsigned long _otherBits : 27;
    unsigned char _pad70[0x91 - 0x70];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
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
};
struct CThingCreatureBase : CThing {
    bool CanSeeTarget(const C3DVector& position, float fieldOfView, bool checkRange, bool checkObstruction);   // 0x006650E0
};
struct CCreatureVirtuals {                      // the creature's vtable up to slot 92
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6) VF10(7) VF10(8)
    VF(90) VF(91)
    virtual float GetFieldOfView();             // slot 92 (+0x170)
#undef VF10
#undef VF
};
struct CTCCreatureNavigation {                  // interface 7
    bool GetNearbyDirectedPosition(const C3DVector& from, float radius, const C3DVector& direction, float spread,
                                   C3DVector* out);     // 0x006CB780
    bool GetNearbyWanderPosition(const C3DVector& from, float radius, C3DVector* out, bool flag);   // 0x006CB6E0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) { SetPItem(item); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
#define IS_ALIVE(p) ((p) != 0 && !((p)->m_flags91 & 1))
inline CThingCreatureBase* ToCreature(const CBaseIntelligentPointer& p)
{
    return (p.GetPItem() && p.GetPItem()->TypeBits == THING_TYPE_CREATURE) ? (CThingCreatureBase*)p.GetPItem() : 0;
}
struct SGameEvent {                             // an entry of the world's event list (descriptive)
    unsigned char _pad00[0x08];
    long Type;                                  // +0x08 (0x18: a hopscotch game)
    unsigned char _pad0c[0x28 - 0x0c];
    long ThingUID[2];                           // +0x28 the board's unique id
    unsigned char _pad30[0x48 - 0x30];
    CBaseIntelligentPointer PPlayer;            // +0x48 the creature playing
};
struct SEventNode {
    SEventNode* Next;
    SEventNode* Prev;
    SGameEvent Data;
};
struct CEventList {
    unsigned char _pad[4];
    SEventNode* Head;                           // +0x04 STLport list sentinel
};
struct CWorld {
    unsigned char _pad[0x60];
    CEventList* Events;                         // +0x60
    static long GetWorldFrame();                // 0x0049D870
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
extern long g_GameFramesPerSecond;              // 0x01375550
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPThingWorld();                   // 0x008FCFA0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
    unsigned long RandomFrames(long framesPerSecond)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long r = (unsigned long)(framesPerSecond * 8.0f);
        *seed = *seed * 0x24A1 + 0x24DF;
        *seed = GFROR13(*seed);
        return r ? *seed % r : 0;
    }
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
                                     bool a, bool b, bool c, CThing* leader, bool d, float e);   // 0x0097E890
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
                            CThing* avoid, bool c, float speed);    // 0x0097EC00
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float turnSpeed);   // 0x0097D6F0
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float turnSpeed);   // 0x0097D580
    long AIAction_DoNothing();                  // 0x0097E200
};
struct CAIStateGroup_KidWatchHopscotch : CAIStateGroupBase {
    CBaseIntelligentPointer PHopscotchBoard;    // +0x14
    C3DVector HopscotchCentre;                  // +0x1c
    float HopscotchRadius;                      // +0x28
    long CreationFrame;                         // +0x2c
    long StartFrame;                            // +0x30
    virtual void Update();
};

void CAIStateGroup_KidWatchHopscotch::Update()
{
    long lastWander = GetPThingWorld()->GetWorldFrame();
    long wanderDelay = (long)(g_GameFramesPerSecond * 2.0f + RandomFrames(g_GameFramesPerSecond));
    do {
        if (!IS_THING_ALIVE(PHopscotchBoard))
            break;
        long result;
        if (Distance2D(GetCreature()->GetPosition(), HopscotchCentre) > HopscotchRadius * 2.0f) {
            bool reached;
            CThing* board = PHopscotchBoard.GetPItem();
            result = AIAction_MoveToPosProximity(1, board->GetPosition(), HopscotchRadius * 1.2f + 0.25f,
                                                 &reached, 5, true, false, true, 0, true, 0.0f);
        } else {
            if (Distance2D(GetCreature()->GetPosition(), HopscotchCentre) < HopscotchRadius * 1.2f) {
                CTCCreatureNavigation* navigation;
                GetCreature()->GetInterfaceT(&navigation, 7);
                C3DVector delta = GetCreature()->GetPosition() - HopscotchCentre;
                float inverse = FastInvSqrt(delta.X * delta.X + delta.Y * delta.Y + delta.Z * delta.Z);
                C3DVector direction;
                direction.X = delta.X * inverse;
                direction.Y = delta.Y * inverse;
                direction.Z = delta.Z * inverse;
                C3DVector target;
                if (navigation->GetNearbyDirectedPosition(GetCreature()->GetPosition(), HopscotchRadius * 1.2f, direction,
                                                          1.0f / 12.0f, &target)) {
                    bool reached;
                    if (AIAction_MoveToPos(1, target, &reached, 5, true, true, 0, true, 0.0f) != 2)
                        break;
                }
            }
            if (GetPThingWorld()->GetWorldFrame() - lastWander > wanderDelay) {
                CTCCreatureNavigation* navigation;
                GetCreature()->GetInterfaceT(&navigation, 7);
                C3DVector target;
                if (navigation->GetNearbyWanderPosition(HopscotchCentre,
                        HopscotchRadius * 1.2f + GFFloatRandom(HopscotchRadius * 0.8f, GetPWorldSeed()), &target, false)) {
                    bool reached;
                    if (AIAction_MoveToPos(1, target, &reached, 5, true, true, 0, true, 0.0f) != 2)
                        break;
                    lastWander = GetPThingWorld()->GetWorldFrame();
                    wanderDelay = (long)(g_GameFramesPerSecond * 2.0f + RandomFrames(g_GameFramesPerSecond));
                }
            }
            CThing* board = PHopscotchBoard.GetPItem();
            long uid0 = board->UID[0];
            long uid1 = board->UID[1];
            CEventList* events = GetPThingWorld()->Events;
            SEventNode* node;
            for (node = events->Head->Next; node != events->Head; node = node->Next) {
                if (node->Data.Type == 0x18 && node->Data.ThingUID[0] == uid0 && node->Data.ThingUID[1] == uid1)
                    break;
            }
            if (node != events->Head && ToCreature(node->Data.PPlayer) && IS_ALIVE(node->Data.PPlayer.GetPItem()) &&
                Distance2D(ToCreature(node->Data.PPlayer)->GetPosition(), HopscotchCentre) < HopscotchRadius * 2.0f) {
                result = AIAction_LookAtThing(1, CIntelligentPointerConstThing(ToCreature(node->Data.PPlayer)), 1, 0.0138888890f);
            } else {
                CThingCreatureBase* creature = GetCreature();
                CThing* boardThing = PHopscotchBoard.GetPItem();
                if (GetCreature()->CanSeeTarget(boardThing->GetPosition(),
                        ((CCreatureVirtuals*)creature)->GetFieldOfView() * (1.0f / 360.0f), false, false))
                    result = AIAction_DoNothing();
                else
                    result = AIAction_LookAtPos(1, PHopscotchBoard.GetPItem()->GetPosition(), 1, 0.0138888890f);
            }
        }
        if (result != 2)
            break;
    } while ((float)(GetPThingWorld()->GetWorldFrame() - StartFrame) < g_GameFramesPerSecond * 30.0f);
    Terminate();
}
