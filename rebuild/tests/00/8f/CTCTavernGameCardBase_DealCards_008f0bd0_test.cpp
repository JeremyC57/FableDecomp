// Behaviour test for 0x008F0BD0 CTCTavernGameCardBase::DealCards(): the row's defines, includes and bodies verbatim (incl. the same-TU
// CCard::Move and GetCardDef, and the shared GetSubDef body) over the shared headers, run on zeroed object images. Doubles: the
// GetCardOrder virtual (slot 57) and the base def's GetPSubDefInfo (slot 14) through fake vtables, CDefinitionManager::GetPDefFromGlobalIndex,
// PlaySoundW, SetState, GetPackPosition, CCharString, DELTA_NOTIONAL_ZERO (0.25 here so the threshold is exact). Cases: empty dealt
// area (each of the four bounds alone non-zero), a stopped card stepping to the next / the last card finishing (state 21), the four
// bound updates both ways, the moving card's height test below / exactly on / above the threshold, and the pack-distance tests exactly
// on / above the card width and height (the drifted card is re-moved to its target at table height); def reference counts are checked.
#include <stdio.h>
#include <string.h>
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
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDORDERS
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_BLACKJACK_DEAL_PASS23
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#include <math.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCTavernGame.h"
#include "engine/CCardPositionsDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThing.h"
#include "engine/CTCTavernGameCardBase.h"
extern const float DELTA_NOTIONAL_ZERO;                                  // 0x0129BA3C
static float mult_to_stay_off_table = 2.0f;                              // 0x0138927C (Ego_d LDATA32 tc_tavern_game_card_base.obj)

void CCard::Move(const C3DVector& new_pos, bool immediate)
{
    if (immediate) {
        LocalPos = new_pos;
        NewLocalPos = new_pos;
        return;
    }
    if (Moving) {
        LocalPos = CurrentPos;
    } else {
        CurrentPos = LocalPos;
        Moving = true;
    }
    NewLocalPos = new_pos;
    MoveFraction = 0.0f;
}

void CTCTavernGameCardBase::DealCards()
{
    CDefPointer<const CTavernGameCardBaseDef> pdef;
    GetCardDef(&pdef);
    long card_order = GetCardOrder();
    bool moving = false;
    if (MinX == 0.0f && MaxX == 0.0f && MinY == 0.0f && MaxY == 0.0f)
        CurrentMovingCard = 0;
    else if (!Cards[CurrentMovingCard]->Moving) {
        ++CurrentMovingCard;
        if (CurrentMovingCard == Cards.size()) {
            SetState(ETGS_CARDS_FINISHED_MOVING);
            return;
        }
    }
    else
        moving = true;
    if (!moving) {
        C3DVector offset(pdef->CardWidth * pdef->CardOrders[card_order].Offset[CurrentMovingCard].X,
                         pdef->CardOrders[card_order].Offset[CurrentMovingCard].Y * pdef->CardHeight,
                         Cards[CurrentMovingCard]->LocalPos.Z);
        if (offset.X < MinX)
            MinX = offset.X;
        if (offset.X > MaxX)
            MaxX = offset.X;
        if (offset.Y < MinY)
            MinY = offset.Y;
        if (offset.Y > MaxY)
            MaxY = offset.Y;
        PlaySoundW("TAVERN_GAME_CARD_DEAL");
        Cards[CurrentMovingCard]->Move(offset, false);
    } else if (fabs(Cards[CurrentMovingCard]->NewLocalPos.Z - ((mult_to_stay_off_table * pdef->CardSeparation) + pdef->TableSeparation)) > DELTA_NOTIONAL_ZERO) {
        C2DVector base_point(GetPackPosition());
        C2DVector current_position(Cards[CurrentMovingCard]->CurrentPos);
        float x_dist = (float)fabs(base_point.X - current_position.X);
        float y_dist = (float)fabs(base_point.Y - current_position.Y);
        if (x_dist > pdef->CardWidth || y_dist > pdef->CardHeight) {
            C3DVector new_pos(Cards[CurrentMovingCard]->NewLocalPos.X, Cards[CurrentMovingCard]->NewLocalPos.Y,
                              (mult_to_stay_off_table * pdef->CardSeparation) + pdef->TableSeparation);
            Cards[CurrentMovingCard]->Move(new_pos, false);
        }
    }
}

void CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* pdef)
{
    CDefPointer<const CTavernGameCardBaseDef> new_pdef;
    GetParentThing().PeekBaseDef().GetSubDef(&new_pdef);
    *pdef = new_pdef;
}

// ---- doubles ----
static int g_bad=0;
static void check(bool ok, int line) { if (!ok) { printf("check failed line %d\n", line); ++g_bad; } }
const float DELTA_NOTIONAL_ZERO = 0.25f;
CCharString::CCharString(const char* text, long) { *(const char**)this=text; }
CCharString::~CCharString() { }
static unsigned char g_game[0x318], g_thing[0x100], g_basedef[0x80], g_carddef[0x90], g_mgr[0x100];
static unsigned char g_orders[2][0x48];
static C2DVector g_offsets[2][4];
static CCard g_cards[4]; static CCard* g_cardptrs[4];
static void* g_game_vtbl[64]; static void* g_def_vtbl[24];
static long g_card_order; static int n_sound, n_state, n_pack, n_info, n_pdef; static long g_state; static char g_sound[64];
static C3DVector g_pack;
struct SFakeCalls {
    long GetCardOrder() { check((void*)this==g_game, __LINE__); return g_card_order; }
    const CDefClassBase::CSubDefInfo* GetInfo(const CCharString& name)
    {
        static CDefClassBase::CSubDefInfo info;
        check((void*)this==g_basedef, __LINE__); check(strcmp(*(const char* const*)&name,"CTavernGameCardBaseDef")==0, __LINE__); ++n_info;
        info.DefIndex=77; return &info;
    }
    void Bad() { check(false, __LINE__); }
};
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index)
{
    check((void*)this==g_mgr, __LINE__); check(index==77, __LINE__); ++n_pdef;
    CDefPointer<CDefClassBase> r; *(void**)&r=g_carddef; ++*(long*)(g_carddef+4); return r;
}
unsigned long CTCTavernGame::PlaySoundW(const CCharString& name) { check((void*)this==g_game, __LINE__); ++n_sound; strcpy(g_sound,*(const char* const*)&name); return 0; }
void CTCTavernGame::SetState(ETavernGameStates state) { check((void*)this==g_game, __LINE__); ++n_state; g_state=state; }
C3DVector CTCTavernGameCardBase::GetPackPosition() { check((void*)this==g_game, __LINE__); ++n_pack; return g_pack; }

static CTCTavernGameCardBase* game() { return (CTCTavernGameCardBase*)g_game; }
static void setup(float minx, float maxx, float miny, float maxy, long index, int ncards)
{
    memset(g_game,0,sizeof g_game); memset(g_thing,0,sizeof g_thing); memset(g_basedef,0,sizeof g_basedef); memset(g_carddef,0,sizeof g_carddef);
    memset(g_cards,0,sizeof g_cards);
    n_sound=n_state=n_pack=n_info=n_pdef=0; g_state=-1; g_sound[0]=0;
    *(void***)g_game=g_game_vtbl; *(void**)(g_game+4)=g_thing; *(void**)(g_thing+0x70)=g_basedef;
    *(void***)g_basedef=g_def_vtbl; *(long*)(g_basedef+4)=5; *(void**)(g_basedef+0x1c)=g_mgr;
    *(void***)g_carddef=g_def_vtbl; *(long*)(g_carddef+4)=5;
    *(float*)(g_carddef+0x2c)=2.0f; *(float*)(g_carddef+0x30)=3.0f; *(float*)(g_carddef+0x5c)=0.25f; *(float*)(g_carddef+0x60)=0.5f;
    *(void**)(g_carddef+0x6c)=g_orders[0]; *(void**)(g_carddef+0x70)=g_orders[0]+2*0x48; *(void**)(g_carddef+0x74)=g_orders[0]+2*0x48;
    for (int o=0;o<2;++o) { *(void**)(g_orders[o]+0x3c)=g_offsets[o]; *(void**)(g_orders[o]+0x40)=g_offsets[o]+4; *(void**)(g_orders[o]+0x44)=g_offsets[o]+4; }
    for (int i=0;i<4;++i) { g_cardptrs[i]=&g_cards[i]; g_cards[i].LocalPos=C3DVector(10.0f+i,20.0f+i,30.0f+i); g_cards[i].CurrentPos=C3DVector(40.0f+i,50.0f+i,60.0f+i); g_cards[i].MoveFraction=0.5f; }
    *(void**)(g_game+0x128)=g_cardptrs; *(void**)(g_game+0x12c)=g_cardptrs+ncards; *(void**)(g_game+0x130)=g_cardptrs+4;
    game()->MinX=minx; game()->MaxX=maxx; game()->MinY=miny; game()->MaxY=maxy; game()->CurrentMovingCard=index;
}
static void run() { game()->CTCTavernGameCardBase::DealCards(); }
static bool eq(const C3DVector& a, float x, float y, float z) { return a.X==x && a.Y==y && a.Z==z; }

