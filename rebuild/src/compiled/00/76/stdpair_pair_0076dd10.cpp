// std::pair::pair(const pair& other) -- STLport copy constructor of pair<C3DVector, C3DVector>: copy both vectors member by member
// Leaf: ??0?$pair@VC3DVector@@V1@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076DD10, 43 bytes, `ret 4` (thiscall, returns `this`).
// Identity: STLport 4.6 `pair(const pair<_T1,_T2>& __o) : first(__o.first), second(__o.second) {}` over a 24-byte pair whose
//   halves copy through C3DVector's memberwise copy constructor (six dword moves through edx). It sits in the
//   CTCWeaponTrail TU right before the landed pair<CCharString,CCharString> value ctor 0x0076DD40 and the
//   pair<C3DVector,C3DVector> helper GetProjectileTrailElementFromPoint 0x0076DEB0 (FableWin.pdb
//   `?GetProjectileTrailElementFromPoint@?A0x22d77832@@YI?AU?$pair@VC3DVector@@V1@@std@@ABVC3DVector@@@Z`); the TU's
//   vector<pair<C3DVector, C3DVector> > (Ego_r.pdb CTCWeaponTrail::GetLastWeaponTrailPoints) is the 24-byte pair in use.
//   No direct callers (an unreferenced inline instantiation kept by the link), so the instantiation is PROBABLE, not
//   proven. Catalog row was the placeholder `_gapscan::sub_0076dd10`.
// Shape: `mov eax,ecx` first, then X/Y/Z of first and second as dword moves through edx (the last through ecx).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template struct pair<C3DVector, C3DVector>;
}