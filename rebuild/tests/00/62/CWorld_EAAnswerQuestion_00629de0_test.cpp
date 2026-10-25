// Behaviour test for 0x00629DE0: CWorld::EAAnswerQuestion(const CGameEvent* event) -- game-event action: read the answer from the event, post message 0x10 carrying it after 50 frames, and while the subtitle box is displaying text play the A (answer 1) or B button sound and clear the subtitles
// A copy of the source model and logic (compiled separately from src/00629de0.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif
class CCharString { public: void* PStringData; };
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CThing;
class CASoundBank;
class CSoundInit {
public:
    float Volume, Pitch, Pan;
    long NoPlays, PlaybackType;
    unsigned long GainFaderBits, PitchFaderBits;
    long OverrideID;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PitchFaderBits(0), OverrideID(-1)
    {
        PlaybackType = 1;
        GainFaderBits = 0;
    }
};
class CSoundPlayer {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, CSoundInit& init, bool attached);   // slot 4 (+0x10)
};
class CSoundSystem {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual CSoundPlayer* GetSoundPlayer();           // slot 9 (+0x24)
};
extern CSoundSystem* g_pSoundSystem;                   // 0x013B8394
class CPlayerGuiDef {
public:
    unsigned char _pad[0x5e8];
    CCharString   TextBoxButtonASoundCriteria;        // +0x5E8 (PDB +0x624)
    CCharString   TextBoxButtonBSoundCriteria;        // +0x5EC (PDB +0x628)
};
CPlayerGuiDef* __fastcall PeekGuiDef();                // 0x00438570
class CSubtitleRenderer {
public:
    unsigned char _pad[0xe5];
    bool          DisplayingText;                      // +0xE5 (PDB +0xED)
    void Clear(bool all);                              // 0x00492BAB
};
class CPlayerGui {
public:
    unsigned char     _pad[0x18];
    CSubtitleRenderer SubtitleRenderer;                // +0x18
};
extern CPlayerGui* g_pPlayerGui;                       // 0x013B8790
class CMessageEventManager {
public:
    void* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data);   // 0x00687540
};
class CMemoryBuffer {
public:
    unsigned char Data[0x20];                          // +0x00
    long          Length;                              // +0x20
    long          ReadPos;                             // +0x24
    long          WritePos;                            // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);       // 0x009F18F0
    __forceinline long ReadLong() { long value = 0; Read(4, &value); return value; }
};
class CGameEvent {
public:
    long          Type;                                // +0x00
    signed char   Player;                              // +0x04
    unsigned char Data[32];                            // +0x05
    unsigned char EndPos;                              // +0x25
    bool          Valid;                               // +0x26
    bool          Replacement;                         // +0x27
    long GetData(CMemoryBuffer& buffer) const;         // 0x009F17A0
};
class CWorld {
public:
    unsigned char         _pad00[0x60];
    CMessageEventManager* PMessageEventManager;        // +0x60 (CCountedPointer data word)
    CASoundBank& GetSampleBank() const;                // 0x0049D9B0
    void EAAnswerQuestion(const CGameEvent* event);
};

void CWorld::EAAnswerQuestion(const CGameEvent* event)
{
    CMemoryBuffer buffer;
    event->GetData(buffer);
    long answer = buffer.ReadLong();
    PMessageEventManager->AddMessage(0x10, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, answer);
    CSubtitleRenderer& subtitles = g_pPlayerGui->SubtitleRenderer;
    if (subtitles.DisplayingText) {
        CPlayerGuiDef* def = PeekGuiDef();
        CSoundInit init;
        init.GainFaderBits = 0x8010;
        if (answer == 1)
            g_pSoundSystem->GetSoundPlayer()->PlaySoundEvent(GetSampleBank(), def->TextBoxButtonASoundCriteria, true, init, false);
        else
            g_pSoundSystem->GetSoundPlayer()->PlaySoundEvent(GetSampleBank(), def->TextBoxButtonBSoundCriteria, true, init, false);
        subtitles.Clear(false);
    }
}

// ---- test stand-ins ----
static int g_eventAnswer = 0;
long CGameEvent::GetData(CMemoryBuffer& buffer) const
{
    std::memcpy(buffer.Data, &g_eventAnswer, 4);
    buffer.Length = 4; buffer.ReadPos = 0;
    return 4;
}
void CMemoryBuffer::Read(unsigned long length, void* data)
{
    std::memcpy(data, Data + ReadPos, length);
    ReadPos += (long)length;
}

