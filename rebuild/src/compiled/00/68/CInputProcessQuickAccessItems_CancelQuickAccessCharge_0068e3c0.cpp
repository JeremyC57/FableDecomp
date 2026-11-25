// CInputProcessQuickAccessItems::CancelQuickAccessCharge() -- if the controlled creature is alive, queue game event 0x29 (cancel quick-access charge) for its player carrying the charging quick-access slot and a zero byte
// Leaf: ?CancelQuickAccessCharge@CInputProcessQuickAccessItems@@AAEXXZ
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068E3C0, length 141 bytes.
// Callees: CPlayer::PeekPControlledCreature @ 0x00487DD0 (MyPlayer +0x18), CMemoryBuffer::Copy(unsigned long, const void*)
// @ 0x009F1760 (landed), CMainGameComponent::AddGameEvent(const CGameEvent&) @ 0x0041649C on the global component
// @ 0x013B86A0, __security_check_cookie @ 0x00BFE9F9. Global 0x0137EE78 = the quick-access slot being charged.
// Model: CGameEvent as the landed CTCInventoryBase::AddCloseInventoryEvent 0x005BC527 (Ego_r.pdb, 40 bytes, over an empty
// CMemoryBuffer); AddLong / AddUBYTE are inline `Copy(sizeof v, &v)` with the by-value argument homed in the frame. The
// creature's player number is the byte at +0x90; its dead flag bit 0 of +0x91.
struct CMemoryBuffer {
};
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x009F1760 ?AddData@CGameEvent@@QAEXJPBX@Z (was ?Copy@CMemoryBuffer@@QAEXKPBX@Z)
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { reinterpret_cast<CGameEvent*>(this)->AddData(sizeof(value), &value); }
    void AddUBYTE(unsigned char value) { reinterpret_cast<CGameEvent*>(this)->AddData(1, &value); }
 public: void AddData(long, void const*); };
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    void AddGameEvent(const CGameEvent& event);    // 0x0041649C
};
extern long g_QuickAccessChargeSlot;               // 0x0137EE78
class CThingCreatureBase { public:
    unsigned char _pad00[0x90];
    signed char   m_playerNumber;                  // +0x90
    unsigned char m_dead : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
class CThingPlayerCreature : public CThingCreatureBase { };
// NF59 (round 7, queue q10503): the input process is engine/CInputProcessQuickAccessItems.h (opt-in
//   FABLE_RETAIL_CINPUTPROCESSQUICKACCESSITEMS_UPDATE declares this member private, Ego_r.pdb 0x006BA6A0 + FableWin.pdb 0x021E61E0
//   ?CancelQuickAccessCharge@CInputProcessQuickAccessItems@@AAEXXZ) over engine/CInputProcessGameBase.h, and the player engine/CPlayer.h
//   (PeekPControlledCreature 0x00487DD0), the include block of its caller 0x0068E450; the local CPlayer / CInputProcessGameBase /
//   CInputProcessQuickAccessItems models (public QAE) are gone.
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CINPUTPROCESSQUICKACCESSITEMS_UPDATE
#include "fable_compiler.h"
#include "engine/CPlayer.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CInputProcessQuickAccessItems.h"

void CInputProcessQuickAccessItems::CancelQuickAccessCharge()
{
    const CThingCreatureBase* creature = MyPlayer->PeekPControlledCreature();
    if (creature && !creature->m_dead) {
        CGameEvent gameEvent(0x29);
        gameEvent.Player = creature->m_playerNumber;
        gameEvent.AddLong(g_QuickAccessChargeSlot);
        gameEvent.AddUBYTE(0);
        CMainGameComponent::Get()->AddGameEvent(gameEvent);
    }
}