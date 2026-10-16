// CAC_Owner::GetRandomSpeech() -- pick one of 5 owner speech lines at random, never either of the last two, yielding a script frame per rejected pick; the empty string when the script is done
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E2E440, length 213 bytes (`ret 4`, CCharString by hidden pointer).
// Class: manifest label CAC_Owner kept (an entity-script class; the same body sits in three script modules at
// 0x00E2E440 / 0x00E362B0 / 0x00E53CB0 with different counts and table offsets -- the label is propagated there).
// Callees: CCharString default ctor @ 0x0099E4B0, copy ctor @ 0x0099EC30, operator= @ 0x0099EFB0, dtor @ 0x0099EAE0,
// rand @ 0x00BFEB16, the script's IsDone @ 0x00F35B30, the game script interface (+4) slot 7 (+0x1c).
// Shape: `for (;;) { if (IsDone()) return none; pick; if (ok) break; yield; }` then IsDone re-checked around the
// bookkeeping (a `while (!IsDone())` loop flips the branch layout); an empty string, a copy of it returned on every IsDone exit, and the chosen line copied out last (no NRVO:
// each return copy-constructs the hidden result); the line table is a CCharString array at +0x80 of the record (+0x14).
#define SPEECH_COUNT 5
#define SPEECH_TABLE_OFFSET 0x80
struct CCharString
{
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

extern "C" int __cdecl rand();                      // 0x00BFEB16 (MSVCR71)

struct CGameScriptInterface
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06();
    virtual void NewScriptFrame();                  // slot 7 (+0x1c): yield one frame
};

struct CSpeechRecord                                // the owner's speech table
{
    unsigned char _pad00[SPEECH_TABLE_OFFSET];
    CCharString Lines[SPEECH_COUNT];
};

struct CAC_Owner
{
    virtual void v00();
    CGameScriptInterface* GSI;                      // +0x04
    unsigned char _pad08[0x14 - 0x08];
    CSpeechRecord* Record;                          // +0x14
    unsigned char _pad18[0x1c - 0x18];
    long LastSpeech;                                // +0x1c
    long PreviousSpeech;                            // +0x20
    bool IsDone();                                  // 0x00F35B30 (the entity script's IsDone; manifest label is propagated)
    CCharString GetRandomSpeech();
};

CCharString CAC_Owner::GetRandomSpeech()
{
    CCharString empty;
    CCharString none(empty);
    CCharString speech;
    long index;
    for (;;)
    {
        if (IsDone())
            return none;
        index = rand() % SPEECH_COUNT;
        if (index != LastSpeech && index != PreviousSpeech)
            break;
        GSI->NewScriptFrame();
    }
    if (IsDone())
        return none;
    PreviousSpeech = LastSpeech;
    LastSpeech = index;
    if (IsDone())
        return none;
    speech = Record->Lines[index];
    return speech;
}
