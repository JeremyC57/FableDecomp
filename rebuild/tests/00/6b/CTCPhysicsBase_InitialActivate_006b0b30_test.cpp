// 006b0b30 exact target/member-helper bodies; controlled thing and map services.
#include <stdio.h>
class C2DVector{public:float X,Y;C2DVector(){}C2DVector(float x,float y):X(x),Y(y){}C2DVector(const C2DVector& v):X(v.X),Y(v.Y){}};
class C3DVector{
public:
    float X,Y,Z;
    C3DVector(){}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}
    C3DVector operator+(const C2DVector& v)const{float x=v.X;return C3DVector(X+x,Y+v.Y,Z);}
};
static int events[16],event_count;
static void Record(int id){events[event_count++]=id;}
class CTCMapwho {
public:
    bool accept;
    mutable int queries;
    int placements;
    mutable C3DVector checked;
    C3DVector placed;
    bool IsToBePlacedInMapwho(const C3DVector& p)const{Record(4);++queries;checked=p;return accept;}
    void PlaceInMapwho(const C3DVector& p){Record(5);++placements;placed=p;}
};
class CThing{
public:
    bool eligible;
    CTCMapwho* map;
    bool IsToBePlacedInMap()const{Record(1);return eligible;}
    bool GetTC(CTCMapwho** out){Record(2);*out=map;return map!=0;}
};
class CTCPhysicsBase{
public:
    CThing* ParentThing;
    C3DVector Position;
    mutable int offset_calls;
    C2DVector offsets[3];
    CThing& GetParentThing(){return *ParentThing;}
    C2DVector GetOffsetVector()const{Record(3);return offsets[offset_calls++];}
    C3DVector GetMapwhoPosition()const{C3DVector pos=Position+GetOffsetVector();return pos;}
    void InitialActivate();
};
void CTCPhysicsBase::InitialActivate(){
    if(GetParentThing().IsToBePlacedInMap()){
        CTCMapwho* pmapwho=0;
        GetParentThing().GetTC(&pmapwho);
        C3DVector mapwho_pos=GetMapwhoPosition();
        if(pmapwho->IsToBePlacedInMapwho(GetMapwhoPosition()))
            pmapwho->PlaceInMapwho(GetMapwhoPosition());
    }
}

static bool Equal(const C3DVector& a,const C3DVector& b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
int main(){
    for(int point=0;point<17;++point)for(int eligible=0;eligible<2;++eligible)for(int accept=0;accept<2;++accept){
        CTCMapwho map;map.accept=accept!=0;map.queries=map.placements=0;
        CThing thing;thing.eligible=eligible!=0;thing.map=&map;
        CTCPhysicsBase physics;physics.ParentThing=&thing;physics.Position=C3DVector((float)point-8,(float)(8-point),0.5f*point);physics.offset_calls=0;
        physics.offsets[0]=C2DVector(100,200);physics.offsets[1]=C2DVector(1.5f,-2.0f);physics.offsets[2]=C2DVector(-4.0f,3.5f);
        C3DVector original=physics.Position;event_count=0;
        physics.InitialActivate();
        if(!Equal(original,physics.Position))return 1;
        if(!eligible){if(event_count!=1||events[0]!=1||physics.offset_calls||map.queries||map.placements)return 2;}
        else{
            const int expected[]={1,2,3,3,4,3,5};
            const int count=accept?7:5;
            if(event_count!=count||physics.offset_calls!=(accept?3:2)||map.queries!=1||map.placements!=accept)return 3;
            for(int i=0;i<count;++i)if(events[i]!=expected[i])return 4;
            if(!Equal(map.checked,C3DVector(original.X+1.5f,original.Y-2.0f,original.Z)))return 5;
            if(accept&&!Equal(map.placed,C3DVector(original.X-4.0f,original.Y+3.5f,original.Z)))return 6;
        }
    }
    puts("006b0b30_TEST PASS");return 0;
}