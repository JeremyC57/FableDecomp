// CCreatureActionBase::InitialiseActionEvents() -- unless the editor is active, for each embedded event name of the action's animation look up the registered action event by the name's first word, allocate a copy at (anim delay + frames-per-second * event time) for the creature and the current game camera's camera manager, and keep it in ActionEvents when it accepts the full name
// Leaf: ?InitialiseActionEvents@CCreatureActionBase@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006946C0, 455 bytes (+int3), thiscall, `and esp,-8` frame, plain `ret`. Ego_r.pdb
//   ?InitialiseActionEvents@CCreatureActionBase@@AAEXXZ (private).
// Callees: CMainGameComponent::IsEditorActive (vtable slot 9 on g_pMainGameComponent 0x013B86A0, manifest label at the slot
//   target 0x004197B0); CBaseIntelligentPointer::GetPItem 0x00A01B50; CThing::GetWorld 0x004C7AA0 ([g_pThingManager]+0x24);
//   CWorld::GetGameCamera() (inline, PCurrentGameCamera +0x34) -> GetCameraManager(CGameCameraManager**) (slot 9, Ego_r.pdb
//   ?GetCameraManager@CGameCameraManager@@UAE_NPAPAV1@@Z); CCreatureActionBase virtual slots 6 (GetAnimDelayTime) / 7
//   (animation index); NAnimationEvents::GetGameEventManager 0x006F90B0; CManager::GetEventNames 0x006F9B20 /
//   GetEventTimeInSeconds 0x006F9790; CCharString::FindCharPos 0x0099E5A0 / GetSubString 0x0099EC70 / ~CCharString 0x0099EAE0;
//   the RegisteredActionEvents map lookup 0x00696340 (static map 0x013BA8BC); CActionEventBase::Alloc (slot 3) /
//   SetDataFromString (slot 4); CCountedPointer<CActionEventBase>::Reset 0x00695A00; the ActionEvents push_back 0x00696FE0;
//   _ftol 0x00BFEA70, free 0x00BFEA14, operator delete 0x00BFE9BC; frames-per-second global 0x01375550.
// Layout (local model): engine/CCreatureActionBase.h flattens the vptr (its virtual slots 6/7 are called here), so the class
//   is a local view: PCreature +0x08, ActionEvents +0x84 (STLport vector). Shared engine/CCountedPointer.h for the event.
// Shape: the decisive lever is a same-TU body for BOTH out-of-line callees that take the event: CCountedPointer::Reset
//   (the header body, explicit noinline specialisation, as 0x00695A00) and the push_back (an STLport-shaped stand-in that
//   copies the pointer). With both visible, VC7.1 knows the event's Info after them and the inline ~CCountedPointer
//   at the loop end loads Info straight into edi and tests the `dec [edi]` flags, as retail. With either one opaque the
//   release loaded eax, copied it to edi and re-compared [edi] (455v455, 10 lines + loop padding). The PDB inline
//   CWorld::GetGameCamera() accessor gives the camera chain's ecx/eax roles (the direct member read swapped them).
// Names (objdump-checked): ?InitialiseActionEvents@CCreatureActionBase@@AAEXXZ, ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ,
//   ?GetWorld@CThing@@IAEAAVCWorld@@XZ (non-const: the result calls the non-const GetGameCamera; PeekWorld is the const twin),
//   ?GetGameEventManager@NAnimationEvents@@YIAAVCManager@1@XZ (probable: Ego_r has Game/Sound twins of this shape),
//   NAnimationEvents::CManager::GetEventNames / GetEventTimeInSeconds (Ego_r QBE spellings). The map type (map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >
//   CCreatureActionBase::RegisteredActionEvents, Ego_r.pdb static) and its STLport _M_find are modelled descriptively here
//   (one-word node view); see NOTES.md.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
void* __cdecl operator new(unsigned int size);
inline void* operator new(unsigned int, void* where) { return where; }
class CCharStringData { public: char* PData; unsigned long StringLength; };
class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                                         // 0x0099EAE0
    long FindCharPos(char c) const;                         // 0x0099E5A0
    CCharString GetSubString(unsigned long start, unsigned long length) const;   // 0x0099EC70
    __forceinline long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
};
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport vector<CCharString> (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    static __forceinline void _Destroy(_Tp* first, _Tp* last)
    {
        for (; first != last; ++first)
            first->~_Tp();
    }
    __forceinline ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
}
namespace NAnimationEvents {
class CManager {
public:
    void GetEventNames(unsigned long animation, std::vector<CCharString>& names) const;          // 0x006F9B20
    float GetEventTimeInSeconds(unsigned long animation, const CCharString& name) const;         // 0x006F9790
};
CManager& __fastcall GetGameEventManager();                 // 0x006F90B0
}
extern long g_FramesPerSecond;                              // 0x01375550 (descriptive)
class CMainGameComponent {
public:
    virtual void UnprovenSlot00(); virtual void UnprovenSlot01(); virtual void UnprovenSlot02(); virtual void UnprovenSlot03();
    virtual void UnprovenSlot04(); virtual void UnprovenSlot05(); virtual void UnprovenSlot06(); virtual void UnprovenSlot07();
    virtual void UnprovenSlot08();
    virtual bool IsEditorActive() const;                    // slot 9
};
extern CMainGameComponent* g_pMainGameComponent;           // 0x013B86A0
class CGameCameraManager;
class CAIGameCameraBase {
public:
    virtual void UnprovenSlot00(); virtual void UnprovenSlot01(); virtual void UnprovenSlot02(); virtual void UnprovenSlot03();
    virtual void UnprovenSlot04(); virtual void UnprovenSlot05(); virtual void UnprovenSlot06(); virtual void UnprovenSlot07();
    virtual void UnprovenSlot08();
    virtual bool GetCameraManager(CGameCameraManager** manager);   // slot 9
};
class CWorld {
public:
    unsigned char _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;                  // +0x34
    CAIGameCameraBase* GetGameCamera() { return PCurrentGameCamera; }   // Ego_r.pdb ?GetGameCamera@CWorld@@QAEPAVCAIGameCameraBase@@XZ
};
class CCreatureActionBase;
class CThing {
    friend class CCreatureActionBase;
protected:
    CWorld& GetWorld();                                     // 0x004C7AA0 (Ego_r.pdb protected IAE)
};
class CThingCreatureBase : public CThing { };
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    void* PData;
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};
class CCreatureActionBase;
namespace NCreatureAction {
class CActionEventBase {
public:
    virtual ~CActionEventBase();
    virtual void Dispatch(CCreatureActionBase& action) = 0;
    virtual CCharString GetHook() const = 0;
    virtual CActionEventBase* Alloc(long activation_frame, CThingCreatureBase& thing, CGameCameraManager& camera) const = 0;   // slot 3
    virtual bool SetDataFromString(const CCharString& data) = 0;   // slot 4
};
}
#include "engine/CCountedPointer.h"
typedef CCountedPointer<NCreatureAction::CActionEventBase> CActionEventPointer;
struct CActionEventVector {                                 // STLport vector<CCountedPointer<CActionEventBase> > (12 bytes)
    CActionEventPointer* _M_start;
    CActionEventPointer* _M_finish;
    CActionEventPointer* _M_end_of_storage;
    void push_back(const CActionEventPointer& value);       // 0x00696FE0
};
#pragma auto_inline(off)
__declspec(noinline) void CActionEventVector::push_back(const CActionEventPointer& value)   // 0x00696FE0 (same-TU stand-in)
{
    if (_M_finish != _M_end_of_storage) {
        new (_M_finish) CActionEventPointer(value);
        ++_M_finish;
    }
}
template <> __declspec(noinline) void CActionEventPointer::Reset(NCreatureAction::CActionEventBase* data)   // 0x00695A00
{
    ReleaseData();
    Data = data;
    if (data)
        Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
}
#pragma auto_inline(on)
struct SRegisteredActionEventNode {                         // map node: key +0x10, CCountedPointer value +0x14
    unsigned char _pad[0x10];
    CCharString Key;                                        // +0x10
    CActionEventPointer Event;                              // +0x14
};
struct CRegisteredActionEventMap {                          // descriptive view of the static map
    SRegisteredActionEventNode* Header;                     // end()
    long Count;
    SRegisteredActionEventNode* Find(const CCharString& key);   // 0x00696340
};

