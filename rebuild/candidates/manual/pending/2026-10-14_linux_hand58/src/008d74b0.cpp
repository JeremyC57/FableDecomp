// NCreatureMode::CModeDragonLanded::OnActivate() -- the CModeStandBase activation (0x008621A0), then the target pointer (+0x74) set to the hero thing (game -> player manager (+0x1c) -> GetPlayer @ 0x00449970 -> GetHeroThing @ 0x00487DC0) through a CIntelligentPointer<CThing> temporary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D74B0, length 94 bytes.
// Callees: CModeStandBase::OnActivate @ 0x008621A0, CPlayerManager::GetPlayer @ 0x00449970, CPlayer::GetHeroThing @ 0x00487DC0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem @ 0x00A01B90, GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10. The temporary (frame +8) is the implicit conversion of the thing for the assignment operator: its constructor runs the base out of line, stores the CIntelligentPointer<CThing> vtable (0x01238C6C) and SetPItem's the thing; the assignment copies GetPItem into the member; the implicit destructor stores no vtable. Vtable slot 5 of CModeDragonLanded. Shape: `this` in esi, the hero in edi, `ret`.
namespace NCreatureMode {
struct CThing;
struct CBaseObject;
struct CBaseIntelligentPointer {                   // 8 bytes: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                     // 0x00A01B10
    ~CBaseIntelligentPointer();                    // 0x00A01C10
    CBaseObject* GetPItem() const;                 // 0x00A01B50
    void SetPItem(CBaseObject* item);              // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CPlayer {
    CThing* GetHeroThing();                        // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetPlayer();                          // 0x00449970
};
struct CGame {
    unsigned char   _pad00[0x1c];
    CPlayerManager* m_pPlayerManager;              // +0x1c
};
}
extern NCreatureMode::CGame* g_pGame;              // 0x013B86A0

namespace NCreatureMode {
struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual ~CModeBase();
    CThing* m_pThing;                              // +0x04
};

struct CModeStandBase : CModeBase {
    unsigned char _pad08[0x60];                    // +0x08 .. +0x68
    void OnActivate();                             // 0x008621A0
};

struct CModeDragonLanded : CModeStandBase {
    unsigned char               _pad68[0xc];       // +0x68 .. +0x74
    CIntelligentPointer<CThing> PTarget;           // +0x74
    virtual void OnActivate();
};

void CModeDragonLanded::OnActivate()
{
    CModeStandBase::OnActivate();
    PTarget = g_pGame->m_pPlayerManager->GetPlayer()->GetHeroThing();
}
}
