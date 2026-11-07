// CTCTavernGameSpotTheAddition::FrameUpdate() -- the spot-the-addition tavern game's per-frame step: run the base state machine, rescale the pointer, then lift / hold / lower the cover and lay out a fresh board of knick-knacks (one added in the addition round), run the round timer with highlight of the pointed-at piece and the out-of-time fail, and track the highlighted object while the round is played
// Frame slots (chunk 821): halton, its position loop and the PositionMesh loop sit in a sibling block before the board-bounds def block, so the
//   bounds CDefPointer reuses halton's slot [esp+0x18] / [esp+0x1C] as retail (0x007D940E); in one enclosing block it took [esp+0x14] (chunk 800 a18).
// Leaf: ?FrameUpdate@CTCTavernGameSpotTheAddition@@UAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_CLEAR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLD
#include "stlport/fable_containers.h"
#include "engine/CTavernGameDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCTavernGame.h"
#include "engine/CPlayerGui.h"
#include "engine/CWorld.h"
#include "engine/C3DVector.h"
#include "fable_game.h"
#include "engine/CRGBColour.h"
#include "engine/CRightHandedSet.h"

class CThingObject;

struct KnickKnack {                                                 // 16 bytes (Ego_r.pdb KnickKnack)
    C3DVector     Pos;                                              // +0x00
    CThingObject* Thing;                                            // +0x0C
    KnickKnack() { }
};

namespace NGraphicAppearance {
enum EAppearanceMeshEffectPriority { };
class CTCGraphicAppearance { public:
    void SetAsHighlighted(float, float, const CRGBColour&, bool, bool, EAppearanceMeshEffectPriority, void*);   // 0x004C04C0
    void ClearMeshEffect_004bffc0(void* key);                                                                  // 0x004BFFC0
};
}

class CTavernGameSpotTheAdditionDef : public CSubDefClassBase {
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CTavernGameSpotTheAdditionDef", -1); }   // (inline)
    long  Pointer;                      // +0x28
    long  Cover;                        // +0x2c
    float CoverWidth;                   // +0x30
    float CoverLength;                  // +0x34
    std::vector<long> Knicknacks;       // +0x38
    std::vector<long> ItemsPerRound;    // +0x44
    float JoystickScale;                // +0x50
    float TimeToThink;                  // +0x54
    float MaxCursorHeight;              // +0x58
    float KnickKnackRadius;             // +0x5c
    float ImpulseScale;                 // +0x60
    long  PointerModel;                 // +0x64
    float PointerHeightOffset;          // +0x68
    float TimeToMoveCover;              // +0x6c
    float TimeToPauseCover;             // +0x70
    float MaxCoverHeight;               // +0x74
    float MinimumKnickKnackGap;         // +0x78
    float LengthMovementScale;          // +0x7c
    float HeightMovementScale;          // +0x80
    float HighlightWidth;               // +0x84
    CRGBColour HighlightColour;         // +0x88
    CRGBColour HighlightColourError;    // +0x8c
};

