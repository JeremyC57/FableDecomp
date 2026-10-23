// CCombatActionBase::GetNextPos() const -- where the owner will be after this animation frame: either the fixed per-frame movement (movement / frame count) or the animation's root movement between the clamped previous/current times, rotated into world space (also re-basing the graphic's orientation); a movement-modifier component (type 0x100) may adjust it; returns owner position + movement
// Grade: DIFFER(1025v1032) -- 60 blocks: register allocation -- retail keeps the animation id in ebx (flag tested via al, the animation pointer later reuses ebx) and `this` in esi / &Owner in edi, ours swaps esi/edi and hoists the push of the animation id above the branch, shifting every frame slot (0xc8 v 0xb0); call order, in-place matrix transforms, fps getter, clamps and component lookup line up
// Leaf: GetNextPos
// Retail 0x008575C0, 1032 bytes, `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; CThing GetAnimDuration 0x006627F0 / GetPosition 0x004C73D0;
//   CCombatActionBase::GetWorld 0x00692950; CMeshDataBank::PeekAnimation 0x00A242C0; the animation's counted-pointer
//   getter 0x00A269C0; the orientation's matrix getter 0x004BC800; the sorted component vector lower_bound 0x004365B0;
//   the movement component 0x007E8A40; operator delete 0x00BFE9BC. Global 0x01375550 = frames per second.
// Layout: from this function's use -- CCombatActionBase {owner pointer +0x08, frame count +0x10, start/end times +0x40/
//   +0x44, fixed movement flag +0xdd, fixed movement +0x108}; CThing {component flags +0x40, components +0x44,
//   graphic +0x60, orientation +0x64}.
void operator delete(void* p);                         // 0x00BFE9BC
extern long g_FramesPerSecond;                         // 0x01375550
inline long GetFramesPerSecond() { return g_FramesPerSecond; }
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    __forceinline C3DVector& operator*=(const struct C3DMatrix& m);
};
struct C3DMatrix { float M[3][3]; C3DVector Pos; };
__forceinline C3DVector& C3DVector::operator*=(const C3DMatrix& m)
{
    float x = X, y = Y;
    X = Z * m.M[2][0] + Y * m.M[1][0] + X * m.M[0][0];
    Y = Z * m.M[2][1] + Y * m.M[1][1] + x * m.M[0][1];
    Z = Z * m.M[2][2] + y * m.M[1][2] + x * m.M[0][2];
    return *this;
}
inline C3DVector operator*(const C3DMatrix& m, const C3DVector& v)
{
    return C3DVector(v.Z * m.M[2][0] + v.Y * m.M[1][0] + v.X * m.M[0][0],
                     v.Z * m.M[2][1] + v.Y * m.M[1][1] + v.X * m.M[0][1],
                     v.Z * m.M[2][2] + v.Y * m.M[1][2] + v.X * m.M[0][2]);
}
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CAnimation {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool HasRotation();                         // slot 4 (+0x10)
    virtual void v05();
    virtual C3DMatrix GetMovement(float from, float to); // slot 6 (+0x18)
};
struct CAnimationPointer {
    CAnimation*     Data;
    CCPPointerInfo* Info;
    ~CAnimationPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
};
struct CAnimationHandle {
    CAnimationPointer GetAnimation();                  // 0x00A269C0
};
struct CMeshDataBank {
    CAnimationHandle* PeekAnimation(long index);       // 0x00A242C0
};
struct CWorld {
    unsigned char  _pad00[0x3c];
    CMeshDataBank* MeshBank;                           // +0x3c
};
struct SOrientation {
    C3DVector Up;
    C3DVector Forward;
};
struct CGraphicVTableUser {
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s0a(); virtual void s0b();
    virtual void s0c(); virtual void s0d(); virtual void s0e(); virtual void s0f(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17();
    virtual void s18(); virtual void s19(); virtual void s1a(); virtual void s1b(); virtual void s1c(); virtual void s1d();
    virtual void s1e(); virtual void s1f(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29();
    virtual void s2a(); virtual void s2b(); virtual void s2c(); virtual void s2d(); virtual void s2e(); virtual void s2f();
    virtual void s30(); virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39(); virtual void s3a(); virtual void s3b();
    virtual void s3c(); virtual void s3d(); virtual void s3e();
    virtual float UpdateOrientation();                 // slot 63 (+0xfc)
    virtual void s40(); virtual void s41(); virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45();
    virtual void GetBounds(void* out);                 // slot 70 (+0x118)
    virtual void s47();
    virtual const SOrientation* GetOrientation();      // slot 72 (+0x120)
    virtual void s49(); virtual void s4a();
    virtual void SetOrientation(const SOrientation* orientation);   // slot 75 (+0x12c)
};
struct COrientationSource {
    const C3DMatrix* GetMatrix(C3DMatrix* out, float scale);   // 0x004BC800
};
struct CThingComponent {
    void ModifyMovement(C3DVector* movement);          // 0x007E8A40
};
struct SComponentEntry { long Type; CThingComponent* Component; };
struct CComponentVector {
    SComponentEntry* _M_start;
    SComponentEntry* _M_finish;
    SComponentEntry* _M_end_of_storage;
    SComponentEntry* lower_bound(const long& type);    // 0x004365B0
    SComponentEntry* find(const long& type)
    {
        SComponentEntry* it = lower_bound(type);
        if (it == _M_finish || type < it->Type)
            return _M_finish;
        return it;
    }
};
struct CThing {
    unsigned char       _pad00[0x40];
    unsigned long       ComponentFlags;                // +0x40
    CComponentVector    Components;                    // +0x44
    unsigned char       _pad50[0x60 - 0x50];
    CGraphicVTableUser* Graphic;                       // +0x60
    COrientationSource* Orientation;                   // +0x64
    float GetAnimDuration(long animation);             // 0x006627F0
    const C3DVector& GetPosition();                    // 0x004C73D0
};
struct CBaseIntelligentPointer {
    void* Data;
    void* Info;
    CThing* GetPItem() const;                          // 0x00A01B50
};
class CCombatActionBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual long GetCurrentFrame() const;              // slot 6 (+0x18)
    virtual long GetAnimation() const;                 // slot 7 (+0x1c)
    unsigned char           _pad04[4];
    CBaseIntelligentPointer Owner;                     // +0x08
    long                    FrameCount;                // +0x10
    unsigned char           _pad14[0x40 - 0x14];
    float                   StartTime;                 // +0x40
    float                   EndTime;                   // +0x44
    unsigned char           _pad48[0xdd - 0x48];
    bool                    FixedMovement;             // +0xdd
    unsigned char           _padde[0x108 - 0xde];
    C3DVector               Movement;                  // +0x108
    CWorld* GetWorld() const;                          // 0x00692950
    C3DVector GetNextPos() const;
};

C3DVector CCombatActionBase::GetNextPos() const
{
    long animation = GetAnimation();
    C3DVector movement;
    if (FixedMovement) {
        Owner.GetPItem()->GetAnimDuration(animation);
        movement = Movement * (1.0f / FrameCount);
    } else {
        CAnimationHandle* handle = GetWorld()->MeshBank->PeekAnimation(animation);
        CAnimationPointer anim = handle->GetAnimation();
        float from = StartTime - (float)GetCurrentFrame() / GetFramesPerSecond();
        if (from < 0.0f)
            from = 0.0f;
        float to = EndTime - (float)GetCurrentFrame() / GetFramesPerSecond();
        if (to < 0.0f)
            to = 0.0f;
        C3DMatrix delta = anim.Data->GetMovement(to, from);
        C3DVector move = delta.Pos;
        C3DMatrix orientation;
        move *= *Owner.GetPItem()->Orientation->GetMatrix(&orientation, 1.0f);
        CGraphicVTableUser* graphic = Owner.GetPItem()->Graphic;
        const SOrientation* current = graphic->GetOrientation();
        C3DVector forward = current->Forward;
        C3DVector up = current->Up;
        if (anim.Data->HasRotation()) {
            forward *= delta;
            up *= delta;
        }
        SOrientation next;
        next.Up = up;
        next.Forward = forward;
        graphic->SetOrientation(&next);
        movement = move;
        Owner.GetPItem()->Graphic->UpdateOrientation();
        C3DVector bounds;
        Owner.GetPItem()->Graphic->GetBounds(&bounds);
    }
    CThing* owner = Owner.GetPItem();
    if (owner->ComponentFlags & 1) {
        owner->Components.find(0x100)->Component->ModifyMovement(&movement);
    }
    const C3DVector& position = Owner.GetPItem()->GetPosition();
    return C3DVector(movement.X + position.X, movement.Y + position.Y, movement.Z + position.Z);
}
