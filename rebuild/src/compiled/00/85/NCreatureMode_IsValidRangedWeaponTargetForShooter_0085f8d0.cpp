// NCreatureMode::IsValidRangedWeaponTargetForShooter(const CThing& shooter, const CThing& target, float angle) -- whether a ranged shot from the shooter can take the target: when the shooter's player is in lock-on mode its targeted thing must exist and be alive, and the direction to the target must lie within `angle` degrees of the shooter's facing
// Leaf: ?IsValidRangedWeaponTargetForShooter@NCreatureMode@@YI_NABVCThing@@0M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085F8D0, 304 bytes, /Gr __fastcall (shooter in ecx, target in edx, angle on the
//   stack), `ret 4`.
// Identity: Ego_r.pdb ?IsValidRangedWeaponTargetForShooter@NCreatureMode@@YI_NABVCThing@@0M@Z; caller
//   NCreatureMode::CModeStrafe::GetBestFrameInAimingAnim 0x008E9460 (landed, 45 degrees).
// Callees: CVectorMap::LowerBound const 0x004365B0 (the inline GetTC 8: CTCTargeting), CTCTargeting slot 36 (+0x90)
//   GetTargetedThing, CThing::PeekPlayer 0x004C7A10 (Ego_r ?PeekPlayer@CThing@@QBEABVCPlayer@@XZ; sweep3 RENAMES), CThing::GetPosition 0x004C73D0 (x2), the physics TC's slot 70
//   (+0x118) GetFacingVector (C3DVector by hidden pointer), C3DVector::AccurateNormalise @ 0x00A14440.
// Data: g_Epsilon 0x0129BA3C (1e-4f), 1/360 0x01238E00 (`/ 360.0f` as a reciprocal multiply), 2pi (double) 0x0124F2B8.
// Layout: CThing interface bitset +0x20, interface map +0x44, physics TC +0x60, flags +0x91 (bit 0 dead); CPlayer +0x20a
//   (the lock-on byte of the landed 0x0087C900 model; no PDB name, CPlayer is padding there).
// Shape: the targeted thing starts null and is fetched only when the shooter has interface 8; the alive test runs only
//   for a locked-on player; the offset target - shooter is normalised when its length^2 beats epsilon^2 (`fcompp`),
//   and `facing . offset < cos(angle / 360 * 2pi)` (fcos inline, compared as double: a (float) cast adds an fxch pair)
//   shares the early `return false` tail. The player is a named local (without it shooter and the targeted thing
//   swap esi/edi). Both dot products load z, y, x only with the dot written `X*o.X + (Y*o.Y + Z*o.Z)` and the length
//   as the self-dot; the flat sum reorders to z, x, y here (the order is context-dependent, cf. 0x00856040).
extern "C" double __cdecl cos(double);
#pragma intrinsic(cos)

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float operator*(const C3DVector& o) const { return X * o.X + (Y * o.Y + Z * o.Z); }   // dot product
    float GetLengthSquared() const { return *this * *this; }
    C3DVector& AccurateNormalise();                                   // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};

extern const float g_Epsilon;                           // 0x0129BA3C (1e-4f)

struct CInterfaceMap {                                  // CVectorMap<long, CTCBase*>
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
__declspec(noinline) const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing;
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacingVector() const;          // slot 70 (+0x118)
};
struct CTCTargeting {                                   // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual const CThing* GetTargetedThing() const;     // slot 36 (+0x90)
};
class CPlayer {
public:
    unsigned char _pad0[0x20a];
    unsigned char m_lockedOnTarget;                     // +0x20a (name descriptive)
};
class CThing {
public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad50[0x60 - 0x50];
    CTCPhysicsBase* PTCPhysics;                         // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    const CPlayer& PeekPlayer() const;                  // 0x004C7A10
    const C3DVector& GetPosition() const;               // 0x004C73D0
};

namespace NCreatureMode {
bool __fastcall IsValidRangedWeaponTargetForShooter(const CThing& shooter, const CThing& target, float angle)
{
    const CThing* targeted = 0;
    if (shooter.HasTC(8))
        targeted = shooter.GetTC<CTCTargeting>(8)->GetTargetedThing();
    const CPlayer& player = shooter.PeekPlayer();
    if (player.m_lockedOnTarget) {
        if (!targeted || (targeted->m_flags91 & 1))
            return false;
    }
    C3DVector offset = target.GetPosition() - shooter.GetPosition();
    C3DVector facing = shooter.PTCPhysics->GetFacingVector();
    if (offset.GetLengthSquared() > g_Epsilon * g_Epsilon)
        offset.AccurateNormalise();
    if (facing * offset < cos(angle / 360.0f * 6.2831854820251465))
        return false;
    return true;
}
}