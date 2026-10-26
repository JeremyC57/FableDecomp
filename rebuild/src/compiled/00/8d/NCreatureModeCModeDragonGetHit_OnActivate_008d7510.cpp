// NCreatureMode::CModeDragonGetHit::OnActivate() -- the CModeStandBase activation, PTarget pointed at the main player's hero, a player creature's animation suspended for this mode
// Leaf: OnActivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D7510, 111 bytes (`ret`).
// Callees: CModeStandBase::OnActivate @ 0x008621A0, CPlayerManager::GetMainPlayer @ 0x00449970 (catalog label is a
// catch funclet; the 0x00449960 twin is GetMainPlayer per GOTCHAS chunk 139), CPlayer::GetHeroThing @ 0x00487DC0
// (GOTCHAS chunk 140), CBaseIntelligentPointer: ctor 0x00A01B10, SetPItem 0x00A01B90, GetPItem 0x00A01B50,
// ~CBaseIntelligentPointer 0x00A01C10, CThingAnimatable::SetSuspendAnimation @ 0x00833A10.
// Globals: the engine singleton @ 0x013B86A0 (player manager at +0x1c).
// Layout: Ego_r.pdb CModeDragonGetHit: Filter +0x68, PTarget +0x74, Timer +0x7c.
// Shape: `PTarget = CIntelligentPointer<CThing>(hero)`: the 8-byte temporary (base ctor, vtable 0x01238C6C, SetPItem)
// is copied through an inline operator= (`SetPItem(other.GetPItem())`) and destroyed by the base's out-of-line dtor.
namespace NCreatureMode {
struct CModeBase;
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_type;                          // +0x10 (1 = the player creature)
    void SetSuspendAnimation(bool suspend, CModeBase* mode);   // 0x00833A10
};
}
using NCreatureMode::CThing;

class CPlayer { public: CThing* GetHeroThing();  };       // 0x00487DC0
class CPlayerManager { public: CPlayer* GetMainPlayer(); };   // 0x00449970
class CEngine { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* m_pPlayerManager;              // +0x1c
};
extern CEngine* g_pEngine;                         // 0x013B86A0

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                          // +4
    CBaseIntelligentPointer();                     // 0x00A01B10
    ~CBaseIntelligentPointer();                    // 0x00A01C10
    void SetPItem(CBaseObject* item);              // 0x00A01B90
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

namespace NCreatureMode {

struct CModeBase {
    virtual ~CModeBase();
    CThing* m_pThing;                              // +0x04
    unsigned char _pad08[0x14];
};

struct CModeStandBase : CModeBase {
    unsigned char _pad1c[0x4c];                    // +0x1c .. +0x68
    void OnActivate();                             // 0x008621A0
};

struct CModeDragonGetHit : CModeStandBase {
    unsigned char       _filter[0xc];              // +0x68 CAnimFilterOfGroup Filter
    CIntelligentPointer PTarget;                   // +0x74
    long                Timer;                     // +0x7c
    virtual void OnActivate();
};

void CModeDragonGetHit::OnActivate()
{
    CModeStandBase::OnActivate();
    PTarget = CIntelligentPointer(g_pEngine->m_pPlayerManager->GetMainPlayer()->GetHeroThing());
    if (m_pThing->m_type == 1)
        m_pThing->SetSuspendAnimation(true, this);
}

}