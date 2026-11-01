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
