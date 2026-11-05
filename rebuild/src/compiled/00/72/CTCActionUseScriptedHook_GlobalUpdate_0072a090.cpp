// CTCActionUseScriptedHook::GlobalUpdate(bool) -- while a confirmation is pending consume the confirm message (setting PerformAction on a yes); otherwise perform the queued action: teleport the hero through the connected entrance (door-latch sound, or the locked-door sound), then start a spline scripted camera mode on the camera track when its camera point is a track point
// Leaf: ?GlobalUpdate@CTCActionUseScriptedHook@@UAEX_N@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base.
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072A090, 844 bytes, thiscall `ret 4`, /GS cookie; vtable 0x0126664C slot 26
//   (Ego_r.pdb ?GlobalUpdate@CTCActionUseScriptedHook@@UAEX_N@Z).
// Callees (Ego_r / FableWin spellings): CTCBase::GetWorld() const 0x00686A70 (IBE, CWorld&), CTCBase::GetPlayerManager()
//   0x00686D40 (IAE), CMessageEventManager::FindMostRecentMessage<CMessageEventFilter_IsOfType> 0x004AB8B0 (member template, QBE)
//   / RemoveMessage 0x00493B5B, CBaseIntelligentPointer::GetPItem 0x00A01B50 (CBaseObject*), CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound 0x0040F020, CTCSoundPlayer::PlayAttachedCriteria(const CCharString&, bool, bool, bool, long) 0x0066BD00
//   (type 0x10), CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CThing::GetPos 0x004C73D0,
//   CGameEvent::AddData 0x009F1760 (inside the inline AddFloat / AddUBYTE), CWorld::HandleMoveHeroToRegionGameEvent 0x0049EAF0,
//   CScriptModuleSpline(long, long) 0x00852710 (Ego_r ??0CScriptModuleSpline@NCameraModes@@QAE@JJ@Z, 0x64 bytes = Ego_r sizeof),
//   CCountedPointer<CScriptModuleBase>(CScriptModuleBase*) 0x005ED270 / ~CCountedPointer 0x005ED2C0 (engine header, COMDATs),
//   CScriptModuleBase::SetPCameraPoint(CThing&) 0x0072A4A0 (the COMDAT right after this function: it sets PCamPoint +0x04, the
//   Ego_r CIntelligentPointer<CThing> member), CGameCameraManager::SetScriptedCameraMode(CCountedPointer<CScriptModuleBase>) 0x00699E60.
//   Virtual: CAIGameCameraBase::GetCameraManager slot 9 (+0x24), CTCCameraPointDefinitionBase::GetCameraPointType slot 28 (+0x70),
//   the physics component's GetFacingAngleXY slot 63 (+0xFC).
// Layout: Ego_r.pdb CTCActionUseScriptedHook (ForceConfirmation +0xd ... PCameraTrack +0x38), CMessageEvent::ExtraData +0x3c,
//   CWorld PCurrentGameCamera +0x34 / PMessageEventManager +0x60 (CCountedPointer), CThing UsingComponents +0x20 / Components +0x44.
// Shape: the FableWin.pdb inline accessors CWorld::GetMessageEventManager() / GetGameCamera(), CThing::GetTC<T> with the one-ternary
//   store (it fixed the camera-point vcall loading the component straight into ecx), the inline template
//   CTCCameraPointDefinitionBase::GetPointType<CTCCameraPointTrack> (its out-of-line COMDAT sits at 0x0072A430) and the engine
//   CCountedPointer (mode Data kept in esi across SetPCameraPoint). The prior's three register residues all came from stand-ins.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };
enum EMessageEventType { EMessageEventType_Force32 = 0x7fffffff };
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
class CTCBase;
class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngleXY() const;             // slot 63 (+0xFC)
};
class CTCSoundPlayer {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x10; }
    unsigned long PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d);   // 0x0066BD00
};
class CTCCameraPointDefinitionBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetCameraPointType() const;            // slot 28 (+0x70)
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x3f; }
    template <class T> __forceinline bool GetPointType(T** out)
    {
        if (GetCameraPointType() == T::POINT_TYPE_ID) {
            *out = static_cast<T*>(this);
            return true;
        }
        return false;
    }
};
class CTCCameraPointTrack : public CTCCameraPointDefinitionBase { public:
    enum { POINT_TYPE_ID = 7 };
};
class CBaseObject { };
class CThing : public CBaseObject {                              // partial view, retail offsets
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      _pad00[0x20];
    unsigned long      UsingComponents[9];                       // +0x20 std::bitset<274>
    TCMap              Components;                               // +0x44
    unsigned char      _pad54[0x60 - 0x54];
    CTCPhysicsBase*    PhysicsTC;                                // +0x60
    unsigned char      _pad64[0x90 - 0x64];
    signed char        PlayerNumber;                             // +0x90
    unsigned char      Dead : 1;                                 // +0x91 bit 0
    bool IsDead() const { return Dead; }
    unsigned long GetPlayerNumber() const { return PlayerNumber; }
    const C3DVector& GetPos() const;                             // 0x004C73D0
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    __forceinline bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::StatGetInterfaceType()))
            return false;
        ETCInterfaceType key = T::StatGetInterfaceType();
        TCMap* map = &Components;
        TCMap::value_type* it = map->LowerBound(key);
        *out = (T*)((it == map->end() || T::StatGetInterfaceType() < it->first) ? map->end() : it)->second;
        return true;
    }
};
class CThingPlayerCreature : public CThing { };
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;                                          // +0x04
    CBaseObject* GetPItem() const;                               // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return (T*)GetPItem(); }
};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
class CPlayerManager { public: CPlayer& GetMainPlayer(); };                        // 0x00449970
class CGameEvent {
public:
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddData(long size, const void* data);          // 0x009F1760
    void AddFloat(float value) { AddData(4, &value); }
    void AddUBYTE(unsigned char value) { AddData(1, &value); }
    void SetPlayer(long player) { Player = (signed char)player; }
};
class CMessageEvent {
public:
    unsigned char _pad[0x3c];
    long ExtraData;                                     // +0x3c
};
class CMessageEventFilter_IsOfType {
public:
    EMessageEventType Type;
    CMessageEventFilter_IsOfType(EMessageEventType type) : Type(type) { }
};
class CMessageEventManager {
public:
    template <class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter) const;   // <CMessageEventFilter_IsOfType> 0x004AB8B0
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};
void operator delete(void* p);                          // 0x00BFE9BC
#include "engine/CCountedPointer.h"
namespace NCameraModes {
class CScriptModuleBase {                               // Ego_r.pdb: PCamPoint CIntelligentPointer<CThing> +0x04
public:
    unsigned char _body[0x4c];
    void SetPCameraPoint(CThing& point);                // 0x0072A4A0 (COMDAT in this object)
};
class CScriptModuleSpline : public CScriptModuleBase {  // 0x64 bytes (Ego_r.pdb sizeof 100)
public:
    unsigned char _body4c[0x64 - 0x4c];
    CScriptModuleSpline(long a, long b);                // 0x00852710
};
}
class CGameCameraManager {
public:
    void SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> mode);   // 0x00699E60
};
class CAIGameCameraBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(const CGameCameraManager** manager) const;   // slot 10 (+0x28)
    virtual bool GetCameraManager(CGameCameraManager** manager);               // slot 9 (+0x24)
};
class CWorld {
public:
    unsigned char _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;              // +0x34
    unsigned char _pad38[0x60 - 0x38];
    CCountedPointer<CMessageEventManager> PMessageEventManager;   // +0x60
    CAIGameCameraBase* GetGameCamera() { return PCurrentGameCamera; }   // FableWin.pdb (inline)
    CMessageEventManager& GetMessageEventManager() { return *PMessageEventManager; }   // FableWin.pdb QAE (inline)
    const CMessageEventManager& GetMessageEventManager() const { return *PMessageEventManager; }   // FableWin.pdb QBE (inline)
    void HandleMoveHeroToRegionGameEvent(const CGameEvent& event);   // 0x0049EAF0
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
protected:
    CWorld& GetWorld() const;                           // 0x00686A70
    CPlayerManager& GetPlayerManager();                 // 0x00686D40
};
class CTCActionUseScriptedHook : public CTCBase {
public:
    bool Usable;                                        // +0x0c
    bool ForceConfirmation;                             // +0x0d
    bool WaitingForConfirmation;                        // +0x0e
    bool PerformAction;                                 // +0x0f
    bool TeleportToRegionEntrance;                      // +0x10
    unsigned __int64 EntranceConnectedToUID;            // +0x18
    CIntelligentPointer<CThing> PEntranceConnectedTo;   // +0x20
    bool EntranceLocked;                                // +0x28
    unsigned __int64 CameraTrackUID;                    // +0x30
    CIntelligentPointer<CThing> PCameraTrack;           // +0x38
    CCharString SoundName;                              // +0x40
    CCharString AnimationName;                          // +0x44
    long ReplacementObject;                             // +0x48
    bool ReversedOnMiniMap;                             // +0x4c
    bool HiddenOnMiniMap;                               // +0x4d
    virtual void GlobalUpdate(bool force);
};

