// 008eebe0 Mass::BackplaneFriction
// Leaf: ?BackplaneFriction@Mass@@QAEXMM@Z
// Flags:
#include <math.h>
// Layout: original PDB C2DVector classV8, X0 Y4; no engine header supplied.
class C2DVector {public:float X,Y;float GetMagnitude()const{return (float)sqrt(X*X+Y*Y);}const C2DVector&operator*=(float scale){X*=scale;Y*=scale;return *this;}};
// Layout: original Mass classV36: vptr0, Pos4, Vel12, IMass20, APos24, AVel28, AIMass32.
class Mass {public:C2DVector Pos,Vel;float IMass,APos,AVel,AIMass;virtual~Mass(){}void BackplaneFriction(float,float);};
void Mass::BackplaneFriction(float friction,float dt){

 float magnitude=Vel.GetMagnitude();
 Vel*=magnitude<=friction?0.0f:1.0f-friction/magnitude;
 float angular=friction/dt;
 float spin=(float)fabs(AVel);
 float scale; if(spin<=angular)scale=0.0f;else scale=1.0f-angular/spin;AVel*=scale;
}

#include <stdio.h>
#define CHECK(condition) do{if(!(condition)){fprintf(stderr,"check failed at line %d\n",__LINE__);return 1;}}while(0)
int main(){
 const float vectors[8][2]={{0,0},{1,0},{0,2},{4,0},{3,4},{-3,4},{0,-8},{-4,0}};
 const float lengths[8]={0,1,2,4,5,5,8,4};
 const float factors[5]={0,0.25f,0.5f,1,2};
 const float times[4]={0.5f,1,2,4};
 const float spins[9]={-8,-4,-2,-1,0,1,2,4,8};
 unsigned int cases=0;
 for(unsigned int v=0;v<8;++v)for(unsigned int f=0;f<5;++f)for(unsigned int d=0;d<4;++d)for(unsigned int a=0;a<9;++a){
  Mass mass;mass.Pos.X=16;mass.Pos.Y=-32;mass.Vel.X=vectors[v][0];mass.Vel.Y=vectors[v][1];mass.IMass=0.25f;mass.APos=-8;mass.AVel=spins[a];mass.AIMass=0.5f;
  const float friction=lengths[v]*factors[f];const float angular=friction/times[d];
  const float linearScale=factors[f]<1?1-factors[f]:0;
  const float wantX=vectors[v][0]*linearScale,wantY=vectors[v][1]*linearScale;
  float wantSpin=0;if(spins[a]>angular)wantSpin=spins[a]-angular;else if(spins[a]<-angular)wantSpin=spins[a]+angular;
  mass.BackplaneFriction(friction,times[d]);
  CHECK(mass.Vel.X==wantX&&mass.Vel.Y==wantY&&mass.AVel==wantSpin);
  CHECK(mass.Pos.X==16&&mass.Pos.Y==-32&&mass.IMass==0.25f&&mass.APos==-8&&mass.AIMass==0.5f);
  ++cases;
 }
 CHECK(cases==1440);puts("008eebe0_TEST PASS");return 0;
}
