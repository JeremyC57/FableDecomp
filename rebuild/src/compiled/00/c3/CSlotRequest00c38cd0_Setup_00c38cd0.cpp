// CSlotRequest_00c38cd0::Setup(long owner0, long owner1, long a, long b, long c, long owner, long rangeLo, long rangeHi) -- resets the +0x138 range to (0,0) then sets it, starts the two +0xFC requests, marks the three slot indices invalid and the request active
// Leaf: Setup
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C38CD0, 135 bytes (+ int3 padding), `__fastcall`, `ret 20h` (eight dword arguments).
// Identity: NOT LHHttp2::ResetRequest (propagated label; that is 0x0083F980 and touches a string). The only caller is
// 0x00C2D299 (inside 0x00C2D260), which calls it on `this + index*0x180 + 0x18` of an array of 0x180-byte records, passing
// [owner+0x31C], a call result, the record's words +0x0C/+0x10/+0x14, the owner pointer, +0x18 and +0x00. The class is not
// identified; descriptive placeholder names.
// Callees: the +0x138 member's Set(lo, hi) @ 0x00C3A950 (thiscall, forwards to 0x00C0B570 as (lo, 0, hi)), the +0xFC member's
// two starts @ 0x00C484B0 and @ 0x00C48300 (thiscall, four arguments each, the last a member address).
// Shape: the two +0x138 calls share `lea edi`; the second member call reuses edi for +0xFC; the -1 chain stores the rightmost
// target first (+0x174, +0x178, +0x170), then the flag.
struct CRange {
    void Set(long lo, long hi);                                  // 0x00C3A950
};

struct CRequestPair {
    void StartFirst(long a, long b, long c, void* target);      // 0x00C484B0
    void StartSecond(long a, long b, long c, void* target);     // 0x00C48300
};

struct CSlotRequest_00c38cd0 {
    unsigned char _pad000[0x40];
    unsigned char m_secondTarget[0x6C];    // +0x040
    unsigned char m_firstTarget[0x4C];     // +0x0AC
    long          m_active;                // +0x0F8
    CRequestPair  m_requests;              // +0x0FC
    unsigned char _pad0fd[0x3B];
    CRange        m_range;                 // +0x138
    unsigned char _pad139[0x37];
    long          m_slotA;                 // +0x170
    long          m_slotB;                 // +0x174
    long          m_slotC;                 // +0x178
    void Setup(long owner0, long owner1, long a, long b, long c, long owner, long rangeLo, long rangeHi);
};

void CSlotRequest_00c38cd0::Setup(long owner0, long owner1, long a, long b, long c, long owner, long rangeLo, long rangeHi)
{
    m_range.Set(0, 0);
    m_range.Set(rangeLo, rangeHi);
    m_requests.StartFirst(owner0, owner1, owner, m_firstTarget);
    m_requests.StartSecond(a, b, c, m_secondTarget);
    m_slotA = m_slotC = m_slotB = -1;
    m_active = 1;
}