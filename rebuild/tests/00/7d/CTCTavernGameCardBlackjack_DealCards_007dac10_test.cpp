// Execute the production DealCards body with observable card, state, and sound dependencies.
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"

namespace CheckDeal {
const float DELTA_NOTIONAL_ZERO = 0.0001f;
static float mult_to_stay_off_table = 2.0f;
const int NULL_DEF=0;
struct CTavernGameCardBaseDef {float CardWidth,CardHeight,CardSeparation,TableSeparation;};
template<class T> class CDefPointer {
public:
    T* value;
    CDefPointer(int):value(0){}
    T* operator->() const {return value;}
};
struct ObservedCard {
    C3DVector LocalPos,NewLocalPos,CurrentPos,destination;
    bool Moving,lastImmediate; int moves;
    void Move(const C3DVector& p,bool immediate) {destination=p;lastImmediate=immediate;++moves;}
};
enum { ETGS_DEAL_CARDS=18, ETGS_DEAL_DEALERS_CARDS=19, ETGS_CARDS_FINISHED_MOVING=21 };
class DealFixture {
public:
    typedef std::vector<ObservedCard*> Hand;
    std::vector<Hand> PlayersCards; Hand dealers; Hand* CurrentDealingCards;
    std::vector<long> Bets; CTavernGameCardBaseDef def; ObservedCard cards[2];
    long state,CurrentMovingCard,BaseBet,sounds,lastOrder,lastIndex;
    float MinX,MaxX,MinY,MaxY;
    DealFixture():PlayersCards(3),CurrentDealingCards(&dealers),state(19),
      CurrentMovingCard(-1),BaseBet(7),sounds(0),lastOrder(-1),lastIndex(-1),
      MinX(0),MaxX(0),MinY(0),MaxY(0) {
        def.CardWidth=2;def.CardHeight=4;def.CardSeparation=2;def.TableSeparation=4;
        for(int i=0;i<2;++i) {
            cards[i].Moving=false;cards[i].lastImmediate=true;cards[i].moves=0;
            cards[i].LocalPos=C3DVector(1,2,3);
            cards[i].NewLocalPos=C3DVector(5,6,8);
            cards[i].CurrentPos=C3DVector(0,0,0);dealers.push_back(&cards[i]);
        }
    }
    void GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* p){p->value=&def;}
    long GetGameState() const {return state;}
    void SetState(long v){state=v;}
    void PlaySoundW(const char* n){sounds+=strcmp(n,"TAVERN_GAME_CARD_DEAL")?100:1;}
    C3DVector GetPosition(long hand,long card) {lastOrder=hand;lastIndex=card;return C3DVector(-4,7,9);}
    C3DVector GetPackPosition()const {return C3DVector(0,0,0);}
    void DealCards();
};
void DealFixture::DealCards()
{
    CDefPointer<const CTavernGameCardBaseDef> pdef(NULL_DEF);
    GetCardDef(&pdef);
    long card_order = GetGameState() == ETGS_DEAL_DEALERS_CARDS ? PlayersCards.size() : 0;
    bool moving = true;
    if (CurrentMovingCard == -1 || !(*CurrentDealingCards)[CurrentMovingCard]->Moving) {
        ++CurrentMovingCard;
        if (CurrentMovingCard == 2) {
            if (GetGameState() == ETGS_DEAL_DEALERS_CARDS) {
                CurrentMovingCard = -1;
                CurrentDealingCards = &PlayersCards[0];
                Bets.push_back(BaseBet);
                SetState(ETGS_DEAL_CARDS);
                return;
            } else {
                SetState(ETGS_CARDS_FINISHED_MOVING);
                return;
            }
        }
        moving = false;
    }

    if (!moving) {
        C3DVector offset;
        offset = GetPosition(card_order, CurrentMovingCard);
        offset.Z = (*CurrentDealingCards)[CurrentMovingCard]->LocalPos.Z;
        if (offset.X < MinX) MinX = offset.X;
        if (offset.X > MaxX) MaxX = offset.X;
        if (offset.Y < MinY) MinY = offset.Y;
        if (offset.Y > MaxY) MaxY = offset.Y;
        PlaySoundW("TAVERN_GAME_CARD_DEAL");
        (*CurrentDealingCards)[CurrentMovingCard]->Move(offset, false);
    } else if ((float)fabs((*CurrentDealingCards)[CurrentMovingCard]->NewLocalPos.Z -
                (pdef->CardSeparation * mult_to_stay_off_table + pdef->TableSeparation)) > DELTA_NOTIONAL_ZERO) {
        C2DVector base_point(GetPackPosition());
        C2DVector current_position((*CurrentDealingCards)[CurrentMovingCard]->CurrentPos);
        float x_dist = (float)fabs(base_point.X - current_position.X);
        float y_dist = (float)fabs(base_point.Y - current_position.Y);
        if (x_dist > pdef->CardWidth || y_dist > pdef->CardHeight) {
            C3DVector new_pos((*CurrentDealingCards)[CurrentMovingCard]->NewLocalPos);
            new_pos.Z = pdef->CardSeparation * mult_to_stay_off_table + pdef->TableSeparation;
            PlaySoundW("TAVERN_GAME_CARD_DEAL");
            (*CurrentDealingCards)[CurrentMovingCard]->Move(new_pos, false);
        }
    }
}

bool TestAll(){
    // A fresh hand, a stopped card, advancement past the second card for both deal states.
    for(int dealer=0;dealer<2;++dealer) for(int index=-1;index<2;++index){
        DealFixture f;f.CurrentMovingCard=index;f.state=dealer?19:18;f.DealCards();
        if(index==1) {
            if(f.sounds || f.lastOrder!=-1)return false;
            if(dealer) {if(f.state!=18||f.CurrentMovingCard!=-1||f.CurrentDealingCards!=&f.PlayersCards[0]||f.Bets.size()!=1||f.Bets[0]!=7)return false;}
            else if(f.state!=21||f.CurrentMovingCard!=2||!f.Bets.empty())return false;
        } else {
            const ObservedCard& c=f.cards[index+1];
            if(c.moves!=1||c.lastImmediate||c.destination.X!=-4||c.destination.Y!=7||c.destination.Z!=3||
               f.sounds!=1||f.CurrentMovingCard!=index+1||f.lastIndex!=index+1||f.lastOrder!=(dealer?3:0)||
               f.MinX!=-4||f.MaxX!=0||f.MinY!=0||f.MaxY!=7)return false;
        }
    }
    // Exercise both outcomes of each bounds update.
    {DealFixture f;f.MinX=f.MaxX=-8;f.MinY=f.MaxY=10;f.DealCards();
     if(f.MinX!=-8||f.MaxX!=-4||f.MinY!=7||f.MaxY!=10)return false;}
    for(int height=-1;height<=1;++height) for(int x=-3;x<=3;++x) for(int y=-5;y<=5;++y) {
        DealFixture f;f.CurrentMovingCard=0;ObservedCard& c=f.cards[0];c.Moving=true;
        c.NewLocalPos.Z=8.0f+(float)height;c.CurrentPos=C3DVector((float)x,(float)y,0);
        f.DealCards();
        bool shouldMove=height!=0&&(x<-2||x>2||y<-4||y>4);
        if(c.moves!=(shouldMove?1:0)||f.sounds!=c.moves||f.state!=19||f.CurrentMovingCard!=0)return false;
        if(shouldMove&&(c.destination.X!=5||c.destination.Y!=6||c.destination.Z!=8||c.lastImmediate))return false;
    }
    return true;
}
}
int main(){if(!CheckDeal::TestAll())return 1;puts("007dac10_TEST PASS");return 0;}