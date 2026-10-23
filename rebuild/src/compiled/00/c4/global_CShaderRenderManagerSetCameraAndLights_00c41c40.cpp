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