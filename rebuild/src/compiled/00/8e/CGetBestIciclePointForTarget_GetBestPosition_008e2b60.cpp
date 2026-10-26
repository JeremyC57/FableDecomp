// CGetBestIciclePointForTarget::GetBestPosition(C3DVector* out) -- when the score at +0xc is above zero, copy the position at +0x10 out and return true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E2B60, length 49 bytes.
// Callees: none. Constant 0.0f @ 0x0122DEDC. Shape: `fcomp ; fnstsw ; test ah,41h ; jne` (not above zero -> false), the vector copied as three words, `ret 4`.
extern const float g_zero_0122dedc;              // 0x0122DEDC (0.0f)
class C3DVector { public: float X, Y, Z; };

struct CGetBestIciclePointForTarget {
    unsigned char _pad[0xc];
    float         m_bestScore;                     // +0x0c
    C3DVector     m_bestPosition;                  // +0x10
    bool GetBestPosition(C3DVector* out);
};

bool CGetBestIciclePointForTarget::GetBestPosition(C3DVector* out)
{
    if (m_bestScore > g_zero_0122dedc) {
        *out = m_bestPosition;
        return true;
    }
    return false;
}