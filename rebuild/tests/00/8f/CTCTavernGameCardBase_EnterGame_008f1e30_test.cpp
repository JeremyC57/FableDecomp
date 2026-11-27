#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
using std::vector;
static void check(bool ok) {if (!ok) exit(2);}
struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
    C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
};
struct CThingObject {long id;};
struct CCard {
    CThingObject* Thing;long Name,Value;
    bool FaceDown,Turning,Moving;float FinalRotation,MoveSpeed,TurnSpeed,Width,Height;
    C3DVector LocalPos;long Index;
    CCard(CThingObject*,long,long,long,const C3DVector&);
};
struct CCardDef { long CardName,CardVal; };
struct CTavernGameCardBaseDef {float CardWidth, CardHeight, PackOffset_X,PackOffset_Y,MoveSpeed,TurnSpeed,CardSeparation,TableSeparation; vector<CCardDef> CardPack;};
struct CTavernGameDef {std::string CameraName;};
static unsigned refs;
template <class T> struct CDefPointer {
    T* p;
    CDefPointer():p(0) {}
    ~CDefPointer(){if(p)--refs;}
    T* operator->() const {return p;}
    void set(T* value){p=value;++refs;}
};
struct CTCTavernGame {
    CTavernGameDef gd;
    std::string events,camera;
    virtual void EnterGame(){events+='B';}
    void GetDef(CDefPointer<const CTavernGameDef>* d){events+='G';d->set(&gd);}
    void CameraUseCameraPoint(const std::string& s){events+='C';camera=s;}
};
struct CTCTavernGameCardBase:CTCTavernGame {
    CTavernGameCardBaseDef cd;
    long CardOrder;float Counter;
    C3DVector GamePositionPos;int GamePositionRHSet;
    vector<CCard*> Pack;
    vector<C3DVector> positioned;
    virtual void ShuffleCards(){events+='S';}
    void EnterGame();
    void GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* d){events+='D';d->set(&cd);}
    C3DVector GetPackPosition();
    CThingObject* CreateGamePiece(long id){events+='P'; CThingObject* t=new CThingObject;t->id=id;return t;}
    void PositionMesh(CThingObject* t,const C3DVector& p,const int& r,const C3DVector& offset,float forward,float up){
        check(&p==&GamePositionPos && &r==&GamePositionRHSet && forward==.5f && up==.5f);
        check(t->id==cd.CardPack[positioned.size()].CardName);events+='M';positioned.push_back(offset);
    }
};
struct Game:CTCTavernGameCardBase {virtual void ShuffleCards(){check(CardOrder==0);events+='V';}};
C3DVector CTCTavernGameCardBase::GetPackPosition()
{
    CDefPointer<const CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    return C3DVector(def->CardWidth * def->PackOffset_X, def->CardHeight * def->PackOffset_Y, 0.0f);
}

CCard::CCard(CThingObject* thing, long name, long value, long index, const C3DVector& pos)
{
    Thing = thing;
    Name = name;
    Value = value;
    FaceDown = false;
    FinalRotation = 0.0f;
    Turning = false;
    Moving = false;
    LocalPos = pos;
    Width = 0.0f;
    Height = 0.0f;
    Index = index;
}

void CTCTavernGameCardBase::EnterGame()
{
    CTCTavernGame::EnterGame();
    CDefPointer<const CTavernGameCardBaseDef> pdef;
    GetCardDef(&pdef);
    CardOrder = 0;
    for (long i = 0; i < pdef->CardPack.size(); ++i) {
        C3DVector base_offset(0.0f, 0.0f, (53-i) * pdef->CardSeparation + pdef->TableSeparation);
        base_offset += GetPackPosition();
        CCard* new_card = new CCard(CreateGamePiece(pdef->CardPack[i].CardName), pdef->CardPack[i].CardName,
                                    pdef->CardPack[i].CardVal, i, base_offset);
        new_card->MoveSpeed = pdef->MoveSpeed;
        new_card->TurnSpeed = pdef->TurnSpeed;
        PositionMesh(new_card->Thing, GamePositionPos, GamePositionRHSet, base_offset, 0.5f, 0.5f);
        new_card->FinalRotation = 0.5f;
        new_card->FaceDown = true;
        new_card->Width = pdef->CardWidth;
        new_card->Height = pdef->CardHeight;
        Pack.push_back(new_card);
    }
    ShuffleCards();
    CDefPointer<const CTavernGameDef> pbasedef;
    GetDef(&pbasedef);
    CameraUseCameraPoint(pbasedef->CameraName);
    Counter = 0.0f;
}
static void run(int count,bool reserve,bool existing)
{
    Game g;
    g.CardOrder=71;g.Counter=35;g.gd.CameraName="Camera_7";
    g.cd.CardWidth=8;g.cd.CardHeight=4;g.cd.PackOffset_X=.25f;g.cd.PackOffset_Y=-.5f;
    g.cd.MoveSpeed=13;g.cd.TurnSpeed=17;g.cd.TableSeparation=3;g.cd.CardSeparation=.25f;
    if(reserve)g.Pack.reserve(count+1);
    if(existing)g.Pack.push_back(0);
    for(int i=0;i<count;++i){CCardDef c={103+i,3*i};g.cd.CardPack.push_back(c);}
    g.EnterGame();
    std::string expected="BD";
    for(int j=0;j<count;++j)expected+="DPM";
    check(g.events==expected+"VGC" && g.camera=="Camera_7" && g.Counter==0 && g.CardOrder==0 && refs==0);
    check(g.Pack.size()==count+existing && g.positioned.size()==count);
    if(existing)check(g.Pack[0]==0);
    for(int k=0;k<count;++k){
        CCard* c=g.Pack[k+existing];const C3DVector& p=g.positioned[k];
        check(c->Index==k && c->Name==103+k && c->Thing->id==c->Name && c->Value==3*k);
        check(!c->Turning && !c->Moving && c->FaceDown && c->FinalRotation==.5f);
        check(c->Width==8 && c->Height==4 && c->MoveSpeed==13 && c->TurnSpeed==17);
        check(p.X==2 && p.Y==-2 && p.Z==(53-k)*.25f+3);
        check(c->LocalPos.X==p.X && c->LocalPos.Y==p.Y && c->LocalPos.Z==p.Z);
        delete c->Thing;delete c;
    }
}
int main(){int counts[]={0,1,4,55};for(int i=0;i<4;++i)for(int r=0;r<2;++r)for(int e=0;e<2;++e)run(counts[i],r!=0,e!=0);puts("008f1e30_TEST PASS");return 0;}