// CObjectAugmentationParticleSet::~CObjectAugmentationParticleSet() -- release the EndPoint then the StartPoint arrays
// Leaf: ??1CObjectAugmentationParticleSet@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E25F1, 33 bytes, plain `ret` (__thiscall).
// Identity: Ego_r.pdb ??1CObjectAugmentationParticleSet@@QAE@XZ (non-virtual) and layout {float PeriodSecs,
// long ParticleEffect, ParticleEffectToBlendTo, AttackParticleEffect, AttackParticleEffectToBlendTo,
// CArray<CDefString> StartPoint, CArray<CDefString> EndPoint}; with retail's 12-byte STLport vectors the arrays sit at
// +0x14 and +0x20, the two blocks freed here in reverse member order. Callers include the
// CObjectAugmentationParticleSet transfer helper 0x004E9550 (its traits value) and the
// map<EObjectAugmentationType, CObjectAugmentationParticleSet> serialisers (0x004EC8B8, 0x004EC996..). CDefString
// (Ego_r.pdb: long TablePos) needs no destruction, so each array's destructor reduces to the block free. The old
// CBufferPair_004e25f1 name was descriptive.
// Callees: free @ 0x00BFEA14 (the STLport malloc allocator's deallocate).
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

class CDefString {
public:
    long TablePos;                                       // +0x00
};

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                       // +0x00
    _Tp* _M_finish;                                      // +0x04
    _Tp* _M_end_of_storage;                              // +0x08
    ~vector() { _Alloc().deallocate(_M_start, _M_end_of_storage - _M_start); }
};
}

template <class T> class CArray : public std::vector<T> {
};

class CObjectAugmentationParticleSet {
public:
    float              PeriodSecs;                       // +0x00
    long               ParticleEffect;                   // +0x04
    long               ParticleEffectToBlendTo;          // +0x08
    long               AttackParticleEffect;             // +0x0C
    long               AttackParticleEffectToBlendTo;    // +0x10
    CArray<CDefString> StartPoint;                       // +0x14
    CArray<CDefString> EndPoint;                         // +0x20
    ~CObjectAugmentationParticleSet();
};

CObjectAugmentationParticleSet::~CObjectAugmentationParticleSet()
{
}
