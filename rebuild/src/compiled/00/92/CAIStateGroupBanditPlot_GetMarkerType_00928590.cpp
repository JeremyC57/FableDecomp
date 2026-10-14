// CAIStateGroup_BanditPlot::GetMarkerType() -- global def index of "MARKER_PLOT"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00928590, length 51 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal @ 0x012917E0,
// length -1), CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall, returns the singleton by
// reference), CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410,
// CCharString::~CCharString @ 0x0099EAE0.
// The name is an explicit named local (one `push ecx` slot, re-addressed with `lea` for both
// the lookup and the destructor); the index rides in esi across the destructor.
struct CCharString {
    char* m_pData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};

struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();            // 0x0044C6B0
};

struct CAIStateGroup_BanditPlot {
    virtual long GetMarkerType();
};

long CAIStateGroup_BanditPlot::GetMarkerType()
{
    CCharString markerName("MARKER_PLOT", -1);
    return CGameDefinitionManager::Get().GetDefGlobalIndexFromName(markerName);
}