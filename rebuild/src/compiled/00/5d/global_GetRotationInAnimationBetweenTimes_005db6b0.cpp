// GetRotationInAnimationBetweenTimes(long bankIndex, float& rotation, float startTime, float endTime) -- the signed turn (in revolutions) of a bank animation's root movement between two times: the angle between the end movement's forward axis and +X, signed by the half-way movement's forward axis
// Leaf: ?GetRotationInAnimationBetweenTimes@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DB6B0, 529 bytes (0x211 + int3 padding), __fastcall `ret 8` (bankIndex in ecx,
//   &rotation in edx). Ego_r.pdb `?GetRotationInAnimationBetweenTimes@?A0x1f24e1a2@@YIXJAAMMM@Z` (an anonymous-namespace
//   helper of the animation-set TU, written here with external linkage so VC7.1 emits it). One caller: the thunk
//   CAnimationEntry::GetRotationInAnimationBetweenTimes(float, float, float&) const 0x005DC010.
// Callees: CMeshDataBank::PeekAnimation(unsigned long) const 0x00A242C0 (twice), C3DAnimationInfo::GetAnimationStats()
//   const 0x00A269C0 (returns CCountedPointer<const C3DAnimationStats> through a hidden pointer), the stats' pure
//   virtuals slot 4 (+0x10, bool: has root movement) and slot 6 (+0x18, CMatrix3x4 by value from (start, end)),
//   operator delete 0x00BFE9BC, _CIacos 0x00BFEBA2 (the acos intrinsic). Globals: g_pMainGameComponent 0x013B86A0,
//   1e-4f 0x0129BA3C (read as a variable, negated with fchs), 0.0f 0x0122DEDC, 1.0f 0x0122DED8, -1.0f 0x0122DEE0,
//   0.5f 0x0122F59C, 1/(2 pi) 0x01237F08.
// Layout: CMainGameComponent::PWorld +0x24, CWorld::PMainMeshBank +0x3c; C3DAnimationInfo::Duration +0x10;
//   CCountedPointer {Data +0, Info +4}, CCPPointerInfo {RefCount, DeleteFunc (__fastcall), Data}; CMatrix3x4 E11..E43.
// Shape: speed TU. The info pointer is a named local (its stats temporary is built after the PeekAnimation call, then
//   released at once: the dtor clears Data before Info); a negative end time means the whole animation (a fresh
//   PeekAnimation); the forward axes are +X through the row-vector product (terms written z, y, x: VC7.1 folds the
//   zero components into `(E2x + E3x) * 0`); the cosine is clamped to [-1, 1]; the sign multiplies the stored result.
extern "C" double __cdecl acos(double x);                                           // _CIacos 0x00BFEBA2
#pragma intrinsic(acos)
void __cdecl operator delete(void* p);                                               // 0x00BFE9BC

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
inline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};
inline C3DVector operator*(const C3DVector& v, const CMatrix3x4& m)
{
    return C3DVector(v.Z * m.E31 + v.Y * m.E21 + v.X * m.E11,
                     v.Z * m.E32 + v.Y * m.E22 + v.X * m.E12,
                     v.Z * m.E33 + v.Y * m.E23 + v.X * m.E13);
}

class CCPPointerInfo { public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void*         Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    T* operator->() const { return Data; }
};

class C3DAnimationStats {
public:
    virtual ~C3DAnimationStats();                                                   // slot 0 (vtable 0x0129D514)
    virtual void VirtualSlot1();                                                    // slot 1 (0x0099A400)
    virtual void VirtualSlot2() const = 0;                                          // slots 2..6 pure
    virtual void VirtualSlot3() const = 0;
    virtual bool HasMovement() const = 0;                                           // slot 4 (+0x10)
    virtual void VirtualSlot5() const = 0;
    virtual CMatrix3x4 GetMovementBetweenTimes(float startTime, float endTime) const = 0;   // slot 6 (+0x18)
};
class C3DAnimationInfo {
public:
    unsigned char _pad_0x00[0x10];
    float         Duration;                                                         // +0x10
    CCountedPointer<const C3DAnimationStats> GetAnimationStats() const;              // 0x00A269C0
};
class CMeshDataBank {
public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;                // 0x00A242C0
};
class CWorld {
public:
    unsigned char  _pad_0x00[0x3c];
    CMeshDataBank* PMainMeshBank;                                                    // +0x3c
};
class CMainGameComponent {
public:
    unsigned char _pad_0x00[0x24];
    CWorld*       PWorld;                                                            // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;                                    // 0x013B86A0

extern const float g_epsilon_0129ba3c;                                              // 0x0129BA3C (1e-4f)

void __fastcall GetRotationInAnimationBetweenTimes(long bankIndex, float& rotation, float startTime, float endTime)
{
    CMeshDataBank* bank = g_pMainGameComponent->PWorld->PMainMeshBank;
    const C3DAnimationInfo* info = bank->PeekAnimation(bankIndex);
    const C3DAnimationStats* stats = info->GetAnimationStats().Data;
    if (endTime < -g_epsilon_0129ba3c)
        endTime = bank->PeekAnimation(bankIndex)->Duration;
    if (!stats->HasMovement()) {
        rotation = 0.0f;
        return;
    }
    CMatrix3x4 endTransform = stats->GetMovementBetweenTimes(startTime, endTime);
    C3DVector endForward = C3DVector(1.0f, 0.0f, 0.0f) * endTransform;
    CMatrix3x4 midTransform = stats->GetMovementBetweenTimes(startTime, (startTime + endTime) * 0.5f);
    C3DVector midForward = C3DVector(1.0f, 0.0f, 0.0f) * midTransform;
    float cosAngle = DotProduct(endForward, C3DVector(1.0f, 0.0f, 0.0f));
    if (cosAngle < -1.0f)
        cosAngle = -1.0f;
    else if (cosAngle > 1.0f)
        cosAngle = 1.0f;
    rotation = (float)acos(cosAngle) * 0.15915494f;
    float sign = 1.0f;
    if (DotProduct(midForward, C3DVector(0.0f, -1.0f, 0.0f)) < 0.0f)
        sign = -1.0f;
    rotation *= sign;
}