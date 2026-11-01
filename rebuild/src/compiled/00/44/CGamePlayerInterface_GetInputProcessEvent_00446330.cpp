// CGamePlayerInterface::GetInputProcessEvent(CProcessedInput& processedInput) const -- scan the queued input events, offer each unprocessed one to every registered input process and keep the highest-priority accepted result (flagging an event a process consumes outright as processed); copy the best result out, flag its event as processed and report whether any process produced one
// Leaf: ?GetInputProcessEvent@CGamePlayerInterface
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1, /GS) @ 0x00446330, 1002 bytes, `ret 4`. Ego_r.pdb ?GetInputProcessEvent@CGamePlayerInterface@@ABE_NAAVCProcessedInput@@@Z (private const).
// Callees (Ego_r.pdb spellings): CInputEvent ctor 0x00A04410 / dtor 0x00A03B30 / SetAsNull 0x00A03BE0 / GetType 0x00A03B40 / GetDeviceType 0x00A03B50 /
//   GetKey 0x00A03B70; CProcessedInput::Clear 0x00A0D300 / SetPlayer 0x00A0D3A0 / GetPriority 0x00A0D3B0; CInputManager::IsKeyboardEventInQueue
//   0x009F50A0 / EventScanStart 0x009F4ED0 / EventScanNext 0x009F4F10 / EventScanEnd 0x009F4AC0 / IsEventProcessed 0x009F4B10 /
//   FlagEventAsProcessed 0x009F55C0 (labels_familypass7 + its store-insert body); CPlayerManager::GetPlayerNumberFromJoystickDeviceNumber 0x00449990;
//   the input process's virtual ProcessInput (slot 4) / ProcessWhilePaused (slot 7) / IsInputEventToBeSentToThisInputProcess (slot 8) as in the landed
//   CInputProcessManager::PreparePrimitivesForEngine 0x00A0D6B0; CMainGameComponent::IsEditorActive (slot 9, 0x004197B0).
// Globals: g_pMainGameComponent 0x013B86A0, PInputManager 0x013B8388 (ESTABLISHED_SPELLINGS).
// Shape: the input-process scan is the landed 0x00A0D6B0 CLinkedListScanInfo model (StartScanAtHead / ScanForward / RemoveScanInfo through the
//   list, so the no-previous-scan branch stores FirstScanInfo through `this` and the empty-list path threads into it). Here the scan info is reused
//   per event, so RemoveScanInfo's trailing resets (Started, NextScan, PreviousScan, List) survive (they are dead in 0x00A0D6B0) and the head step
//   zeroes Current/Next/Previous on an empty list; the scan reads the stored Current back (`s->Current->Next`), which keeps retail's single
//   register for the node. The input-event scan info follows bestPriority/found (retail's store order).
// Names: StartScanAtHead / ScanForward / RemoveScanInfo and CInputProcessList are the landed sibling's descriptive inline model of
//   CLinkedList<CAInputProcess> (no PDB symbols); result type values 1 (accepted) / 2 (consumed) are used numerically.
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"
#include "engine/CGameEvent.h"
#define CProcessedInput CProcessedInputLayout
#include "engine/CProcessedInput.h"
#undef CProcessedInput
#define CPlayerManager CPlayerManagerLayout
#include "engine/CPlayerManager.h"
#undef CPlayerManager
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld

enum EInputEventType { };                          // W4EInputEventType@@ (values used numerically here)
enum EInputDeviceType { };                         // W4EInputDeviceType@@
enum EInputKey { };                                // W4EInputKey@@
enum EGameEventPriority { };                       // W4EGameEventPriority@@
enum EProcessedEventType { };                      // W4EProcessedEventType@@

