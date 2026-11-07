// CInputProcessInventory::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- route input on the main inventory screen: while a text box or key redefiner takes keys, character / key events go to the front end's key slot; while the key-redefiner list is active, redefinable events or the mouse wheel become front-end events; then the left stick pans the hero's inventory map (component 0x1B) when it is in pan mode or moves the front-end selection, the right stick drives the map zoom, pressed / released / held menu actions become front-end events (page actions 0x27 / 0x28 honour the save / quit menus of the hero's CTCInventory, action 3 queues the inventory-closed game event, action 0x48 toggles the live GUI, the shortcut actions open inventory pages), mouse moves update the cursor and the wheel scrolls the menu
// Leaf: ?ProcessInput@CInputProcessInventory@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00689DF0, 3827 bytes incl. the 9-entry jump table 0x0068ACA8 and its byte index 0x0068ACCC; `ret 8`.
// Identity: vtable 0x012376F8 slot 4 (.?AVCInputProcessInventory@@), PDB ?ProcessInput@CInputProcessInventory@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z.
// Names: 0x00687B30 / 0x00687BD0 keep the landed spellings IsEventButtonReleased / IsEventButtonHeld (one symbol, one address);
//   chunk 829 evidence says they are swapped (queued land0711e).
// GS note: /GS -- the CGameEvent's 32-byte data array gives the cookie frame (as the landed siblings 0x0068B7C0 / 0x0068BC70).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/ETCInterfaceType.h"
#define CPlayerGui CPlayerGuiDonor
#include "engine/CPlayerGui.h"
#undef CPlayerGui
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CWorld; class CGameDefinitionManager; class CGamePlayerInterface;
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
    C2DVector     MouseMovement;                   // +0x0C
    EInputEventType GetType() const;               // 0x00A03B40 ?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ
    unsigned short GetChar() const;                // 0x00A03B80 ?GetChar@CInputEvent@@QBEGXZ
    C2DVector GetJoystickPosition() const;         // 0x00A043A0 ?GetJoystickPosition@CInputEvent@@QBE?AVC2DVector@@XZ
    float GetMouseWheelMovement() const;           // 0x00A03BB0 ?GetMouseWheelMovement@CInputEvent@@QBEMXZ
};
class CProcessedInput {
public:
    void SetAsPerformedEvent();                    // 0x00A0D390 ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ
};
float __cdecl floor(float value);                  // 0x00689370 (float overload of MSVCR71 floor)
extern const float DELTA_NOTIONAL_ZERO;            // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB
extern bool EditActive_013b8ab0;                   // 0x013B8AB0 (set by NUISystem::CEditBox::SetActive 0x005401B0)
namespace NUISystem {
enum EEvent { };
class CObserver;
class CKeyRedefiner {
public:
    static CKeyRedefiner* s_pSelected;             // 0x013B8AC8 ?s_pSelected@CKeyRedefiner@NUISystem@@2PAV12@A
};
class CRedefinerList {
public:
    static bool __fastcall IsRedefinerListActive();   // 0x00556520 ?IsRedefinerListActive@CRedefinerList@NUISystem@@SI_NXZ
};
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
    virtual bool IsExclusivelyObserved();          // slot 8 (+0x20)
    virtual void AddConcurrentExclusiveObserver(CObserver* observer);
    virtual void RemoveConcurrentExclusiveObserver(CObserver* observer);
    virtual bool GetMouseDisabled();               // slot 11
    virtual void SetKeyPressed(unsigned short key);   // slot 12 (+0x30)
    virtual void v13(); virtual void v14(); virtual void v15();
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
    void AddLong(long value) { Copy(sizeof(value), &value); }
};
class CPlayer;
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
class CPlayerGui : public CPlayerGuiDonor {
public:
    void ToggleLiveGui();                         // 0x00438310 ?ToggleLiveGui@CPlayerGui@@QAEXXZ
};
class CTCInventoryMap { public:
    unsigned char _pad00[0x58];
    bool          PanMode;                        // +0x58
    unsigned char _pad59[0x248 - 0x59];
    bool          ZoomedIn;                       // +0x248
    void PanMap(C2DVector direction);             // 0x005FB140 ?PanMap@CTCInventoryMap@@QAEXVC2DVector@@@Z
};
class CTCInventory { public:
    unsigned char _pad00[0x21c];
    long          CurrentScreen;                  // +0x21C
    bool IsSaveMenuReceivingInput() const;        // 0x0058376C ?IsSaveMenuReceivingInput@CTCInventory@@QBE_NXZ
    bool IsQuitMenuReceivingInput() const;        // 0x005836D2 ?IsQuitMenuReceivingInput@CTCInventory@@QBE_NXZ
    void OpenInventoryShortcut(long shortcut);    // 0x0058E99F ?OpenInventoryShortcut@CTCInventory@@QAEXJ@Z
};
class CThingPlayerCreature : public CThing { };
// IS_THING_ALIVE<CThingPlayerCreature*> 0x0043DBD0: the header template, visible here and out of line as in retail (as the landed
// 0x0056FBED), so VC7.1 knows the hero is only read through the reference and keeps it in esi.
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && thing->IsDead() == 0;
}
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
    virtual void _v31();
    virtual void AddCloseInventoryEvent();                 // slot 32 (+0x80) (landed 0x005BC527)
};
}
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
    bool IsEventButtonReleased(const CInputEvent& event) const;             // 0x00687B30
    bool IsEventButtonHeld(const CInputEvent& event) const;                 // 0x00687BD0
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
class CInputProcessInventory : public CInputProcessInventoryBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
protected:
    virtual NInventory::CTCInventoryBase* GetPInventory() const;   // 0x006896A0
