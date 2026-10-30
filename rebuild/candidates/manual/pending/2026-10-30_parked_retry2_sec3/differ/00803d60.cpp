// Grade: DIFFER(682v699) -- structural draft: frame 0xbc v 0xa4 (filter/segment temporaries; retail keeps hit=0 in ebx and zeroes the creatures vector from it), the camera manager's this loaded straight into ecx in retail; GetFOV now on the CGameCameraBase::InternalCam CCamera at +4 (Ego_r.pdb) as retail
// Tried: one draft (named filter locals).
// CTCTargetingPlayer::UpdateProjectileTargeting() -- aim along the camera: gather the creatures in the camera's field of view within range, cast a line of sight from the camera and remember the point (and a hit, targetable creature)
// Leaf: UpdateProjectileTargeting
// Retail: Fable.exe (TLC, VC7.1) @ 0x00803D60, 699 bytes, `ret`.
// Callees: CTCBase::GetWorld @ 0x00686A70, the camera manager's current-camera getter (vslot +0x24, out pointer),
//   CThing::GetPosition @ 0x004C73D0, C3DVector::Normalise @ 0x00A14440 (catalog "GetScaled"), the camera FOV getter @ 0x00A0BEB0
//   (catalog "DrawGetSeaLevel"), CThingFilter_IsInFOV ctor @ 0x006A7150 / operator() @ 0x006A7270, ftol @ 0x00BFEA70,
//   CTCBase::GetThingSearchTools @ 0x00686D90, the boxed creature search @ 0x0080A550, CTools::IsLineOfSight @ 0x00877840,
//   CIntelligentPointer<CThing> ctor @ 0x004A96F0 / operator= @ 0x004A9760 / dtor @ 0x00A01C10, GetPItem @ 0x00A01B50,
//   free @ 0x00BFEA14.
// Layout: retail CTCTargetingPlayer projectile range +0x48, TargetedPos +0x84, PProjectileTargetedThing +0xdc; the camera's
//   position +0x04, look-at owner +0x98 (position +0x10), line tools +0x78.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    void operator*=(float s) { X *= s; Y *= s; Z *= s; }
    void Normalise();                                   // 0x00A14440
};
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double);
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
template <class T> struct CRetailVector {
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) {}
    ~CRetailVector() { if (first) free(first); }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    unsigned char _pad44[0x91 - 0x44];
    unsigned char LifeFlags;                            // +0x91
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CThingCreatureBase;
struct CThingFilter_IsInFOV {
    unsigned char _body[0x40];
    CThingFilter_IsInFOV(const C3DVector& eye, const C3DVector& dir, float fov, float minDistance, bool flag);   // 0x006A7150
    bool operator()(C3DVector* hit);                    // 0x006A7270
};
struct CThingFilter_IsNot { CThing* Thing; };
struct CThingFilter_In2DRange { C3DVector Centre; float Range; };
struct CThingFilter_Empty { };
struct CFilter_Bind5 {
    CThingFilter_IsInFOV*   F0;
    CThingFilter_IsNot*     F1;
    CThingFilter_In2DRange* F2;
    CThingFilter_IsNot*     F3;
    CThingFilter_Empty*     F4;
};
struct CThingSearchTools {
    void FindCreatures(const C2DBoxI& box, const CFilter_Bind5& filter, CRetailVector<CThingCreatureBase*>* out);   // 0x0080A550
};
struct CTools {
    bool IsLineOfSight(CRetailVector<CThingCreatureBase*>* things, const C3DVector* from, const C3DVector* to, C3DVector* hitPos,
                       CThingFilter_IsInFOV* filter, CThing** hitThing, long test, bool flag);   // 0x00877840
};
struct CCamera {                                        // Ego_r.pdb CCamera (0x38): WorldPosition +0x0
    C3DVector WorldPosition;
    unsigned char _pad0c[0x38 - 0x0c];
    float GetFOV() const;                               // 0x00A0BEB0
};
struct CGameCameraBase {
    void*     __vftable;
    CCamera   InternalCam;                              // +0x04
    unsigned char _pad3c[0x78 - 0x3c];
    CTools    Tools;                                    // +0x78
    unsigned char _pad79[0x98 - 0x79];
    struct SLookAt { unsigned char _pad[0x10]; C3DVector Position; }* PLookAt;   // +0x98
};
struct CCameraManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void GetCurrentCamera(CGameCameraBase** out);       // +0x24
};
struct CWorld { unsigned char _pad[0x34]; CCameraManager* CameraManager; };
struct CBaseObject;
struct CBaseIntelligentPointer {
    void*        __vftable;
    CBaseObject* PData;
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item);                       // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};

struct CTCTargetingPlayer {
    void*     __vftable;
    CThing*   ParentThing;                              // +0x04
    unsigned char _pad08[0x48 - 0x08];
    float     ProjectileRange;                          // +0x48
    unsigned char _pad4c[0x84 - 0x4c];
    C3DVector TargetedPos;                              // +0x84
    unsigned char _pad90[0xdc - 0x90];
    CIntelligentPointer<CThing> PProjectileTargetedThing;   // +0xdc
    CWorld* GetWorld();                                 // 0x00686A70
    CThingSearchTools* GetThingSearchTools();           // 0x00686D90
    void UpdateProjectileTargeting();
};

void CTCTargetingPlayer::UpdateProjectileTargeting()
{
    CGameCameraBase* camera;
    GetWorld()->CameraManager->GetCurrentCamera(&camera);
    const C3DVector& pos = ParentThing->GetPosition();
    const C3DVector& eye = camera->InternalCam.WorldPosition;
    C3DVector dir = camera->PLookAt->Position - eye;
    dir.Normalise();
    dir *= ProjectileRange;
    float dx = pos.X - eye.X;
    float dy = pos.Y - eye.Y;
    CThing* hit = 0;
    CRetailVector<CThingCreatureBase*> creatures;
    CThingFilter_IsInFOV fov(eye, dir, camera->InternalCam.GetFOV(), 0, false);
    CThingFilter_IsNot notMe = { ParentThing };
    CThingFilter_In2DRange range = { eye, (float)sqrt(dx * dx + dy * dy) };
    CThingFilter_IsNot notMe2 = { ParentThing };
    CThingFilter_Empty affected;
    CFilter_Bind5 filter = { &fov, &notMe, &range, &notMe2, &affected };
    long x = (long)eye.X;
    long y = (long)eye.Y;
    long r = (long)ProjectileRange;
    GetThingSearchTools()->FindCreatures(C2DBoxI(x - r, y - r, x + r, y + r), filter, &creatures);
    TargetedPos = eye + dir;
    C3DVector end = eye + dir;
    C3DVector hitPos;
    if (camera->Tools.IsLineOfSight(&creatures, &eye, &end, &hitPos, &fov, &hit, 0xf, false))
        return;
    if (!fov(&hitPos))
        return;
    if (hit && !(hit->LifeFlags & 1) && (CThing*)PProjectileTargetedThing.GetPItem() != hit && (hit->m_presentBits[0] & 0x200))
        PProjectileTargetedThing = CIntelligentPointer<CThing>(hit);
    TargetedPos = hitPos;
}
