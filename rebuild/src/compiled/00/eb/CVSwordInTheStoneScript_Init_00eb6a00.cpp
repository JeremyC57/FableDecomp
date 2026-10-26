// CV_SwordInTheStoneScript::Init() -- clear the sword/crowd state and give the two per-slot vectors two empty entries each (CScriptThing at +0x60, scripted-thing handles at +0x7c)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EB6A00, length 83 bytes.
// Callees: CScriptThing::CScriptThing() @ 0x006E7B40 and ~CScriptThing @ 0x004AA840, the thing vector's
// resize(unsigned, const CScriptThing&) @ 0x00D34AC0; CScriptGameResourceObjectScriptedThingBase::
// CScriptGameResourceObjectScriptedThingBase() @ 0x007E72A0 and its destructor @ 0x007E74D0, the handle
// vector's resize @ 0x00CD3CCB (the manifest labels on all four resize/ctor rows are propagated template names).
// Shape: a 16-byte frame shared by the two full-expression temporaries (12 and 16 bytes); each explicit
// temporary `T()` reuses the constructor's returned pointer for the push (`push eax`), is destroyed right after
// its resize, and the four scalar clears (two words, two bytes from the zero register) come first.
namespace N_006e7b40 { struct CScriptThing {                              // 12 bytes
    unsigned char _body[0xc];
    CScriptThing();                                // 0x006E7B40
    ~CScriptThing();                               // 0x004AA840
}; } using N_006e7b40::CScriptThing;

class CScriptGameResourceObjectScriptedThingBase { public:   // 16 bytes
    unsigned char _body[0x10 - 4];
    CScriptGameResourceObjectScriptedThingBase();     // 0x007E72A0
    virtual ~CScriptGameResourceObjectScriptedThingBase();    // 0x007E74D0
};

struct CScriptThingVector {                        // 12-byte retail vector
    CScriptThing* m_pFirst; CScriptThing* m_pLast; CScriptThing* m_pEnd;
    void resize(unsigned int n, const CScriptThing& value);                               // 0x00D34AC0
};

struct CScriptedThingVector {                      // 12-byte retail vector
    CScriptGameResourceObjectScriptedThingBase* m_pFirst; CScriptGameResourceObjectScriptedThingBase* m_pLast; CScriptGameResourceObjectScriptedThingBase* m_pEnd;
    void resize(unsigned int n, const CScriptGameResourceObjectScriptedThingBase& value);   // 0x00CD3CCB
};

struct CV_SwordInTheStoneScript {
    virtual void v00();
    unsigned char        _pad04[0x44];             // +0x04..+0x47
    long                 m_swordState;             // +0x48
    long                 m_crowdState;             // +0x4c
    unsigned char        m_swordTaken;             // +0x50
    unsigned char        m_crowdGathered;          // +0x51
    unsigned char        _pad52[0xe];              // +0x52..+0x5f
    CScriptThingVector   m_things;                 // +0x60
    unsigned char        _pad6c[0x10];             // +0x6c..+0x7b
    CScriptedThingVector m_scriptedThings;         // +0x7c
    void Init();
};

void CV_SwordInTheStoneScript::Init()
{
    m_swordState = 0;
    m_crowdState = 0;
    m_swordTaken = 0;
    m_crowdGathered = 0;
    m_things.resize(2, CScriptThing());
    m_scriptedThings.resize(2, CScriptGameResourceObjectScriptedThingBase());
}