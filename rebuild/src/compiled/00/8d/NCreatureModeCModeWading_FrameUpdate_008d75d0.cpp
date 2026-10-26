// NCreatureMode::CModeWading::FrameUpdate() -- vtable slot 3: pick the wading animation group by whether the creature carries a melee weapon, then the base frame update
// Leaf: ?FrameUpdate@CModeWading@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D75D0, 81 bytes (+15 int3 padding).
// Name: slot 3 of the vtable 0x01267F48 (rtti-vtables.tsv); it ends with the landed CModeBase::FrameUpdate 0x0073AC50, so
// the slot is FrameUpdate (as CModeUnderground's slot 3).
// Callees: IsThingCarryingMeleeWeapon(CThing*) @ 0x006A15A0 (`__fastcall`), CCharString(const char*, long) @ 0x0099EBF0,
// CAnimFilterOfGroup::SetGroup(const CCharString&) @ 0x0073DFF0 (out of line here), ~CCharString @ 0x0099EAE0,
// CTCCreatureModeManager::CModeBase::FrameUpdate @ 0x0073AC50.
// Data: "WADING_WITH_WEAPON" @ 0x012856C8, "WADING_NO_WEAPON" @ 0x01267F78 (read from Fable.exe .rdata).
// Layout: as the landed constructor 0x00740A90: the CAnimFilterOfGroup member at +0x68 (built from WADING_NO_WEAPON).
// Shape: an if/else whose arms each build an implicit CCharString temporary (the shared `push -1` default length and
// the `lea ecx` are hoisted above the branch, each arm re-addresses its temporary with `lea`) and tail-merge into one
// SetGroup call and destructor. Plain `ret`.
class CThing;

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

bool __fastcall IsThingCarryingMeleeWeapon(CThing* thing);   // 0x006A15A0

class CAnimFilterOfGroup { public:                    // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                       // +4
    unsigned long GroupCRC;                    // +8
    void SetGroup(const CCharString& group);   // 0x0073DFF0
};

namespace NCreatureMode {

struct CModeBase {                             // CTCCreatureModeManager::CModeBase
    virtual ~CModeBase();
    CThing* m_pThing;                          // +0x04
    void FrameUpdate();                        // 0x0073AC50
};

struct CModeWading : CModeBase {
    unsigned char      _pad08[0x68 - 0x08];    // CModeStandBase state
    CAnimFilterOfGroup m_filter;               // +0x68
    virtual void FrameUpdate();
};

void CModeWading::FrameUpdate()
{
    if (IsThingCarryingMeleeWeapon(m_pThing))
        m_filter.SetGroup("WADING_WITH_WEAPON");
    else
        m_filter.SetGroup("WADING_NO_WEAPON");
    CModeBase::FrameUpdate();
}

} // namespace NCreatureMode