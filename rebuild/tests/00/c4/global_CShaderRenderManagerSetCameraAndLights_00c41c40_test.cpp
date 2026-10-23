// 00c41c40 Row00c41c40::CameraConstants::Apply
// Flags: /O2 /Oy /W3 
// Leaf: Apply
namespace Row00c41c40 {
struct Vec {float x,y,z; Vec(){} Vec(const Vec&v):x(v.x),y(v.y),z(v.z){} };
struct Input { Vec a,b,c; unsigned i,j;float k,l,m,n,o,p;unsigned q,r;};
// Layout: retail vector setters use +158/+164/+170; scalar stores +17c..1a0 and dirty mask +1d8. No engine header exists for this class.
struct CameraConstants {
 char pad[0x158]; Vec a,b,c; float n,l,m,o,p; unsigned i,j; float k; unsigned q,r;char gap[0x34];unsigned dirty;
 void SetA(const Vec&);void SetB(const Vec&);void SetC(const Vec&);
 __forceinline void Set_n(float value){if(n!=value){n=value;dirty|=0x40000;}}
 __forceinline void Set_l(float value){if(l!=value){l=value;dirty|=0x40000;}}
 __forceinline void Set_m(float value){if(m!=value){m=value;dirty|=0x40000;}}
 __forceinline void Set_k(float value){if(k!=value){k=value;dirty|=0x200000;}}
 __forceinline void Set_o(float value){if(o!=value){o=value;dirty|=0x40000;}}
 __forceinline void Set_p(float value){if(p!=value){p=value;dirty|=0x10000;}}
 void Apply(const Input*);
};

#pragma auto_inline(off)
__declspec(noinline) void CameraConstants::SetA(const Vec&v){if(a.x!=v.x||a.y!=v.y||a.z!=v.z){a=v;dirty|=0x8000;}}
__declspec(noinline) void CameraConstants::SetB(const Vec&v){if(b.x!=v.x||b.y!=v.y||b.z!=v.z){b=v;dirty|=0x10000;}}
__declspec(noinline) void CameraConstants::SetC(const Vec&v){if(c.x!=v.x||c.y!=v.y||c.z!=v.z){c=v;dirty|=0x80000;}}
#pragma auto_inline(on)
void CameraConstants::Apply(const Input*v){
 {Vec t=v->a;SetA(t);} {Vec t=v->b;SetB(t);} {Vec t=v->c;SetC(t);}
 Set_n(v->n);
 Set_l(v->l);
 Set_m(v->m);
 if(i!=v->i){i=v->i;dirty|=0x100000;}
 if(j!=v->j){j=v->j;dirty|=0x100000;}
 Set_k(v->k);
 Set_o(v->o);
 Set_p(v->p);
 if(q!=v->q){q=v->q;dirty|=0x400000;}
 if(r!=v->r){r=v->r;dirty|=0x400000;}
}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <float.h>
int main(){using namespace Row00c41c40;
 assert(sizeof(Vec)==12&&sizeof(Input)==76&&offsetof(CameraConstants,a)==0x158&&offsetof(CameraConstants,b)==0x164&&offsetof(CameraConstants,c)==0x170&&offsetof(CameraConstants,dirty)==0x1d8);
 CameraConstants c;Input v;memset(&c,0,sizeof c);memset(&v,0,sizeof v);
 c.Apply(&v);assert(!c.dirty);
 v.a.x=1;v.a.y=2;v.a.z=4;v.b.x=8;v.b.y=16;v.b.z=32;v.c.x=64;v.c.y=128;v.c.z=256;
 v.n=1;v.l=2;v.m=4;v.o=8;v.p=16;v.i=1;v.j=2;v.k=32;v.q=1;v.r=2;
 c.Apply(&v);assert(c.dirty==0x7d8000&&c.n==1&&c.l==2&&c.m==4&&c.o==8&&c.p==16&&c.k==32&&c.i==1&&c.j==2&&c.q==1&&c.r==2);
 assert(!memcmp(&c.a,&v.a,36));c.dirty=0;c.Apply(&v);assert(!c.dirty);
 c.dirty=0x10;v.a.y=-2;c.Apply(&v);assert(c.dirty==0x8010&&c.a.x==1&&c.a.y==-2&&c.a.z==4);
 c.dirty=0;v.b.z=-32;c.Apply(&v);assert(c.dirty==0x10000&&c.b.z==-32);
 c.dirty=0;v.c.x=-64;c.Apply(&v);assert(c.dirty==0x80000&&c.c.x==-64);
 unsigned bits=0x7fc00000;memcpy(&v.a.x,&bits,4);c.dirty=0;c.Apply(&v);assert(c.dirty==0x8000&&_isnan(c.a.x));c.dirty=0;c.Apply(&v);assert(c.dirty==0x8000);
 memset(&c,0,sizeof c);memset(&v,0,sizeof v);bits=0x80000000;memcpy(&v.a.x,&bits,4);memcpy(&v.n,&bits,4);c.Apply(&v);assert(!c.dirty);unsigned observed;memcpy(&observed,&c.n,4);assert(observed==0);

 memset(&c,0,sizeof c);memset(&v,0,sizeof v);bits=0x7fc00000;memcpy(&v.n,&bits,4);c.Apply(&v);assert(c.dirty==0x40000&&_isnan(c.n));c.dirty=0;c.Apply(&v);assert(c.dirty==0x40000);
 memset(&c,0,sizeof c);memset(&v,0,sizeof v);v.k=2;c.Apply(&v);assert(c.dirty==0x200000&&c.k==2);
 c.dirty=0;v.p=4;c.Apply(&v);assert(c.dirty==0x10000&&c.p==4);
 c.dirty=0;v.i=3;c.Apply(&v);assert(c.dirty==0x100000&&c.i==3);c.dirty=0;v.j=7;c.Apply(&v);assert(c.dirty==0x100000&&c.j==7);
 c.dirty=0;v.q=3;c.Apply(&v);assert(c.dirty==0x400000&&c.q==3);c.dirty=0;v.r=7;c.Apply(&v);assert(c.dirty==0x400000&&c.r==7);
 puts("00c41c40_TEST PASS");return 0;}