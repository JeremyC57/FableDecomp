// SetOrientationVectorsFromCopies(CRightHandedSet* set, const C3DVector* forward, const C3DVector* up) -- copy both vectors into stack temporaries and hand them to CRightHandedSet::SetOrientationVectors
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C33920, length 74 bytes.
// Leaf: SetOrientationVectorsFromCopies
// Callees: CRightHandedSet::SetOrientationVectors(const C3DVector&, const C3DVector&) @ 0x00C404F0 (__thiscall; it compares
// the incoming vectors with the set's +0x24.. axes before rebuilding).
// The chunk label "CTCLook::IsTurningBody" is propagated: the body is a free three-argument `__stdcall` function (`ret 0Ch`,
// ecx never read on entry) of the 0x00C3xxxx module, called from 0x00C1C316. Shape: VC evaluates the two temporary
// arguments right to left, so the THIRD argument is copied first (into [esp]) and the second into [esp+0Ch]; each copy goes
// through three registers. The copies need C3DVector's memberwise user copy constructor: with it the z word is stored
// first and eax is reused for the second source pointer (a POD C3DVector, or named locals, allocate ecx/edx/eax differently).
struct C3DVector {
    float x, y, z;
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
};

struct CRightHandedSet {
    void SetOrientationVectors(const C3DVector& forward, const C3DVector& up);   // 0x00C404F0
};

void __stdcall SetOrientationVectorsFromCopies(CRightHandedSet* set, const C3DVector* forward, const C3DVector* up)
{
    set->SetOrientationVectors(C3DVector(*forward), C3DVector(*up));
}
