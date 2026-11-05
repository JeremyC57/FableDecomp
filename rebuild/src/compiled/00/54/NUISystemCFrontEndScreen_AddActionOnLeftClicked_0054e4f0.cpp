// NUISystem::CFrontEndScreen::AddActionOnLeftClicked(CCountedPointer<CAction>&) -- store the screen's left-click action (counted-pointer assignment)
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members Count->RefCount, Destroy->DeleteFunc, Item->Data are the header CCPPointerInfo's (same offsets).
// Retail 0x0054E4F0, 86 bytes (primary vtable 0x012497E4 slot 71, `ret 4`). Inline CCountedPointer::operator=: the source
// Data/Info bound to locals first, the old block released (destroy thunk +4 on the item +8, operator delete 0x00BFE9BC).
// Same shape as the landed 0x00992470 (member at +0x170 there). Name: Ego_r.pdb
// ?AddActionOnLeftClicked@CFrontEndScreen@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z.
// Local model after the landed CFrontEndScreen rows 0x0054E4B0 / 0x0054E3D0: CComponent = CUIInterface (+0) + CObserver
// (+4, 0x14 bytes) + CActionParam (+0x18); retail Time +0x30, m_fLastInputTime +0x15c, PLeftClickAction +0x160.
// (rebuild/include/engine/CFrontEndScreen.h is the donor PDB layout, 0x30 larger.)
namespace NUISystem {
enum EEvent { EVENT_DUMMY = 0 };
class CAction;
#include "engine/CCountedPointer.h"
class CUIInterface { public: virtual ~CUIInterface(); };
class CObserver {
public:
 virtual ~CObserver();
 virtual void ProcessEvent(EEvent);           // slot 1
 unsigned char _body[0x10];
};
class CActionParam { public: virtual ~CActionParam(); };
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
 unsigned char _body1c[0x30 - 0x1c];
 float Time;                                  // +0x30
 unsigned char _body34[0x15c - 0x34];
};
class CChangingStateComponent : public CComponent { };
class CFrontEndManager {
public:
 virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3();
 virtual void Slot4(); virtual void Slot5(); virtual void Slot6(); virtual void Slot7();
 virtual void Action(CCountedPointer<CAction>&);   // slot 8 (+0x20)
 static CFrontEndManager* __fastcall GetInstance();   // 0x00595582
 void GotoPreviousScreen(bool);               // 0x00597BF2
};
class CManager {
public:
 virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
 virtual void Slot5(); virtual void Slot6(); virtual void Slot7(); virtual void Slot8(); virtual void Slot9();
 virtual void Slot10(); virtual void Slot11(); virtual void Slot12();
 virtual unsigned short GetKeyPressed();      // slot 13 (+0x34), 0x0042D506 (wchar_t at +0xc0)
 static CManager* __fastcall GetInstance();   // 0x0041E5F2
};
}
class CIME {
public:
 static CIME* __fastcall GetInstance();       // 0x00404A80 (returns the static 0x013B7CD8)
 bool IsIMEActive();                          // 0x00404C50
};
namespace NUISystem {
class CFrontEndScreen : public CChangingStateComponent {
public:
 float m_fLastInputTime;                      // +0x15c
 CCountedPointer<CAction> PLeftClickAction;   // +0x160
 void ProcessEvent(EEvent);
 virtual void AddActionOnLeftClicked(CCountedPointer<CAction>&);
};
void CFrontEndScreen::AddActionOnLeftClicked(CCountedPointer<CAction>& action)
{
 PLeftClickAction = action;
}
}