// CSoundBankLH::CSoundBankLH(long bankIndex) -- constructor over the CBaseClass root: own vtable, the bank index at +0xc, every other word zeroed and the flag byte at +0x1c cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0BAC0, length 47 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2F0 (stores vtable 0x0129A7C4). This class's vtable is 0x0129C9DC (rtti-vtables.tsv: CSoundBankLH);
// the manifest label "CEntityScriptInfo::CEntityScriptInfo" is propagated. Shape: the argument is read into ecx once; the zero register
// feeds +4/+8/+0x10 BEFORE the vtable store (plain initialiser-list members: VC7.1 hoists register-zero initialisers above the vtable
// store, and the argument store stays below it) while +0x14/+0x18/+0x1c come AFTER it: words zeroed after the vtable store belong to a
// member object with its own inline constructor (body assignments get hoisted just like plain initialisers), so +0x14..+0x1d is modelled
// as a 3-field record with an inline constructor.
struct CBaseClass {
    virtual void v00();
    CBaseClass();                                  // 0x0099A2F0
};

struct SBankCounters_00a0bac0 {                    // +0x14: two counters and a flag with an inline constructor
    long          m_count;                         // +0x00
    long          m_limit;                         // +0x04
    bool          m_flag;                          // +0x08
    SBankCounters_00a0bac0() : m_count(0), m_limit(0), m_flag(false) { }
};

struct CSoundBankLH : CBaseClass {                 // vtable 0x0129C9DC
    long                   m_4;                    // +0x04
    long                   m_8;                    // +0x08
    long                   m_bankIndex;            // +0x0c
    long                   m_10;                   // +0x10
    SBankCounters_00a0bac0 m_counters;             // +0x14..+0x1d
    virtual void v00();
    CSoundBankLH(long bankIndex);
};

CSoundBankLH::CSoundBankLH(long bankIndex)
    : CBaseClass()
    , m_4(0), m_8(0)
    , m_bankIndex(bankIndex)
    , m_10(0)
    , m_counters()
{
}
