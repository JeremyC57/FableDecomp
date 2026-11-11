// CTCHeroGuide::OpenQuestSelectInterface() -- find the hero centre this guide belongs to; when the world allows it and the
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 004365b0: ?LowerBound@CInterfaceMap@@QBEPBUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z; 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// centre has quest cards on offer, clear the hero's quest-select flag and queue game event 0x1a (payload 0xd) for the main
// player, otherwise show "TEXT_GUI_QUESTS_NO_AVAILABLE" as a subtitle
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007800B0, 466 bytes (plain `ret`, /GS cookie 0x0139C8A8).
// Callees: CTCBase::GetPlayerManager const 0x00686D50, CPlayerManager::GetMainPlayer 0x00449960, CPlayer::GetPControlledCreature
// 0x00487DC0, the const interface LowerBound 0x004365B0 and the non-const one 0x0040F020, CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CTCBase::PeekWorld 0x00686A80 (+0xDA a bool), CTCHeroCentre::GetQuestCards 0x0077DBC0, CMemoryBuffer::Copy
// 0x009F1760, the second player-manager getter 0x00686D40, CPlayerManager::GetMainPlayerNumber 0x00449700,
// CMainGameComponent::AddGameEvent 0x0041649C, CCharString 0x0099EBF0 / 0x0099EAE0, CTCBase::PeekComponent() const 0x00686D60 (Ego_r.pdb)
// (+0x1614C the font bank), CSubtitleRenderer::SetText 0x00492ED1 on the player GUI 0x013B8790 (+0x18),
// ~vector<CIntelligentPointer<CThing> > 0x004406D0, __security_check_cookie 0x00BFE9F9.
// Layout: CGameEvent as the landed CTCShop::OpenShoppingInterface 0x00773B20; CThing interface bitset +0x20, map +0x44,
// the centre pointer +0x170; the 0x33 component's centre pointer +0x0D; the 0x19 component's flag +0x14D (retail).
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
#include "fable_compiler.h"
class CThing;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad54[0x11c];
    CBaseIntelligentPointer HeroCentre;                 // +0x170
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> FABLE_FORCEINLINE const T* FindTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)key);
        if (it == map->end() || type < it->first)
            it = map->end();
        return (const T*)it->second;
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long type)
    {
        T* result;
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)key);
            if (it == map->end() || type < it->first)
                it = map->end();
            result = (T*)it->second;
        }
        return result;
    }
};
class CThingPlayerCreature : public CThing { };
struct CTCCentreOwner {                                 // interface 0x33
    unsigned char _pad00[0x0d];
    CBaseIntelligentPointer HeroCentre;                 // +0x0D
};
struct CThingVector {                                   // vector<CIntelligentPointer<CThing> >
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector();                                    // 0x004406D0
};
class CTCHeroCentre { public:                                  // interface 0x6A
    bool GetQuestCards(CThingVector* cards) const;      // 0x0077DBC0
};
struct CTCQuestSelect {                                 // the hero's interface 0x19 component
    unsigned char _pad00[0x14d];
    bool QuestSelectPending;                            // +0x14D
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                             // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);  // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddULONG(unsigned long value) { Copy(4, &value); }
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    void AddGameEvent(const CGameEvent& event);         // 0x0041649C
    unsigned char _pad00[0x1614c];
    class CFontBank* PCutSceneMainFont_Data;            // +0x1614C CCountedPointer<CFontBank>::Data (Ego_r.pdb PCutSceneMainFont; was CGameSubsystem::SubtitleFont)
};
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CFontBank;
class CSubtitleRenderer { public:
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer Subtitles;                        // +0x18
};
class CWorld { public:
    unsigned char _pad00[0xda];
    bool QuestCardsEnabled;                             // +0xDA
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
protected:
    const CMainGameComponent& PeekComponent() const;   // 0x00686D60 Ego_r.pdb ?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ (was GetGameSubsystem)
    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)
    CPlayerManager& GetPlayerManager();   // 0x00686D40 (Ego_r.pdb CTCBase, protected)
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50 (Ego_r.pdb CTCBase, protected)
};
class CTCHeroGuide : public CTCBase { public:
    void OpenQuestSelectInterface();
};

void CTCHeroGuide::OpenQuestSelectInterface()
{
    CThing* hero = PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
    const CThing* parent = ParentThing;
    const CThing* centre;
    if (parent->HasTC(0x33))
        centre = ((CThing*)parent->FindTC<CTCCentreOwner>(0x33)->HeroCentre.GetPItem());
    else
        centre = ((CThing*)parent->HeroCentre.GetPItem());
    if (centre && PeekWorld().QuestCardsEnabled && centre->HasTC(0x6a)) {
        const CTCHeroCentre* heroCentre = centre->FindTC<CTCHeroCentre>(0x6a);
        CThingVector cards;
        if (heroCentre->GetQuestCards(&cards)) {
            hero->GetTC<CTCQuestSelect>(0x19)->QuestSelectPending = false;
            CGameEvent event(0x1a);
            event.AddULONG(0xd);
            event.Player = (signed char)GetPlayerManager().GetMainPlayerNumber();
            CMainGameComponent::Get()->AddGameEvent(event);
        } else {
            CCharString text("TEXT_GUI_QUESTS_NO_AVAILABLE", -1);
            const CMainGameComponent* component = &PeekComponent();
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().Subtitles.SetText(component->PCutSceneMainFont_Data, text, 0x122, 4.0f, false, true);
        }
    }
}