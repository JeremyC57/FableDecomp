// CCombatant::ComputeDerivedCombatStats(CListenerList* listeners, CSoundSettings* settings, unsigned long dirtyFlags) -- (label is an autoname: the body is a 3D sound source's parameter update) merge the dirty flags, compute the position / velocity / direction relative to the nearest listener (or copy them when head-relative), then the distance, the cone / roll-off factors, and the clamped DirectSound volumes (-10000 floor), and publish the changed flags
// Grade: DIFFER(1469v1570) -- 81 blocks: first pass; retail rounds the four levels with a bare dword fistp through dead argument slots (inline-asm rounding helper, asm_bake family per GOTCHAS), ours calls __ftol2
// Leaf: ComputeDerivedCombatStats
// Retail 0x00C41FB0, 1584 bytes, `ret 0xc`.
// Callees: C3DVector normalise 0x00C402E0 (returns the length on the x87 stack); distance 0x00C418F0; attenuation
//   0x00C41990 / 0x00C41A20 / 0x00C41A70 / 0x00C41BB0; level-to-millibel 0x00C40F50 (`__cdecl`). Globals 0x0143BBE4
//   (listener X scale), 0x0143BBE0 (default level).
// Layout: local model from this function's use -- source {flags +0x0c / +0x10 / +0x14, relative position +0xfc,
//   velocity +0x108, direction +0x114, distance origin +0x120, distance +0x12c, factors +0x130..+0x138, levels
//   +0x13c..+0x150, position +0x158, velocity +0x164, direction +0x170, base levels +0x19c..+0x1a8, weights +0x1ac..+0x1c8,
//   callback +0x1d0 / +0x1d4, pending flags +0x1d8}; listener {next +8, position +0xc, velocity +0x18, axes +0x24..+0x44}.
extern float g_ListenerXScale;                         // 0x0143BBE4
extern long g_DefaultLevel;                            // 0x0143BBE0
long __cdecl LevelToMillibels(float level);            // 0x00C40F50
struct C3DVector {
    float X, Y, Z;
    float Normalise();                                 // 0x00C402E0
};
struct CListener {
    unsigned char _pad00[8];
    CListener*    Next;                                // +0x08
    C3DVector     Position;                            // +0x0c
    C3DVector     Velocity;                            // +0x18
    C3DVector     Axis[3];                             // +0x24
};
struct CListenerList {
    unsigned char _pad00[4];
    CListener*    First;                               // +0x04
};
struct CSoundEnvironment {
    unsigned char _pad00[0x68];
    long          Level;                               // +0x68
    unsigned char _pad6c[0x70 - 0x6c];
    long          Room;                                // +0x70
    long          RollOff;                             // +0x74
};
struct CSoundSettings {
    unsigned char      _pad00[4];
    CSoundEnvironment* Environment;                    // +0x04
};
struct CSoundSource;
typedef float (__cdecl* TAttenuationCallback)(CSoundSource* source, CListener* listener, float distance, void* user);
__forceinline long RoundLevel(float value)
{
    float biased = value - 0.5f;
    long result = (long)biased;
    if (result < 0)
        ++result;
    return result;
}
struct CSoundSource {
    unsigned char        _pad00[0x0c];
    unsigned long        Flags0c;                      // +0x0c
    unsigned long        Flags10;                      // +0x10
    unsigned long        ChangedFlags;                 // +0x14
    unsigned char        _pad18[0xfc - 0x18];
    C3DVector            RelativePosition;             // +0xfc
    C3DVector            RelativeVelocity;             // +0x108
    C3DVector            RelativeDirection;            // +0x114
    C3DVector            DistanceOrigin;               // +0x120
    float                Distance;                     // +0x12c
    float                Attenuation;                  // +0x130
    float                Cone;                         // +0x134
    float                Doppler;                      // +0x138
    long                 Level0;                       // +0x13c
    long                 Level1;                       // +0x140
    long                 Level2;                       // +0x144
    long                 Level3;                       // +0x148
    long                 Level4;                       // +0x14c
    long                 Level5;                       // +0x150
    unsigned char        _pad154[0x158 - 0x154];
    C3DVector            Position;                     // +0x158
    C3DVector            Velocity;                     // +0x164
    C3DVector            Direction;                    // +0x170
    unsigned char        _pad17c[0x19c - 0x17c];
    long                 BaseLevel0;                   // +0x19c
    long                 BaseLevel1;                   // +0x1a0
    long                 BaseLevel3;                   // +0x1a4
    long                 BaseLevel4;                   // +0x1a8
    long                 DirectLevel;                  // +0x1ac
    float                DirectMix;                    // +0x1b0
    long                 RoomLevel;                    // +0x1b4
    float                RoomMix;                      // +0x1b8
    long                 ReverbLevel;                  // +0x1bc
    float                ReverbMix;                    // +0x1c0
    unsigned char        _pad1c4[4];
    float                RollOffScale;                 // +0x1c8
    unsigned char        _pad1cc[4];
    TAttenuationCallback Callback;                     // +0x1d0
    void*                CallbackUser;                 // +0x1d4
    unsigned long        Pending;                      // +0x1d8
    float ComputeDistance(const C3DVector* origin, const C3DVector* relative);   // 0x00C418F0
    float ComputeAttenuation();                        // 0x00C41990
    float ComputeDoppler();                            // 0x00C41A20
    float ComputeCone();                               // 0x00C41A70
    float ComputeRoomAttenuation(long room);           // 0x00C41BB0
    void ComputeDerivedCombatStats(CListenerList* listeners, CSoundSettings* settings, unsigned long dirtyFlags);
};

