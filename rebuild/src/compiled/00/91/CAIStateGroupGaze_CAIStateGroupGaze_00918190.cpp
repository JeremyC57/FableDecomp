// CAIStateGroup_Gaze::CAIStateGroup_Gaze() -- default constructor: base chain, the gaze-marker pointer, the running parameters and the two marker definition indices
// Leaf: ??0CAIStateGroup_Gaze
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00918190, length 156 bytes (+ int3 padding).
// Callees: CBase::CBase @ 0x0099A2D0 (the root, out of line), CCharString::CCharString() @ 0x0099E4B0 (Name at +0xc),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CCharString::CCharString(const char*, long) @ 0x0099EBF0
// / ~CCharString @ 0x0099EAE0 on "GAZE_OUT_OF_BUILDING_MARKER" @ 0x01266DBC and "GAZE_AT_WORK_MARKER" @ 0x0128FDD4,
// CGameDefinitionManager::Get @ 0x0044C6B0 (static __fastcall), CDefinitionManager::GetDefGlobalIndexFromName
// @ 0x009AD410.
// Shape: the inline CAIStateGroupBase constructor (its vtable 0x01288C60, then the Name member's default constructor;
// the other base fields are left alone), the Gaze vtable 0x0128FDEC, the GazeMarker CIntelligentPointer member
// (inline default constructor: base ctor, vtable 0x0127B4D8), ChanceOfRunning = 0.05f and MinJourneyForRunning = 8.0f
// as immediates, then PrimaryMarkerDef / SecondaryMarkerDef from `Get().GetDefGlobalIndexFromName("...")` -- each
// name an implicit-conversion CCharString temporary re-addressed with `lea` and destroyed right after the store.
// MaxDistanceForCreaturesToLookAt (+0x24) is not initialised here.
// Model: local (engine/CAIStateGroup_Gaze.h flattens the base; member names from it).
struct CAIBrain;

struct CCharString {
    void* m_pData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();            // 0x0044C6B0
};

namespace N_0099a2d0 { struct CBase {
    virtual ~CBase();
    CBase();                                                // 0x0099A2D0
}; } using N_0099a2d0::CBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                          // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
};

struct CIntelligentPointerGameObject : CBaseIntelligentPointer {    // CIntelligentPointer<CThingGameObject>, vtable 0x0127B4D8
    virtual void v00();
    CIntelligentPointerGameObject() { }
};

struct CAIStateGroupBase : CBase {                          // vtable 0x01288C60
    CAIBrain* PBrain;                                       // +0x04
    long ScriptAIPriority;                                  // +0x08
    CCharString Name;                                       // +0x0c
    bool Terminated;                                        // +0x10
    unsigned char Priority;                                 // +0x11
    bool Enabled;                                           // +0x12
    virtual void Initialise();
    CAIStateGroupBase() { }
};

struct CAIStateGroup_Gaze : CAIStateGroupBase {             // vtable 0x0128FDEC
    CIntelligentPointerGameObject GazeMarker;               // +0x14
    float ChanceOfRunning;                                  // +0x1c
    float MinJourneyForRunning;                             // +0x20
    float MaxDistanceForCreaturesToLookAt;                  // +0x24
    long PrimaryMarkerDef;                                  // +0x28
    long SecondaryMarkerDef;                                // +0x2c
    virtual void Initialise();
    CAIStateGroup_Gaze();
};

CAIStateGroup_Gaze::CAIStateGroup_Gaze()
    : ChanceOfRunning(0.05f)
    , MinJourneyForRunning(8.0f)
    , PrimaryMarkerDef(CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GAZE_OUT_OF_BUILDING_MARKER"))
    , SecondaryMarkerDef(CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GAZE_AT_WORK_MARKER"))
{
}