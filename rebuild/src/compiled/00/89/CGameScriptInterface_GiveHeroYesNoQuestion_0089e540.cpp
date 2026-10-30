// CGameScriptInterface::GiveHeroYesNoQuestion(const CCharString& question, const CCharString& yesText, const CCharString& noText, const CCharString& extraText, bool pauseGame) const -- close the hero's boast UI, optionally pause, show the yes/no question as a subtitle (the four-answer form only when the extra text is non-empty), then let the boast UI open again
// Leaf: GiveHeroYesNoQuestion
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089E540, 458 bytes, `ret 0x14`.
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CBaseIntelligentPointer ctor 0x00A01B10 /
//   GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10, the component-map LowerBound 0x0040F020 (TC 0xe8 boast UI),
//   CTCBoastUI::AllowUIToOpen 0x0056A930, own slot 376 Pause, CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0,
//   CBasicString<char>::operator==(const char*) 0x004115A0, CSubtitleRenderer::SetText 0x00493049 / 0x00492FB4 (thunks).
// Layout: the player GUI 0x013B8790 holds the subtitle renderer at +0x18; the main game component (+8) its font at +0x1614c.
//   The generated CGameScriptInterface header flattens the vptr, so `this` is a local projection.
class CThingPlayerCreature;
#include <string.h>
#pragma pack(push, 1)
template <class T> class CBasicString { public:
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharStringData { public: CBasicString<char> Data; long NoRefs; };
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CFontBank;
class CSubtitleRenderer { public:
    void SetText(const CFontBank& font, const CCharString& a, const CCharString& b, const CCharString& c, const CCharString& d,
                 const CCharString& e, unsigned long colour, float time, bool f, bool g);   // 0x00493049
    void SetText(const CFontBank& font, const CCharString& a, const CCharString& b, const CCharString& c, const CCharString& d,
                 unsigned long colour, float time, bool f, bool g);                         // 0x00492FB4
};
class CPlayerGui { public: unsigned char Data[0x18]; CSubtitleRenderer Subtitles; };
extern CPlayerGui* g_pPlayerGui;                         // 0x013B8790
class CMainGameComponent { public: unsigned char Data[0x1614c]; CFontBank* PFont; };
class CTCBoastUI { public: void AllowUIToOpen(bool allow); };   // 0x0056A930
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    ComponentEntry* Start;
    ComponentEntry* Finish;
    ComponentEntry* LowerBound(const ETCInterfaceType& key);         // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap;
class CThing { public:
    void*          VTable;
    unsigned char  Base[0x1c];
    unsigned long  UsingComponents[9];                   // +0x20
    ComponentMap   Components;                           // +0x44
    unsigned char  Metadata[0x45];
    unsigned char  LifeFlags;                            // +0x91
};
class CThingPlayerCreature : public CThing { };
class CBaseObject;
struct CBaseIntelligentPointer {
    void* Info;
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                            // 0x00A01B50
    void SetPItem(CThing* item);                         // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01231C4C
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }

};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };              // 0x00487DC0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
     // 0x00449970
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::GiveHeroYesNoQuestion(const CCharString& question, const CCharString& yesText, const CCharString& noText, const CCharString& extraText, bool pauseGame) const
{
    CIntelligentPointer<CThingPlayerCreature> hero((CThingPlayerCreature*)PlayerManager->GetMainPlayer().GetPControlledCreature());
    CTCBoastUI* boastUI = 0;
    if (((CThing*)hero.GetPItem()) && !(((CThing*)hero.GetPItem())->LifeFlags & 1)) {
        CThing* thing = ((CThing*)hero.GetPItem());
        if (thing->UsingComponents[7] & 0x100) {
        ComponentMap* map = &thing->Components;
        ComponentEntry* it = map->LowerBound((ETCInterfaceType)0xe8);
        ComponentEntry* end = map->Finish;
        boastUI = (CTCBoastUI*)((void*)((it == end || it->first > 0xe8) ? end : it)->second);
        boastUI->AllowUIToOpen(false);
        }
    }
    if (pauseGame)
        Pause(1.0f);
    if (extraText == "")
        g_pPlayerGui->Subtitles.SetText(*Component->PFont, question, "", yesText, noText, 0x24, 0.0f, false, true);
    else
        g_pPlayerGui->Subtitles.SetText(*Component->PFont, question, "", yesText, noText, extraText, 0x24, 0.0f, false, true);
    if (((CThing*)hero.GetPItem()) && !(((CThing*)hero.GetPItem())->LifeFlags & 1) && boastUI)
        boastUI->AllowUIToOpen(true);
}