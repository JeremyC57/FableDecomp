// Self-contained fixture using actual shared classes and normal object lifetimes.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
enum EBlackjackWinStates {};
enum ECardGameType {};
#endif
#include "engine/ETCInterfaceType.h"
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define private public
#define protected public
// 007daef0 CTCTavernGameCardBlackjack::DealCard
// Flags: none
// Leaf: ?DealCard@CTCTavernGameCardBlackjack@@QAEXXZ
// Identity: both original PDBs and native caller/state-machine operands.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCARD_POSITIONS
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#include "fable_game.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#define FABLE_RETAIL_BLACKJACK_DEAL_PASS23
#include "engine/CTCTavernGameCardBlackjack.h"
#include "engine/C2DVector.h"
#include <math.h>

// Original FableWin module332 local data at04a55ac4; native01383c98.
static float mult_to_stay_off_table=2.0f;

void CTCTavernGameCardBlackjack::DealCard()
{
    CDefPointer<const CTavernGameDef> pdef(NULL_DEF);
    GetDef(&pdef);
    bool moving=true;
    if (CurrentMovingCard == -1) {
        CurrentMovingCard = CurrentDealingCards->size();
        Cards.push_back(Pack[CurrentPackCardIndex]);
        NewlyDealtCard = Pack[CurrentPackCardIndex];
        CurrentDealingCards->push_back(Pack[CurrentPackCardIndex++]);
        moving=false;
        SetState(ETGS_BLACKJACK_NEW_CARD_DEALT);
    } else if (!(*CurrentDealingCards)[CurrentMovingCard]->Moving) {
        PlaySoundW("TAVERN_GAME_CARD_FLIP");
        (*CurrentDealingCards)[CurrentMovingCard]->Turn();
        SetState(ETGS_CARDS_TURNING_UP);
        return;
    }
    if(!moving) {
        C3DVector offset = GetPosition(CurrentDealingCardsIndex, CurrentMovingCard);
        offset.Z = (*CurrentDealingCards)[CurrentMovingCard]->LocalPos.Z;
        if (offset.X < MinX)
            MinX = offset.X;
        if (offset.X > MaxX)
            MaxX = offset.X;
        if (offset.Y < MinY)
            MinY = offset.Y;
        if (offset.Y > MaxY)
            MaxY = offset.Y;
        PlaySoundW("TAVERN_GAME_CARD_DEAL");
        (*CurrentDealingCards)[CurrentMovingCard]->Move(offset, false);
    } else {
        CDefPointer<const CTavernGameCardBaseDef> pcard_def(NULL_DEF);
        GetCardDef(&pcard_def);
        CCard* card = (*CurrentDealingCards)[CurrentMovingCard];
        float z = (((float)CurrentDealingCards->size() - 1.0f) * 0.33333334f + mult_to_stay_off_table) * pcard_def->CardSeparation +
                  pcard_def->TableSeparation;
        if (card->NewLocalPos.Z - z > DELTA_NOTIONAL_ZERO) {
            C2DVector base_point(GetPackPosition());
            C2DVector current_position(card->CurrentPos);
            if (fabs(base_point.X - current_position.X) > pcard_def->CardWidth || fabs(base_point.Y - current_position.Y) > pcard_def->CardHeight) {
                C3DVector new_pos(card->NewLocalPos.X, card->NewLocalPos.Y, z);
                PlaySoundW("TAVERN_GAME_CARD_DEAL");
                card->Move(new_pos, false);
            }
        }
    }
}

#include "engine/CDefString.h"
#undef private
#undef protected