class CInputEvent { public:                        // Ego_r.pdb, 52 bytes
    long             Data[8];                      // +0x00 UInputEventData (32 bytes; a long array keeps the /GS local order)
    EInputDeviceType DeviceType;                   // +0x20
    long             DeviceNumber;                 // +0x24
    EInputEventType  Type;                         // +0x28
    float            CurrentTime;                  // +0x2c
    float            StartTime;                    // +0x30
    CInputEvent();                                 // 0x00A04410
    ~CInputEvent();                                // 0x00A03B30
    void SetAsNull();                              // 0x00A03BE0 (the only void() CInputEvent member in Ego_r/FableWin.pdb; zeroes Type and DeviceType)
    EInputEventType GetType() const;               // 0x00A03B40
    EInputDeviceType GetDeviceType() const;        // 0x00A03B50
    EInputKey GetKey() const;                      // 0x00A03B70
};
class CInputEventScanInfo { public:                // Ego_r.pdb, 8 bytes
    bool Started;                                  // +0x00
    long Index;                                    // +0x04
    CInputEventScanInfo() : Started(false) { }     // ??0CInputEventScanInfo@@QAE@XZ (inline)
};
class CInputManager { public:
    bool IsKeyboardEventInQueue(EInputEventType type, EInputKey key, CInputEvent& event) const;   // 0x009F50A0
    bool EventScanStart(CInputEventScanInfo& scan, CInputEvent& event) const;                    // 0x009F4ED0
    bool EventScanNext(CInputEventScanInfo& scan, CInputEvent& event) const;                     // 0x009F4F10
    void EventScanEnd(CInputEventScanInfo& scan) const;                                          // 0x009F4AC0
    bool IsEventProcessed(const CInputEvent& event) const;                                       // 0x009F4B10
    void FlagEventAsProcessed(const CInputEvent& event);                                         // 0x009F55C0
};
extern CInputManager* PInputManager;               // 0x013B8388

class CProcessedInput : public CProcessedInputLayout { public:
    CProcessedInput()                              // ??0CProcessedInput@@QAE@XZ (inline): each CGameEvent ctor (inline) sets Player -1, Valid false
    {
        GameEvent(0).Player = -1; GameEvent(0).Valid = false;
        GameEvent(1).Player = -1; GameEvent(1).Valid = false;
        GameEvent(2).Player = -1; GameEvent(2).Valid = false;
        GameEvent(3).Player = -1; GameEvent(3).Valid = false;
    }
    CGameEvent& GameEvent(long index) { return reinterpret_cast<CGameEvent*>(GameEvents)[index]; }
    EProcessedEventType GetType() const { return *reinterpret_cast<const EProcessedEventType*>(Type); }
    void Clear();                                  // 0x00A0D300
    void SetPlayer(unsigned long player);          // 0x00A0D3A0
    EGameEventPriority GetPriority();              // 0x00A0D3B0
};
class CPlayerManager : public CPlayerManagerLayout { public:
    long GetPlayerNumberFromJoystickDeviceNumber(long deviceNumber) const;   // 0x00449990
};
class CWorld : public CWorldLayout { };
class CMainGameComponent { public:                 // local model: vptr, PWorld (scoped_ptr<CWorld>) at +0x24 (Ego_r.pdb)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsEditorActive() const;           // slot 9 (+0x24) 0x004197B0 (RTTI vtable 0x0122F180 + FableWin.pdb ?IsEditorActive@CMainGameComponent@@UBE_NXZ)
    unsigned char _pad04[0x24 - 0x04];
    CWorld*       PWorld;                          // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

class CInputEvent;
class CEnginePrimitivePrepareInfo;
struct CBaseClassNonCopyable : CBaseClass { virtual ~CBaseClassNonCopyable(); };
class CLinkedListPosition { public: CBaseClass* Data; CLinkedListPosition* Next; CLinkedListPosition* Previous; };
class CInputProcessManager;
class CAInputProcess : public CBaseClassNonCopyable { public:      // the landed 0x00A0D6B0 model (Ego_r.pdb virtual names)
    CInputProcessManager* PPlayerInterface;
    CLinkedListPosition InputProcessListPos;
    virtual ~CAInputProcess();
    virtual bool AddToInterface(CInputProcessManager&);
    virtual void Init();
    virtual void Cleanup();
    virtual void ProcessInput(const CInputEvent&, CProcessedInput&) = 0;
    virtual void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo&) const;
    virtual void Update(CProcessedInput&);
    virtual bool ProcessWhilePaused() const;
    virtual bool IsInputEventToBeSentToThisInputProcess(const CInputEvent&) const;
};
class CLinkedListScanInfo;
struct CInputProcessList {                         // CLinkedList<CAInputProcess>, 16 bytes
    CLinkedListPosition* Head; CLinkedListPosition* Tail; int NoEntries; mutable CLinkedListScanInfo* FirstScanInfo;
    CAInputProcess* StartScanAtHead(CLinkedListScanInfo*) const;
    CAInputProcess* ScanForward(CLinkedListScanInfo*) const;
    void RemoveScanInfo(CLinkedListScanInfo*) const;
};
class CLinkedListScanInfo { public:                // Ego_r.pdb, 28 bytes
    CLinkedListPosition* Current; CLinkedListPosition* Next; CLinkedListPosition* Previous;
    bool Started, Forwards;
    CLinkedListScanInfo* NextScan; CLinkedListScanInfo* PreviousScan;
    const CInputProcessList* List;
    CLinkedListScanInfo() : Started(false), NextScan(0), PreviousScan(0), List(0) { }
};
inline CAInputProcess* CInputProcessList::StartScanAtHead(CLinkedListScanInfo* s) const {
    s->NextScan = FirstScanInfo; if (FirstScanInfo) FirstScanInfo->PreviousScan = s;
    FirstScanInfo = s; s->List = this; s->PreviousScan = 0; s->Started = true;
    CLinkedListPosition* p = Head;
    if (p == 0) { s->Current = 0; s->Next = 0; s->Previous = 0; return 0; }
    s->Current = p; s->Next = s->Current->Next; s->Previous = 0;
    return static_cast<CAInputProcess*>(s->Current->Data);
}
inline CAInputProcess* CInputProcessList::ScanForward(CLinkedListScanInfo* s) const {
    s->Current = s->Next;
    if (s->Current == 0) return 0;
    s->Next = s->Current->Next; s->Previous = s->Current->Previous;
    return static_cast<CAInputProcess*>(s->Current->Data);
}
inline void CInputProcessList::RemoveScanInfo(CLinkedListScanInfo* s) const {
    if (!s->PreviousScan) {
        FirstScanInfo = s->NextScan;
        if (s->NextScan) s->NextScan->PreviousScan = 0;
    } else {
        s->PreviousScan->NextScan = s->NextScan;
        if (s->NextScan) s->NextScan->PreviousScan = s->PreviousScan;
    }
    s->Started = false; s->NextScan = 0; s->PreviousScan = 0; s->List = 0;
}
#define CGamePlayerInterface CGamePlayerInterfaceLayout
#include "engine/CGamePlayerInterface.h"
#undef CGamePlayerInterface
class CGamePlayerInterface : public CGamePlayerInterfaceLayout {
    // +0x04: CInputProcessManager::InputProcessList (the header flattens the CInputProcessManager base)
    const CInputProcessList& InputProcessList() const { return *reinterpret_cast<const CInputProcessList*>(_pad_0x04); }
    bool GetInputProcessEvent(CProcessedInput& processedInput) const;
};

