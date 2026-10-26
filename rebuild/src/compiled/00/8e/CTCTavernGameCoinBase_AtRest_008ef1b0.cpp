// CTCTavernGameCoinBase::AtRest(float speedThreshold) -- true when every coin's planar velocity is within the threshold (|v|^2 <= t^2), false at the first faster coin
// Retail: Fable.exe @ 0x008EF1B0, 80 bytes (`ret 4`), no callees.
// The coins are a retail 12-byte vector of 0x3C-byte Coin records at +0xFC/+0x100 (the same vector
// UpdateCoinSounds 0x008EF910 walks); each record holds its 2D velocity at +0x0C/+0x10.
// Shape: the squared threshold is written inside the loop and hoisted after the loop-entry test by
//        VC7.1, which homes it in the dead argument slot (`fld ; fmul ; fstp [esp+4]`); the squared
//        speed `x*x + y*y` loads y first (right operand), squares each through stack copies;
//        `fcomp ; test ah,41h ; je` = `>` early return.
class C2DVector { public:
    float x;
    float y;
    float GetLengthSquared() const { return x * x + y * y; }
};

struct CTavernCoin {                    // 0x3C bytes
    unsigned char _pad[0xc];
    C2DVector m_velocity;               // +0x0c
    unsigned char _pad_0x14[0x28];
};

struct CTCTavernGameCoinBase {
    unsigned char _pad[0xfc];
    CTavernCoin* m_coinsFirst;          // +0xfc vector<Coin> begin
    CTavernCoin* m_coinsLast;           // +0x100 end
    bool AtRest(float speedThreshold);
};

bool CTCTavernGameCoinBase::AtRest(float speedThreshold)
{
    for (CTavernCoin* coin = m_coinsFirst; coin != m_coinsLast; ++coin) {
        if (coin->m_velocity.GetLengthSquared() > speedThreshold * speedThreshold)
            return false;
    }
    return true;
}