public:
    CTCInventory* PInventory;                      // +0x34
};

void CInputProcessInventory::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CTCInventoryMap* map = 0;
    MyPlayer->GetPControlledCreature()->GetTC(TCI_INVENTORY_MAP, &map);
    NInventory::CTCInventoryBase* inventory = GetPInventory();
    bool typing = EditActive_013b8ab0 || NUISystem::CKeyRedefiner::s_pSelected;
    if (event.GetType() == 1 && typing) {
        NUISystem::CManager::GetInstance()->SetKeyPressed(event.GetChar());
        NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x21);
        processed.SetAsPerformedEvent();
        return;
    }
    if (event.GetType() == 0xf && typing) {
        NUISystem::CManager::GetInstance()->SetKeyPressed(event.GetChar());
        NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x22);
        processed.SetAsPerformedEvent();
        return;
    }
    if (NUISystem::CRedefinerList::IsRedefinerListActive()) {
        if (NUISystem::CKeyRedefiner::s_pSelected) {
            NUISystem::EEvent uiEvent;
            switch (event.GetType()) {
            case 4:    uiEvent = (NUISystem::EEvent)0x1a; break;
            case 7:    uiEvent = (NUISystem::EEvent)0x23; break;
            case 10:   uiEvent = (NUISystem::EEvent)0x1b; break;
            case 0x16: uiEvent = (NUISystem::EEvent)0x26; break;
            case 0x17: uiEvent = (NUISystem::EEvent)0x27; break;
            case 0x18: uiEvent = (NUISystem::EEvent)0x28; break;
            case 0x19: uiEvent = (NUISystem::EEvent)0x29; break;
            case 0x1a: uiEvent = (NUISystem::EEvent)0x2a; break;
            default: goto notRedefined;
            }
            NUISystem::CManager::GetInstance()->SetEvent(uiEvent);
            processed.SetAsPerformedEvent();
            return;
        } else if (event.GetType() == 0xe) {
            NUISystem::EEvent uiEvent;
            if (event.GetMouseWheelMovement() > DELTA_NOTIONAL_ZERO)
                uiEvent = (NUISystem::EEvent)0x24;
            else if (event.GetMouseWheelMovement() < -DELTA_NOTIONAL_ZERO)
                uiEvent = (NUISystem::EEvent)0x25;
            else
                goto notRedefined;
            NUISystem::CManager::GetInstance()->SetEvent(uiEvent);
            processed.SetAsPerformedEvent();
            return;
        }
    }
