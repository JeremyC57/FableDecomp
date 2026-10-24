// CAnimSet::SetAlpha(float alpha) -- clamp the requested alpha to [0, 1], roll the current alpha into the last one and store the new value
// Leaf: SetAlpha
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070BFC0, length 70 bytes. __fastcall, `ret 4`. Ego_r.pdb:
//         `?SetAlpha@CAnimSet@@QAEXM@Z`.
// Callees: none. Float pool: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Identity: the manifest label "Animation_ClampAlpha" is descriptive. Retail CAnimSet (Ego_r.pdb minus 8: the two 16-byte
//   vectors are 12 in retail): CurrentAlpha +0x2C, LastAlpha +0x30 -- the two floats it rolls.
// Shape: `if (alpha < 0.0f) alpha = 0.0f; else if (alpha > 1.0f) alpha = 1.0f;` (`fcom 0.0 ; test ah,5 ; jp`, then
//   `fcom 1.0 ; test ah,41h ; jne`), then `LastAlpha = CurrentAlpha; CurrentAlpha = alpha;` -- VC7.1 duplicates the two
//   stores into the zero arm (fstp st(0) ; fld 0.0) and lets the 1.0 arm fall into the shared tail.
struct CTransitionBlendAlpha {
    float Current;
    float Last;
};

template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};

class CAnimSet {
public:
    long UpdateMode;                                     // +0x00
    CRetailVector<short> Anims;                          // +0x04
    CRetailVector<CTransitionBlendAlpha> InternalAlphas; // +0x10
    float CurrentPercentageComplete;                     // +0x1C
    float LastPercentageComplete;                        // +0x20
    float Speed;                                         // +0x24
    unsigned long FrameInternalAlphasUpdated;            // +0x28
    float CurrentAlpha;                                  // +0x2C
    float LastAlpha;                                     // +0x30
    void SetAlpha(float alpha);
};

void CAnimSet::SetAlpha(float alpha)
{
    if (alpha < 0.0f)
        alpha = 0.0f;
    else if (alpha > 1.0f)
        alpha = 1.0f;
    LastAlpha = CurrentAlpha;
    CurrentAlpha = alpha;
}