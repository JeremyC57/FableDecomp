// CLimiter::~CLimiter() -- destructor of the GameCODA output limiter: restores its vtable (0x012B8F64); nothing else to release
// Leaf: ??1CLimiter@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C592C0, 7 bytes (+ int3 padding), thiscall, plain `ret`.
// Retail bytes:  mov dword ptr [ecx],012B8F64h ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 441 "0xC592C0 = CLimiter dtor"):
//   - 0x012B8F64 is the CLimiter vtable: the landed CLimiter::CLimiter 0x00C59570 stores it, and its only slot is the
//     scalar deleting destructor 0x00C595A0 (same store, then the global operator delete 0x00BFE9BC);
//   - the limited mix buffer constructor 0x00C3EFA0 passes this function with CLimiter::CLimiter 0x00C59570 to
//     `__ehvec_ctor` for its six 0x17C-byte limiters at +0xC34 (the element destructor of the array).
//   The landed global CtorSetVtable012b8f64 (`T::~T`) was the right shape under a placeholder name; GameCODA has no PDB/RTTI.
// Shape: an out-of-line empty virtual destructor of a root class: the vtable store only.
class CLimiter {                                        // vtable 0x012B8F64, 0x17C bytes
public:
    virtual ~CLimiter();
    long          m_delay[88];                          // +0x004 delay line of mixed samples
    unsigned long m_writeIndex;                         // +0x164
    float         m_peak;                               // +0x168
    float         m_envelope;                           // +0x16c
    long          m_holdCount;                          // +0x170
    long          m_state;                              // +0x174 0 pass-through, 1 attack, 2 release
    long          m_releaseCount;                       // +0x178
};

CLimiter::~CLimiter()
{
}
