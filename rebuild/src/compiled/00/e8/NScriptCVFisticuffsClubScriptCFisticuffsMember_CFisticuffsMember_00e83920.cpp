// NScript::CV_FisticuffsClubScript::CFisticuffsMember::CFisticuffsMember(CV_FisticuffsClubScript& parent, CScriptDataBase* data, const CScriptThing& me) -- a fisticuffs-club member's entity script: entity-script base over the quest's interface and Me, the parent, the ShoutTimer, the cheer/boo strings, and the master data last
// Leaf: ??0CFisticuffsMember@CV_FisticuffsClubScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E83920, 89 bytes (+ int3 padding), `ret 0Ch`; its only caller is the
//   factory 0x00E838A0 (`push 30h`, CFisticuffsMember::Alloc), bound as "FisticuffsMember" at 0x00E822D4.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, g_pGameScriptInterface (0x0143E8F8)->RegisterTimer
//   (slot 87, +0x15C) for ShoutTimer, CCharString::CCharString() @ 0x0099E4B0 (x2).
// Layout = Ego_r.pdb CFisticuffsMember (48 bytes, entity scripts keep their PDB offsets): CEntityScriptBase {vptr
//   0x012C3224, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, PerformingCheerOrBoo +0x1c, FC_Status +0x20 (both
//   uninitialised), ShoutTimer +0x24, CheerName +0x28, BooName +0x2c. Vtable 0x012EB0F8.
// Shape: the landed entity-script constructors (CDarkwoodTrader 0x00E01670): Game is a plain initialiser of the base stored
//   before the base vtable, the derived vtable precedes ParentClass, the timer asks the interface for an index, the strings
//   follow, PMasterData is a body assignment stored last. The interface model is the landed 88-slot one.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CScriptDataBase;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
template <class T> class CCountedPointer { public:
    void* Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
class CEntityScriptBase { public:                    // vtable 0x012C3224, 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;               // +0x04 (a plain initialiser: stored before the vtable)
    CScriptThing          Me;                 // +0x08
    CEntityScriptBase(const CScriptThing& thing, CGameScriptInterface* game) : Game(game), Me(thing) { }
};
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
class CCharString { public:
    void* PStringData;
    CCharString();                            // 0x0099E4B0
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FisticuffsClubScript {                         // the owning quest script: only Game is read
public:
    unsigned char         _pad00[0x40];                 // CScriptBase
    CGameScriptInterface* Game;                         // +0x40 (CLevelScriptBase<T>::Game)
    class CFisticuffsMember;
};
class CV_FisticuffsClubScript::CFisticuffsMember : public CEntityScriptBase {   // vtable 0x012EB0F8
public:
    CV_FisticuffsClubScript* ParentClass;               // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;               // +0x18
    bool                     PerformingCheerOrBoo;      // +0x1c
    long                     FC_Status;                 // +0x20
    CTimer                   ShoutTimer;                // +0x24
    CCharString              CheerName;                 // +0x28
    CCharString              BooName;                   // +0x2c
    virtual void v00();
    CFisticuffsMember(CV_FisticuffsClubScript& parent, CScriptDataBase* data, const CScriptThing& me);
};
CV_FisticuffsClubScript::CFisticuffsMember::CFisticuffsMember(CV_FisticuffsClubScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(me, parent.Game), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}