// CTCLook::LookAtPos(const C3DVector& pos, unsigned long flags) -- start looking at a position: re-arm the look timer, store the flags, target and direction, set the head / body turn states (maybe starting a turning animation) and the recovery angles
// Grade: DIFFER(443v447) 25 difflines (was 46): frame 0xc + lengthSq in the dead pos slot + pos in ebp now match retail (lengthSquared written as two GetLengthSquared() calls + memberwise PosToLookAt copy); residue: retail copies pos via mov edx,ebp + implicit struct copy (edi temp) and keeps &PosToLookAt in ebx for the last two calls (retry wave 2 sec2b)
// Was: DIFFER(447v447) -- lengthSq is not homed in the dead pos-argument slot (ours has its own slot, frame 0x10 vs 0xc) and pos lives in edi instead of ebp
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
    C3DVector     Position;                             // +0x0c
};
struct CTCCreatureNavigation {                          // interface 7
    bool IsCurrentlyNavigating() const;                 // 0x006CB8F0
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    const C3DVector& GetPosition() const;               // 0x004C73D0
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
struct CFrameSource {
    long GetFrameNumber();                              // 0x004167C8
};
bool __fastcall IsLookTurningOverridenByParent(CThing* thing);                          // 0x0085F200
float __fastcall AngleBetweenPositions(const C3DVector& from, const C3DVector& to);     // 0x00661540
float __fastcall AngleYZBetweenPositions(const C3DVector& from, const C3DVector& to);   // 0x00754E70
extern float g_epsilon_0129ba3c;                                                        // 0x0129BA3C (1e-4f)

#pragma pack(push, 1)
struct CTCLook {
    void*         __vftable;                            // +0x00
    CThing*       ParentThing;                          // +0x04
    unsigned char _pad08[0x1c - 0x08];
    float         RecoveryAngleToLookAtXY;              // +0x1c
    float         RecoveryAngleToLookAtYZ;              // +0x20
    unsigned char _pad24[0x3c - 0x24];
    long          LookTimer;                            // +0x3c
    bool          UsingOldStyleTurning;                 // +0x40
    unsigned char _pad41[0x48 - 0x41];
    long          HeadTurnState;                        // +0x48
    long          BodyTurnState;                        // +0x4c
    bool          CanTurnBody;                          // +0x50
    unsigned char _pad51[0x64 - 0x51];
    unsigned long LookFlags;                            // +0x64
    C3DVector     PosToLookAt;                          // +0x68
    C3DVector     DirectionToLookIn;                    // +0x74
    unsigned char _pad80[0xb4 - 0x80];
    long          TimeToStopLooking;                    // +0xb4 (PDB +0xb8)
    bool          OneFrameDelay;                        // +0xb8 (PDB +0xbc)
    CFrameSource* GetFrameSource();                     // 0x00686D60 (CTCBase)
    void ChangeHeadTurnState(long state)
    {
        if (HeadTurnState != state) {
            HeadTurnState = state;
            OneFrameDelay = true;
        }
    }
    void SetBodyTurnState(long state)
    {
        if (BodyTurnState != state)
            BodyTurnState = state;
    }
    bool IsAbleToTurnViaAnimation();                    // 0x00752480
    bool IsCurrentlyTurningViaAnimation();              // 0x00753B40
    bool IsTurningBody();                               // 0x00753020
    void SetupTurningAnimation(const C3DVector& pos, unsigned long flags);   // 0x00753630
    void LookAtPos(const C3DVector& pos, unsigned long flags);
};
#pragma pack(pop)

void CTCLook::LookAtPos(const C3DVector& pos, unsigned long flags)
{
    C3DVector direction = pos - ParentThing->GetPosition();
    if (direction.GetLengthSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c) {
        LookTimer = GetFrameSource()->GetFrameNumber() + 90;
        if (IsLookTurningOverridenByParent(ParentThing) && (flags & 1))
            flags ^= 1;
        LookFlags = flags;
        PosToLookAt.X = pos.X; PosToLookAt.Y = pos.Y; PosToLookAt.Z = pos.Z;
        DirectionToLookIn = direction;
        UsingOldStyleTurning = true;
        if (flags & 2)
            ChangeHeadTurnState(2);
        else
            ChangeHeadTurnState(1);
        TimeToStopLooking = -1;
        if (flags & 1) {
            SetBodyTurnState(2);
            CanTurnBody = true;
            if (IsAbleToTurnViaAnimation() && direction.GetLengthSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c) {
                CTCCreatureNavigation* navigation;
                if (!ParentThing->GetInterface(&navigation, 7) || !navigation->IsCurrentlyNavigating()) {
                    if (!IsCurrentlyTurningViaAnimation() && IsTurningBody())
                        SetupTurningAnimation(pos, flags);
                }
            }
        } else
            SetBodyTurnState(1);
        CThing* parent = ParentThing;
        RecoveryAngleToLookAtXY = AngleBetweenPositions(parent->PhysicsTC->Position, PosToLookAt);
        RecoveryAngleToLookAtYZ = AngleYZBetweenPositions(parent->PhysicsTC->Position, PosToLookAt);
    }
}
