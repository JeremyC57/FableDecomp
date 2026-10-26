// CTCActionUseScriptedHook::CTCActionUseScriptedHook(CThing* owner) -- CTCBase(owner), own vtable, usable and every flag cleared, the two UIDs zeroed, the two intelligent pointers and the two strings default-built, then registered for global updates
// Leaf: ??0CTCActionUseScriptedHook
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00729A90, length 142 bytes (+2 int3 padding). `ret 4`. Vtable 0x0126664C.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 and
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (x2: the inline CIntelligentPointer(T*) ctor with a null item, vtable
// 0x01238C6C), CCharString::CCharString() @ 0x0099E4B0 (x2), CTCBase::AddToGlobalTCs @ 0x00686ED0 (body).
// Shape: one shared zero register (ebx) for every false/zero initialiser; members in declaration order, each 64-bit UID as two
// dword stores; the intelligent pointer's vtable store sits between its base ctor and SetPItem(0).
// Layout: PDB CTCActionUseScriptedHook (0x50; derives from CTCBase directly, OnActionUse is non-virtual): Usable +0xC,
// ForceConfirmation +0xD, WaitingForConfirmation +0xE, PerformAction +0xF, TeleportToRegionEntrance +0x10,
// EntranceConnectedToUID +0x18, PEntranceConnectedTo +0x20, EntranceLocked +0x28, CameraTrackUID +0x30, PCameraTrack +0x38,
// SoundName +0x40, AnimationName +0x44, ReplacementObject +0x48, ReversedOnMiniMap +0x4C, HiddenOnMiniMap +0x4D.
class CThing;
class CBaseObject;

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {     // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CCharString { public:
    void* PString;
    CCharString();                              // 0x0099E4B0
};

class CTCBase {
public:
    CTCBase(CThing* owner);                     // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
public:
    void AddToGlobalTCs();   // 0x00686ED0
};

class CTCActionUseScriptedHook : public CTCBase {  // vtable 0x0126664C
public:
    bool Usable;                                // +0x0c
    bool ForceConfirmation;                     // +0x0d
    bool WaitingForConfirmation;                // +0x0e
    bool PerformAction;                         // +0x0f
    bool TeleportToRegionEntrance;              // +0x10
    unsigned __int64 EntranceConnectedToUID;    // +0x18
    CIntelligentPointer<CThing> PEntranceConnectedTo;   // +0x20
    bool EntranceLocked;                        // +0x28
    unsigned __int64 CameraTrackUID;            // +0x30
    CIntelligentPointer<CThing> PCameraTrack;           // +0x38
    CCharString SoundName;                      // +0x40
    CCharString AnimationName;                  // +0x44
    long ReplacementObject;                     // +0x48
    bool ReversedOnMiniMap;                     // +0x4c
    bool HiddenOnMiniMap;                       // +0x4d
    CTCActionUseScriptedHook(CThing* owner);
    virtual ~CTCActionUseScriptedHook();
};

CTCActionUseScriptedHook::CTCActionUseScriptedHook(CThing* owner)
    : CTCBase(owner)
    , Usable(true)
    , ForceConfirmation(false)
    , WaitingForConfirmation(false)
    , PerformAction(false)
    , TeleportToRegionEntrance(false)
    , EntranceConnectedToUID(0)
    , PEntranceConnectedTo(0)
    , EntranceLocked(false)
    , CameraTrackUID(0)
    , PCameraTrack(0)
    , ReplacementObject(0)
    , ReversedOnMiniMap(false)
    , HiddenOnMiniMap(false)
{
    AddToGlobalTCs();
}