static int g_messages = 0;
static long g_msgType = 0, g_msgData = 0;
static unsigned long g_msgDelay = 0;
static float g_msgX = 1.0f, g_msgY = 1.0f, g_msgZ = 1.0f;
void* CMessageEventManager::AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data)
{
    ++g_messages; g_msgType = type; g_msgDelay = delay; g_msgData = data;
    g_msgX = position.X; g_msgY = position.Y; g_msgZ = position.Z;
    if (from || to || value1 != 0.0f || value2 != 0.0f) g_msgType = -1;
    return 0;
}

static CPlayerGuiDef g_def;
static int g_peeks = 0;
CPlayerGuiDef* __fastcall PeekGuiDef() { ++g_peeks; return &g_def; }

static int g_clears = 0;
static bool g_clearAll = true;
void CSubtitleRenderer::Clear(bool all) { ++g_clears; g_clearAll = all; }

static int g_plays = 0;
static const CCharString* g_playedName = 0;
static unsigned long g_playedFader = 0;
static bool g_playedLoop = false, g_playedAttached = true;
static CASoundBank* g_bank = (CASoundBank*)&g_def;
void CSoundPlayer::v000() { } void CSoundPlayer::v001() { } void CSoundPlayer::v002() { } void CSoundPlayer::v003() { }
unsigned long CSoundPlayer::PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, CSoundInit& init, bool attached)
{
    ++g_plays; g_playedName = &name; g_playedLoop = loop; g_playedAttached = attached; g_playedFader = init.GainFaderBits;
    if (&bank != g_bank || init.NoPlays != 1 || init.PlaybackType != 1 || init.OverrideID != -1) g_playedFader = 0;
    return 0;
}
static CSoundPlayer g_player;
void CSoundSystem::v000() { } void CSoundSystem::v001() { } void CSoundSystem::v002() { } void CSoundSystem::v003() { }
void CSoundSystem::v004() { } void CSoundSystem::v005() { } void CSoundSystem::v006() { } void CSoundSystem::v007() { }
void CSoundSystem::v008() { }
CSoundPlayer* CSoundSystem::GetSoundPlayer() { return &g_player; }
static CSoundSystem g_system;
CSoundSystem* g_pSoundSystem = &g_system;
static CPlayerGui g_gui;
CPlayerGui* g_pPlayerGui = &g_gui;
CASoundBank& CWorld::GetSampleBank() const { return *g_bank; }

static int Fail(int line) { std::printf("00629de0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)

int main()
{
    CMessageEventManager manager;
    static CWorld world;
    world.PMessageEventManager = &manager;
    CGameEvent event;
    event.Type = 3; event.Player = 0;

    // no text on screen: only the message
    g_gui.SubtitleRenderer.DisplayingText = false;
    g_eventAnswer = 1;
    world.EAAnswerQuestion(&event);
    CHECK(g_messages == 1 && g_msgType == 0x10 && g_msgDelay == 0x32 && g_msgData == 1);
    CHECK(g_msgX == 0.0f && g_msgY == 0.0f && g_msgZ == 0.0f);
    CHECK(g_peeks == 0 && g_plays == 0 && g_clears == 0);

    // answer 1 with text: button A sound
    g_gui.SubtitleRenderer.DisplayingText = true;
    world.EAAnswerQuestion(&event);
    CHECK(g_messages == 2 && g_peeks == 1 && g_plays == 1);
    CHECK(g_playedName == &g_def.TextBoxButtonASoundCriteria && g_playedLoop && !g_playedAttached && g_playedFader == 0x8010);
    CHECK(g_clears == 1 && g_clearAll == false);

    // any other answer: button B sound
    g_eventAnswer = 0;
    world.EAAnswerQuestion(&event);
    CHECK(g_messages == 3 && g_msgData == 0 && g_plays == 2);
    CHECK(g_playedName == &g_def.TextBoxButtonBSoundCriteria && g_playedFader == 0x8010);
    g_eventAnswer = 2;
    world.EAAnswerQuestion(&event);
    CHECK(g_msgData == 2 && g_playedName == &g_def.TextBoxButtonBSoundCriteria && g_clears == 3);
    std::printf("00629de0_TEST PASS\n");
    return 0;
}