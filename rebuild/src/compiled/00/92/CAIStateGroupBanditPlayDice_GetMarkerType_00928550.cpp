// CAIStateGroup_BanditPlayDice::GetMarkerType() -- global def index of "MARKER_DICE_GAME"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00928550, length 51 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal @ 0x012917CC,
// length -1), CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall, returns the singleton by
// reference), CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410,
// CCharString::~CCharString @ 0x0099EAE0.
// The name is an explicit named local (one `push ecx` slot, re-addressed with `lea` for both
// the lookup and the destructor); the index rides in esi across the destructor.
class CCharString { public:
    char* m_pData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};

class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();            // 0x0044C6B0
};

struct CAIStateGroup_BanditPlayDice {
    virtual long GetMarkerType();
};

long CAIStateGroup_BanditPlayDice::GetMarkerType()
{
    CCharString markerName("MARKER_DICE_GAME", -1);
    return CGameDefinitionManager::Get().GetDefGlobalIndexFromName(markerName);
}