// Unrelated virtual methods abort if the selected row calls them.
void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
void CTCBase::OnSerialise(CPersistContext&, bool) { abort(); }
void CTCBase::OnRemove() { abort(); }
void CTCBase::ConstructFromParams(const CTCParamsBase&) { abort(); }
void CTCBase::OnCreate() { abort(); }
void CTCBase::InitialActivate() { abort(); }
void CTCBase::OnInitialActivate() { abort(); }
void CTCBase::RenderUpdate(float) { abort(); }
bool CTCBase::HasFrameUpdate() const { abort(); }
void CTCBase::FrameUpdate() { abort(); }
void CTCBase::BulletTimeFrameUpdate() { abort(); }
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
void CTCBase::Copy(const CTCBase&) { abort(); }
void CTCBase::OnDie() { abort(); }
void CTCBase::OnKill() { abort(); }
void CTCBase::OnLevelUnload() { abort(); }
void CTCBase::OnSetInLimbo(bool) { abort(); }
bool CTCBase::Validate(CCharString&) const { abort(); }
bool CTCBase::IsFreeForDeletion() const { abort(); }
bool CTCBase::IsSerialiseCreatable() const { abort(); }
bool CTCBase::IsSerialisableForHeroState() const { abort(); }
bool CTCBase::IsComponentSerialisable() const { abort(); }
bool CTCBase::IsDriver() const { abort(); }
bool CTCBase::IsCopyable() const { abort(); }
void CTCBase::GlobalUpdate(bool) { abort(); }
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) { abort(); }
void CTCTavernGame::OnSerialise(CPersistContext&, bool) { abort(); }
void CTCTavernGame::OnInitialActivate() { abort(); }
bool CTCTavernGame::HasFrameUpdate() const { abort(); }
void CTCTavernGame::FrameUpdate() { abort(); }
void CTCTavernGame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
ETCInterfaceType CTCTavernGame::GetInterfaceType() const { abort(); }
void CTCTavernGame::ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*) { abort(); }
void CTCTavernGame::EnterGame() { abort(); }
void CTCTavernGame::LeaveGame() { abort(); }
void CTCTavernGame::NewGame() { abort(); }
void CTCTavernGame::NewRound() { abort(); }
void CTCTavernGame::EndRound() { abort(); }
void CTCTavernGame::QuitMidGame() { abort(); }
void CTCTavernGame::Greeting() { abort(); }
void CTCTavernGame::Options() { abort(); }
void CTCTavernGame::Instructions() { abort(); }
void CTCTavernGame::Betting() { abort(); }
void CTCTavernGame::ReactionAndPayout() { abort(); }
bool CTCTavernGame::UpdateValues() { abort(); }
void CTCTavernGame::Farewell() { abort(); }
CWideString CTCTavernGame::GetWinPhrase() { abort(); }
CWideString CTCTavernGame::GetAdditionalInfo() { abort(); }
CWideString CTCTavernGame::GetBet() { abort(); }
CWideString CTCTavernGame::GetPlayersMoney() { abort(); }
CWideString CTCTavernGame::GetTotalWinnings() { abort(); }
void CTCTavernGame::DrawBetAndMoney(const CEnginePrimitivePrepareInfo&, float, float, float, float, float, float) { abort(); }
void CTCTavernGameCardBase::ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*) { abort(); }
void CTCTavernGameCardBase::DealCards() { abort(); }
void CTCTavernGameCardBase::CardsMoving() { abort(); }
void CTCTavernGameCardBase::CardsFinishedMoving() { abort(); }
void CTCTavernGameCardBase::CardsTurning() { abort(); }
void CTCTavernGameCardBase::TurnCardsBack() { abort(); }
void CTCTavernGameCardBase::CardsMainGame() { abort(); }
long CTCTavernGameCardBase::GetCardOrder() { abort(); }
void CTCTavernGameCardBase::GetRandomCard(long&, long&, long, long) { abort(); }
bool CTCTavernGameCardBase::SelectCard() { abort(); }
void CTCTavernGameCardBase::ShuffleCards() { abort(); }
void CTCTavernGameCardBase::UpdateHighlightedCard() { abort(); }
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
bool CDefClassBase::UnloadResource() { abort(); }
CDefString CDefClassBase::GetInstantiationName() const { abort(); }
void CDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CDefClassBase::IsParentDef() const { abort(); }
void CDefClassBase::PostTransfer() { abort(); }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { abort(); }
void CTavernGameCardBaseDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CTavernGameCardBaseDef::GetSizeofClass() { abort(); }
void CTavernGameCardBaseDef::Transfer(CPersistContext&) { abort(); }
void CTavernGameDef::Transfer(CPersistContext&) { abort(); }
void CTavernGameDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CTavernGameDef::GetSizeofClass() { abort(); }

