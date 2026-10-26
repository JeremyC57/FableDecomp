// std::_For_each<CThingCreatureBase**,CKeepOutOffSmearedOutLine>(CThingCreatureBase** first, CThingCreatureBase** last, CKeepOutOffSmearedOutLine func) -- apply the obstruction's keep-out functor (push each creature off the smeared-out line) to every creature in the range and return the functor
// Leaf: ??$_For_each@PAPAVCThingCreatureBase@@VCKeepOutOffSmearedOutLine@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FDA80, 65 bytes (`ret 10h` + int3 padding); /Gr TU: hidden result in ecx, first in
//   edx, last and the 12-byte functor on the stack. Ego_r.pdb / FableWin.pdb
//   `??$_For_each@PAPAVCThingCreatureBase@@VCKeepOutOffSmearedOutLine@@@std@@YI?AVCKeepOutOffSmearedOutLine@@PAPAVCThingCreatureBase@@0V1@@Z`;
//   called by CTCDPhysicalObstruction::UpdateAdditionalImpulse 0x007FD580 (landed; its stand-in calls this `ForEach`).
//   The catalog label's functor `CApplyAdditionalImpulseIfInCombatAndTooClose` is propagated (that PDB class is 24 bytes).
// Callees: CKeepOutOffSmearedOutLine::operator()(CThing*) const @ 0x007FD8D0 (landed as
//   "CApplyImpulseIfCloseToCombatLine::operator()").
// Layout: CKeepOutOffSmearedOutLine (Ego_r.pdb, 12 bytes): StartPos +0 (C3DVector*), EndPos +4, Radius +8.
// Shape: STLport/Dinkumware `for (; first != last; ++first) func(*first); return func;` as a `__fastcall` template with an
//   explicit instantiation (the landed 0x007F7B70 pattern); the functor's implicit copy writes the three words out. The
//   functor needs its user constructor (FableWin.pdb has it): a POD result type moves the hidden result pointer from ecx
//   to the stack (first/last then arrive in ecx/edx).
class C3DVector { public: float X, Y, Z; };

class CThing { };
class CThingCreatureBase : public CThing { };

class CKeepOutOffSmearedOutLine {
public:
    C3DVector* StartPos;                                        // +0x00
    C3DVector* EndPos;                                          // +0x04
    float      Radius;                                          // +0x08
    CKeepOutOffSmearedOutLine(const C3DVector& start, const C3DVector& end, float radius)   // FableWin.pdb ??0 (inline)
        : StartPos((C3DVector*)&start), EndPos((C3DVector*)&end), Radius(radius) { }
    void operator()(CThing* thing) const;                       // 0x007FD8D0
};

namespace std {
template <class _InIt, class _Fn1> _Fn1 __fastcall _For_each(_InIt first, _InIt last, _Fn1 func)
{
    for (; first != last; ++first)
        func(*first);
    return func;
}
}

template CKeepOutOffSmearedOutLine __fastcall std::_For_each<CThingCreatureBase**, CKeepOutOffSmearedOutLine>(CThingCreatureBase**, CThingCreatureBase**, CKeepOutOffSmearedOutLine);