// GetSubDef<CTavernGameSpotTheAdditionDef> 0x007D9A80 (landed, same body): defined in this TU as in retail's, so VC7.1 keeps it
// out of line and sees that it does not capture the def slot (the def pointer stays in ebx across the case bodies).
template <>
bool CDefClassBase::GetSubDef<CTavernGameSpotTheAdditionDef>(CDefPointer<const CTavernGameSpotTheAdditionDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CTavernGameSpotTheAdditionDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
void __fastcall GetNextHaltonSequenceNumber(float& value, int base);   // 0x007D7AE0
namespace std {
template<class _RandomAccessIter> void __fastcall random_shuffle(_RandomAccessIter, _RandomAccessIter);

template<> void vector<KnickKnack>::push_back(const KnickKnack&);
template<> void vector<KnickKnack>::resize(size_t);
}

class CTCTavernGameSpotTheAddition : public CTCTavernGame {
public:
    virtual void FrameUpdate();
    long            CurrentRound;                 // +0xf0
    float           CurrentTime;                  // +0xf4
    CThing*         Cover;                        // +0xf8
    float           CoverTimer;                   // +0xfc
    bool            UpdatedKnickKnacks;           // +0x100
    CThingObject*   Pointer;                      // +0x104
    float           PointerScale;                 // +0x108
    float           DistFromObject;               // +0x10c
    std::vector<KnickKnack> KnickKnacks;          // +0x110
    C3DVector       BoardPos;                     // +0x11c
    CRightHandedSet BoardRHSet;                   // +0x128
    long            HeldTime;                     // +0x140
    long            Selected;                     // +0x144
    CThingObject*   NewKnickKnack;                // +0x148
    bool            FirstRound;                   // +0x14c
    unsigned char   GamePositionRHSet[0x18];      // +0x14d
    unsigned char   GamePositionPos[0xc];         // +0x165
    long            GamePositionActionPointIndex; // +0x174
    float           MinX;                         // +0x178
    float           MaxX;                         // +0x17c
    float           MinY;                         // +0x180
    float           MaxY;                         // +0x184
    CThing* FindTarget(float range);              // 0x007D7C40
    void UpdatePointerScale();                    // 0x007D8C20
    void PositionCover(float amount);             // 0x007D7F10
    void UpdateHighlightedObject();               // 0x007D8290
    void KillThing(CThing* thing);                // 0x005E9A60 (CTCTavernGame)
    static void __fastcall PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation,
                                        const C3DVector& offset, float tilt, float angle);   // 0x005EABC0
};