static unsigned int turns=0,moves=0,deals=0,flips=0,defGets=0,cardDefGets=0,deletes=0,stringLive=0;
static CCard* movedCard=0;
static C3DVector movedTo,positionResult,packResult;
static long expectedHand=0,expectedCard=0;
static CTavernGameCardBaseDef* fixtureDef=0;
static CTavernGameDef* fixtureGameDef=0;
static bool provideGameDef=false;
const float DELTA_NOTIONAL_ZERO=0.0001f;
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CBaseObject::CBaseObject() : IntelligentPointer(0) {}
CBaseObject::~CBaseObject() {}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char* text,long length):PStringData(0) {
 assert(length==-1);PStringData=reinterpret_cast<CCharStringData*>(const_cast<char*>(text));++stringLive;
}
CCharString::~CCharString(){if(PStringData){assert(stringLive);--stringLive;}}
CWideString::CWideString():PStringData(0){}
CWideString::~CWideString(){}
CTCBase::CTCBase(CThing& thing):ParentThing(&thing){}
CTCBase::~CTCBase(){}
CTCTavernGame::CTCTavernGame(CThing& thing):CTCBase(thing){}
CTCTavernGame::~CTCTavernGame(){}
CTCTavernGameCardBase::CTCTavernGameCardBase(CThing& thing):CTCTavernGame(thing){}
CTCTavernGameCardBlackjack::CTCTavernGameCardBlackjack(CThing& thing):CTCTavernGameCardBase(thing){}
void CTCTavernGameCardBlackjack::NewRound(){abort();}
CCharString CTCTavernGameCardBlackjack::GetClassTypeName() const {abort();}
CWideString CTCTavernGameCardBlackjack::GetBestScore(){abort();}
CWideString CTCTavernGameCardBlackjack::GetCurrentScore(){abort();}
CWideString CTCTavernGameCardBlackjack::GetRequiredScore(){abort();}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}
CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CSubDefClassBase::CSubDefClassBase(){}
CTavernGameDef::CTavernGameDef(){}
CTavernGameCardBaseDef::CTavernGameCardBaseDef(){}
CTavernGameCardBaseDef::~CTavernGameCardBaseDef(){}
void CIVCountedPointeeBase::Delete() const {assert(IVRefCount==0);++deletes;}
void CTCTavernGame::GetDef(CDefPointer<const CTavernGameDef>* p){++defGets;assert(!p->Object);if(provideGameDef){p->Object=fixtureGameDef;fixtureGameDef->AddRef();}}
void CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* p){++cardDefGets;assert(!p->Object);p->Object=fixtureDef;fixtureDef->AddRef();}
void CTCTavernGame::SetState(ETavernGameStates state){TavernGameState=state;}
unsigned long CTCTavernGame::PlaySoundW(const CCharString& s){
 const char* text=reinterpret_cast<const char*>(s.PStringData);
 if(strcmp(text,"TAVERN_GAME_CARD_DEAL")==0)++deals;else{assert(strcmp(text,"TAVERN_GAME_CARD_FLIP")==0);++flips;}
 return 0;
}
C3DVector CTCTavernGameCardBlackjack::GetPosition(long hand,long card){assert(hand==expectedHand&&card==expectedCard);return positionResult;}
C3DVector CTCTavernGameCardBase::GetPackPosition(){return packResult;}
void CCard::Move(const C3DVector& p,bool snap){assert(!snap);++moves;movedCard=this;movedTo=p;}
void CCard::Turn(){++turns;}

