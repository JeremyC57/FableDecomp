// CBurningPerson::Init() -- set the person's thing on fire through the game interface and reset the speech counter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EED580, length 29 bytes (+ int3 padding).
// Callees: the game interface at +0x04, virtual slot 477 (+0x774) with (&m_me, 1).
// Layout from Ego_r.pdb (CBurningPerson, 32 bytes): SpeakCounter +0x1c; the interface pointer at
// +0x04 and the CScriptThing at +0x08 belong to the script base the PDB leaves unnamed.
#define SCRIPT_SLOT(n) virtual void slot##n()
#define SCRIPT_SLOTS10(d) SCRIPT_SLOT(d##0); SCRIPT_SLOT(d##1); SCRIPT_SLOT(d##2); SCRIPT_SLOT(d##3); SCRIPT_SLOT(d##4); SCRIPT_SLOT(d##5); SCRIPT_SLOT(d##6); SCRIPT_SLOT(d##7); SCRIPT_SLOT(d##8); SCRIPT_SLOT(d##9)

struct CScriptThing { long m_id; long m_uid; long m_type; };   // 12 bytes

struct CGameInterface {                         // only the slot used is typed
    SCRIPT_SLOTS10(0); SCRIPT_SLOTS10(1); SCRIPT_SLOTS10(2); SCRIPT_SLOTS10(3); SCRIPT_SLOTS10(4);
    SCRIPT_SLOTS10(5); SCRIPT_SLOTS10(6); SCRIPT_SLOTS10(7); SCRIPT_SLOTS10(8); SCRIPT_SLOTS10(9);
    SCRIPT_SLOTS10(10); SCRIPT_SLOTS10(11); SCRIPT_SLOTS10(12); SCRIPT_SLOTS10(13); SCRIPT_SLOTS10(14);
    SCRIPT_SLOTS10(15); SCRIPT_SLOTS10(16); SCRIPT_SLOTS10(17); SCRIPT_SLOTS10(18); SCRIPT_SLOTS10(19);
    SCRIPT_SLOTS10(20); SCRIPT_SLOTS10(21); SCRIPT_SLOTS10(22); SCRIPT_SLOTS10(23); SCRIPT_SLOTS10(24);
    SCRIPT_SLOTS10(25); SCRIPT_SLOTS10(26); SCRIPT_SLOTS10(27); SCRIPT_SLOTS10(28); SCRIPT_SLOTS10(29);
    SCRIPT_SLOTS10(30); SCRIPT_SLOTS10(31); SCRIPT_SLOTS10(32); SCRIPT_SLOTS10(33); SCRIPT_SLOTS10(34);
    SCRIPT_SLOTS10(35); SCRIPT_SLOTS10(36); SCRIPT_SLOTS10(37); SCRIPT_SLOTS10(38); SCRIPT_SLOTS10(39);
    SCRIPT_SLOTS10(40); SCRIPT_SLOTS10(41); SCRIPT_SLOTS10(42); SCRIPT_SLOTS10(43); SCRIPT_SLOTS10(44);
    SCRIPT_SLOTS10(45); SCRIPT_SLOTS10(46);
    SCRIPT_SLOT(470); SCRIPT_SLOT(471); SCRIPT_SLOT(472); SCRIPT_SLOT(473); SCRIPT_SLOT(474); SCRIPT_SLOT(475); SCRIPT_SLOT(476);
    virtual void SetThingOnFire(const CScriptThing& thing, long on);   // slot 477 (+0x774)
};

struct CBurningPerson {
    void*           _vptr;                      // +0x00
    CGameInterface* m_pGame;                    // +0x04
    CScriptThing    m_me;                       // +0x08
    unsigned char   _pad[8];                    // +0x14 ParentClass, +0x18 PMasterData
    long            SpeakCounter;               // +0x1c
    void Init();
};

void CBurningPerson::Init()
{
    m_pGame->SetThingOnFire(m_me, 1);
    SpeakCounter = 0;
}