bool CGamePlayerInterface::GetInputProcessEvent(CProcessedInput& processedInput) const
{
    CLinkedListScanInfo scan;
    CInputEvent event;
    CInputEvent bestEvent;
    CProcessedInput result;
    CProcessedInput bestResult;
    bestEvent.SetAsNull();
    bestResult.Clear();
    bool altDown = false;
    if (g_pMainGameComponent && !g_pMainGameComponent->IsEditorActive()) {
        CInputEvent query;
        altDown = PInputManager->IsKeyboardEventInQueue((EInputEventType)2, (EInputKey)0x38, query);
    }
    long bestPriority = 0;
    bool found = false;
    CInputEventScanInfo eventScan;
    for (bool more = PInputManager->EventScanStart(eventScan, event); more; more = PInputManager->EventScanNext(eventScan, event)) {
        if (altDown && event.GetDeviceType() == 2 && event.GetKey() == 0xf)
            continue;
        if (event.GetType() == 0)
            continue;
        if (PInputManager->IsEventProcessed(event))
            continue;
        for (CAInputProcess* process = InputProcessList().StartScanAtHead(&scan); process; process = InputProcessList().ScanForward(&scan)) {
            result.Clear();
            if (process->IsInputEventToBeSentToThisInputProcess(event)) {
                result.SetPlayer(PlayerManager->GetPlayerNumberFromJoystickDeviceNumber(event.DeviceNumber));
                if (process->ProcessWhilePaused() || !g_pMainGameComponent->PWorld->Paused)
                    process->ProcessInput(event, result);
            }
            switch (result.GetType()) {
            case 1:
                found = true;
                if (result.GetPriority() > bestPriority) {
                    bestPriority = result.GetPriority();
                    bestResult = result;
                    bestEvent = event;
                }
                break;
            case 2:
                PInputManager->FlagEventAsProcessed(event);
                goto endScan;
            }
        }
    endScan:
        InputProcessList().RemoveScanInfo(&scan);
    }
    PInputManager->EventScanEnd(eventScan);
    if (found) {
        processedInput = bestResult;
        if (bestEvent.GetType())
            PInputManager->FlagEventAsProcessed(bestEvent);
        return true;
    }
    return false;
}