int main()
{
    union { long (SFakeCalls::*m)(); void* p; } u1; u1.m=&SFakeCalls::GetCardOrder;
    union { const CDefClassBase::CSubDefInfo* (SFakeCalls::*m)(const CCharString&); void* p; } u2; u2.m=&SFakeCalls::GetInfo;
    union { void (SFakeCalls::*m)(); void* p; } u3; u3.m=&SFakeCalls::Bad;
    for (int i=0;i<64;++i) g_game_vtbl[i]=u3.p;
    for (int i=0;i<24;++i) g_def_vtbl[i]=u3.p;
    g_game_vtbl[57]=u1.p; g_def_vtbl[14]=u2.p;
    g_card_order=1;
    g_offsets[1][0]=C2DVector(-1.5f,2.0f); g_offsets[1][1]=C2DVector(3.0f,-0.5f); g_offsets[1][2]=C2DVector(0.25f,0.25f);
    g_offsets[0][0]=C2DVector(100.0f,100.0f);
    // empty dealt area: card 0 placed at (2*-1.5, 2*3, LocalPos.Z) = (-3, 6, 30); MinX -3, MaxY 6; sound; Move (not moving)
    setup(0,0,0,0,5,3); run();
    check(game()->CurrentMovingCard==0, __LINE__); check(n_info==1 && n_pdef==1, __LINE__);
    check(game()->MinX==-3.0f && game()->MaxX==0.0f && game()->MinY==0.0f && game()->MaxY==6.0f, __LINE__);
    check(n_sound==1 && strcmp(g_sound,"TAVERN_GAME_CARD_DEAL")==0, __LINE__); check(n_state==0 && n_pack==0, __LINE__);
    check(g_cards[0].Moving && eq(g_cards[0].CurrentPos,10,20,30) && eq(g_cards[0].NewLocalPos,-3,6,30) && g_cards[0].MoveFraction==0.0f, __LINE__);
    check(eq(g_cards[0].LocalPos,10,20,30), __LINE__); check(*(long*)(g_carddef+4)==5, __LINE__);
    // each bound alone non-zero -> not the empty case: card 0 is stopped, step to card 1 = (6, -1.5, 31)
    for (int b=0;b<4;++b) {
        setup(b==0?-0.5f:0, b==1?0.5f:0, b==2?-0.5f:0, b==3?0.5f:0, 0, 3); run();
        check(game()->CurrentMovingCard==1, __LINE__); check(eq(g_cards[1].NewLocalPos,6.0f,-1.5f,31) && g_cards[1].Moving, __LINE__);
        check(!g_cards[0].Moving && g_cards[0].NewLocalPos.X==0.0f, __LINE__);
        check(game()->MaxX==6.0f && game()->MinY==-1.5f, __LINE__);
        check(game()->MinX==(b==0?-0.5f:0.0f) && game()->MaxY==(b==3?0.5f:0.0f), __LINE__);
    }
    // bounds kept when the new card lies inside them
    setup(-10,10,-10,10,0,3); run(); check(game()->MinX==-10 && game()->MaxX==10 && game()->MinY==-10 && game()->MaxY==10, __LINE__);
    // a stopped moving card that was already moving when placed: LocalPos takes CurrentPos (Move's moving branch)
    setup(-10,10,-10,10,0,3); g_cards[1].Moving=true; g_cards[0].Moving=false; run();
    check(game()->CurrentMovingCard==1 && eq(g_cards[1].LocalPos,41,51,61) && eq(g_cards[1].NewLocalPos,6.0f,-1.5f,31), __LINE__);
    // the last card stopped: state 21, nothing placed, def released
    setup(-10,10,-10,10,2,3); run();
    check(game()->CurrentMovingCard==3 && n_state==1 && g_state==21 && n_sound==0, __LINE__); check(*(long*)(g_carddef+4)==5, __LINE__);
    // moving card: table height h = 2*0.25+0.5 = 1.0; NewLocalPos.Z on h, exactly eps away -> nothing; beyond eps -> pack test
    setup(-10,10,-10,10,1,3); g_cards[1].Moving=true; g_cards[1].NewLocalPos=C3DVector(7,8,1.0f); run(); check(n_pack==0 && n_sound==0 && game()->CurrentMovingCard==1, __LINE__);
    setup(-10,10,-10,10,1,3); g_cards[1].Moving=true; g_cards[1].NewLocalPos=C3DVector(7,8,1.25f); run(); check(n_pack==0, __LINE__);
    setup(-10,10,-10,10,1,3); g_cards[1].Moving=true; g_cards[1].NewLocalPos=C3DVector(7,8,0.75f); run(); check(n_pack==0, __LINE__);
    // beyond eps; pack at CurrentPos + (width, height) exactly -> no move
    setup(-10,10,-10,10,1,3); g_cards[1].Moving=true; g_cards[1].NewLocalPos=C3DVector(7,8,1.5f); g_pack=C3DVector(41+2.0f,51+3.0f,99); run();
    check(n_pack==1 && eq(g_cards[1].NewLocalPos,7,8,1.5f) && g_cards[1].MoveFraction==0.5f, __LINE__); check(*(long*)(g_carddef+4)==5, __LINE__);
    setup(-10,10,-10,10,1,3); g_cards[1].Moving=true; g_cards[1].NewLocalPos=C3DVector(7,8,0.5f); g_pack=C3DVector(41-2.0f,51-3.0f,99); run();
    check(n_pack==1 && eq(g_cards[1].NewLocalPos,7,8,0.5f), __LINE__);
    // x beyond the width -> re-moved to (7, 8, 1.0); LocalPos takes CurrentPos
    setup(-10,10,-10,10,1,3); g_cards[1].Moving=true; g_cards[1].NewLocalPos=C3DVector(7,8,1.5f); g_pack=C3DVector(41+2.5f,51,99); run();
    check(eq(g_cards[1].NewLocalPos,7,8,1.0f) && eq(g_cards[1].LocalPos,41,51,61) && g_cards[1].MoveFraction==0.0f && n_sound==0, __LINE__);
    // audit fix: x beyond the width on the negative side (pack left of the card) -> re-moved (kills a dropped fabs on x_dist)
    setup(-10,10,-10,10,1,3); g_cards[1].Moving=true; g_cards[1].NewLocalPos=C3DVector(7,8,1.5f); g_pack=C3DVector(41-2.5f,51,99); run();
    check(eq(g_cards[1].NewLocalPos,7,8,1.0f) && eq(g_cards[1].LocalPos,41,51,61), __LINE__);
    // y beyond the height (x inside) -> re-moved
    setup(-10,10,-10,10,1,3); g_cards[1].Moving=true; g_cards[1].NewLocalPos=C3DVector(7,8,1.5f); g_pack=C3DVector(41,51-3.5f,99); run();
    check(eq(g_cards[1].NewLocalPos,7,8,1.0f), __LINE__); check(*(long*)(g_carddef+4)==5, __LINE__);
    // CCard::Move(new_pos, true) (the same-TU body's immediate arm): LocalPos and NewLocalPos take the position, nothing else changes
    setup(-10,10,-10,10,0,3); g_cards[3].MoveFraction=0.5f; g_cards[3].Move(C3DVector(1,2,3), true);
    check(eq(g_cards[3].LocalPos,1,2,3) && eq(g_cards[3].NewLocalPos,1,2,3) && !g_cards[3].Moving && eq(g_cards[3].CurrentPos,43,53,63) && g_cards[3].MoveFraction==0.5f, __LINE__);
    if (g_bad==0) printf("008f0bd0_TEST PASS\n"); else printf("008f0bd0_TEST FAIL %d\n", g_bad);
    return 0;
}