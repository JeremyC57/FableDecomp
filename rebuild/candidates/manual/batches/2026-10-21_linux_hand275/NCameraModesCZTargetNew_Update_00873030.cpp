// NCameraModes::CZTargetNew::Update() -- refresh the coordinate system, place the camera, look at the observed thing's focal point and pull the look-at towards a live target's firing point
// Leaf: ?Update@CZTargetNew
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873030, 293 bytes (+ int3 padding), plain `ret`.
// Callees: virtual slot 24 UpdateCoordSystem (+0x60, 0x00872EC0), CZTargetNew::GetCameraPosition() 0x00872AF0 (C3DVector
//   by hidden pointer), NCameraModes::CBase::GetObservedThing() const 0x0084F1B0, CTCGraphicAppearance::GetCameraFocalPos(float)
//   0x004C12D0 (hidden pointer), the const CVectorMap::LowerBound 0x004365B0 (interface lookup, key 8 = the targeting TC),
//   the targeting component's virtual slot 36 (+0x90) GetTargetedThing, NCameraModes::CBase::GetWorld() const 0x0084D050,
//   NTargeting::GetTargetingPositionOnThing 0x007EA2F0 (`__fastcall`, hidden result in ecx, world in edx, as the landed
//   CProjectileWeapon::GetPhiForTargetedThing 0x008798C0).
// Layout (retail NCameraModes::CBase is 0xA4 bytes, PDB 0xC8; see the landed CZTargetNew::OnInit 0x00872A20): LookAt
//   CPhysicsState +0x10 (position first), CameraManager +0x40, PCameraModeDef +0x44 (CCameraModeDef::ZTargetLookAtTargetOffset at +0x68 is the
//   look-at blend towards the target), GoTo CPhysicsState +0x54 (position first). CThing: TC bit mask +0x20, interface
//   map +0x44, graphic appearance TC +0x64, dead bit +0x91 bit 0.
// Shape: the appearance is reached through an inline CThing getter in the call chain (a named pointer local or the bare
//   member chain moves the load to eax or below the pushes); HasTC is a forced-inline `unsigned long` mask test (`test ch,1`); the component is the one-ternary entry pick over a template constant key
//   loaded straight into ecx; the blend is a named float read first, then `lookAt += (aim - lookAt) * blend` through inline C3DVector operators; the two
//   positions are stored through the implicit operator= (`lea ecx,[esi+54h]` copy, then `add esi,10h`).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};

struct CThing;
struct CWorld;

struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual const CThing* GetTargetedThing() const;     // slot 36 (+0x90)
};

struct CTCGraphicAppearance {
    C3DVector GetCameraFocalPos(float heightFraction) const;    // 0x004C12D0
};

struct SInterfaceEntry { long type; CTCBase* component; };
struct CInterfaceMap {                                  // CVectorMap<long, CTCBase*>, 12 bytes
    SInterfaceEntry* _M_start; SInterfaceEntry* _M_finish; SInterfaceEntry* _M_end_of_storage;
    const SInterfaceEntry* LowerBound(const long& key) const;   // 0x004365B0
    const SInterfaceEntry* end() const { return _M_finish; }
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long TCFlags;                              // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap Interfaces;                           // +0x44
    unsigned char _pad50[0x14];
    CTCGraphicAppearance* PGraphicAppearance;           // +0x64
    unsigned char _pad68[0x29];
    unsigned char Dead : 1;                             // +0x91 bit 0
    const CTCGraphicAppearance* GetGraphicAppearance() const { return PGraphicAppearance; }
    __forceinline unsigned long HasTC(long type) const { return TCFlags & (1 << type); }
    template<long TYPE> __forceinline CTCBase* GetTC() const
    {
        long type = TYPE;
        const CInterfaceMap* map = &Interfaces;
        const SInterfaceEntry* it = map->LowerBound(type);
        const SInterfaceEntry* last = map->end();
        return (it == last || TYPE < it->type ? last : it)->component;
    }
};

template<class T> inline bool IS_THING_ALIVE(T* thing) { return thing != 0 && !thing->Dead; }

namespace NTargeting {
C3DVector __fastcall GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float height);   // 0x007EA2F0
}

struct CCameraModeDef {
    unsigned char _pad[0x68];
    float ZTargetLookAtTargetOffset;                    // +0x68 (Ego_r.pdb +0x6C; retail defs sit 4 below)
};
struct CPhysicsState {                                  // retail 0x30 bytes
    C3DVector Position;                                 // +0x00
    unsigned char _rest[0x24];
};

namespace NCameraModes {
class CBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void UpdateCoordSystem();                   // slot 24 (+0x60)
    float DefaultFOV;                                   // +0x04
    float DefaultRollAngle;                             // +0x08
    float DefaultObservedPositionZOffset;               // +0x0c
    CPhysicsState LookAt;                               // +0x10
    void* CameraManager;                                // +0x40
    const CCameraModeDef* PCameraModeDef;               // +0x44 (CDefPointer data)
    void* PCameraModeDefInfo_SubComponents[3];          // +0x48 SubComponents vector
    CPhysicsState GoTo;                                 // +0x54
    unsigned char _pad84[0x20];
    const CThing* GetObservedThing() const;             // 0x0084F1B0
    const CWorld& GetWorld() const;                     // 0x0084D050
};
class CZTargetNew : public CBase {
public:
    C3DVector GetCameraPosition();                      // 0x00872AF0
    void Update();
};
}

void NCameraModes::CZTargetNew::Update()
{
    UpdateCoordSystem();
    C3DVector position = GetCameraPosition();
    C3DVector lookAt = GetObservedThing()->GetGraphicAppearance()->GetCameraFocalPos(1.0f);
    const CThing* thing = GetObservedThing();
    if (thing->HasTC(8)) {
        const CThing* target = thing->GetTC<8>()->GetTargetedThing();
        if (IS_THING_ALIVE(target)) {
            C3DVector aim = NTargeting::GetTargetingPositionOnThing(GetWorld(), *target, 1.0f);
            float blend = PCameraModeDef->ZTargetLookAtTargetOffset;
            lookAt += (aim - lookAt) * blend;
        }
    }
    GoTo.Position = position;
    LookAt.Position = lookAt;
}
