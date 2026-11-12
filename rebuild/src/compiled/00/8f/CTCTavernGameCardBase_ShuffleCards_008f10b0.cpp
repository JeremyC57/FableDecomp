// CTCTavernGameCardBase::ShuffleCards() -- shuffle the pack, then stack every card face down at the pack position: card i sits (53 - i) card separations above the table
// Leaf: ?ShuffleCards@CTCTavernGameCardBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F10B0, 410 bytes, thiscall, plain `ret`, no SEH frame. Flags: none.
// Identity: Ego_r.pdb ?ShuffleCards@CTCTavernGameCardBase@@UAEXXZ; slot 60 of the CTCTavernGameCardBase vtable 0x01288294 (inherited by
//   CTCTavernGameCardSorting 0x0126F5B4 and CTCTavernGameCardBlackjack 0x0126F8F4; CTCTavernGameCardPairs overrides it with 0x007D71E0).
// Callees: CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20 (out of line for the outer def);
//   std::random_shuffle<CCard**> 0x007D7860 (__fastcall, first ecx / last edx) on Pack (+0x134 / +0x138);
//   CDefClassBase::GetSubDef<CTavernGameCardBaseDef> 0x008F27A0 (from the GetCardDef expanded inside the loop's GetPackPosition);
//   CTCTavernGame::HideMesh(CThing*) 0x005E9B10 and CTCTavernGame::PositionMesh(...) 0x005EABC0 (static __fastcall);
//   the def's virtual slot 1 (+4) when a reference is the last one.
// Shape: retail expands GetPackPosition 0x008F0B80 in the loop, with GetCardDef expanded inside it (VC7.1 auto-inlines the calls in
//   the loop and keeps the one before the loop a call), so both bodies sit in this TU as retail's source file has them: they are the
//   real bodies of the landed rows 0x008F0B20 / 0x008F0B80, spelled on the shared headers. The GetSubDef template body is visible
//   too (engine/CDefClassBase_GetSubDef.h, kept a call), which tells VC7.1 that the def pointers' addresses do not escape: the outer
//   def is read once into edi and its frame slot is reused for the height counter (the strength-reduced `53 - i`, `fild`), and the
//   inner add-ref / release pair folds in registers (`inc edx ; mov eax,edx ; dec eax`). `pos += GetPackPosition()` leaves the
//   `Z += 0.0f` as a dword copy of pos.Z onto itself. The CDefPointer release is the pointee form
//   (FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE): it schedules the count store after the second product (0x008F116B).
// Members: Pack +0x134 (vector<CCard*>), GamePositionRHSet +0xF0, GamePositionPos +0x108; CTavernGameCardBaseDef CardWidth +0x2C,
//   CardHeight +0x30, CardSeparation +0x5C, TableSeparation +0x60, PackOffset +0x64, CardPack +0x78 (vector<CCardDef>, 0x44 each);
//   CCard Thing +0x00, FinalRotation +0x10, FaceDown +0x25, LocalPos +0x27, NewLocalPos +0x33.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE          // count-store schedule of the expanded GetPackPosition (0x008F116B)
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAME_HIDEMESH
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDPACK
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_TYPENAME
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCARD_POSITIONS
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET
#define FABLE_RETAIL_STLP_RANDOM_SHUFFLE_2ARG
#include "stlport/fable_containers.h"                   // first: the two-argument random_shuffle opt-in
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardDef.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
#include "engine/CDefClassBase_GetSubDef.h"

// 0x008F0B20 (landed row; real body, same TU in retail)
void CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* pdef)
{
    CDefPointer<const CTavernGameCardBaseDef> def;
    ParentThing->PeekBaseDef().GetSubDef(&def);
    *pdef = def;
}

// 0x008F0B80 (landed row; real body, same TU in retail)
C3DVector CTCTavernGameCardBase::GetPackPosition()
{
    CDefPointer<const CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    return C3DVector(def->CardWidth * def->PackOffset_X, def->CardHeight * def->PackOffset_Y, 0.0f);
}

void CTCTavernGameCardBase::ShuffleCards()
{
    CDefPointer<const CTavernGameCardBaseDef> pdef;
    GetCardDef(&pdef);
    std::random_shuffle(Pack.begin(), Pack.end());
    for (unsigned int i = 0; i < pdef->CardPack.size(); ++i) {
        C3DVector pos(0.0f, 0.0f, (float)(53 - (int)i) * pdef->CardSeparation + pdef->TableSeparation);
        pos += GetPackPosition();
        CCard* card = Pack[i];
        HideMesh(card->Thing);
        PositionMesh(card->Thing, GamePositionPos, GamePositionRHSet, pos, 0.5f, 0.5f);
        card->LocalPos = pos;
        card->NewLocalPos = pos;
        card->FinalRotation = 0.5f;
        card->FaceDown = true;
    }
}