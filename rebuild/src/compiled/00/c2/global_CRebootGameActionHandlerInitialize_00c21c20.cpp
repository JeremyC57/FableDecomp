// 00c21c20 Row00c21c20::Object::Init
// Flags: /O2 /Oy /W3 
// Leaf: Init
namespace Row00c21c20 {
struct Object;
struct Observer{virtual void A(){}virtual void B(){}virtual void C(){}virtual void D(){}virtual void E(){}virtual void F(){}virtual void G(){}virtual void H(){}virtual void I(){}virtual void Add(Object*);};
struct Owner{char pad[0x104];Observer a;char gap[0xbc];Observer b;char gap2[0x2c];Observer c;};
struct Blend{char data[16];void Set(unsigned,unsigned,unsigned,unsigned);};struct Transform{char data[0x70];void Set(const void*,unsigned);};
// Layout: exact oracle writes +4,+8,+c,+14,+18,+28,+2c,+38,+a8,+ac,+ae,+b0.
struct Object{unsigned vtable,x,y,z,unused,w;Blend blend;unsigned value;unsigned char flag;char gap[11];Transform transform;unsigned index;unsigned short state,flags;unsigned char active;
 void Clear(int);void Bind(Owner*);int Check(Owner*);void Yes(Owner*);void No(Owner*);
 void Init(Owner*,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,const void*,const unsigned*,unsigned,bool,bool,bool,unsigned char);
};
void Object::Init(Owner*owner,unsigned a,unsigned b,unsigned c,unsigned d,unsigned e,unsigned f,unsigned g,unsigned h,const void*matrix,const unsigned*pair,unsigned extra,bool xflag,bool yflag,bool zflag,unsigned char byte){
 Clear(0);flags=0;if(xflag)flags=1;if(yflag)*(unsigned char*)&flags|=2;if(zflag)*(unsigned char*)&flags|=4;
 z=a;w=c;blend.Set(d,e,f,g);value=h;flag=byte;index=0;x=pair[0];y=pair[1];transform.Set(matrix,extra);state=1;owner->a.Add(this);owner->b.Add(this);owner->c.Add(this);active=1;Bind(owner);if(Check(owner)){Yes(owner);return;}No(owner);
}
}