// Grade: DIFFER(335v372) -- first attempt only: frame (retail keeps the float sums at the top of a 0x44 frame and the probe walk on two pointers), local list construction order and the 5/len accumulation order differ; x87 order not converged
// CPhysicsForceModelBase::DoThemeEvasion() -- collect the nearby themes into the model's theme list and return an evasion direction: for four probe offsets around the current position sum 5/distance to every theme and return the differences of the last three sums against the first, times 10
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
};
extern const C3DVector g_ThemeProbeOffsets[4];          // 0x013893AC
struct CThemeNode { CThemeNode* Next; CThemeNode* Prev; C3DVector Pos; };
struct CThemeList {
    CThemeNode* Head;
    void Tidy();                                        // 0x008FA9F0
};
struct CLocalList {                                     // STLport list (sentinel only)
    CThemeNode* Head;
    CLocalList() { Head = (CThemeNode*)malloc(0x14); Head->Next = Head; Head->Prev = Head; }
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
    CollectThemes(&Themes, World->Themes->GetThemeManager(), &pos, 2, 50.0f);
    Themes.Tidy();
    float sums[4];
    sums[0] = 0.0f; sums[1] = 0.0f; sums[2] = 0.0f; sums[3] = 0.0f;
    for (CThemeNode* node = Themes.Head->Next; node != Themes.Head; node = node->Next) {
        float* sum = sums;
        for (const C3DVector* offset = g_ThemeProbeOffsets; offset != g_ThemeProbeOffsets + 4; ++offset, ++sum) {
            float px = offset->X * 0.1f + pos.X;
            float py = offset->Y * 0.1f + pos.Y;
            float pz = offset->Z * 0.1f + pos.Z;
            float dx = px - node->Pos.X;
            float dy = py - node->Pos.Y;
            float dz = pz - node->Pos.Z;
            *sum += 5.0f / (float)sqrt(dz * dz + dy * dy + dx * dx);
        }
    }
    return C3DVector((sums[1] - sums[0]) * 10.0f, (sums[2] - sums[0]) * 10.0f, (sums[3] - sums[0]) * 10.0f);
}
