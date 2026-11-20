#ifndef FABLE_ENGINE_CV_BOOKCOLLECTINGSCRIPT_H
#define FABLE_ENGINE_CV_BOOKCOLLECTINGSCRIPT_H
// Retail opt-in (chunk 845): NScript::CV_BookCollectingScript, new header (no includer before chunk 845; the landed sibling rows
// keep their local models).
// Layout: Ego_r.pdb CV_BookCollectingScript (struct_layouts_egor.tsv, 228 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48), a further - 4 after each STLport vector (0xC; Dinkumware 0x10): ReadingBook +0x48, BookReactions
// +0x4c, seh_Boy +0x58, seh_Girl +0x68, DoneIntro +0x78, HatRewarded +0x79, KeyRewarded +0x7a, GossipState +0x7c, HasInformation
// +0x80, BooksDonated +0x84, GoodBooksDonated +0x88, LastBookRequested +0x8c, BooksInGame +0x90, BookDef +0x94, BookDonated +0xa0,
// BookOwned +0xb4 (vector<bool> 0x14 in both), sizeof 0xc8 -- the offsets the landed CBS_Teacher::AskForBook 0x00E55CE0 model uses
// and CBS_Teacher::Main 0x00E54E90 reads (GossipState +0x7c, HasInformation +0x80, BooksDonated +0x84, LastBookRequested +0x8c,
// BookDef begin +0x94, DoneIntro +0x78). vector<bool> has no shared STLport model yet: BookDonated / BookOwned are opaque
// 0x14-byte members here (header debt).
// Retail methods (Ego_r.pdb / FableWin.pdb ?<Name>@CV_BookCollectingScript@NScript@@): AddGossip 0x00E55C60 (four CCharString by
// value), BookReaction 0x00E566F0.
// Nested CBS_Teacher (Ego_r.pdb 68 bytes: CEntityScriptBase 0x14, ParentClass +0x14, PMasterData +0x18, BadBooksForHat +0x1c,
// GoodBooksForHat +0x20, BooksWanted +0x24, BooksAccepted +0x28, BooksComment +0x2c, MoralityReward +0x30, seh_me +0x34; sizeof
// 0x44); vtable 0x012E5F40: 0 scalar deleting dtor 0x00E546A0, 1 Main 0x00E54E90, 2 Init 0x00E54630, 3 GetParentScript 0x00E54620.
// Names: Ego_r.pdb / FableWin.pdb ?<Name>@CBS_Teacher@CV_BookCollectingScript@NScript@@ (Main / Init EAEXXZ, AskForBook
// AAE_NJVCCharString@@@Z 0x00E55CE0, LookForBook AAEXXZ 0x00E57530, AdjustHeroReputation 0x00E56D10, GetBookSpecificArgs 0x00E57020).
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "stlport/fable_containers.h"
class CConversation;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BookCollectingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void AddGossip(CCharString category, CCharString rumour, CCharString village, CCharString faction);   // 0x00E55C60
    void BookReaction(long book);                                        // 0x00E566F0
    bool ReadingBook;                                                    // +0x48
    std::vector<CConversation> BookReactions;                            // +0x4c
    CScriptGameResourceObjectScriptedThingBase seh_Boy;                  // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_Girl;                 // +0x68
    bool DoneIntro;                                                      // +0x78
    bool HatRewarded;                                                    // +0x79
    bool KeyRewarded;                                                    // +0x7a
    long GossipState;                                                    // +0x7c
    bool HasInformation;                                                 // +0x80
    long BooksDonated;                                                   // +0x84
    long GoodBooksDonated;                                               // +0x88
    long LastBookRequested;                                              // +0x8c
    long BooksInGame;                                                    // +0x90
    std::vector<CCharString> BookDef;                                    // +0x94
    unsigned char BookDonated[0x14];                                     // +0xa0 vector<bool> (opaque, header debt)
    unsigned char BookOwned[0x14];                                       // +0xb4 vector<bool> (opaque, header debt)
    class CBS_Teacher : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BookCollectingScript&, CScriptDataBase*, const CScriptThing&);   // 0x00E54580
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00E54620
    private:
        virtual void Main();                                             // slot 1 0x00E54E90
        virtual void Init();                                             // slot 2 0x00E54630
        bool AskForBook(long book, CCharString refusal);                 // 0x00E55CE0
        void LookForBook();                                              // 0x00E57530
    protected:
        CV_BookCollectingScript* ParentClass;                            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        long BadBooksForHat;                                             // +0x1c
        long GoodBooksForHat;                                            // +0x20
        long BooksWanted;                                                // +0x24
        long BooksAccepted;                                              // +0x28
        long BooksComment;                                               // +0x2c
        long MoralityReward;                                             // +0x30
        CScriptGameResourceObjectScriptedThingBase seh_me;               // +0x34
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BookCollectingScript) == 0xc8);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BookCollectingScript::CBS_Teacher) == 0x44);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_BOOKCOLLECTINGSCRIPT_H