class CCreatureActionBase {
public:
    virtual void UnprovenSlot00(); virtual void UnprovenSlot01(); virtual void UnprovenSlot02(); virtual void UnprovenSlot03();
    virtual void UnprovenSlot04(); virtual void UnprovenSlot05();
    virtual long GetAnimDelayTime() const;                  // slot 6
    virtual long GetAnimationIndex() const;                 // slot 7 (descriptive)
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;      // +0x08
    unsigned char _pad10[0x84 - 0x10];
    CActionEventVector ActionEvents;                        // +0x84
    static CRegisteredActionEventMap RegisteredActionEvents;   // 0x013BA8BC
private:
    void InitialiseActionEvents();
};

void CCreatureActionBase::InitialiseActionEvents()
{
    if (g_pMainGameComponent->IsEditorActive())
        return;
    CGameCameraManager* camera_manager;
    PCreature->GetWorld().GetGameCamera()->GetCameraManager(&camera_manager);
    long animation = GetAnimationIndex();
    if (animation <= 0)
        return;
    long frames_per_second = g_FramesPerSecond;
    NAnimationEvents::CManager* manager = &NAnimationEvents::GetGameEventManager();
    std::vector<CCharString> names;
    manager->GetEventNames(animation, names);
    for (CCharString* it = names._M_start; it != names._M_finish; ++it) {
        long end = it->FindCharPos(' ');
        if (end == -1)
            end = it->GetLength();
        CCharString hook = it->GetSubString(0, end);
        SRegisteredActionEventNode* node = RegisteredActionEvents.Find(hook);
        if (node != RegisteredActionEvents.Header) {
            float seconds = manager->GetEventTimeInSeconds(animation, *it);
            long frame = (long)(GetAnimDelayTime() + frames_per_second * seconds);
            NCreatureAction::CActionEventBase* prototype = node->Event.Data;
            CActionEventPointer event;
            event.Reset(prototype->Alloc(frame, *PCreature.operator->(), *camera_manager));
            if (event->SetDataFromString(*it))
                ActionEvents.push_back(event);
        }
    }
}