// CInputProcessInventoryQuestsScreen::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- route input on the inventory quests screen: when the quests inventory is the hero-information inventory screen, released A/B/Y/White go to the inventory's button handlers and the base process runs; otherwise the left stick moves the front-end selection, the right stick drives the map zoom, pressed / held / released menu actions become front-end events (held d-pad actions zoom or pan the hero's inventory map, component 0x1B), released action 0x48 queues the close-inventory game event, mouse moves update the cursor and the wheel scrolls the menu
// Leaf: ?ProcessInput@CInputProcessInventoryQuestsScreen@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068BC70, 2481 bytes, `ret 8`; vtable 0x0123782C slot 4 (.?AVCInputProcessInventoryQuestsScreen@@).
// Names: 0x00687B30 / 0x00687BD0 keep the landed spellings IsEventButtonReleased / IsEventButtonHeld (one symbol, one address); chunk 829 evidence says they are swapped (queued land0711e).
// GS note: /GS -- the CGameEvent's 32-byte data array gives the cookie frame (as the landed siblings 0x0068B7C0 / 0x0068AE30).
class CPlayer;
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputEvent;
class CProcessedInput;
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X), Y(p.Y) { }
    const C2DVector operator-() const { return C2DVector(-X, -Y); }
};
enum EInputEventType { };
enum EGameAction { };
class CInputEvent {
public:
    unsigned char _pad00[0x0c];
    C2DVector     MouseMovement;                   // +0x0C (UInputEventData: the mouse movement's X / Y)
    EInputEventType GetType() const;               // 0x00A03B40 ?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ
    C2DVector GetJoystickPosition() const;         // 0x00A043A0 ?GetJoystickPosition@CInputEvent@@QBE?AVC2DVector@@XZ
    float GetMouseWheelMovement() const;           // 0x00A03BB0 ?GetMouseWheelMovement@CInputEvent@@QBEMXZ
};
class CProcessedInput {
public:
    void SetAsPerformedEvent();                    // 0x00A0D390 ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ
};
float __cdecl floor(float value);                  // 0x00689370 (float overload of MSVCR71 floor)
namespace NUISystem {
enum EEvent { };
class CObserver;
class CManager {
public:
    virtual void SetEvent(EEvent event);           // slot 0 (CObservable)
    virtual void SetOwnedEvent(CObserver* observer, EEvent event);
    virtual void AddObserver(CObserver* observer);
    virtual void AddExclusiveObserver(CObserver* observer);
    virtual void RemoveExclusiveObserver(CObserver* observer);
    virtual void RemoveObserver(CObserver* observer);
    virtual void ClearObservers();
    virtual CObserver* GetExclusiveObserver();     // slot 7 (+0x1C)
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37();
    virtual void SetRightStickForce(C2DVector& force);   // slot 38 (+0x98)
    unsigned char _pad04[0xac];
    C2DVector     MouseMovement;                   // +0xB0
    void SetMouseMovement(C2DVector& movement) { MouseMovement = movement; }   // Ego_r ?SetMouseMovement@CManager@NUISystem@@QAEXAAVC2DVector@@@Z (inline)
    void ScrollMenu(long amount);                  // 0x0041DA2D ?ScrollMenu@CManager@NUISystem@@QAEXJ@Z
    static CManager* __fastcall GetInstance();     // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
};
}
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                           // +0x00
    signed char   Player;                         // +0x04
    unsigned char Data[32];                       // +0x05
    unsigned char EndPos;                         // +0x25
    bool          Valid;                          // +0x26
    bool          Replacement;                    // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
