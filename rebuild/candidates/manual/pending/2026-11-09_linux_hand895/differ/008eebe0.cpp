// Mass::BackplaneFriction(float strength, float radius) -- damp the linear velocity by strength (zero it when slower) and the angular velocity by strength/radius
// Grade: DIFFER(129v131) -- one x87 pop: in the non-clamped angular arm retail drops |AVel| with `fxch st(1) ; fstp st(0)` before the tail-duplicated `fmul [ecx+1Ch]`, ours with the 2-byte `fstp st(1)`; every other instruction matches. 11 normalised diff lines (4 are relocation-masked float constants). 13 probes here + ChatGPT's 61: if/else per arm, shared friction helper, reversed/positive compares, named scale, early return, strength/=radius, reuse of the PDB locals speed/aspeed. next: lane gap (x87 stack fix-up peephole at the arm merge); try only with new evidence (e.g. a sibling Mass:: row that matched with the same fixup).
// Leaf: ?BackplaneFriction@Mass@@QAEXMM@Z
// Retail: Fable.exe @ 0x008EEBE0, 131 bytes (`ret 8`), no calls (sqrt/fabs intrinsic). Ego_r/FableWin: ?BackplaneFriction@Mass@@
//   QAEXMM@Z; DBI params strength/radius and locals speed/aspeed (ChatGPT pass 10's raw DBI read). Constants 0x0122DEDC = 0.0f,
//   0x0122DED8 = 1.0f.
#include <math.h>
// Layout: original PDB C2DVector classV8, X0 Y4; no engine header supplied.
class C2DVector {public:float X,Y;float GetMagnitude()const{return (float)sqrt(X*X+Y*Y);}const C2DVector&operator*=(float scale){X*=scale;Y*=scale;return *this;}};
// Layout: original Mass classV36: vptr0, Pos4, Vel12, IMass20, APos24, AVel28, AIMass32.
class Mass {public:C2DVector Pos,Vel;float IMass,APos,AVel,AIMass;virtual~Mass(){}void BackplaneFriction(float,float);};
void Mass::BackplaneFriction(float strength, float radius)
{
    float speed = Vel.GetMagnitude();
    Vel *= speed <= strength ? 0.0f : 1.0f - strength / speed;
    float angular = strength / radius;
    float aspeed = (float)fabs(AVel);
    float scale;
    if (aspeed <= angular) scale = 0.0f; else scale = 1.0f - angular / aspeed;
    AVel *= scale;
}
