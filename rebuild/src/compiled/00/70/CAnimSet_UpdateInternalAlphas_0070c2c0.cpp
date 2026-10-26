// CAnimSet::UpdateInternalAlphas() -- once per frame, copies every transition blend alpha's Current into Last
// and stamps the frame the roll happened on.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C2C0, length 67 bytes.
// Callees: GFGetFrameCount 0x0049D870 (__fastcall, no arguments; twice -- the guard and the stamp).
// Layout: as 0x0070C280 -- InternalAlphas is a retail 12-byte vector at +0x10 of {Current, Last} elements,
// FrameInternalAlphasUpdated at +0x28. The guard is an unsigned compare (`jae`), the loop re-derives
// size() = (last-first)>>3 every iteration because the element stores may alias the vector pointers.
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
    void UpdateInternalAlphas();
};

void CAnimSet::UpdateInternalAlphas()
{
    if (FrameInternalAlphasUpdated < (unsigned long)GFGetFrameCount()) {
        for (unsigned long i = 0; i < InternalAlphas.size(); ++i)
            InternalAlphas[i].Last = InternalAlphas[i].Current;
        FrameInternalAlphasUpdated = GFGetFrameCount();
    }
}