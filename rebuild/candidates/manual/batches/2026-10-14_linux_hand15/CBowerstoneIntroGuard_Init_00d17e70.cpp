// CBowerstoneIntroGuard::Init() -- two script-interface calls around a flag clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D17E70, length 40 bytes (+ int3 padding).
// Callees: the script interface at +0x40, virtual slots 321 (+0x504, one zero argument) and 338
// (+0x548, the long at +0x7c as argument); the byte at +0x84 is cleared between the two calls.
// Shape: the interface pointer is re-read for the second call (the first call may replace it);
// the flag store is scheduled between the second call's loads. The manifest name is kept
// (0x00D1xxxx quest-script module); the PDB's 28-byte CBowerstoneIntroGuard (Ego_r) does not
// cover +0x40/+0x7c/+0x84, so the members are modelled locally.
#define SCRIPT_SLOT(n) virtual void slot##n()
#define SCRIPT_SLOTS10(d) SCRIPT_SLOT(d##0); SCRIPT_SLOT(d##1); SCRIPT_SLOT(d##2); SCRIPT_SLOT(d##3); SCRIPT_SLOT(d##4); SCRIPT_SLOT(d##5); SCRIPT_SLOT(d##6); SCRIPT_SLOT(d##7); SCRIPT_SLOT(d##8); SCRIPT_SLOT(d##9)

struct CScriptInterface {                       // only the two slots used are typed
    SCRIPT_SLOTS10(0); SCRIPT_SLOTS10(1); SCRIPT_SLOTS10(2); SCRIPT_SLOTS10(3); SCRIPT_SLOTS10(4);
    SCRIPT_SLOTS10(5); SCRIPT_SLOTS10(6); SCRIPT_SLOTS10(7); SCRIPT_SLOTS10(8); SCRIPT_SLOTS10(9);
    SCRIPT_SLOTS10(10); SCRIPT_SLOTS10(11); SCRIPT_SLOTS10(12); SCRIPT_SLOTS10(13); SCRIPT_SLOTS10(14);
    SCRIPT_SLOTS10(15); SCRIPT_SLOTS10(16); SCRIPT_SLOTS10(17); SCRIPT_SLOTS10(18); SCRIPT_SLOTS10(19);
    SCRIPT_SLOTS10(20); SCRIPT_SLOTS10(21); SCRIPT_SLOTS10(22); SCRIPT_SLOTS10(23); SCRIPT_SLOTS10(24);
    SCRIPT_SLOTS10(25); SCRIPT_SLOTS10(26); SCRIPT_SLOTS10(27); SCRIPT_SLOTS10(28); SCRIPT_SLOTS10(29);
    SCRIPT_SLOTS10(30); SCRIPT_SLOTS10(31);
    SCRIPT_SLOT(320);
    virtual void SetMode(long mode);            // slot 321 (+0x504)
    SCRIPT_SLOT(322); SCRIPT_SLOT(323); SCRIPT_SLOT(324); SCRIPT_SLOT(325); SCRIPT_SLOT(326); SCRIPT_SLOT(327); SCRIPT_SLOT(328); SCRIPT_SLOT(329);
    SCRIPT_SLOT(330); SCRIPT_SLOT(331); SCRIPT_SLOT(332); SCRIPT_SLOT(333); SCRIPT_SLOT(334); SCRIPT_SLOT(335); SCRIPT_SLOT(336); SCRIPT_SLOT(337);
    virtual void SetTarget(long thing);         // slot 338 (+0x548)
};

struct CBowerstoneIntroGuard {
    unsigned char     _pad0[0x40];              // +0x00..+0x3f
    CScriptInterface* m_pScript;                // +0x40
    unsigned char     _pad1[0x38];              // +0x44..+0x7b
    long              m_target;                 // +0x7c
    unsigned char     _pad2[4];                 // +0x80
    bool              m_done;                   // +0x84
    void Init();
};

void CBowerstoneIntroGuard::Init()
{
    m_pScript->SetMode(0);
    m_done = false;
    m_pScript->SetTarget(m_target);
}
