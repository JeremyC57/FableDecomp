// CTCAnimationComplex::GetAnimIndex() const -- the first animation index of the animator's main animation set
// Leaf: ?GetAnimIndex@CTCAnimationComplex@@QBEJXZ
// Retail: Fable.exe @ 0x0070B490, 10 bytes: `mov eax,[ecx+0Ch] ; mov ecx,[eax+4] ; movsx eax,word ptr [ecx] ; ret`.
// Identity (proven): Ego_r.pdb names ?GetAnimIndex@CTCAnimationComplex@@QBEJXZ and ?GetAnimIndex@CAnimSet@@QBEJXZ; the
// manifest label is right (neighbour of the landed GetAnimDuration 0x0070B4A0, which takes the index it returns); callers
// 0x008DBC10 / 0x008DBCE8 (creature-mode animation code).
// Layout: CTCAnimationComplex::PAnimation +0x0C (CAnimateThing*, Ego_r); CAnimateThing::CurrentAnim.MainAnimSet at +0;
// CAnimSet::Anims (std::vector<short>, STLport 4.6: _M_start first) at +0x04. The short is sign-extended to the long result.
namespace std {
template<class _Tp> class allocator { public: };
template<class _Tp, class _Alloc = allocator<_Tp> > class vector { public:     // STLport 4.6 layout (12 bytes)
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* begin() const { return _M_start; }
    const _Tp& operator[](unsigned int __n) const { return *(begin() + __n); }
};
}
class CAnimSet { public:
    long               UpdateMode;              // +0x00 EAnimUpdateMode
    std::vector<short> Anims;                   // +0x04
    long GetAnimIndex() const { return Anims[0]; }
};
class CCompleteAnimation { public:
    CAnimSet MainAnimSet;                       // +0x00
};
class CAnimateThing { public:
    CCompleteAnimation CurrentAnim;             // +0x00
};
class CThing;
class CTCBase { public:                         // engine/CTCBase.h layout, modelled polymorphic (landed 0x0070B4A0 model)
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCAnimationComplex : public CTCBase { public:
    CAnimateThing* PAnimation;                  // +0x0C
    long GetAnimIndex() const;
};

long CTCAnimationComplex::GetAnimIndex() const
{
    return PAnimation->CurrentAnim.MainAnimSet.GetAnimIndex();
}