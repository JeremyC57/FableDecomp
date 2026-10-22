// NCreatureMode::CModeStrafe::GetBestFrameInAimingAnim(float& frame) -- aim direction (camera forward when the player is first-person
//   targeting or locked on, else focus->target when the target is valid, else the driver's forward) -> acos of its normalised Z
//   as a fraction of a turn, clamped to [1/90, 151/360], mapped onto the aiming animation's frame range as a 0..1 fraction;
//   returns false when the direction is degenerate
// Leaf: GetBestFrameInAimingAnim
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E9460, 683 bytes (`ret 4`).
// Callees: CTCGraphicAppearance::GetDummyObject 0x004C08C0 ("WEAPON_FOCUS_01" 0x0126515C), CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   CVectorMap::LowerBound const 0x004365B0 (inline GetTC 8, unchecked miss path), driver slot 70 (+0x118, GetForward),
//   targeting slots 33 (+0x84, GetTarget) / 37 (+0x94, GetTargetPosition), IsValidRangedWeaponTargetForShooter 0x0085F8D0
//   (__fastcall, 45 degrees), CThing::GetPlayer 0x004C7A30, CPlayer::IsFirstPersonTargeting 0x004874D0, camera manager slot 10
//   (+0x28) via the engine singleton 0x013B86A0, _CIacos 0x00BFEBA2.
// Data: 1e-4 0x0129BA3C, 1.0 0x0122DED8, -1.0 0x0122DEE0, 1/(2pi) 0x01237F08, 1/90 0x0125D1E4, 151/360 0x01287CC0,
//   1/(151/360 - 1/90) 0x01287CBC, 0.0 0x0122DEDC.
// Shape: ParentThing is a named local only for the TC lookup and the forward; the target alive test is an inline helper (one
//   GetTarget call); the divisor is a second `(float)AimingNumFrames` (CSE temp + named local = retail's two float slots; one
//   named local lets VC7.1 reassociate the division).
extern "C" double __cdecl sqrt(double);
extern "C" double __cdecl acos(double);
#pragma intrinsic(sqrt, acos)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength2() const { return X * X + Y * Y + Z * Z; }
};
struct CRightHandedSet { C3DVector Up, Forward; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
extern const float g_epsilon_0129ba3c;                 // 0x0129BA3C (1e-4f)
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
struct CThing;
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CTCDriverBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetForward();                     // slot 70 (+0x118)
};
struct CTCTargeting {                                   // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTarget();                        // slot 33 (+0x84)
    virtual void v34(); virtual void v35(); virtual void v36();
    virtual bool GetTargetPosition(C3DVector* position, float scale);   // slot 37 (+0x94)
};
struct CPlayer {
    unsigned char _pad0[0x21a];
    bool          m_bLockedOn;                          // +0x21a
    bool IsFirstPersonTargeting() const;                // 0x004874D0
};
struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad50[0x60 - 0x50];
    CTCDriverBase*        PTCDriver;                    // +0x60
    CTCGraphicAppearance* Graphic;                      // +0x64
    unsigned char  _pad68[0x91 - 0x68];
    unsigned char  m_flags91;                           // +0x91 bit 0: dead
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    CPlayer* GetPlayer() const;                         // 0x004C7A30
};
inline bool IsAlive(const CThing* thing) { return thing && !(thing->m_flags91 & 1); }
bool __fastcall IsValidRangedWeaponTargetForShooter(CThing* shooter, CThing* target, float angle);   // 0x0085F8D0
struct CCamera {
    unsigned char _pad0[0x1c];
    C3DVector     Forward;                              // +0x1c
};
struct CGameCameraManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void GetCurrentCamera(CCamera** camera);    // slot 10 (+0x28)
};
struct CEngineWorld {
    unsigned char       _pad0[0x34];
    CGameCameraManager* m_pCameraManager;               // +0x34
};
struct CEngineCalls {
    unsigned char _pad0[0x24];
    CEngineWorld* m_pWorld;                             // +0x24
};
extern CEngineCalls* g_pEngine;                         // 0x013B86A0
struct CModeBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _body[0x14];
};
namespace NCreatureMode {
struct CModeStrafe : CModeBase {
    unsigned char _pad1c[0x3c - 0x1c];
    long          AimingNumFrames;                      // +0x3c
    bool GetBestFrameInAimingAnim(float& frame);
};

bool CModeStrafe::GetBestFrameInAimingAnim(float& frame)
{
    C3DVector focus;
    CRightHandedSet axes;
    ParentThing->Graphic->GetDummyObject("WEAPON_FOCUS_01", 0, 0, &focus, &axes, 1.0f);
    CThing* parent = ParentThing;
    CTCTargeting* targeting;
    if (parent->HasTC(8))
        targeting = parent->GetTC<CTCTargeting>(8);
    C3DVector direction = parent->PTCDriver->GetForward();
    bool validTarget = false;
    if (IsAlive(targeting->GetTarget())
        && IsValidRangedWeaponTargetForShooter(ParentThing, targeting->GetTarget(), 45.0f))
        validTarget = true;
    if (ParentThing->GetPlayer()->IsFirstPersonTargeting() || ParentThing->GetPlayer()->m_bLockedOn) {
        CCamera* camera;
        g_pEngine->m_pWorld->m_pCameraManager->GetCurrentCamera(&camera);
        direction = camera->Forward;
    } else {
        C3DVector targetPos;
        if (targeting->GetTargetPosition(&targetPos, 1.0f) && validTarget)
            direction = targetPos - focus;
    }
    float length2 = direction.GetLength2();
    if (length2 > g_epsilon_0129ba3c * g_epsilon_0129ba3c) {
        float cosAngle = direction.Z * (1.0f / (float)sqrt(length2));
        if (cosAngle < -1.0f)
            cosAngle = -1.0f;
        else if (cosAngle > 1.0f)
            cosAngle = 1.0f;
        float angle = (float)acos(cosAngle) * 0.15915493667125702f;
        float numFrames = (float)AimingNumFrames;
        if (angle < 0.011111111f)
            angle = 0.011111111f;
        else if (angle > 0.41944444f)
            angle = 0.41944444f;
        float result = (numFrames - 1.0f) * (angle - 0.011111111f) * 2.4489796f / (float)AimingNumFrames;
        if (result < 0.0f)
            result = 0.0f;
        else if (result > 1.0f)
            result = 1.0f;
        frame = result;
        return true;
    }
    return false;
}
}
