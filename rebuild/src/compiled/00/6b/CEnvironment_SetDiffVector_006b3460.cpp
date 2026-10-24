// CEnvironment::SetDiffVector(CEnvironment& env, const vector<CGenericVar>& args) -- console: set the diffuse light direction (normalised x, y, z) in the current and previous game-tick states
// Leaf: ?SetDiffVector@CEnvironment@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B3460, length 87 bytes, private static __fastcall (`CI`):
//   ecx = env, edx = args, plain `ret`.
// Callees: C3DVector::Normalise() @ 0x00A14440 (in place; the manifest's "GetScaled" is a propagated label,
//   corrected by chunks 217/239/262/270/273).
// Identity: Ego_r.pdb `?SetDiffVector@CEnvironment@@CIXAAV1@ABV?$vector@VCGenericVar@@...@Z` (name set only).
// Layout (local model): retail CEnvironment puts GameTickState at +0x54 and PrevGameTickState at +0xc74 (the
//   12-byte retail CArray at +0x48, see 0x006B3880), each a 0xc20-byte CEnvironmentGameTickState with
//   DiffuseVector at +0xbf4 (stores at +0xc48 / +0x1868). The donor header/layout (0x58 / 0xd08, 0xcb0 bytes,
//   DiffuseVector +0xc84) is 0x90 bytes larger per state, so engine/CEnvironment.h does not fit.
//   CGenericVar (0x1c): ValFloat +0x18 (args[0..2] = +0x18, +0x34, +0x50).
// Shape: C3DVector built from the three float args, normalised in place, then copied into both states.
class CCharString { public: void* PData; };
class CGenericVar {
public:
    void*         _vftable;
    long          Type;                                     // +0x04
    CCharString   ValString;                                // +0x08
    long          ValSLONG;                                 // +0x0c
    unsigned long ValULONG;                                 // +0x10
    bool          ValBool;                                  // +0x14
    float         ValFloat;                                 // +0x18
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T& operator[](unsigned int n) const { return *(_M_start + n); }
};
}
using std::vector;

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise();                                       // 0x00A14440
};

class CEnvironmentGameTickState {                           // retail 0xc20 bytes
public:
    unsigned char _pad000[0xbf4];
    C3DVector     DiffuseVector;                            // +0xbf4
    unsigned char _padc00[0xc20 - 0xc00];
};

class CEnvironment {
public:
    unsigned char             _pad00[0x54];
    CEnvironmentGameTickState GameTickState;                // +0x54
    CEnvironmentGameTickState PrevGameTickState;            // +0xc74
private:
    static void __fastcall SetDiffVector(CEnvironment& env, const vector<CGenericVar>& args);
};

void __fastcall CEnvironment::SetDiffVector(CEnvironment& env, const vector<CGenericVar>& args)
{
    C3DVector diffuse(args[0].ValFloat, args[1].ValFloat, args[2].ValFloat);
    diffuse.Normalise();
    env.GameTickState.DiffuseVector = diffuse;
    env.PrevGameTickState.DiffuseVector = diffuse;
}