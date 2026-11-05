// CTCActionUseScriptedHook::~CTCActionUseScriptedHook() -- the scripted-hook action-use component's destructor: its vtable, removal from the global TC update list, then AnimationName / SoundName, PCameraTrack / PEntranceConnectedTo in reverse order, then ~CTCBase
// Leaf: ??1CTCActionUseScriptedHook@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00729A50, 54 bytes, thiscall, tail `jmp ~CTCBase` 0x00686830.
// Identity (proven, two sources): it stores the CTCActionUseScriptedHook vtable 0x0126664C first (rtti-vtables.tsv, RTTI
//   .?AVCTCActionUseScriptedHook@@ : CTCBase : CBaseClassNonCopyable : CBaseClass); Ego_r.pdb ??1CTCActionUseScriptedHook@@UAE@XZ
//   (0x00740070) sits right before ??0 (0x007400B0), as retail's 0x00729A50 sits right before the landed ctor 0x00729A90; FableWin.pdb
//   carries the same symbol (0x0253CA60). No direct caller: the scalar deleting dtor 0x0072A450 (slot 0) expands this body inline,
//   so the out-of-line copy is the user destructor's own COMDAT.
// Callees: CTCBase::RemoveFromGlobalTCs 0x00686DF0 (?RemoveFromGlobalTCs@CTCBase@@QAEXXZ, Ego_r + FableWin), ~CCharString 0x0099EAE0,
//   ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10 (the intelligent pointers' generated dtors call the base dtor directly),
//   ??1CTCBase@@UAE@XZ 0x00686830 (tail).
// Layout: Ego_r.pdb CTCActionUseScriptedHook (0x50; no STL member, so the offsets are retail's, as the landed ctor 0x00729A90 and
//   ??_G 0x0072A450 confirm): Usable +0xC .. TeleportToRegionEntrance +0x10, EntranceConnectedToUID +0x18, PEntranceConnectedTo +0x20,
//   EntranceLocked +0x28, CameraTrackUID +0x30, PCameraTrack +0x38, SoundName +0x40, AnimationName +0x44, ReplacementObject +0x48,
//   ReversedOnMiniMap +0x4C, HiddenOnMiniMap +0x4D. engine/CTCBase.h retail branch (FABLE_RETAIL_CTCBASE; it declares
//   RemoveFromGlobalTCs since chunk 712); CIntelligentPointer / CBaseIntelligentPointer have no shared header (local model as in the landed ctor).
// Shape: a user destructor `{ RemoveFromGlobalTCs(); }` defined out of line: VC7.1 stores the vtable, runs the body, then the members
//   in reverse declaration order, then tail-jumps the base dtor.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                 // CTCBase : CBaseClassNonCopyable (vtable 0x0125BE0C)
#include "fable_string.h"                                   // CCharString (~CCharString 0x0099EAE0)

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCActionUseScriptedHook : public CTCBase {           // vtable 0x0126664C
public:
    CTCActionUseScriptedHook(CThing& thing);                // ??0CTCActionUseScriptedHook@@QAE@AAVCThing@@@Z (0x00729A90)
    virtual ~CTCActionUseScriptedHook();
    virtual ETCInterfaceType GetInterfaceType() const;      // slot 21 0x0072A3E0
    virtual CCharString GetClassTypeName() const;           // slot 22 0x0072A3F0
    bool                        Usable;                     // +0x0C
    bool                        ForceConfirmation;          // +0x0D
    bool                        WaitingForConfirmation;     // +0x0E
    bool                        PerformAction;              // +0x0F
    bool                        TeleportToRegionEntrance;   // +0x10
    unsigned __int64            EntranceConnectedToUID;     // +0x18
    CIntelligentPointer<CThing> PEntranceConnectedTo;       // +0x20
    bool                        EntranceLocked;             // +0x28
    unsigned __int64            CameraTrackUID;             // +0x30
    CIntelligentPointer<CThing> PCameraTrack;               // +0x38
    CCharString                 SoundName;                  // +0x40
    CCharString                 AnimationName;              // +0x44
    long                        ReplacementObject;          // +0x48
    bool                        ReversedOnMiniMap;          // +0x4C
    bool                        HiddenOnMiniMap;            // +0x4D
};
FABLE_STATIC_ASSERT(sizeof(CTCActionUseScriptedHook) == 0x50);

CTCActionUseScriptedHook::~CTCActionUseScriptedHook()
{
    RemoveFromGlobalTCs();
}