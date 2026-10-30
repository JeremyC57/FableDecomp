// NScript::CQ_ArenaScript::CFlick::Init() -- clear the talk flags and the hint number, switch off the entity's opinion and deed reactions
// Leaf: ?Init@CFlick@CQ_ArenaScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00f14a30, 53 bytes, thiscall, plain `ret` (virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the entity vtable 0x012F8FE8 that the landed NScript::CQ_ArenaScript::CFlick::Alloc 0x00F14980 stores; Ego_r.pdb ?Init@CFlick@CQ_ArenaScript@NScript@@EAEXXZ; the cleared members are Ego_r.pdb CFlick::EarlyTalk +0x1C, ChamTalk +0x1D, HintNumber +0x20.
// Callees: CGameScriptInterface (ghidra_out/cgsi_slots_aligned.tsv) slots 594 EntitySetOpinionReactionsEnabled(const CScriptThing&,
//   bool) (+0x948) and 595 EntitySetDeedReactionsEnabled(const CScriptThing&, bool) (+0x94c).
// Shape (family L146): the member clears are scheduled between the first call's argument pushes.
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_ArenaScript { public:
    struct CFlick {                                      // entity vtable 0x012F8FE8
        CGameScriptInterface* Game;                     // +0x04
        CScriptThing          Me;                       // +0x08
        CQ_ArenaScript*       ParentClass;              // +0x14
        void*                 PMasterData;              // +0x18
        bool                  EarlyTalk;                // +0x1c
        bool                  ChamTalk;                 // +0x1d
        long                  HintNumber;               // +0x20
        virtual void v0();
        virtual void Main();
    private:
        virtual void Init();                            // slot 2
    };
};
void CQ_ArenaScript::CFlick::Init()
{
    EarlyTalk = false;
    ChamTalk = false;
    HintNumber = 0;
    Game->EntitySetOpinionReactionsEnabled(Me, false);
    Game->EntitySetDeedReactionsEnabled(Me, false);
}
}