notRedefined:
    if (event.GetType() == 0x11) {
        C2DVector stick = event.GetJoystickPosition();
        if (map->PanMode && map->ZoomedIn) {
            map->PanMap(stick);
            processed.SetAsPerformedEvent();
        } else if (stick.Y > 0.6f) {
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
            if (!NUISystem::CManager::GetInstance()->IsExclusivelyObserved()) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)5);
                processed.SetAsPerformedEvent();
            }
        } else if (IsEventGameAction((EGameAction)0x25, event)) {
            if (!map->ZoomedIn)
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0);
            if (NUISystem::CManager::GetInstance()->GetExclusiveObserver())
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x10);
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)0x26, event)) {
            if (!map->ZoomedIn)
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)1);
            if (NUISystem::CManager::GetInstance()->GetExclusiveObserver())
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x11);
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)0x27, event)) {
            if (NUISystem::CManager::GetInstance()->GetExclusiveObserver()) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0xe);
            } else {
                CTCInventory* heroInventory;
                if (CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &heroInventory)
                    && heroInventory && heroInventory->CurrentScreen == 4) {
                    if (heroInventory->IsSaveMenuReceivingInput() || heroInventory->IsQuitMenuReceivingInput()) {
                        processed.SetAsPerformedEvent();
                        return;
                    }
                }
                if (!NUISystem::CManager::GetInstance()->IsExclusivelyObserved())
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)5);
            }
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)0x28, event)) {
            if (NUISystem::CManager::GetInstance()->GetExclusiveObserver()) {
                NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0xf);
            } else {
                CTCInventory* heroInventory;
                if (CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &heroInventory)
                    && heroInventory && heroInventory->CurrentScreen == 4) {
                    if (heroInventory->IsSaveMenuReceivingInput() || heroInventory->IsQuitMenuReceivingInput()) {
                        processed.SetAsPerformedEvent();
                        return;
                    }
                }
                if (!NUISystem::CManager::GetInstance()->IsExclusivelyObserved())
                    NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)4);
            }
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)3, event)) {
            GetPInventory()->AddCloseInventoryEvent();
            CThingPlayerCreature* hero = MyPlayer->GetPControlledCreature();
            if (IS_THING_ALIVE(hero)) {
                CGameEvent closedEvent(0x1a);
                closedEvent.Player = (signed char)hero->GetPlayerNumber();
                closedEvent.AddLong(0x17);
                CMainGameComponent::Get()->AddGameEvent(closedEvent);
            }
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)0x48, event)) {
            if (!EditActive_013b8ab0 && !NUISystem::CManager::GetInstance()->IsExclusivelyObserved())
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().ToggleLiveGui();
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)0x58, event)) {
            NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)6);
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)0x59, event)) {
            NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)7);
            processed.SetAsPerformedEvent();
        } else if (!NUISystem::CManager::GetInstance()->IsExclusivelyObserved()) {
            if (IsEventGameAction((EGameAction)0x5a, event)) {
                CTCInventory* heroInventory;
                if (CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &heroInventory))
                    heroInventory->OpenInventoryShortcut(0);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x5b, event)) {
                CTCInventory* heroInventory;
                if (CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &heroInventory))
                    heroInventory->OpenInventoryShortcut(1);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x5c, event)) {
                CTCInventory* heroInventory;
                if (CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &heroInventory))
                    heroInventory->OpenInventoryShortcut(2);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x5d, event)) {
                CTCInventory* heroInventory;
                if (CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &heroInventory))
                    heroInventory->OpenInventoryShortcut(3);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x64, event)) {
                CTCInventory* heroInventory;
                if (CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &heroInventory))
                    heroInventory->OpenInventoryShortcut(4);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x61, event)) {
                CTCInventory* heroInventory;
                if (CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &heroInventory))
                    heroInventory->OpenInventoryShortcut(5);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x62, event)) {
                CTCInventory* heroInventory;
                if (CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &heroInventory))
                    heroInventory->OpenInventoryShortcut(6);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x63, event)) {
                CTCInventory* heroInventory;
                if (CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &heroInventory))
                    heroInventory->OpenInventoryShortcut(7);
                processed.SetAsPerformedEvent();
            } else if (IsEventGameAction((EGameAction)0x60, event)) {
                CTCInventory* heroInventory;
                if (CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &heroInventory))
                    heroInventory->OpenInventoryShortcut(8);
                processed.SetAsPerformedEvent();
            }
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
        } else if (IsEventGameAction((EGameAction)0x6c, event)) {
            NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x14);
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)0x6d, event)) {
            NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x15);
            processed.SetAsPerformedEvent();
        }
    } else if (IsEventButtonHeld(event)) {
        if (IsEventGameAction((EGameAction)0x19, event)) {
            NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x1c);
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)0x4c, event)) {
            NUISystem::CManager::GetInstance()->SetEvent((NUISystem::EEvent)0x1d);
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
}