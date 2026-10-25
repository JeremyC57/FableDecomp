// CInstanceManager::Update() -- update every busy instance with the three update words (+0x0C/+0x10/+0x14); when one goes idle while a request is queued, start the request on that instance (UseInstance), make it current, update it once more and clear the queue flag
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D490, length 144 bytes (130 + int3 padding), __fastcall, plain `ret`.
// Caller: 0x00C15CA4. Callees: CInstance::IsIdle @ 0x00C38840, CInstance::Update @ 0x00C38D60 (`ret 0Ch`),
//   UseInstance(index, request) @ 0x00C2D260 (same TU; the landed Assign's "Use").
// Identity: libpk's CInstanceManager (the file string `.\\CInstanceManager.cpp` of the landed Assign @ 0x00C2D5E0): two
//   0x180-byte instances at +0x18 (state word +0xF8; volume CRampedParam at +0x138, wrap flag +0x168), the current index
//   +0x318, a queued request +0x320 (0x1c bytes) with its flag +0x33c; the destructor @ 0x00C2D3B0 fades out with time 0
//   (0x00C2D210) and destroys the two instances through `eh vector destructor iterator` (element destructor 0x00C38B20).
//   No PDB covers libpk: the member and method names are descriptive.
// Shape: an unsigned index counted up (`inc edi ; cmp edi,2 ; jb`) because UseInstance and the current-index store need it,
//   the instance pointer in ebp; the three words re-read for each Update call.
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
    void UseInstance(unsigned int index, SThemeRequest* request);   // 0x00C2D260
    void Update();
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

void CInstanceManager::Update()
{
    for (unsigned int i = 0; i < 2; ++i) {
        CInstance& instance = m_instances[i];
        if (!instance.IsIdle()) {
            instance.Update(m_updateA, m_updateB, m_updateC);
            if (instance.IsIdle() && m_bPending) {
                UseInstance(i, &m_pending);
                m_current = i;
                instance.Update(m_updateA, m_updateB, m_updateC);
                m_bPending = false;
            }
        }
    }
}