static void resetCounters(){turns=moves=deals=flips=defGets=cardDefGets=deletes=0;movedCard=0;assert(stringLive==0);}
static void seedCard(CCard& c,float height){
 c.Thing=0;c.Name=0;c.Value=1;c.Turning=false;c.FaceDown=true;c.Moving=false;
 c.LocalPos=C3DVector(0,0,height);c.NewLocalPos=C3DVector(0,0,height);c.CurrentPos=C3DVector(0,0,height);
}
static bool nearFloat(float a,float b){return fabs(a-b)<0.0001;}
int main(){
 CThing owner;CTavernGameCardBaseDef def;CTavernGameDef gameDef;fixtureDef=&def;fixtureGameDef=&gameDef;
 def.CardWidth=2.0f;def.CardHeight=4.0f;def.CardSeparation=0.75f;def.TableSeparation=1.5f;
 unsigned int count=0;
 const int sizes[4]={0,1,2,4};
 const float px[4]={-4.0f,-1.0f,1.0f,4.0f};
 for(int ni=0;ni<4;++ni)for(int packIndex=0;packIndex<3;++packIndex)for(int spare=0;spare<2;++spare)
 for(int point=0;point<4;++point)for(int zi=0;zi<3;++zi)for(int hand=0;hand<2;++hand)for(int withDef=0;withDef<2;++withDef){
  resetCounters();provideGameDef=withDef!=0;gameDef.IVRefCount=7;
  CTCTavernGameCardBlackjack game(owner);CCard cards[8];int n=sizes[ni];
  for(int i=0;i<8;++i)seedCard(cards[i],(float)zi*0.5f);
  for(int i=0;i<n+spare;++i){game.Cards.push_back(&cards[i]);game.DealersCards.push_back(&cards[i]);}
  if(spare){game.Cards.pop_back();game.DealersCards.pop_back();}
  for(int i=0;i<3;++i)game.Pack.push_back(&cards[n+i]);
  game.CurrentDealingCards=&game.DealersCards;game.CurrentMovingCard=-1;game.CurrentPackCardIndex=packIndex;
  game.CurrentDealingCardsIndex=hand;game.NewlyDealtCard=0;game.MinX=-2;game.MaxX=2;game.MinY=-2;game.MaxY=2;
  expectedHand=hand;expectedCard=n;positionResult=C3DVector(px[point],px[3-point],99);
  game.DealCard();
  assert(game.CurrentMovingCard==n&&game.CurrentPackCardIndex==packIndex+1);
  assert(game.NewlyDealtCard==&cards[n+packIndex]);
  assert(game.Cards.size()==(unsigned int)n+1&&game.DealersCards.size()==(unsigned int)n+1);
  assert(game.Cards[n]==&cards[n+packIndex]&&game.DealersCards[n]==&cards[n+packIndex]);
  for(int i=0;i<n;++i)assert(game.Cards[i]==&cards[i]&&game.DealersCards[i]==&cards[i]);
  assert(game.Pack.size()==3&&game.Pack[packIndex]==&cards[n+packIndex]);
  assert(game.TavernGameState==CTCTavernGame::ETGS_BLACKJACK_NEW_CARD_DEALT);
  assert(game.MinX==(px[point]<-2?px[point]:-2)&&game.MaxX==(px[point]>2?px[point]:2));
  assert(game.MinY==(px[3-point]<-2?px[3-point]:-2)&&game.MaxY==(px[3-point]>2?px[3-point]:2));
  assert(moves==1&&turns==0&&deals==1&&flips==0&&cardDefGets==0&&defGets==1);
  assert(movedCard==&cards[n+packIndex]&&movedTo.X==positionResult.X&&movedTo.Y==positionResult.Y&&movedTo.Z==(float)zi*0.5f);
  assert(gameDef.IVRefCount==7&&stringLive==0&&deletes==0);++count;
 }
 for(int n=1;n<=4;++n)for(int cardIndex=0;cardIndex<n;++cardIndex)for(int withDef=0;withDef<2;++withDef){
  resetCounters();provideGameDef=withDef!=0;gameDef.IVRefCount=7;
  CTCTavernGameCardBlackjack game(owner);CCard cards[4];for(int i=0;i<n;++i){seedCard(cards[i],0);game.DealersCards.push_back(&cards[i]);}
  game.CurrentDealingCards=&game.DealersCards;game.CurrentMovingCard=cardIndex;game.CurrentPackCardIndex=3;
  game.DealCard();
  assert(turns==1&&flips==1&&moves==0&&deals==0&&cardDefGets==0&&defGets==1);
  assert(game.TavernGameState==CTCTavernGame::ETGS_CARDS_TURNING_UP&&game.CurrentPackCardIndex==3);
  assert(game.DealersCards.size()==(unsigned int)n&&gameDef.IVRefCount==7&&stringLive==0);++count;
 }
 const float distances[7]={-8,-4,-2,0,2,4,8};
 for(int length=0;length<2;++length)for(int height=0;height<3;++height)
 for(int xi=0;xi<7;++xi)for(int yi=0;yi<7;++yi)for(int withDef=0;withDef<2;++withDef){
  resetCounters();provideGameDef=withDef!=0;gameDef.IVRefCount=7;def.IVRefCount=5;
  CTCTavernGameCardBlackjack game(owner);CCard cards[4];int n=length?4:1;
  for(int i=0;i<n;++i){seedCard(cards[i],0);game.DealersCards.push_back(&cards[i]);}
  CCard& c=cards[n-1];c.Moving=true;
  const float desired=(2.0f+(float)length)*def.CardSeparation+def.TableSeparation;
  c.NewLocalPos=C3DVector(7,-5,desired+(float)(height-1)*0.5f);
  c.CurrentPos=C3DVector(distances[xi],distances[yi],0);packResult=C3DVector(0,0,0);
  game.CurrentDealingCards=&game.DealersCards;game.CurrentMovingCard=n-1;game.CurrentPackCardIndex=2;
  game.DealCard();
  bool expectedMove=height==2&&(fabs(distances[xi])>2||fabs(distances[yi])>4);
  assert(moves==(expectedMove?1U:0U)&&deals==moves&&turns==0&&flips==0&&defGets==1&&cardDefGets==1);
  if(expectedMove)assert(movedCard==&c&&movedTo.X==7&&movedTo.Y==-5&&nearFloat(movedTo.Z,desired));
  assert(game.CurrentPackCardIndex==2&&game.CurrentMovingCard==n-1&&game.DealersCards.size()==(unsigned int)n);
  assert(gameDef.IVRefCount==7&&def.IVRefCount==5&&deletes==0&&stringLive==0);++count;
 }
 assert(count==1760);
 puts("007daef0_TEST PASS");return 0;
}