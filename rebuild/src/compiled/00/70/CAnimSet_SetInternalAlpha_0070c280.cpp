// CAnimSet::SetInternalAlpha(long index, float alpha, bool setLastToo) -- rolls the indexed transition
// blend alpha (last = current, current = alpha, optionally last = alpha) and stamps the update frame.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C280, length 60 bytes.
// Callees: GFGetFrameCount 0x0049D870 (__fastcall, no arguments; the manifest label "GFGetPreMainMemoryUsed"
//          is propagated -- it is the frame counter compared in UpdateInternalAlphas 0x0070C2C0).
// Layout: retail std::vector is {first,last,end}; InternalAlphas sits at +0x10 (PDB +0x14 minus 4 for the
// 12-byte Anims vector before it), its element CTransitionBlendAlpha is {float Current; float Last};
// FrameInternalAlphasUpdated at +0x28 (PDB 48 minus 8). Each element access re-indexes through begin()
// (the stores may alias the vector pointers), which is why `[esi+10h]` is reloaded three times.
extern "C" long __fastcall GFGetFrameCount();          // 0x0049D870

class CTransitionBlendAlpha { public:
    float Current;
    float Last;
};

template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    T& operator[](unsigned long n) { return first[n]; }
    unsigned long size() const { return (unsigned long)(last - first); }
};

class CAnimSet { public:
    long UpdateMode;                                     // +0
    CRetailVector<short> Anims;                          // +4
    CRetailVector<CTransitionBlendAlpha> InternalAlphas; // +0x10
    float CurrentPercentageComplete;                     // +0x1c
    float LastPercentageComplete;                        // +0x20
    float Speed;                                         // +0x24
    unsigned long FrameInternalAlphasUpdated;            // +0x28
    void SetInternalAlpha(long index, float alpha, bool setLastToo);
};

void CAnimSet::SetInternalAlpha(long index, float alpha, bool setLastToo)
{
    InternalAlphas[index].Last = InternalAlphas[index].Current;
    InternalAlphas[index].Current = alpha;
    if (setLastToo)
        InternalAlphas[index].Last = alpha;
    FrameInternalAlphasUpdated = GFGetFrameCount();
}