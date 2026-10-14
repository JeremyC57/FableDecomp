// CTrackedState_00863750::CTrackedState_00863750(void* pOwner) -- a non-polymorphic 0x28-byte record: a three-word member record (+0x18) cleared by its inline constructor, then the body stores the owner (+0x24) and clears the flag bytes and words (+0..+0x14) with the enabled byte (+0) set from the global disable flag being clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00863750, length 53 bytes (+ int3 padding). No callees; the global byte at 0x0137573C.
// The chunk label "CInputProcessJumpingAndRolling::Init" is propagated; `mov eax,ecx` first and `ret 4` with `this` in eax is a constructor.
// Shape: the three words are a member record cleared by its inline constructor (plain initialisers or body stores let the
// lane hoist the owner store above them; retail stores the zeros first), the owner pointer a body assignment; one zero register
// (ecx) feeds every clear; the global is compared against it (`cmp byte ptr [g],cl ; sete dl`) and the
// result byte stored at +0; the body stores follow the source order +0, +1, +8, +0xc, +0x14, +9, +4, +0x10.
// Leaf: ??0CTrackedState_00863750
extern unsigned char g_trackingDisabled;            // 0x0137573C
struct STrackWords_00863750 {                      // 12 bytes, cleared by its inline constructor
    long m_value18;
    long m_value1c;
    long m_value20;
    STrackWords_00863750() : m_value18(0), m_value1c(0), m_value20(0) { }
};
struct CTrackedState_00863750 {
    bool          m_enabled;                        // +0x00 (g_trackingDisabled == 0)
    bool          m_started;                        // +0x01
    unsigned char _pad02[2];
    long          m_value04;                        // +0x04
    bool          m_flag08;                         // +0x08
    bool          m_flag09;                         // +0x09
    unsigned char _pad0a[2];
    long          m_value0c;                        // +0x0c
    long          m_value10;                        // +0x10
    bool          m_flag14;                         // +0x14
    unsigned char _pad15[3];
    STrackWords_00863750 m_words;                   // +0x18 three words cleared by an inline constructor
    void*         m_pOwner;                         // +0x24
    CTrackedState_00863750(void* pOwner);
};

CTrackedState_00863750::CTrackedState_00863750(void* pOwner)
    : m_words()
{
    m_pOwner = pOwner;
    m_enabled = g_trackingDisabled == 0;
    m_started = false;
    m_flag08 = false;
    m_value0c = 0;
    m_flag14 = false;
    m_flag09 = false;
    m_value04 = 0;
    m_value10 = 0;
}