void CTCActionUseScriptedHook::GlobalUpdate(bool force)
{
    if (ForceConfirmation && WaitingForConfirmation) {
        CMessageEventManager& manager = GetWorld().GetMessageEventManager();
        const CMessageEvent* message = manager.FindMostRecentMessage(CMessageEventFilter_IsOfType((EMessageEventType)0x10));
        if (message) {
            if (message->ExtraData == 1)
                PerformAction = true;
            WaitingForConfirmation = false;
            manager.RemoveMessage(message);
        }
        return;
    }
    if (!PerformAction)
        return;
    if (TeleportToRegionEntrance && PEntranceConnectedTo.Get()) {
        CTCSoundPlayer* sound = 0;
        ParentThing->GetTC(&sound);
        if (EntranceLocked) {
            if (sound)
                sound->PlayAttachedCriteria("DOOR_OPEN_FAIL", false, false, false, -1);
        } else {
            CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            if (hero && !hero->IsDead()) {
                CGameEvent event(0x2c);
                event.AddFloat(PEntranceConnectedTo->GetPos().X);
                event.AddFloat(PEntranceConnectedTo->GetPos().Y);
                event.AddFloat(PEntranceConnectedTo->GetPos().Z);
                event.AddFloat(PEntranceConnectedTo->GetPhysicsTC()->GetFacingAngleXY());
                event.AddUBYTE(false);
                event.AddUBYTE(false);
                event.AddUBYTE(true);
                event.SetPlayer(hero->GetPlayerNumber());
                GetWorld().HandleMoveHeroToRegionGameEvent(event);
                sound->PlayAttachedCriteria("BIG_DOOR_UNLATCH", true, false, false, -1);
            }
        }
    }
    CTCCameraPointDefinitionBase* point;
    CTCCameraPointTrack* track;
    if (PCameraTrack.Get() && PCameraTrack->GetTC(&point) && point->GetPointType(&track)) {
        CGameCameraManager* cameraManager = 0;
        GetWorld().GetGameCamera()->GetCameraManager(&cameraManager);
        CCountedPointer<NCameraModes::CScriptModuleBase> mode(new NCameraModes::CScriptModuleSpline(0, -1));
        mode->SetPCameraPoint(*PCameraTrack.Get());
        cameraManager->SetScriptedCameraMode(mode);
    }
    PerformAction = false;
}