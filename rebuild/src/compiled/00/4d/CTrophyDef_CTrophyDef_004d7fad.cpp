// CTrophyDef::~CTrophyDef() -- destroys the seven sound-name CCharStrings in reverse order (+0x60 .. +0x48), then the inlined CDefPointeeBase destructor stores its vtable and tail-calls the root destructor
// Leaf: ??1CTrophyDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D7FAD, length 73 bytes.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (seven times; manifest "_Cons_val<...>", propagated), the
// root CResource destructor @ 0x009FC550 (tail jump; the manifest labels it "~CMemoryBlock", propagated).
// Vtable 0x01230BA0 = CDefPointeeBase (rtti-vtables.tsv).
// Layout: engine/CTrophyDef.h (0x64 bytes, sound names at +0x48..+0x60); the header is a flat struct, so the
// class is modelled locally over the same offsets.
// Shape: the landed def-destructor family (0x007ACAC0, 0x00624DC0): no vtable store of its own -- the
// most-derived class and the CSubDefClassBase layer are `__declspec(novtable)` with inline empty
// destructors, CDefPointeeBase's inline destructor keeps its store, and a non-virtual destructor next to a
// virtual function keeps the plain `??1` as the TU's body.
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};

class CResource { public:                           // the root base
    virtual void v00();
    virtual ~CResource();                            // 0x009FC550
};

class CDefPointeeBase : public CResource { public:         // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }
};

struct __declspec(novtable) CSubDefClassBase : CDefPointeeBase {
    unsigned char _body[0x24];                     // +0x04 .. +0x28
    ~CSubDefClassBase() { }
};

class CDefString { public:
    long TablePos;
};

struct __declspec(novtable) CTrophyDef : CSubDefClassBase {
    float         MinigameInitialTimerSecs;        // +0x28
    float         TimeBonusPerWitness;             // +0x2c
    long          RenownGainedPerWitnessAhead;     // +0x30
    bool          EvilTrophy;                      // +0x34
    bool          FakeTrophy;                      // +0x35
    long          EmoteIconForAvailableWitness;    // +0x38
    CDefString    ShowTrophyAnimName;              // +0x3c
    float         GameDebugTextScale;              // +0x40
    float         SecsDelayPerGainedWitnessSound;  // +0x44
    CCharString   SoundStartGame;                  // +0x48
    CCharString   SoundShowTrophy;                 // +0x4c
    CCharString   SoundGainedWitness;              // +0x50
    CCharString   SoundOutOfTime;                  // +0x54
    CCharString   SoundGotAllWitnesses;            // +0x58
    CCharString   SoundEndNewHighScore;            // +0x5c
    CCharString   SoundEndNoNewScore;              // +0x60
    virtual void v00();
    ~CTrophyDef();
};

CTrophyDef::~CTrophyDef()
{
}