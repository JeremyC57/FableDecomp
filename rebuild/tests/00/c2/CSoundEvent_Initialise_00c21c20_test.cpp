// 00c21c20 Row00c21c20::Object::Initialize (catalog CRebootGameActionHandler_Initialize; that class is in no PDB, so the owner name is kept)
// Flags: /O2 /Oy /W3 
// Leaf: Initialize
namespace Row00c21c20 {
struct Object;
struct Observer{virtual void A(){}virtual void B(){}virtual void C(){}virtual void D(){}virtual void E(){}virtual void F(){}virtual void G(){}virtual void H(){}virtual void I(){}virtual void Add(Object*);};
struct Owner{char pad[0x104];Observer a;char gap[0xbc];Observer b;char gap2[0x2c];Observer c;};
struct Blend{char data[16];void Set(unsigned,unsigned,unsigned,unsigned);};struct Transform{char data[0x70];void Set(const void*,unsigned);};
// Layout: exact oracle writes +4,+8,+c,+14,+18,+28,+2c,+38,+a8,+ac,+ae,+b0.
struct Object{unsigned vtable,x,y,z,unused,w;Blend blend;unsigned value;unsigned char flag;char gap[11];Transform transform;unsigned index;unsigned short state,flags;unsigned char active;
 void Clear(int);void Bind(Owner*);int Check(Owner*);void Yes(Owner*);void No(Owner*);
 void Initialize(Owner*,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,const void*,const unsigned*,unsigned,bool,bool,bool,unsigned char);
};
void Object::Initialize(Owner*owner,unsigned a,unsigned b,unsigned c,unsigned d,unsigned e,unsigned f,unsigned g,unsigned h,const void*matrix,const unsigned*pair,unsigned extra,bool xflag,bool yflag,bool zflag,unsigned char byte){
 Clear(0);flags=0;if(xflag)flags=1;if(yflag)*(unsigned char*)&flags|=2;if(zflag)*(unsigned char*)&flags|=4;
 z=a;w=c;blend.Set(d,e,f,g);value=h;flag=byte;index=0;x=pair[0];y=pair[1];transform.Set(matrix,extra);state=1;owner->a.Add(this);owner->b.Add(this);owner->c.Add(this);active=1;Bind(owner);if(Check(owner)){Yes(owner);return;}No(owner);
}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace Row00c21c20 {static int added,yes,no;static bool check;void Observer::Add(Object*){++added;}void Blend::Set(unsigned a,unsigned b,unsigned c,unsigned d){((unsigned*)data)[0]=a;((unsigned*)data)[1]=b;((unsigned*)data)[2]=c;((unsigned*)data)[3]=d;}void Transform::Set(const void*p,unsigned n){((unsigned*)data)[0]=*(const unsigned*)p;((unsigned*)data)[1]=n;}void Object::Clear(int){active=0;}void Object::Bind(Owner*){}int Object::Check(Owner*){return check;}void Object::Yes(Owner*){++yes;}void Object::No(Owner*){++no;}}
int main(){using namespace Row00c21c20;Object o;Owner owner;memset(&o,0,sizeof(o));unsigned pair[2]={1,2},m=9;o.Initialize(&owner,3,4,5,6,7,8,9,10,&m,pair,11,true,false,true,12);assert(o.flags==5&&o.x==1&&o.y==2&&o.z==3&&o.w==5&&o.value==10&&o.active&&added==3&&no==1);check=true;o.Initialize(&owner,0,0,0,0,0,0,0,0,&m,pair,0,false,true,false,0);assert(o.flags==2&&yes==1&&added==6);puts("00c21c20_TEST PASS");}
