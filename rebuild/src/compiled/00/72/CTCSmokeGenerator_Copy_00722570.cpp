// CTCSmokeGenerator::Copy(const CTCBase& other) -- copy another smoke generator's GenerationDistance and its GenerationPositions vector
// Leaf: ?Copy@CTCSmokeGenerator@@UAEXABVCTCBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00722570, 25 bytes, thiscall, tail `jmp` to the vector assignment (its result is dropped).
// Identity (proven): slot 12 of the CTCSmokeGenerator vtable 0x01265D74 (rtti-vtables.tsv); slot 12 of the CTCBase vtables is
//   Copy(const CTCBase&) (landed CTCQuestCompletionUI::Copy 0x005E49E0, CTCHeroMorph::Copy 0x0071CD90); Ego_r.pdb and FableWin.pdb
//   ?Copy@CTCSmokeGenerator@@UAEXABVCTCBase@@@Z (Ego_r donor 0x00739A40, after the ctor 0x00739A10). The catalog label
//   "CTCSmokeGenerator::vfunc_12" is a placeholder.
// Callee: std::vector<CTCSmokeGenerator::CGenerationPos>::operator= 0x00722F30 (STLport 4.6 _vector.c, out of line; the landed
//   CGenerationPos helpers bind the vector's _M_clear / __copy / __uninitialized_copy / _Destroy around it).
// Layout: Ego_r.pdb CTCSmokeGenerator = CTCBase (0xC) + GenerationDistance (float) +0xC + GenerationPositions (vector<CGenerationPos>)
//   +0x10 + PositionsCreated (bool, retail +0x1C, not copied).
// Shape: the float is copied as a dword (edx), then the vector; no CTCBase::Copy call in retail.
class CThing;

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);    // 0x00722F30 for <CTCSmokeGenerator::CGenerationPos>
};
}

class CTCBase {                                             // engine/CTCBase.h is flattened (vptr as a data member)
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCSmokeGenerator : public CTCBase {                  // vtable 0x01265D74
public:
    class CGenerationPos;
    virtual void Copy(const CTCBase& other);
    float                               GenerationDistance;     // +0x0C
    std::vector<CGenerationPos>         GenerationPositions;    // +0x10
    bool                                PositionsCreated;       // +0x1C
};

void CTCSmokeGenerator::Copy(const CTCBase& other)
{
    const CTCSmokeGenerator& o = (const CTCSmokeGenerator&)other;
    GenerationDistance = o.GenerationDistance;
    GenerationPositions = o.GenerationPositions;
}