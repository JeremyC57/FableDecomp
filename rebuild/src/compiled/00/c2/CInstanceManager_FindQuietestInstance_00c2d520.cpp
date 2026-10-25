// CInstanceManager::FindQuietestInstance() -- of the busy instances, return the one whose volume (the +0x138 ramped value, angle-wrapped when flagged) is lowest; null when both are idle
// Leaf: FindQuietestInstance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D520, length 192 bytes (181 + int3 padding), __fastcall, plain `ret`.
// Caller: Assign @ 0x00C2D5E0 (call 0x00C2D62D: the instance to take over when neither is free; the landed source calls it
//   "Create"). Callees: CInstance::IsIdle @ 0x00C38840. Constants: pi 0x0128F604, 2*pi 0x0128F608, -pi 0x012B1EB8.
// Identity: libpk's CInstanceManager (the file string `.\\CInstanceManager.cpp` of the landed Assign @ 0x00C2D5E0): two
//   0x180-byte instances at +0x18 (state word +0xF8; volume CRampedParam at +0x138, wrap flag +0x168), the current index
//   +0x318, a queued request +0x320 (0x1c bytes) with its flag +0x33c; the destructor @ 0x00C2D3B0 fades out with time 0
//   (0x00C2D210) and destroys the two instances through `eh vector destructor iterator` (element destructor 0x00C38B20).
//   No PDB covers libpk: the member and method names are descriptive.
// Shape: CRampedParam::GetValue (out of line at 0x00C0B730) inlined twice in register form; the candidate's value is
//   loaded first, the best's second, then `fxch ; fcomp st(1)` and the ordered `<` (`test ah,5 ; jp`); the first busy
//   instance is taken without a compare (`test edi,edi ; je`); a down-counter for the unused index.
class CRampedParam {                        // 0x34 bytes (chunk 431's model; SetTarget @ 0x00C0B570)
public:
    void RampTo(float target, float time);  // 0x00C3A950 (SetTarget(target, 0, time))
    float GetValue() const                  // the angle wrap of 0x00C0B730, inlined
    {
        float value = m_value;
        if (m_bAngle) {
            if (value >= kPi)
                return value - kTwoPi;
            if (value < kMinusPi)
                return value + kTwoPi;
        }
        return value;
    }
    static const float kPi;                 // 0x0128F604
    static const float kTwoPi;              // 0x0128F608
    static const float kMinusPi;            // 0x012B1EB8
    float         m_value;                  // +0x00 current value
    unsigned char _pad_0x04[0x2c];
    bool          m_bAngle;                 // +0x30 wrap the value into [-pi, pi)
};

class CInstance {                           // 0x180 bytes: one playing instance (state word +0xF8)
public:
    bool IsIdle() const;                    // 0x00C38840 (state == 0)
    bool IsFinished() const;                // 0x00C38830 (state == 6)
    void Start();                           // 0x00C38A20 (state = 5)
    void Update(long a, long b, long c);    // 0x00C38D60
    long GetInfo170() const;                // 0x00C38890
    long GetInfo174() const;                // 0x00C38880
    long GetInfo178() const;                // 0x00C38870
    unsigned char _pad_0x000[0x138];
    CRampedParam  m_volume;                 // +0x138
    unsigned char _pad_0x16c[0x14];
};

struct SThemeRequest {                      // 0x1c bytes (the landed Assign 0x00C2D5E0 copies it member by member)
    long m_words[7];
};

class CInstanceManager {                    // libpk, .\CInstanceManager.cpp
public:
    CInstance* FindQuietestInstance();
    unsigned char _pad_0x000[0x0c];
    long          m_updateA;                // +0x0c
    long          m_updateB;                // +0x10
    long          m_updateC;                // +0x14
    CInstance     m_instances[2];           // +0x18
    long          m_current;                // +0x318 index of the current instance
    unsigned char _pad_0x31c[4];
    SThemeRequest m_pending;                // +0x320 the queued request
    bool          m_bPending;               // +0x33c a request is queued
};

const float CRampedParam::kPi = 3.14159265f;
const float CRampedParam::kTwoPi = 6.28318531f;
const float CRampedParam::kMinusPi = -3.14159265f;

CInstance* CInstanceManager::FindQuietestInstance()
{
    CInstance* best = 0;
    for (int i = 0; i < 2; ++i) {
        CInstance* instance = &m_instances[i];
        if (!instance->IsIdle()) {
            if (!best || instance->m_volume.GetValue() < best->m_volume.GetValue())
                best = instance;
        }
    }
    return best;
}