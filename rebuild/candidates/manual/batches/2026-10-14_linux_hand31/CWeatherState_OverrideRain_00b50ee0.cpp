// CWeatherState::OverrideRain(float amount) -- a non-negative amount switches the rain override on with the amount clamped to the maximum at +0x12f8; a negative amount switches it off
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B50EE0, length 86 bytes.
// Callees: none. Constants: 0.0 (0x0122DEDC). Shape: `fcomp 0.0 ; test ah,1 ; jne` is the `>= 0` test (its false arm, the byte cleared, sits at the end);
// `fcomp [max] ; test ah,5 ; jp` is the ordered `amount < max`, and the flag store that follows the clamp is duplicated into both arms (between their `fld` and `fstp`). The
// chunk label "CEngine::OverrideRain" names the method; the object (amount/flag pairs at +0x12e8.. and the shared maximum) is the weather state, not CEngine
// (sizeof 0x188), so it is modelled locally.
struct CWeatherState {
    unsigned char _pad[0x12e8];
    float         m_rainAmount;                    // +0x12e8
    unsigned char m_rainOverride;                  // +0x12ec
    unsigned char _pad12ed[3];
    float         m_snowAmount;                    // +0x12f0
    unsigned char m_snowOverride;                  // +0x12f4
    unsigned char _pad12f5[3];
    float         m_maxAmount;                     // +0x12f8
    void OverrideRain(float amount);
};

void CWeatherState::OverrideRain(float amount)
{
    if (amount >= 0.0f) {
        m_rainAmount = amount < m_maxAmount ? amount : m_maxAmount;
        m_rainOverride = 1;
    } else {
        m_rainOverride = 0;
    }
}
