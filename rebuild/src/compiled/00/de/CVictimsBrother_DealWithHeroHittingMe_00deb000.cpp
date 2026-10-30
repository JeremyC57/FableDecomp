// CVictimsBrother::DealWithHeroHittingMe() -- count the hero's hit on the evil brother; after the first hit he just says "run away", on the first hit (while alive) a paused movie plays his "on hit" line and he and the hero become allies
// Leaf: ?DealWithHeroHittingMe@CVictimsBrother@CQ_RansomVictimScript@NScript@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DEB000, 0x27C bytes, plain `ret`.
// Callees: IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CBaseObject 0x0099A380 / ~ 0x0099A430,
//   movie PImp dtor 0x006E7AB0, movie dtor 0x006E7B80 (out of line at the in-loop and after-loop exits), ~CScriptThing
//   0x004AA840; Game slots 7, 70 GetHero, 264 GetHealth, 364 AddNewConversation, 365 AddPersonToConversation, 366
//   AddLineToConversation, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities, 599 EntitySetThingAsAllyOfThing; the
//   brother handle's slots 12 GetScriptThing / 13 Speak / 26 IsPerformingScriptTask. 0.0f 0x0122DEDC.
// Shape: the landed CMurderTwistSheriff::DoRemindingHeroToKillSheriff 0x00EB15C0 (movie object, cached `game` pointer).
// Layout (retail = Ego_r.pdb - 0x14): ParentClass +0x14; CQ_RansomVictimScript HitEvilBrotherAndProtection +0x58,
//   seh_Brother +0x9c.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
enum EHeroAbility { HERO_ABILITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CBaseObject { public:                                    // CBaseIntelligentPointer, vtable 0x0129A7D8
    virtual void b000();
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                      // 0x0099A380
public:
};
struct CBaseObjectDtor {                                // destructor-only view of CBaseObject
    void* m_pIntelligentPointer;
    virtual ~CBaseObjectDtor();                         // 0x0099A430
};
struct CScriptGameResourceObjectBaseDtor : CBaseObjectDtor {   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBaseDtor() { }
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    ~CMovieImpPointer();                                // 0x006E7AB0
};
struct CMovieImpWords { void* Data; void* Info; };
class CScriptGameResourceObjectMovieBase : public CBaseObject { public:   // vtable 0x01260EF4; destructor-less view, destroyed by hand
    CMovieImpWords PImp;                                // +0x08
    __forceinline CScriptGameResourceObjectMovieBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void m001();
    __forceinline void DestroyInline()
    {
        reinterpret_cast<CMovieImpPointer*>(&PImp)->~CMovieImpPointer();
        reinterpret_cast<CScriptGameResourceObjectBaseDtor*>(this)->CScriptGameResourceObjectBaseDtor::~CScriptGameResourceObjectBaseDtor();
    }
    void DestroyOutOfLine();                            // the implicit destructor's out-of-line copy 0x006E7B80
};
class CScriptGameResourceObjectScriptedThingBase { public:    // 0x10 bytes, vtable 0x0127094C
    virtual void h000(); virtual void h001(); virtual void h002(); virtual void h003(); virtual void h004(); virtual void h005(); virtual void h006(); virtual void h007();
    virtual void h008(); virtual void h009(); virtual void h010(); virtual void h011();
    virtual CScriptThing GetScriptThing() const;   // slot 12 (+0x30) 0x007E7490
    virtual void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // slot 13 (+0x34) 0x007E7390
    virtual void h014(); virtual void h015(); virtual void h016(); virtual void h017(); virtual void h018(); virtual void h019(); virtual void h020(); virtual void h021();
    virtual void h022(); virtual void h023(); virtual void h024(); virtual void h025();
    virtual bool IsPerformingScriptTask() const;   // slot 26 (+0x68) 0x007E7450
    unsigned char _body[0xc];
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CQ_RansomVictimScript {
    unsigned char _pad00[0x58];
    long HitEvilBrotherAndProtection;                  // +0x58 (PDB 108)
    unsigned char _pad5c[0x9c - 0x5c];
    CScriptGameResourceObjectScriptedThingBase seh_Brother;   // +0x9c (PDB 176)
};
struct CVictimsBrother : CEntityScriptBase {
    CQ_RansomVictimScript* ParentClass;                // +0x14
    void*                  PMasterData;                // +0x18
    void DealWithHeroHittingMe();
};

void CVictimsBrother::DealWithHeroHittingMe()
{
    ++ParentClass->HitEvilBrotherAndProtection;
    if (ParentClass->HitEvilBrotherAndProtection > 1) {
        if (IsActiveThreadTerminating())
            return;
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_B06_BROTHER_ON_RUN_AWAY_10", false, Me, Game->GetHero());
    } else if (Game->GetHealth(Me) > 0.0f) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        if (Game->GetHealth(ParentClass->seh_Brother.GetScriptThing()) > 0.0f) {
            ParentClass->seh_Brother.Speak(Game->GetHero(), "TEXT_QST_B06_BROTHER_ON_HIT", (ETextGroupSelectionMethod)2, false, true, false);
            while (ParentClass->seh_Brother.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    movie.DestroyOutOfLine();
                    return;
                }
            }
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                movie.DestroyOutOfLine();
                return;
            }
        }
        Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
        Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
        game->PauseAllNonScriptedEntities(false);
        movie.DestroyInline();
    }
}