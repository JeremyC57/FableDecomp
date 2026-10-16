// CThing::GetChecksum() const -- sync checksum: type + player + def index, plus the physics component's two vectors when present
// Leaf: ?GetChecksum@CThing
// Grade: DIFFER(106v108) -- only the PhysicsTC register: retail loads it into edx, tests edx and moves it to ecx (`mov ecx,edx`) before the inline sum; ours loads ecx directly (2 bytes short)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7DA0, 108 bytes (+4 int3). __thiscall virtual, tail-jumps to __ftol2 0x00BFEA70.
// Class: catalog "CThing::vfunc_23" -- slot 23 of CThing's vtable 0x01239ABC; the PDB virtual ?GetChecksum@CThing@@UBEKXZ
//        (returns unsigned long).
// Shape: a thing type flagged in the global systems' 20-byte type table (0x013B8A1C +4, byte +9; descriptive name) returns 0;
//        else Type (+0x10) + PlayerNumber (+0x90, signed) + DefGlobalIndex (+0x8C); with a physics component (+0x60) the
//        unsigned sum (`fild` + 2^32 fix-up 0x0122DCB4) is added to Position.Z+Y+X+Velocity.Z+Y+X, accumulated with
//        `sum +=` statements (a single `a+b+...` expression is reordered by address by VC7.1), and converted back.
// Layout: retail CThing = Ego_r.pdb - 0x10 (PhysicsTC +0x60, DefGlobalIndex +0x8C, PlayerNumber +0x90); CTCPhysicsBase (PDB):
//         Position +0x0C, Velocity +0x28.

struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
    C3DVector Position;                          // +0x0c
    unsigned char _pad18[0x28 - 0x18];
    C3DVector Velocity;                          // +0x28
    float GetChecksum() const
    {
        float sum = Position.Z;
        sum += Position.Y;
        sum += Position.X;
        sum += Velocity.Z;
        sum += Velocity.Y;
        sum += Velocity.X;
        return sum;
    }
};
struct CThingTypeInfo {                          // 20-byte record per thing type
    unsigned char _pad00[9];
    bool ExcludeFromChecksum;                    // +0x09
    unsigned char _pad0a[0x14 - 0x0a];
};
struct CGlobalSystems {
    unsigned char _pad00[4];
    CThingTypeInfo* ThingTypes;                  // +0x04
};
extern CGlobalSystems* g_pGlobalSystems;         // 0x013B8A1C
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x10 - 0x04];
    unsigned char Type;                          // +0x10
    unsigned char _pad11[0x60 - 0x11];
    CTCPhysicsBase* PhysicsTC;                   // +0x60
    unsigned char _pad64[0x8c - 0x64];
    unsigned short DefGlobalIndex;               // +0x8c
    short CurrentMapNumber;                      // +0x8e
    signed char PlayerNumber;                    // +0x90
    virtual unsigned long GetChecksum() const;
};
unsigned long CThing::GetChecksum() const
{
    if (g_pGlobalSystems->ThingTypes[Type].ExcludeFromChecksum)
        return 0;
    unsigned long checksum = Type + PlayerNumber + DefGlobalIndex;
    if (PhysicsTC) {
        checksum = (unsigned long)(checksum + PhysicsTC->GetChecksum());
    }
    return checksum;
}
