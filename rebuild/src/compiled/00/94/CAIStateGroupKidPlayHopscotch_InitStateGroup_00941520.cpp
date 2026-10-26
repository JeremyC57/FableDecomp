// CAIStateGroup_KidPlayHopscotch::InitStateGroup(const CWorld&, CThingAICreature&) -- reset the hopscotch game state.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00941520, length 40 bytes.
// Callee: GFGetCurrentFrame @ 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated; it returns the global
// frame counter, see the landed CTCTargetingPlayer::SetAsTargetNextThingInDirection). The global at 0x01375550
// is the frames-per-second constant (seconds * 60 frames).
//
// Shape: body assignments in order -- the board flag at +0x14 cleared, the start frame at +0x54 stamped, the
// duration at +0x58 set to g_framesPerSecond * 60 (one minute of play; the global holds 15), then the hop
// counter +0x60, the square index +0x64 and the throw flag +0x5c zeroed from one register IN THAT ORDER (the
// scheduler sinks the duration store below them; the zero-store order is the source order). Both arguments are unused (`ret 8`). No engine header exists for this class; the layout is local.
class CWorld;
class CThingAICreature;

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};               // 0x0049D870
extern long g_framesPerSecond;                 // 0x01375550

class CAIStateGroupBase { public:
    virtual void v00();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_KidPlayHopscotch : CAIStateGroupBase {
    bool m_foundBoard;                          // +0x14
    unsigned char _game[0x3f];                  // +0x15 board pointer, square positions
    long m_startFrame;                          // +0x54
    long m_playDurationFrames;                  // +0x58
    bool m_thrown;                              // +0x5c
    long m_hopCount;                            // +0x60
    long m_currentSquare;                       // +0x64
    void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_KidPlayHopscotch::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    m_foundBoard = false;
    m_startFrame = CWorld::GetFrame();
    m_playDurationFrames = g_framesPerSecond * 60;
    m_hopCount = 0;
    m_currentSquare = 0;
    m_thrown = false;
}