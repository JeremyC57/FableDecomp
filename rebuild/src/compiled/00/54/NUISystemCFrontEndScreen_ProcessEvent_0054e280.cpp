// NUISystem::CFrontEndScreen::ProcessEvent(EEvent) -- 0.2 s debounced screen input: left click runs the screen action, back returns to the previous screen, the cancel keys too unless the IME is active
// Retail 0x0054E280, 197 bytes incl. the embedded jump table (CObserver vtable 0x012497BC slot 1: `this` is the observer
// subobject, members at -4). Callees: CFrontEndManager::GetInstance 0x00595582, GotoPreviousScreen 0x00597BF2, the manager's
// Action slot 8 (+0x20), CManager::GetInstance 0x0041E5F2 + GetKeyPressed (slot 13), CIME::GetInstance 0x00404A80 +
// IsIMEActive 0x00404C50. Float 0.2f @ 0x012305A0. Jump table: index bytes 00 01 03 03 03 03 03 02 02 for events 0x1a..0x22.
// Name: Ego_r.pdb ?ProcessEvent@CFrontEndScreen@NUISystem@@UAEXW4EEvent@2@@Z.
// Local model after the landed CFrontEndScreen rows 0x0054E4B0 / 0x0054E3D0: CComponent = CUIInterface (+0) + CObserver
// (+4, 0x14 bytes) + CActionParam (+0x18); retail Time +0x30, m_fLastInputTime +0x15c, PLeftClickAction +0x160.
// (rebuild/include/engine/CFrontEndScreen.h is the donor PDB layout, 0x30 larger.)
namespace NUISystem {
enum EEvent { EVENT_DUMMY = 0 };
class CAction;
struct CCPPointerInfo {
 long Count;                                  // +0x00
 void (__fastcall *Destroy)(void* item);      // +0x04
 void* Item;                                  // +0x08
};
template <class T> class CCountedPointer {
public:
 T* Data;
 CCPPointerInfo* Info;
 CCountedPointer& operator=(const CCountedPointer& o)
 {
  T* data = o.Data;
  CCPPointerInfo* info = o.Info;
  if (Info != info) {
   if (Info) {
    if (--Info->Count == 0) {
     Info->Destroy(Info->Item);
     ::operator delete(Info);                 // 0x00BFE9BC
    }
   }
   Data = data;
   Info = info;
   if (info)
    ++info->Count;
  }
  return *this;
 }
};
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
void CFrontEndScreen::ProcessEvent(EEvent event)
{
 if (Time - m_fLastInputTime < 0.2f)
  return;
 switch (event) {
 case 0x1b:
  CFrontEndManager::GetInstance()->GotoPreviousScreen(false);
  break;
 case 0x21:
 case 0x22:
  if (CManager::GetInstance()->GetKeyPressed() == 1 && !CIME::GetInstance()->IsIMEActive())
   CFrontEndManager::GetInstance()->GotoPreviousScreen(true);
  break;
 case 0x1a:
  if (PLeftClickAction.Data)
   CFrontEndManager::GetInstance()->Action(PLeftClickAction);
  break;
 }
 if (event != 0x19)
  m_fLastInputTime = Time;
}
}