void CSoundSource::ComputeDerivedCombatStats(CListenerList* listeners, CSoundSettings* settings, unsigned long dirtyFlags)
{
    unsigned long pending = Pending | dirtyFlags;
    Pending = pending;
    if (!pending)
        return;
    CListener* listener;
    if (Flags10 & 0x40000) {
        listener = 0;
        if (pending & 0x8000)
            RelativePosition = Position;
        if (Pending & 0x10000)
            RelativeVelocity = Velocity;
        if (Pending & 0x80000) {
            Direction.Normalise();
            RelativeDirection = Direction;
        }
    } else {
        if (pending & 0x20000)
            Pending = pending | 0x8000;
        CListener* nearest = listeners->First;
        C3DVector delta;
        delta.X = Position.X - nearest->Position.X;
        delta.Y = Position.Y - nearest->Position.Y;
        delta.Z = Position.Z - nearest->Position.Z;
        float bestSquared = delta.Z * delta.Z + delta.Y * delta.Y + delta.X * delta.X;
        for (CListener* other = nearest->Next; other; other = other->Next) {
            C3DVector d;
            d.X = Position.X - other->Position.X;
            d.Y = Position.Y - other->Position.Y;
            d.Z = Position.Z - other->Position.Z;
            float squared = d.Z * d.Z + d.Y * d.Y + d.X * d.X;
            if (squared < bestSquared) {
                bestSquared = squared;
                delta = d;
                nearest = other;
            }
        }
        listener = nearest;
        unsigned long flags = Pending;
        C3DVector right;
        if (flags & 0x88000) {
            right.X = nearest->Axis[2].X * g_ListenerXScale;
            right.Y = nearest->Axis[2].Y;
            right.Z = nearest->Axis[2].Z;
        }
        if (flags & 0x8000) {
            RelativePosition.X = right.Z * delta.Z + right.Y * delta.Y + delta.X * right.X;
            RelativePosition.Y = delta.Z * nearest->Axis[1].Z + delta.Y * nearest->Axis[1].Y + delta.X * nearest->Axis[1].X;
            RelativePosition.Z = delta.Z * nearest->Axis[0].Z + delta.Y * nearest->Axis[0].Y + delta.X * nearest->Axis[0].X;
        }
        if (flags & 0x10000) {
            RelativeVelocity.X = Velocity.X - nearest->Velocity.X;
            RelativeVelocity.Y = Velocity.Y - nearest->Velocity.Y;
            RelativeVelocity.Z = Velocity.Z - nearest->Velocity.Z;
        }
        if (Pending & 0x80000) {
            C3DVector& direction = Direction;
            direction.Normalise();
            RelativeDirection.X = right.Z * direction.Z + right.Y * direction.Y + right.X * direction.X;
            RelativeDirection.Y = nearest->Axis[1].Z * direction.Z + nearest->Axis[1].Y * direction.Y + nearest->Axis[1].X * direction.X;
            RelativeDirection.Z = nearest->Axis[0].Z * direction.Z + nearest->Axis[0].Y * direction.Y + nearest->Axis[0].X * direction.X;
        }
    }
    if (Pending & 0x8000) {
        Distance = ComputeDistance(&DistanceOrigin, &RelativePosition);
        Pending |= 0xd0000;
    }
    if (Pending & 0x40000) {
        TAttenuationCallback callback = Callback;
        float attenuation;
        if (!callback)
            attenuation = ComputeAttenuation();
        else
            attenuation = callback(this, listener, Distance, CallbackUser);
        Attenuation = attenuation;
        Pending |= 0x1800000;
    }
    unsigned long flags = Pending;
    if (flags & 0x10000)
        Doppler = ComputeDoppler();
    if (flags & 0x380000) {
        Cone = ComputeCone();
        Pending = flags | 0x200000;
    }
    if (Pending & 0x1400000) {
        Level0 = BaseLevel0;
        Level1 = BaseLevel1;
        Level2 = g_DefaultLevel;
        Pending |= 0x1400000;
    }
    if (!(Flags0c & 0x1000))
        Pending &= ~0x400000;
    if (!(Flags0c & 0x80))
        Pending &= ~0x1000000;
    if (Pending & 0x1000000) {
        CSoundEnvironment* environment = settings->Environment;
        if (environment) {
            float room = ComputeRoomAttenuation(environment->Room);
            Level0 += RoundLevel(DirectLevel * DirectMix + RoomLevel * RoomMix);
            if (Level0 < -10000)
                Level0 = -10000;
            Level1 += RoundLevel((1.0f - DirectMix) * DirectLevel + (1.0f - RoomMix) * RoomLevel +
                                 environment->RollOff * RollOffScale * Distance);
            if (Level1 < -10000)
                Level1 = -10000;
            Level2 = environment->Level;
            Level3 = BaseLevel3;
            Level3 += RoundLevel(RoomLevel * RoomMix + ReverbLevel * ReverbMix);
            if (room != 1.0f)
                Level3 += LevelToMillibels(room);
            if (Level3 < -10000)
                Level3 = -10000;
            else if (Level3 > 0)
                Level3 = 0;
            Level4 = BaseLevel4;
            Level4 += RoundLevel((1.0f - RoomMix) * RoomLevel + (1.0f - ReverbMix) * ReverbLevel);
            if (Level4 < -10000)
                Level4 = -10000;
            Level5 = environment->Level;
        }
    }
    if (!(Flags0c & 0x800000))
        Pending &= ~0x800000;
    ChangedFlags |= Pending;
    Pending = 0;
}