#include "engine/CPlayerManager.h"
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
private: public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                // +0x1C
    void AddGameEvent(const CGameEvent& event);   // 0x0041649C
};
class CTCInventoryMap { public:
    void PanMap(C2DVector direction);             // 0x005FB140 ?PanMap@CTCInventoryMap@@QAEXVC2DVector@@@Z
};
class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const ETCInterfaceType& kind);   // 0x0040F020
    __forceinline CTCInventoryMap* Find(long kind)
    {
        STCEntry* it = LowerBound((ETCInterfaceType)kind);
        return ((CTCInventoryMap*)((it == m_pLast || kind < it->first) ? m_pLast : it)->second);
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                     // +0x20
    CTCMap        m_tcs;                           // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCInventoryMap* GetTC(long kind) { return m_tcs.Find(kind); }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
namespace NInventory {
class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase();
    virtual void _v01(); virtual void _v02(); virtual void _v03(); virtual void _v04(); virtual void _v05();
    virtual void _v06(); virtual void _v07(); virtual void _v08(); virtual void _v09(); virtual void _v10();
    virtual void _v11(); virtual void _v12(); virtual void _v13(); virtual void _v14(); virtual void _v15();
    virtual void _v16(); virtual void _v17(); virtual void _v18(); virtual void _v19(); virtual void _v20();
    virtual void _v21(); virtual void _v22(); virtual void _v23(); virtual void _v24(); virtual void _v25();
    virtual void _v26(); virtual void _v27(); virtual void _v28(); virtual void _v29(); virtual void _v30();
    virtual void _v31(); virtual void _v32(); virtual void _v33(); virtual void _v34(); virtual void _v35();
    virtual void _v36(); virtual void _v37();
    virtual void ProcessInventoryInputUp();                // slot 38 (+0x98)
    virtual void ProcessInventoryInputDown();              // slot 39 (+0x9c)
    virtual void ProcessInventoryInputLeft();              // slot 40 (+0xa0)
    virtual void ProcessInventoryInputRight();             // slot 41 (+0xa4)
    virtual void _v42(); virtual void _v43(); virtual void _v44(); virtual void _v45();
    virtual void _v46(); virtual void _v47(); virtual void _v48(); virtual void _v49();
    virtual void ProcessButtonAPressed();                  // slot 50 (+0xc8)
    virtual void ProcessButtonBPressed();                  // slot 51 (+0xcc)
    virtual void ProcessButtonXPressed();                  // slot 52 (+0xd0)
    virtual void ProcessButtonYPressed();                  // slot 53 (+0xd4)
    virtual void ProcessButtonAHeld();                     // slot 54 (+0xd8)
    virtual void ProcessButtonBHeld();                     // slot 55 (+0xdc)
    virtual void ProcessButtonXHeld();                     // slot 56 (+0xe0)
    virtual void ProcessButtonYHeld();                     // slot 57 (+0xe4)
    virtual void ProcessButtonAReleased();                 // slot 58 (+0xe8)
    virtual void ProcessButtonBReleased();                 // slot 59 (+0xec)
    virtual void ProcessButtonXReleased();                 // slot 60 (+0xf0)
    virtual void ProcessButtonYReleased();                 // slot 61 (+0xf4)
    virtual void ProcessDPadUpReleased();                  // slot 62 (+0xf8)
    virtual void ProcessDPadDownReleased();                // slot 63 (+0xfc)
    virtual void ProcessDPadLeftReleased();                // slot 64 (+0x100)
    virtual void ProcessDPadRightReleased();               // slot 65 (+0x104)
    virtual void ProcessButtonWhitePressed();              // slot 66 (+0x108)
    virtual void ProcessButtonWhiteReleased();             // slot 67 (+0x10c)
    unsigned char _pad04[0x12c];                           // +0x04 .. 0x130 (Ego_r.pdb sizeof 0x130)
};
}
namespace NHeroInformationScreens {
class CBase {                                              // RTTI base of CTCInventoryQuests at +0x130 (COL 0x0133C1E8)
public:
    virtual void GetScreenName() const;                    // slot 0 (returns CWideString; unused here)
    virtual long GetPlayerMode() const;                    // slot 1
    virtual long GetScreenType() const;                    // slot 2
    virtual bool IsInventoryScreen() const;                // slot 3 (+0x0C) ?IsInventoryScreen@CBase@NHeroInformationScreens@@UBE_NXZ
};
}
class CTCInventoryQuests : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase { };
class CInputProcessGameBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);   // slot 4
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    unsigned char           _pad04[0x10];
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
protected:
    bool IsEventButtonPressed(const CInputEvent& event) const;              // 0x00687A90
    bool IsEventButtonReleased(const CInputEvent& event) const;                 // 0x00687B30
    bool IsEventButtonHeld(const CInputEvent& event) const;             // 0x00687BD0
public:
    bool IsEventGameAction(EGameAction action, const CInputEvent& event) const;   // 0x00687CF0
    C2DVector GetMouseMovement(const CInputEvent& event) const { return event.MouseMovement; }
};
class CInputProcessInventoryBase : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);   // 0x006894C0
protected:
    virtual NInventory::CTCInventoryBase* GetPInventory() const = 0;   // slot 9 (+0x24)
public:
    long          CurrentMovement;                 // +0x28
    unsigned long FrameSinceLastMovement;          // +0x2c
    bool          StartedRepeatingMovement;        // +0x30
};
class CInputProcessInventoryQuestsScreen : public CInputProcessInventoryBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
protected:
    virtual NInventory::CTCInventoryBase* GetPInventory() const;   // 0x00689B00
};