void CTCTavernGameSpotTheAddition::FrameUpdate()
{
    CTCTavernGame::FrameUpdate();
    CDefPointer<const CTavernGameSpotTheAdditionDef> def;
    ParentThing->PeekBaseDef().GetSubDef(&def);
    UpdatePointerScale();
    switch (TavernGameState) {
    case 0x29:
    case 0x2a:
        CurrentTime += 1.0f / CMainGameComponent::GetConstantFPS();
        if (TavernGameState == 0x2a) {
            for (unsigned long i = 0; i < KnickKnacks.size(); ++i) {
                NGraphicAppearance::CTCGraphicAppearance* appearance;
                ((CThing*)KnickKnacks[i].Thing)->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &appearance);
                appearance->ClearMeshEffect_004bffc0(this);
            }
            if (Selected < KnickKnacks.size() && Selected >= 0 && KnickKnacks[Selected].Thing) {
                NGraphicAppearance::CTCGraphicAppearance* appearance;
                FindTarget(100000.0f)->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &appearance);
                appearance->SetAsHighlighted(def->HighlightWidth, 0.0f, def->HighlightColour, false, false,
                                             (NGraphicAppearance::EAppearanceMeshEffectPriority)3, this);
            }
        }
        if (CurrentTime > TargetTime) {
            GameWon = false;
            CPlayerGui::Get().GetSubtitleRenderer().Clear(false);
            CDefPointer<const CTavernGameDef> gameDef;
            CTCTavernGame::GetDef(&gameDef);
            SetStateAfterOK(GetTextWithNarrator(gameDef->OutOfTimePhrase), GetText(gameDef->ClickToContinue), (ETavernGameStates)6);
        }
        break;
    case 0x27:
    case 0x28: {
        float t = CoverTimer + 1.0f / CMainGameComponent::GetConstantFPS();
        CoverTimer = t;
        float coverPos;
        if (t < def->TimeToMoveCover) {
            coverPos = (def->TimeToMoveCover - t) / def->TimeToMoveCover;
        } else if (t < def->TimeToMoveCover + def->TimeToPauseCover || !UpdatedKnickKnacks) {
            if (!UpdatedKnickKnacks) {
                UpdatedKnickKnacks = true;
                if (TavernGameState == 0x27) {
                    for (unsigned long i = 0; i < KnickKnacks.size(); ++i)
                        KillThing((CThing*)KnickKnacks[i].Thing);
                    KnickKnacks.resize(0);
                    long items = 0;
                    if (def->ItemsPerRound.size() > 0)
                        items = def->ItemsPerRound[CurrentRound + (FirstRound ? 0 : 1) % def->ItemsPerRound.size()];
                    std::vector<long> knicknacks(def->Knicknacks);
                    std::random_shuffle(knicknacks.begin(), knicknacks.end());
                    if ((unsigned long)items > knicknacks.size())
                        items = knicknacks.size();
                    for (long j = 0; j < items; ++j) {
                        KnickKnack knick;
                        knick.Thing = CreateGamePiece(knicknacks[j]);
                        KnickKnacks.push_back(knick);
                    }
                    NewKnickKnack = KnickKnacks.back().Thing;
                }
                GFFloatRandom(1.0f, PeekWorld().GetWorldSeed());
                {
                    float halton = GFFloatRandom(1.0f, PeekWorld().GetWorldSeed());
                    KnickKnack* last = KnickKnacks.end();
                    if (TavernGameState == 0x27)
                        --last;
                    std::random_shuffle(KnickKnacks.begin(), last);
                    for (KnickKnack* it = KnickKnacks.begin(); it != last; ++it) {
                        it->Pos.X = (((float)(it - KnickKnacks.begin()) + 0.5f) / (last - KnickKnacks.begin()) * 2.0f - 1.0f) * def->CoverWidth;
                        it->Pos.Y = (halton * 2.0f - 1.0f) * def->CoverLength;
                        it->Pos.Z = 0;
                        GetNextHaltonSequenceNumber(halton, 3);
                    }
                    Selected = (last - KnickKnacks.begin()) / 2;
                    for (unsigned long k = 0; k < KnickKnacks.size(); ++k) {
                        if (TavernGameState != 0x27 || k != KnickKnacks.size() - 1)
                            PositionMesh((CThing*)KnickKnacks[k].Thing, BoardPos, BoardRHSet, KnickKnacks[k].Pos, 0.0f,
                                         GFFloatRandom(1.0f, PeekWorld().GetWorldSeed()));
                    }
                }
                {
                    CDefPointer<const CTavernGameSpotTheAdditionDef> d;
                    ParentThing->PeekBaseDef().GetSubDef(&d);
                    float radius = d->KnickKnackRadius;
                    MinX = -(radius + d->CoverWidth);
                    MaxX = radius + d->CoverWidth;
                    MinY = -(radius + d->CoverLength);
                    MaxY = radius + d->CoverLength;
                    if (TavernGameState == 0x27) {
                        if (!FirstRound)
                            ++CurrentRound;
                        else
                            FirstRound = false;
                    }
                }
            }
            coverPos = 0.0f;
        } else {
            if (t < def->TimeToMoveCover + def->TimeToMoveCover + def->TimeToPauseCover) {
                coverPos = (t - def->TimeToMoveCover - def->TimeToPauseCover) / def->TimeToMoveCover;
            } else {
                coverPos = 1.0f;
                CoverTimer = 0;
                UpdatedKnickKnacks = false;
                if (TavernGameState == 0x27) {
                    CDefPointer<const CTavernGameDef> gameDef;
                    CTCTavernGame::GetDef(&gameDef);
                    SetStateAfterOK(GetText(gameDef->ClickToContinue), GetText(gameDef->Continue), (ETavernGameStates)0x29);
                } else
                    SetState((ETavernGameStates)0x2a);
            }
        }
        PositionCover(coverPos);
        break;
    }
    case 0x2c:
        if (YesState == 0x29) {
            CurrentTime += 1.0f / CMainGameComponent::GetConstantFPS();
            if (CurrentTime > TargetTime) {
                GameWon = false;
                CPlayerGui::Get().GetSubtitleRenderer().Clear(false);
                CDefPointer<const CTavernGameDef> gameDef;
                CTCTavernGame::GetDef(&gameDef);
                SetStateAfterOK(GetTextWithNarrator(gameDef->OutOfTimePhrase), GetText(gameDef->ClickToContinue), (ETavernGameStates)6);
            }
        }
        break;
    }
    if (IsBeingPlayed && TavernGameState == 0x2a)
        UpdateHighlightedObject();
}