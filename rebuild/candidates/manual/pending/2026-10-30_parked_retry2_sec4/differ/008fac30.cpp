// CPhysicsForceModelBase::DoThemeEvasion() -- collect the nearby themes into the model's theme list and return an evasion direction: for four probe offsets around the current position sum 5/distance to every theme and return the differences of the last three sums against the first, times 10
// Grade: DIFFER(372v372) nd=12 (was 335v372 nd=99) -- C3DVector operators (ctor-return *,+,- and a user copy ctor) with a z,y,x Length(), probe loop as a signed index loop over g_ThemeProbeOffsets, list ctor node-first, named return vector, named theme manager before CollectThemes; residue: pos-copy stores scheduled before the malloc 'add esp,4' (retail after)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FAC30, length 370 bytes, `ret 4` (C3DVector through a hidden pointer).
// Callees: malloc 0x00BFEA0E (the local STLport list's sentinel), the theme collector 0x008FC430 and list tidy-up
// 0x008FA9F0 (descriptive), the local list's destructor 0x0048CA60 (manifest labels propagated); the world's +0x74
// object virtual slot 13 (+0x34).
// Data: the four probe offsets @ 0x013893AC (C3DVector[4]), 0.1f 0x0122EE58, 5.0f 0x012462A8, 10.0f 0x01237F0C.
#include <math.h>
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    float Length() const { return (float)sqrt(Z * Z + Y * Y + X * X); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
extern const C3DVector g_ThemeProbeOffsets[4];          // 0x013893AC
struct CThemeNode { CThemeNode* Next; CThemeNode* Prev; C3DVector Pos; };
struct CThemeList {
    CThemeNode* Head;
    void Tidy();                                        // 0x008FA9F0
};
struct CLocalList {                                     // STLport list (sentinel only)
    CThemeNode* Head;
    CLocalList() { CThemeNode* node = (CThemeNode*)malloc(0x14); node->Next = node; node->Prev = node; Head = node; }
    ~CLocalList();                                      // 0x0048CA60
};
struct CThemeSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual void* GetThemeManager();                    // slot 13 (+0x34)
};
struct CWorldView { unsigned char _pad[0x74]; CThemeSource* Themes; };
struct CPhysicsState { C3DVector Position; };
struct CPhysicsForceModelBase {
    void*          __vftable;
    CPhysicsState* LinkedState;                         // +0x04
    unsigned char  _pad08[0x2c - 8];
    CThemeList     Themes;                              // +0x2c
    unsigned char  _pad30[0xa8 - 0x30];
    CWorldView*    World;                               // +0xa8
    void CollectThemes(CThemeList* list, void* manager, const C3DVector* pos, long type, float radius);   // 0x008FC430
    C3DVector DoThemeEvasion();
};
C3DVector CPhysicsForceModelBase::DoThemeEvasion()
{
    CLocalList scratch;
    C3DVector pos = LinkedState->Position;
    void* manager = World->Themes->GetThemeManager();
    CollectThemes(&Themes, manager, &pos, 2, 50.0f);
    Themes.Tidy();
    float sums[4];
    sums[0] = 0.0f; sums[1] = 0.0f; sums[2] = 0.0f; sums[3] = 0.0f;
    for (CThemeNode* node = Themes.Head->Next; node != Themes.Head; node = node->Next) {
        for (long i = 0; i < 4; ++i)
            sums[i] += 5.0f / ((g_ThemeProbeOffsets[i] * 0.1f + pos) - node->Pos).Length();
    }
    C3DVector result((sums[1] - sums[0]) * 10.0f, (sums[2] - sums[0]) * 10.0f, (sums[3] - sums[0]) * 10.0f);
    return result;
}