void CInputProcessInventoryQuestsScreen::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CTCInventoryQuests* inventory = static_cast<CTCInventoryQuests*>(GetPInventory());
    CTCInventoryMap* map = 0;
    CThing* hero = MyPlayer->GetPControlledCreature();
    if (hero->HasTC(0x1b))
        map = hero->GetTC(0x1b);
    if (!inventory->IsInventoryScreen()) {
        if (event.GetType() == 0x11) {
            C2DVector stick = event.GetJoystickPosition();
            if (stick.Y > 0.6f) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)1);
                if (NUISystem::CManager::GetInstance()->GetExclusiveObserver())
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x11);
                processed.SetAsPerformedEvent();
            } else if (stick.Y < -0.6f) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0);
                if (NUISystem::CManager::GetInstance()->GetExclusiveObserver())
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x10);
                processed.SetAsPerformedEvent();
            } else if (stick.X > 0.6f) {
                if (NUISystem::CManager::GetInstance()->GetExclusiveObserver())
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0xf);
                processed.SetAsPerformedEvent();
            } else if (stick.X < -0.6f) {
                if (NUISystem::CManager::GetInstance()->GetExclusiveObserver())
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0xe);
                processed.SetAsPerformedEvent();
            }
        } else if (event.GetType() == 0x12) {
            C2DVector stick = event.GetJoystickPosition();
            if (stick.Y > 0.1) {
                NUISystem::CManager::GetInstance()->SetRightStickForce(stick);
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)7);
                processed.SetAsPerformedEvent();
            } else if (stick.Y < -0.1) {
                NUISystem::CManager::GetInstance()->SetRightStickForce(stick);
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)6);
                processed.SetAsPerformedEvent();
            } else if (stick.X < -0.1) {
                C2DVector force = -stick;
                NUISystem::CManager::GetInstance()->SetRightStickForce(force);
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0xc);
                processed.SetAsPerformedEvent();
            } else if (stick.X > 0.1) {
                C2DVector force = -stick;
                NUISystem::CManager::GetInstance()->SetRightStickForce(force);
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0xd);
                processed.SetAsPerformedEvent();
            }
        } else if (IsEventButtonPressed(event)) {
            if (IsEventGameAction((EGameAction)0x19, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x1a);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x4c, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)5);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x25, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0);
                if (NUISystem::CManager::GetInstance()->GetExclusiveObserver())
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x10);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x26, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)1);
                if (NUISystem::CManager::GetInstance()->GetExclusiveObserver())
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x11);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x27, event)) {
                if (NUISystem::CManager::GetInstance()->GetExclusiveObserver())
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0xe);
                else
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)5);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x28, event)) {
                if (NUISystem::CManager::GetInstance()->GetExclusiveObserver())
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0xf);
                else
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)4);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x21, event)) {
                if (NUISystem::CManager::GetInstance()->GetExclusiveObserver())
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x12);
                else
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)4);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x22, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)5);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x24, event)) {
                if (NUISystem::CManager::GetInstance()->GetExclusiveObserver()) {
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x13);
                    processed.SetAsPerformedEvent();
                }
            } else if (IsEventGameAction((EGameAction)0x58, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)6);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x59, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)7);
                processed.SetAsPerformedEvent();
            }
        } else if (IsEventButtonReleased(event)) {
            if (IsEventGameAction((EGameAction)0x19, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x1f);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x4c, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x20);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x25, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x26, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)1);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x58, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)6);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x59, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)7);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x67, event)) {
                C2DVector zoom(1.0f, 1.0f);
                NUISystem::CManager::GetInstance()->SetRightStickForce(zoom);
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0xc);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x66, event)) {
                C2DVector zoom(-1.0f, -1.0f);
                NUISystem::CManager::GetInstance()->SetRightStickForce(zoom);
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0xd);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x6b, event)) {
                if (map)
                    map->PanMap(C2DVector(0.0f, -1.0f));
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x6a, event)) {
                if (map)
                    map->PanMap(C2DVector(0.0f, 1.0f));
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x68, event)) {
                if (map)
                    map->PanMap(C2DVector(-1.0f, 0.0f));
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x69, event)) {
                if (map)
                    map->PanMap(C2DVector(1.0f, 0.0f));
                processed.SetAsPerformedEvent();
            }
        } else if (IsEventButtonHeld(event)) {
            if (IsEventGameAction((EGameAction)0x19, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x1c);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x4c, event)) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x1d);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x48, event)) {
                CGameEvent closeEvent(0x1b);
                closeEvent.Player = (signed char)CMainGameComponent::Get()->PlayerManager->GetMainPlayerNumber();
                CMainGameComponent::Get()->AddGameEvent(closeEvent);
                processed.SetAsPerformedEvent();
            }
        } else if (IsEventGameAction((EGameAction)0x47, event)) {
            C2DVector movement = GetMouseMovement(event);
            NUISystem::CManager::GetInstance()->SetMouseMovement(movement);
            NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x19);
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)0x65, event)) {
            NUISystem::CManager::GetInstance()->ScrollMenu((long)floor(event.GetMouseWheelMovement() * 0.008333334f));
            processed.SetAsPerformedEvent();
        }
    } else {
        if (IsEventButtonHeld(event)) {
            if (IsEventGameAction((EGameAction)0x23, event)) {
                GetPInventory()->ProcessButtonAReleased();
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x22, event)) {
                GetPInventory()->ProcessButtonBReleased();
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x24, event)) {
                GetPInventory()->ProcessButtonYReleased();
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x29, event)) {
                GetPInventory()->ProcessButtonWhiteReleased();
                processed.SetAsPerformedEvent();
            }
        }
        CInputProcessInventoryBase::ProcessInput(event, processed);
    }
}