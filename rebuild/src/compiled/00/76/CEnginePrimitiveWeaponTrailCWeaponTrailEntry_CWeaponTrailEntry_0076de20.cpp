// CEnginePrimitiveWeaponTrail::CWeaponTrailEntry::CWeaponTrailEntry(const C3DVector& top, const C3DVector& bottom, float creationTime) -- one weapon-trail sample: the blade's top and bottom points and the time it was taken
// Leaf: ??0CWeaponTrailEntry@CEnginePrimitiveWeaponTrail@@QAE@ABVC3DVector@@0M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076DE20, 53 bytes (+11 int3 padding), thiscall, `ret 0Ch`.
// Identity: the Ego_r.pdb name set lists ??0CWeaponTrailEntry@CEnginePrimitiveWeaponTrail@@QAE@ABVC3DVector@@0M@Z (the
//   nested entry class of the weapon-trail primitive); callers 0x0076F08C, 0x0076F0E3, 0x0076F13F.
// Layout: Ego_r.pdb CWeaponTrailEntry (0x1c): Top +0x00, Bottom +0x0c, CreationTime +0x18. No generated engine header.
// Shape: `this` goes to eax first (the constructor's return value), each vector is copied word by word through edx/ecx
//   straight from the argument pointer: C3DVector needs a memberwise user copy constructor (the implicit copy forms a
//   `lea edx,[eax+0Ch]` destination pointer and an esi temporary); the float argument is copied as a dword.
class C3DVector {                                        // 12 bytes
public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry {
    public:
        CWeaponTrailEntry(const C3DVector& top, const C3DVector& bottom, float creationTime);
        C3DVector Top;                                   // +0x00
        C3DVector Bottom;                                // +0x0c
        float     CreationTime;                          // +0x18
    };
};

CEnginePrimitiveWeaponTrail::CWeaponTrailEntry::CWeaponTrailEntry(const C3DVector& top, const C3DVector& bottom, float creationTime)
    : Top(top), Bottom(bottom), CreationTime(creationTime)
{
}