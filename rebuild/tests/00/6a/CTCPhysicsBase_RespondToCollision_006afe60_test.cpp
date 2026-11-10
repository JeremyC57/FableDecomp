// 006afe60 exact target body; original vector arithmetic and controlled def access.
#include <stdio.h>
#include <string.h>
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
};
class CPhysicsCollideResponse {
public:
    C3DVector CollidePos;
    C3DVector CollideNormal;
    float Friction;
};
// Fixture supplies the def fields at their service boundary. No unrelated virtual services run.
class CPhysicsDef {
public:
    float Diameter,Friction,Elasticity,AirResistance,Mass;
    long InteractionFlags;
    bool PushableByHero;
};
class CTCPhysicsBase {
public:
    const CPhysicsDef* PhysicsDef;
    float GetFriction()const{return PhysicsDef->Friction;}
    float GetElasticity()const{return PhysicsDef->Elasticity;}
    void RespondToCollision(const CPhysicsCollideResponse& response_info,C3DVector& velocity)const;
};
void CTCPhysicsBase::RespondToCollision(const CPhysicsCollideResponse& response_info,C3DVector& velocity) const
{
    const C3DVector& normal=response_info.CollideNormal;
    float friction=-GetFriction()*response_info.Friction;
    float elastic=GetElasticity();
    float velocity_z=velocity.Z;
    float dot=normal.Y*velocity.Y;
    dot+=normal.Z*velocity_z;
    dot+=normal.X*velocity.X;
    dot*=(-1.0f-friction-elastic);
    velocity+=velocity*friction+normal*dot;
}

static bool Equal(const C3DVector& a,const C3DVector& b){return a.X==b.X && a.Y==b.Y && a.Z==b.Z;}
static C3DVector Expected(const C3DVector& v,const C3DVector& n,float friction,float surface,float elastic){
    double loss=(double)friction*surface;
    double projection=(double)n.X*v.X+(double)n.Y*v.Y+(double)n.Z*v.Z;
    double scale=projection*(loss-1.0-elastic);
    return C3DVector((float)((double)v.X*(1.0-loss)+n.X*scale),(float)((double)v.Y*(1.0-loss)+n.Y*scale),(float)((double)v.Z*(1.0-loss)+n.Z*scale));
}
int main(){
    const C3DVector normals[]={C3DVector(0,0,0),C3DVector(1,0,0),C3DVector(0,1,0),C3DVector(0,0,1),C3DVector(-1,0,0),C3DVector(0.5f,0.5f,0),C3DVector(-0.5f,0.25f,1)};
    const float scalars[]={0.0f,0.25f,0.5f,1.0f};
    CPhysicsDef def;memset(&def,0,sizeof(def));CTCPhysicsBase tc;tc.PhysicsDef=&def;
    for(int n=0;n<7;++n)for(int v=0;v<9;++v)for(int f=0;f<4;++f)for(int e=0;e<4;++e){
        CPhysicsCollideResponse response;response.CollidePos=C3DVector(8,4,2);response.CollideNormal=normals[n];response.Friction=(v&1)?0.5f:1.0f;
        def.Friction=scalars[f];def.Elasticity=scalars[e];CPhysicsDef before=def;
        C3DVector velocity((float)(v-4),(float)((v*3)%7-3),(float)(4-v));
        C3DVector expected=Expected(velocity,response.CollideNormal,def.Friction,response.Friction,def.Elasticity);
        tc.RespondToCollision(response,velocity);
        if(!Equal(velocity,expected)||!Equal(response.CollideNormal,normals[n])||!Equal(response.CollidePos,C3DVector(8,4,2)))return 1;
        if(memcmp(&def,&before,sizeof(def))!=0||tc.PhysicsDef!=&def)return 2;
        // Aliasing a vector inside the response must still use its old components for every term.
        expected=Expected(response.CollideNormal,response.CollideNormal,def.Friction,response.Friction,def.Elasticity);
        tc.RespondToCollision(response,response.CollideNormal);
        if(!Equal(response.CollideNormal,expected))return 3;
    }
    puts("006afe60_TEST PASS");return 0;
}