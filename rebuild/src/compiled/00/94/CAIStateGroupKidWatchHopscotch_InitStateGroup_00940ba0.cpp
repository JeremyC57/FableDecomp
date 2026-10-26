// CAIStateGroup_KidWatchHopscotch::InitStateGroup(const CWorld&, CThingAICreature&) -- stamp the start frame,
// give the watch a 60-second budget and clear the per-game counters.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00940BA0, length 37 bytes (+ int3 padding), `ret 8` (both
// reference arguments unused).
// Callees: GFGetCurrentFrame @ 0x0049D870 (__fastcall, no arguments; the manifest's "GFGetPreMainMemoryUsed"
// is a propagated label). Global: the frame-rate long @ 0x01375550 (`imul eax,eax,3Ch` = 60 seconds).
//
// Shape: the frame store at +0x2c, the budget at +0x34, then one `xor eax,eax` feeding the two dword zero
// stores (+0x3c, +0x40) and finally the byte at +0x38 (the statements are in that order: VC keeps store order). No engine header exists for this class; the layout is local
// with descriptive names (the PDB has no CAIStateGroup_KidWatchHopscotch entry).
class CWorld;
class CThingAICreature;

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};        // 0x0049D870
extern long g_framesPerSecond;              // 0x01375550

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];               // +0x04..+0x13 PBrain, ScriptAIPriority, Name, flags
};

struct CAIStateGroup_KidWatchHopscotch : CAIStateGroupBase {
    unsigned char _pad14[0x18];             // +0x14 hopscotch pointers / markers (untouched here)
    long m_frameStarted;                    // +0x2c
    long _pad30;                            // +0x30
    long m_framesToWatch;                   // +0x34
    bool m_gameSeen;                        // +0x38
    long m_gamesWatched;                    // +0x3c
    long m_lastGameFrame;                   // +0x40
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_KidWatchHopscotch::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    m_frameStarted = CWorld::GetFrame();
    m_framesToWatch = g_framesPerSecond * 60;
    m_gamesWatched = 0;
    m_lastGameFrame = 0;
    m_gameSeen = false;
}