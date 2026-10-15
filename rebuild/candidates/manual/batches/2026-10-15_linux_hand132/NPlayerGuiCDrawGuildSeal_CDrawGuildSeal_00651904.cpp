#pragma optimize("s", on)
// NPlayerGui::CDrawGuildSeal::CDrawGuildSeal(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), own vtable, flags/flash cleared, fade built, reminder timer and level-up positions cleared, the level-up graphic read from the GUI def, the message queue built and ten primitive handles reserved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00651904, length 104 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase @ 0x0064CC65, CFadeInFadeOutBase::CFadeInFadeOutBase() @ 0x00436560 (on +0x30),
// PeekGuiDef @ 0x00438570 (`__fastcall`, no arguments; its +0x1bc word is the guild-seal level-up graphic),
// queue<pair<CCharString,long>>::queue() @ 0x00656D0B (Messages +0x68), vector<CEnginePrimitiveHandle>::Resize(long) @ 0x00443F90
// (PrimitiveHandles +0x1c, 10). Vtable 0x0125A6BC.
// PDB CDrawGuildSeal (136 bytes): Enabled +0x2c, ReadyToLevelUp +0x2d, FlashAlpha +0x2e, FlashCycles +0x30, FadeInOut +0x34,
// ReminderTime +0x50, LevelUpPositions +0x54, LevelUpGraphic +0x64, CurrentMessageEndFrame +0x68, NextMessageDelayEndFrame +0x6c,
// Messages +0x70 -- retail 4 lower up to the positions vector and 8 lower after it (12-byte vectors in the shipped build).
// Shape: size TU with pass-through base arguments; one zero register (ebx) shared by every integer zero; the positions vector
// is an inline member constructor stored directly; all members in the initialiser list in declaration order.
struct CPlayerGui;

struct CPlayerGuiDef {
    unsigned char _pad[0x1bc];
    long          GuildSealLevelUpGraphic;         // +0x1bc
};

extern CPlayerGuiDef* __fastcall PeekGuiDef();     // 0x00438570

namespace NPlayerGui {

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    void Resize(long count);                       // 0x00443F90
};

struct CRetailVectorVector {                       // VC6 Dinkumware vector<C2DVector>: three words
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CRetailVectorVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CRetailMessageQueue {                       // queue<pair<CCharString,long>> over a deque
    unsigned char _body[0x20];
    CRetailMessageQueue();                         // 0x00656D0B
};

struct CFadeInFadeOutBase {
    unsigned char _body[0x1c];
    CFadeInFadeOutBase();                          // 0x00436560
};

struct CDrawBase {
    virtual void v00();
    unsigned char       _body[0x1c - 4];           // +0x04 .. +0x1c
    CRetailHandleVector PrimitiveHandles;          // +0x1c
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

struct CDrawGuildSeal : CDrawBase {                // vtable 0x0125A6BC
    virtual void v00();
    bool                Enabled;                   // +0x28
    bool                ReadyToLevelUp;            // +0x29
    unsigned char       FlashAlpha;                // +0x2a
    long                FlashCycles;               // +0x2c
    CFadeInFadeOutBase  FadeInOut;                 // +0x30
    float               ReminderTime;              // +0x4c
    CRetailVectorVector LevelUpPositions;          // +0x50
    long                LevelUpGraphic;            // +0x5c
    long                CurrentMessageEndFrame;    // +0x60
    long                NextMessageDelayEndFrame;  // +0x64
    CRetailMessageQueue Messages;                  // +0x68
    CDrawGuildSeal(CPlayerGui* gui, void* drawList);
};

CDrawGuildSeal::CDrawGuildSeal(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), Enabled(false), ReadyToLevelUp(false), FlashAlpha(0), FlashCycles(0), FadeInOut(),
      ReminderTime(0.0f), LevelUpPositions(), LevelUpGraphic(PeekGuiDef()->GuildSealLevelUpGraphic), CurrentMessageEndFrame(0),
      NextMessageDelayEndFrame(0), Messages()
{
    PrimitiveHandles.Resize(10);
}

}
