#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
enum {TCI_GRAPHIC_APPEARANCE_NEW=0x5b, ETGS_CARDS_MAIN_GAME=23};
struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
    float Dot(const C3DVector& rhs) const { return X*rhs.X+Y*rhs.Y+Z*rhs.Z; }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X-o.X,Y-o.Y,Z-o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X+o.X,Y+o.Y,Z+o.Z); }
    C3DVector& operator*=(float scale) { X*=scale; Y*=scale; Z*=scale; return *this; }
    void Normalise() { *this *= 1.0f/(float)sqrt(X*X+Y*Y+Z*Z); }
};

struct C2DVector { float X,Y; };
struct CRightHandedSet {
    C3DVector GetRight() const { return C3DVector(1,0,0); }
    C3DVector GetForward() const { return C3DVector(0,1,0); }
    void ToLocal(const C3DVector& d,float& x,float& y) const { x=d.X; y=d.Y; }
};
typedef std::string CCharString;
struct CRGBColour { unsigned char R,G,B,A; };
namespace NGraphicAppearance {
typedef int EAppearanceMeshEffectPriority;
enum { PRIORITY_3=3 };
struct CTCGraphicAppearance {
    unsigned Clears,Highlights;
    void* Key;
    bool Bad;
    CTCGraphicAppearance():Clears(0),Highlights(0),Key(0),Bad(false) {}
    void ClearMeshEffect(void* key) { Key=key; ++Clears; }
    void SetAsHighlighted(float width,float start,const CRGBColour& color,bool first,bool second,int priority,void* owner)
    {
        Key=owner; ++Highlights; Bad=width!=2||start!=0||color.R!=11||first||second||priority!=3;
    }
};
}
struct CThing {
    C3DVector Position;
    NGraphicAppearance::CTCGraphicAppearance Appearance;
    CThing(float x=0,float y=0,float z=0):Position(x,y,z) {}
    const C3DVector& GetPos() const { return Position; }
    bool GetTC(int id,NGraphicAppearance::CTCGraphicAppearance** out) { *out=&Appearance; return id==0x5b; }
};
struct CCard { CThing* Thing; };
struct CCardVector { CCard** _M_start; CCard** _M_finish; };
struct CTavernGameCardBaseDef {
    float CardWidth,CardHeight,HighlightWidth;
    CRGBColour HighlightColourGood;
    mutable int RefCount;
    CTavernGameCardBaseDef():CardWidth(4),CardHeight(2),HighlightWidth(2),RefCount(0)
    { HighlightColourGood.R=11; }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer():Object(0) {}
    ~CDefPointer() { if(Object) --Object->RefCount; }
    T* operator->() const { return Object; }
};
struct CTCTavernGameCardBase {
    long TavernGameState;
    CRightHandedSet GamePositionRHSet;
    C3DVector GamePositionPos;
    std::vector<CCard*> Cards;
    CThing* Pointer;
    CCard* HighlightedCard;
    CTavernGameCardBaseDef Definition;
    unsigned Sounds;
    CTCTavernGameCardBase():TavernGameState(0),GamePositionPos(5,5,0),Pointer(0),HighlightedCard(0),Sounds(0) {}
    void GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* out) { out->Object=&Definition; ++Definition.RefCount; }
    void PlaySoundW(const CCharString& name) { if(name=="SOUND_CARD_HIGHLIGHT") ++Sounds; }
    void UpdateHighlightedCard();
};

void CTCTavernGameCardBase::UpdateHighlightedCard()
{
    C3DVector relative_pos = Pointer->GetPos() - GamePositionPos;
    float pointer_x = relative_pos.Dot(GamePositionRHSet.GetRight());
    float pointer_y = relative_pos.Dot(GamePositionRHSet.GetForward());
    CDefPointer<const CTavernGameCardBaseDef> pdef;
    GetCardDef(&pdef);
    float half_card_width = pdef->CardWidth * 0.5f;
    float half_card_height = pdef->CardHeight * 0.5f;
    CCard* previously_highlighted_card = HighlightedCard;
    NGraphicAppearance::CTCGraphicAppearance* tc_graphic_appearance;
    if (previously_highlighted_card) {
        previously_highlighted_card->Thing->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &tc_graphic_appearance);
        tc_graphic_appearance->ClearMeshEffect(this);
        HighlightedCard = 0;
    }
    if (TavernGameState == ETGS_CARDS_MAIN_GAME) {
        for (std::vector<CCard*>::iterator card_it = Cards.begin(); card_it != Cards.end(); ++card_it) {
            C3DVector card_relative_pos = (*card_it)->Thing->GetPos() - GamePositionPos;
            float card_x = card_relative_pos.Dot(GamePositionRHSet.GetRight());
            float card_y = card_relative_pos.Dot(GamePositionRHSet.GetForward());
            if (card_x - half_card_width <= pointer_x && card_x + half_card_width >= pointer_x &&
                card_y - half_card_height <= pointer_y && card_y + half_card_height >= pointer_y) {
                HighlightedCard = *card_it;
                break;
            }
        }
        if (HighlightedCard) {
            if (HighlightedCard != previously_highlighted_card)
                PlaySoundW("SOUND_CARD_HIGHLIGHT");
            HighlightedCard->Thing->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &tc_graphic_appearance);
            tc_graphic_appearance->SetAsHighlighted(pdef->HighlightWidth, 0.0f, pdef->HighlightColourGood, false, false, (NGraphicAppearance::EAppearanceMeshEffectPriority)3, this);
        }
    }
}

int main()
{
    CThing cursor(13,8),thing1(8,8),thing2(15,8);
    CCard card1={&thing1},card2={&thing2};
    CCard* cards[]={&card1,&card2};
    CTCTavernGameCardBase table;
    table.Pointer=&cursor;table.Cards.assign(cards,cards+2);
    table.HighlightedCard=&card1;
    table.UpdateHighlightedCard();
    if(table.HighlightedCard || thing1.Appearance.Clears!=1 || table.Sounds || table.Definition.RefCount)
        return 1;
    table.TavernGameState=0x17;
    table.UpdateHighlightedCard();
    if(table.HighlightedCard!=&card2 || thing2.Appearance.Highlights!=1 || table.Sounds!=1 ||
       thing2.Appearance.Key!=&table || thing2.Appearance.Bad)
        return 2;
    table.UpdateHighlightedCard();
    if(table.HighlightedCard!=&card2 || thing2.Appearance.Highlights!=2 || thing2.Appearance.Clears!=1 || table.Sounds!=1)
        return 3;
    cursor.Position.Z=10;cursor.Position.X=8;
    table.UpdateHighlightedCard();
    if(table.HighlightedCard!=&card1 || thing2.Appearance.Clears!=2 || table.Sounds!=2 || table.Definition.RefCount)
        return 4;
    cursor.Position.X=40;
    table.UpdateHighlightedCard();
    if(table.HighlightedCard || thing1.Appearance.Clears!=2 || table.Sounds!=2 || table.Definition.RefCount)
        return 5;
    puts("008f18c0_TEST PASS");
    return 0;
}
