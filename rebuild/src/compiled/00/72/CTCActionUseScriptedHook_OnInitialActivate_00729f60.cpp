// CTCActionUseScriptedHook::OnInitialActivate() -- base activation, make sure the thing is targetable (adding CTCTargeted when missing) and has entity events, resolve the connected entrance, and look up the camera track by UID
// Leaf: ?OnInitialActivate@CTCActionUseScriptedHook@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00729F60, 290 bytes (+14 int3 padding), `ret`; Ego_r.pdb
//   ?OnInitialActivate@CTCActionUseScriptedHook@@UAEXXZ (CTCBase slot 6).
// Callees: CTCBase::OnInitialActivate 0x00686870 (called non-virtually), CCharString(const char*, long)
//   0x0099EBF0, ~CCharString 0x0099EAE0, CThing::AddTC(const CCharString&, bool, const CTCParamsBase*) 0x004C9D60,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined lookup of CTCTargeted, type 9),
//   CTCTargeted::AddTargetingFlag(ETargetingType) 0x007EA240, CTCActionUseScriptedHook::SetUpPEntranceConnectedToFromUID 0x00729BC0 (Ego_r.pdb private
//   ?SetUpPEntranceConnectedToFromUID@CTCActionUseScriptedHook@@AAEXXZ; it resolves EntranceConnectedToUID),
//   CTCBase::GetThingManager 0x00686AE0, CThingManager::GetPThingWithUID(unsigned __int64) 0x0051ED80,
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10.
//   Literals "CTCTargeted" 0x01239FC0, "CTCEntityEvents" 0x0123ADF8.
// Layout = Ego_r.pdb CTCActionUseScriptedHook: EntranceConnectedToUID +0x18, PEntranceConnectedTo +0x20, CameraTrackUID
//   +0x30, PCameraTrack +0x38; CTCTargeted::Targetable +0x10; interface 0x42 = CTCEntityEvents.
// Shape: retail headers (CTCBase, CThing with the STLport bitset<274>::test HasTC, CTCTargeted, CThingManager,
//   CIntelligentPointer). The range-checked bitset test keeps the parent thing a value, so VC7.1 loads it into ecx right
//   after the CCharString ctor (the chunk 993 fix of the retry-10 "AddTC this-load before the pushes" gap); each class
//   name is AddTC's implicit const-reference temporary; the fresh CTCTargeted comes from GetTC (an unset local when
//   absent) and gets SetTargetable(true) before AddTargetingFlag(1); the camera track is assigned through the inline
//   CIntelligentPointer operator= (GetPItem then SetPItem) from a CIntelligentPointer(thing) temporary.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTCTARGETED
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingManager.h"
#include "engine/CTCTargeted.h"
enum ETargetingType { TARGETING_TYPE_1 = 1 };          // descriptive (the value AddTargetingFlag receives here)
class CTCActionUseScriptedHook : public CTCBase {
public:
    virtual void OnInitialActivate();
private:
    void SetUpPEntranceConnectedToFromUID();            // 0x00729BC0
    unsigned char    _pad0c[0x18 - 0x0c];
    unsigned __int64 EntranceConnectedToUID;            // +0x18
    CIntelligentPointer<CThing> PEntranceConnectedTo;   // +0x20
    bool             EntranceLocked;                    // +0x28
    unsigned char    _pad29[0x30 - 0x29];
    unsigned __int64 CameraTrackUID;                    // +0x30
    CIntelligentPointer<CThing> PCameraTrack;           // +0x38
};

void CTCActionUseScriptedHook::OnInitialActivate()
{
    CTCBase::OnInitialActivate();
    if (!ParentThing->HasTC(TCI_TARGETED)) {
        ParentThing->AddTC("CTCTargeted", false, 0);
        CTCTargeted* targeted;
        ParentThing->GetTC(&targeted);
        targeted->SetTargetable(true);
        targeted->AddTargetingFlag(TARGETING_TYPE_1);
    }
    if (!ParentThing->HasTC(TCI_ENTITY_EVENTS)) {
        ParentThing->AddTC("CTCEntityEvents", false, 0);
    }
    if (EntranceConnectedToUID != 0)
        SetUpPEntranceConnectedToFromUID();
    if (CameraTrackUID != 0)
        PCameraTrack = CIntelligentPointer<CThing>(GetThingManager().GetPThingWithUID(CameraTrackUID));
}