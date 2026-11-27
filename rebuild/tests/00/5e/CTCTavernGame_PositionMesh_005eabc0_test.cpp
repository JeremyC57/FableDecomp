#include <stdio.h>
#include <stdlib.h>
static void expect(bool x){if(!x)exit(2);}
static int calls[20],n;
static void record(int x){expect(n<20);calls[n++]=x;}
struct C3DVector {
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator*(float s) const{return C3DVector(X*s,Y*s,Z*s);}
 void operator+=(const C3DVector& b){X+=b.X;Y+=b.Y;Z+=b.Z;}
};
static bool same(const C3DVector&a,const C3DVector&b){return a.X==b.X && a.Y==b.Y && a.Z==b.Z;}
struct CRightHandedSet {
 C3DVector up,forward;
 C3DVector GetUp()const{return up;} C3DVector GetForward()const{return forward;}
 C3DVector GetRight()const{return C3DVector(forward.Y*up.Z-forward.Z*up.Y,forward.Z*up.X-forward.X*up.Z,forward.X*up.Y-forward.Y*up.X);}
 void Rotate(const C3DVector& a,float f);
};
static CRightHandedSet input,placed;static C3DVector result;
static void rotateFirst(CRightHandedSet& r,float f){r.forward.X+=f;r.forward.Z-=f;}
static void rotateSecond(CRightHandedSet& r,float f){r.up.Y-=f;r.up.Z+=f;}
void CRightHandedSet::Rotate(const C3DVector& a,float f){
 if(n==4){expect(same(a,input.up)&&f==5);rotateFirst(*this,f);record(5);}
 else{expect(n==5&&same(a,forward)&&f==-3);rotateSecond(*this,f);record(6);}
}
struct CTCPhysicsBase {
 void SetAsMoving(bool b){expect(b);record(3);}
 void ResetPosition(const C3DVector& p){result=p;record(40);}
 void SetPosition(const C3DVector& p){result=p;record(41);}
 void ResetRHSet(const CRightHandedSet&r){placed=r;record(70);}
 void SetRHSet(const CRightHandedSet&r){placed=r;record(71);}
};
static CTCPhysicsBase physicsObject;
struct CThing {
 bool drawable;
 bool DrawIsDrawable(){record(1);return drawable;}
 void GetTC(CTCPhysicsBase** p){record(2);*p=&physicsObject;}
 void SetAsFrameUpdated(){record(8);}
 void SetAsRenderedThisFrame(unsigned long a){expect(a==4);record(9);}
 void SetAsDrawable(bool b){expect(b);drawable=b;record(10);}
};
struct CTCTavernGame {static void PositionMesh(CThing*,const C3DVector&,const CRightHandedSet&,const C3DVector&,float,float);};
void CTCTavernGame::PositionMesh(CThing* thing, const C3DVector& world_pos, const CRightHandedSet& world_rhset, const C3DVector& local_pos,
                                 float local_angleXZ, float local_angleXY)
{
    if (thing) {
        bool reset = !thing->DrawIsDrawable();
        CRightHandedSet rhset = world_rhset;
        CTCPhysicsBase* physics;
        thing->GetTC(&physics);
        physics->SetAsMoving(true);
        C3DVector pos(0.0f, 0.0f, 0.0f);
        pos += rhset.GetRight() * local_pos.X;
        pos += rhset.GetForward() * local_pos.Y;
        pos += rhset.GetUp() * local_pos.Z;
        pos += world_pos;
        if (reset)
            physics->ResetPosition(pos);
        else
            physics->SetPosition(pos);
        rhset.Rotate(rhset.GetUp(), local_angleXY);
        rhset.Rotate(rhset.GetForward(), local_angleXZ);
        if (reset)
            physics->ResetRHSet(rhset);
        else
            physics->SetRHSet(rhset);
        thing->SetAsFrameUpdated();
        thing->SetAsRenderedThisFrame(4);
        thing->SetAsDrawable(true);
    }
}
int main(){
 CThing t;
 for(int d=0;d<2;++d)for(int sign=-1;sign<=1;sign+=2)
 for(int x=-2;x<=2;++x)for(int y=-2;y<=2;++y)for(int z=-2;z<=2;++z){
  n=0;t.drawable=d!=0;
  C3DVector world(2,-5,7),local((float)x,(float)y,(float)z);
  input.forward=C3DVector(0,(float)sign,0);input.up=C3DVector(0,0,1);
  CTCTavernGame::PositionMesh(0,world,input,local,-3,5);expect(n==0);
  CTCTavernGame::PositionMesh(&t,world,input,local,-3,5);
  const int expected[]={1,2,3,40+d,5,6,70+d,8,9,10};
  expect(n==10&&t.drawable);
  for(int i=0;i<n;++i)expect(calls[i]==expected[i]);
  expect(same(result,C3DVector((float)(2+sign*x),(float)(-5+sign*y),(float)(7+z))));
  expect(same(input.forward,C3DVector(0,(float)sign,0))&&same(input.up,C3DVector(0,0,1)));
  expect(same(placed.forward,C3DVector(5,(float)sign,-5))&&same(placed.up,C3DVector(0,3,-2)));
 }
 puts("005eabc0_TEST PASS");return 0;
}