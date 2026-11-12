#ifndef FABLE_CARD_SORTING_TYPES_H
#define FABLE_CARD_SORTING_TYPES_H
#include "engine/CTCTavernGame.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
class CCardDef; class CCardPositionsDef; class CTavernGameCardBaseDef; class CRightHandedSet; class C2DVector; enum ETavernGameControlType;
class CCard {
public: CCard(const CCard&);
public: CCard(CThingObject*,long,long,long,const C3DVector&);
public: void Turn();
public: void Move(const C3DVector&,bool);
public: void FrameUpdate(const C3DVector&,const CRightHandedSet&);
public: CThingObject* Thing;
public: long Name;
public: long Value;
public: float InitialRotation;
public: float FinalRotation;
public: float TurnSpeed;
public: float TurnFraction;
public: float MoveSpeed;
public: float MoveFraction;
public: bool Turning;
public: bool FaceDown;
public: bool Moving;
#ifdef FABLE_RETAIL_CCARD_POSITIONS
// Retail opt-in (chunk Q21, row 0x007DAEF0 CTCTavernGameCardBlackjack::DealCard): the three positions with their Ego_r.pdb type
// (C3DVector at +0x27 / +0x33 / +0x3f, byte aligned: needs FABLE_RETAIL_C3DVECTOR_PACKED, the owner-approved alignment-1 C3DVector).
// DealCard reads LocalPos.Z +0x2f, NewLocalPos X/Y/Z +0x33/+0x37/+0x3b and CurrentPos X/Y +0x3f/+0x43 (0x007DB0A4, 0x007DB1E5..0x007DB21A).
// Same offsets and size as the opaque default below.
#ifndef FABLE_RETAIL_C3DVECTOR_PACKED
#error FABLE_RETAIL_CCARD_POSITIONS needs FABLE_RETAIL_C3DVECTOR_PACKED
#endif
public: C3DVector LocalPos;
public: C3DVector NewLocalPos;
public: C3DVector CurrentPos;
#else
public: unsigned char LocalPos[12]; // Original C3DVector, unaligned member storage.
public: unsigned char NewLocalPos[12]; // Original C3DVector, unaligned member storage.
public: unsigned char CurrentPos[12]; // Original C3DVector, unaligned member storage.
#endif
public: float Width;
public: float Height;
public: long Index;
};
class CTCTavernGameCardBase : public CTCTavernGame {
public: enum ECardGameType { ECGT_TIME_LIMIT=0,ECGT_ROUND_LIMIT=1 };
public: CTCTavernGameCardBase(const CTCTavernGameCardBase&);
public: CTCTavernGameCardBase(CThing&);
public: virtual void OnInitialActivate();
public: virtual void FrameUpdate();
public: virtual void ProcessInput(const CInputEvent&,CProcessedInput&,CInputProcessTavernGame*);
public: virtual void OnSerialise(CPersistContext&,bool);
public: virtual void EnterGame();
public: virtual void LeaveGame();
public: virtual void NewGame();
public: virtual void NewRound();
public: virtual void EndRound();
public: virtual void QuitMidGame();
public: virtual void DealCards();
public: virtual void CardsMoving();
public: virtual void CardsFinishedMoving();
public: virtual void CardsTurning();
public: virtual void TurnCardsBack();
public: virtual void CardsMainGame();
public: virtual long GetCardOrder();
public: virtual void GetRandomCard(long&,long&,long,long);
public: virtual bool SelectCard();
public: void GetCardDef(CDefPointer<CTavernGameCardBaseDef const >*);
public: virtual void ShuffleCards();
public: virtual CWideString GetCurrentScore();
public: virtual void ReactionAndPayout();
public: void AddPointerParticleEffect(long);
public: virtual void UpdateHighlightedCard();
public: C3DVector GetPackPosition();
public: void UpdatePointerScale();
public: virtual ~CTCTavernGameCardBase();
public: CTCTavernGameCardBase& operator=(const CTCTavernGameCardBase&);
protected: unsigned char GamePositionRHSet[24]; // Original CRightHandedSet.
protected: C3DVector GamePositionPos;
protected: long GamePositionActionPointIndex;
protected: float MinX;
protected: float MaxX;
protected: float MinY;
protected: float MaxY;
protected: std::vector<CCard *,std::allocator<CCard *> > Cards;
protected: std::vector<CCard *,std::allocator<CCard *> > Pack;
protected: CThingObject* Pointer;
protected: float PointerScale;
protected: std::vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > > ParticleEffects;
protected: float Counter;
protected: bool RoundFinished;
protected: float Times[52];
protected: float BestTimes[52];
protected: long NumCards;
protected: CTCTavernGameCardBase::ECardGameType GameType;
protected: long CardOrder;
protected: CCard* SelectedCard;
protected: CCard* HighlightedCard;
protected: long CurrentMovingCard;
};
class CTCTavernGameCardSorting : public CTCTavernGameCardBase {
public: static CTCBase* FABLE_FASTCALL Alloc(CThing&);
public: static CCharString FABLE_FASTCALL StatGetClassTypeName();
public: virtual CCharString GetClassTypeName() const;
public: CTCTavernGameCardSorting(const CTCTavernGameCardSorting&);
public: CTCTavernGameCardSorting(CThing&);
public: virtual void OnSerialise(CPersistContext&,bool);
public: virtual void EnterGame();
public: virtual void LeaveGame();
public: virtual void NewGame();
public: virtual void NewRound();
public: virtual void EndRound();
public: virtual bool UpdateValues();
public: virtual void CardsMainGame();
public: virtual long GetCardOrder();
public: virtual bool SelectCard();
public: virtual CWideString GetBestScore();
public: virtual CWideString GetCurrentScore();
public: virtual CWideString GetRequiredScore();
public: virtual CWideString GetAdditionalInfo();
public: virtual ~CTCTavernGameCardSorting();
public: CTCTavernGameCardSorting& operator=(const CTCTavernGameCardSorting&);
private: long TimeLimitNumRoundsToWin;
private: long TimeLimitBestNumRounds;
};
FABLE_STATIC_ASSERT(sizeof(CCard)==0x58);
FABLE_STATIC_ASSERT(sizeof(CTCTavernGameCardBase)==0x318);
FABLE_STATIC_ASSERT(sizeof(CTCTavernGameCardSorting)==0x320);
#endif
