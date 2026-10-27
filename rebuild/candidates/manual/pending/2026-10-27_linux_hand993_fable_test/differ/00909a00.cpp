// CCombatSequence_WillForcePushLongRange::IsValid() const -- valid unless the owner's will-power component (interface 0xCE) is inactive; for an active one the flat direction from the camera to the owner is normalised and dotted with the camera's forward axis, a test whose outcome is discarded (both branches report valid)
// Grade: DIFFER(298v336) -- retail keeps the normalise + dot + fcomp/fnstsw whose outcome is discarded (both arms return true); every form tried lets VC7.1 drop the dead float work, and the y*y-first length order differs Dead-compare candidate (the ADDENDUM one-grade park): retail keeps a dead normalise + dot + fcomp whose both arms return true; parked after one grade (STLport bitset::test model kept).
// Leaf: IsValid
// Retail: Fable.exe (TLC, VC7.1) @ 0x00909A00, 336 bytes. Virtual IsValid (`_N`, const).
// Callees: CVectorMap::LowerBound 0x004365B0 (inlined GetInterface, type 0xCE = bit 14 of the bitset dword +0x38),
//          CThing::GetPosition 0x004C73D0; the current camera's virtual slots 11 (+0x2c, GetPosition, C3DVector by
//          hidden pointer) and 1 (+0x04, GetMatrix(float), a 0x30-byte matrix by hidden pointer), reached through the
//          main game component 0x013B86A0 +0x24 (camera manager) +0x34 (camera). Globals: the reciprocal-square-root
//          mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.0f 0x0122DEDC.
//
// Shape: the landed WillForcePush IsValid (0x009098E0) GetInterface + nesting; the flat direction is a C3DVector
// built from the two differences and 0.0f, normalised by the inline fast Normalise of the landed
// CTCAssassinRush::SetupFacingDirection (0x007A6E20; `z*z` folds away, `Z *= r` stays an `fmul [0.0f]`); the dot
// with (forward.X, forward.Y, 0) is compared with 0.0f and the result thrown away (`fcomp ; fnstsw` then
// `mov al,1`).
// Layout: sequence owner +0x1c (landed sibling rows); the will-power component's active byte +0xc.
extern "C" int __cdecl puts(const char* text);              // 0x00BFEAF2 (MSVCR71 import thunk)
extern "C" __declspec(noreturn) void __cdecl abort(void);   // 0x00BFEAEC
namespace std {
template <unsigned int _Nb>
class bitset {                                              // STLport 4.6 bitset<274>: 9 words, test() range-checked (no exceptions)
public:
    enum { _Words = (_Nb + 31) / 32 };
    unsigned long _M_w[_Words];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static unsigned int _S_whichbit(unsigned int __pos) { return __pos % 32; }
    static unsigned long _S_maskbit(unsigned int __pos) { return 1UL << _S_whichbit(__pos); }
    unsigned long _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
    bool _Unchecked_test(unsigned int __pos) const { return (_M_getword(__pos) & _S_maskbit(__pos)) != 0; }
    bool test(unsigned int __pos) const
    {
        if (__pos >= _Nb) {
            puts("bitset");
            abort();
        }
        return _Unchecked_test(__pos);
    }
};
}
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct C3DMatrix {                          // three axes and the origin (0x30 bytes)
    C3DVector Right;                        // +0x00
    C3DVector Up;                           // +0x0c
    C3DVector Forward;                      // +0x18
    C3DVector Origin;                       // +0x24
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    std::bitset<274> UsingComponents;         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    const C3DVector& GetPosition() const;   // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents.test(interfaceType)) {
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
struct CThingCreatureBase : CThing { };

struct CCamera {
    virtual void v00();
    virtual C3DMatrix GetMatrix(float interpolation);      // slot 1 (+0x04)
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual C3DVector GetPosition();                       // slot 11 (+0x2c)
};
struct CCameraManager {
    unsigned char _pad[0x34];
    CCamera* CurrentCamera;                 // +0x34
};
struct CMainGameComponent_Retail {
    unsigned char _pad[0x24];
    CCameraManager* CameraManager;          // +0x24
};
extern CMainGameComponent_Retail* g_pMainGameComponent;   // 0x013B86A0

struct CTCWillPower {                       // interface type 0xCE
    unsigned char _pad[0xc];
    bool Active;                            // +0x0c
};
struct CCombatSequenceBase {
    virtual void v00();
    unsigned char _pad[0x18];
    CThingCreatureBase* m_pOwner;           // +0x1c
};
struct CCombatSequence_WillForcePushLongRange : CCombatSequenceBase {
    virtual bool IsValid() const;
};

bool CCombatSequence_WillForcePushLongRange::IsValid() const
{
    CThingCreatureBase* owner = m_pOwner;
    CTCWillPower* willPower;
    if (owner->GetInterface(&willPower, 0xce)) {
        if (willPower->Active) {
            C3DVector delta = owner->GetPosition() - g_pMainGameComponent->CameraManager->CurrentCamera->GetPosition();
            C3DVector direction(delta.X, delta.Y, 0.0f);
            direction.Normalise();
            C3DMatrix matrix = g_pMainGameComponent->CameraManager->CurrentCamera->GetMatrix(1.0f);
            if (Dot(direction, C3DVector(matrix.Forward.X, matrix.Forward.Y, 0.0f)) > 0.0f)
                return true;
            return true;
        }
        return false;
    }
    return true;
}
