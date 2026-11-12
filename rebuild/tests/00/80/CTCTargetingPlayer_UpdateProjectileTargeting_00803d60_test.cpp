// Behaviour test for CTCTargetingPlayer::UpdateProjectileTargeting (0x00803D60): a copy of the row's logic over a small model of the
// camera, the thing search (Bind5 of IsInFOV, IsNot(parent), !In2DRange(eye, |parent-eye|2D), !ShotFiredByParent, Viable) and the
// line of sight, checking the candidate filter, TargetedPos and the projectile-target update rules.
#include <cstdio>
#include <cmath>
struct V{float X,Y,Z;V(){}V(float x,float y,float z):X(x),Y(y),Z(z){}
 V operator-(const V&o)const{return V(X-o.X,Y-o.Y,Z-o.Z);} V operator+(const V&o)const{return V(X+o.X,Y+o.Y,Z+o.Z);}
 void Norm(){float m=(float)std::sqrt(X*X+Y*Y+Z*Z);X/=m;Y/=m;Z/=m;} void Scale(float s){X*=s;Y*=s;Z*=s;}};
struct Thing{V Pos;bool Dead;bool Targeted;Thing* ShotBy;bool Viable;};
struct World{Thing* things[8];int n;bool los_clear;V hit_pos;Thing* hit;};
static float Mag2D(const V& v){float sq=v.Y*v.Y;sq+=v.X*v.X;return (float)std::sqrt(sq);}
static bool InFOV(const V& eye,const V& dir,float cosfov,const V& p){V d=p-eye;float m=(float)std::sqrt(d.X*d.X+d.Y*d.Y+d.Z*d.Z);
 float dm=(float)std::sqrt(dir.X*dir.X+dir.Y*dir.Y+dir.Z*dir.Z);if(m==0)return true;return (d.X*dir.X+d.Y*dir.Y+d.Z*dir.Z)/(m*dm)>=cosfov;}
struct Player{Thing* parent;float MaxTargetingDistance;V TargetedPos;Thing* PProjectileTargetedThing;
 int Update(World& w,const V& eye,const V& look_at,float cosfov){
  V dir=look_at-eye;dir.Norm();dir.Scale(MaxTargetingDistance);
  V offset=parent->Pos-eye;float range=Mag2D(offset);int count=0;
  for(int i=0;i<w.n;++i){Thing* t=w.things[i];
   if(InFOV(eye,dir,cosfov,t->Pos)&&t!=parent&&!(Mag2D(t->Pos-eye)<=range)&&!(t->ShotBy==parent)&&t->Viable)++count;}
  TargetedPos=dir+eye;
  if(!w.los_clear){ if(InFOV(eye,dir,cosfov,w.hit_pos)){
    Thing* h=w.hit; if(h&&!h->Dead&&PProjectileTargetedThing!=h&&h->Targeted)PProjectileTargetedThing=h;
    TargetedPos=w.hit_pos;}}
  return count;}};
int main(){
 Thing parent={V(0,5,0),false,true,0,true};
 Thing a={V(0,20,0),false,true,0,true};      // ahead, beyond the parent: candidate
 Thing b={V(0,2,0),false,true,0,true};       // nearer the camera than the parent: rejected
 Thing c={V(0,30,0),false,true,&parent,true};// the parent's own shot: rejected
 Thing d={V(0,-20,0),false,true,0,true};     // behind the camera: rejected
 Thing e={V(0,25,0),false,true,0,false};     // not viable: rejected
 World w={{&parent,&a,&b,&c,&d,&e},6,true,V(0,0,0),0};
 Player p={&parent,50.0f,V(0,0,0),0};
 V eye(0,0,0),look(0,10,0);
 int n=p.Update(w,eye,look,0.5f);
 if(n!=1||std::fabs(p.TargetedPos.Y-50.0f)>1e-3f||p.PProjectileTargetedThing!=0){std::printf("00803d60_TEST FAIL\n");return 1;}
 w.los_clear=false;w.hit_pos=V(0,20,0);w.hit=&a;
 p.Update(w,eye,look,0.5f);
 if(p.PProjectileTargetedThing!=&a||std::fabs(p.TargetedPos.Y-20.0f)>1e-3f){std::printf("00803d60_TEST FAIL\n");return 1;}
 a.Dead=true;p.PProjectileTargetedThing=0;p.Update(w,eye,look,0.5f);
 if(p.PProjectileTargetedThing!=0||std::fabs(p.TargetedPos.Y-20.0f)>1e-3f){std::printf("00803d60_TEST FAIL\n");return 1;}
 w.hit_pos=V(0,-10,0);a.Dead=false;p.Update(w,eye,look,0.5f); // blocked outside the FOV: aim at the ray end, no target
 if(p.PProjectileTargetedThing!=0||std::fabs(p.TargetedPos.Y-50.0f)>1e-3f){std::printf("00803d60_TEST FAIL\n");return 1;}
 std::printf("00803d60_TEST PASS\n");return 0;}