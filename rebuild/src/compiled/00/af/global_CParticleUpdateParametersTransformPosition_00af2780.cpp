// 00af2780 NPosition00af2780::Parameters::TransformPosition
// Flags: /O2 /Oy /W3
// Leaf: TransformPosition
namespace NPosition00af2780 {
struct V {float x,y,z;V(){} __forceinline V(const V& v){z=v.z;x=v.x;y=v.y;}__forceinline V(float a,float b,float c){x=a;y=b;z=c;}};
struct Matrix {float m[12];};
// ABI: 0x00af3080 dereferences second stack argument as a key; 0x00af2c50 receives its address.
struct Request {bool Get(V&,const unsigned&,int,int,int);};
// Layout: retail owner +4, owner flags +0x50, fallback virtual slot30.
struct Owner {char pad[80];unsigned char flags;void GetMatrix(Matrix&);__forceinline V Transform(const V& p){V q=p;if(!(flags&64)){Matrix m;GetMatrix(m);const Matrix& mat=m;float x=q.x,y=q.y;q.x=mat.m[6]*q.z+mat.m[3]*q.y+mat.m[0]*q.x+mat.m[9];q.y=mat.m[7]*q.z+mat.m[4]*q.y+mat.m[1]*x+mat.m[10];q.z=mat.m[8]*q.z+mat.m[5]*y+mat.m[2]*x+mat.m[11];}return V(q.x,q.y,q.z);}};
struct Parameters {
virtual void v0()=0;
virtual void v1()=0;
virtual void v2()=0;
virtual void v3()=0;
virtual void v4()=0;
virtual void v5()=0;
virtual void v6()=0;
virtual void v7()=0;
virtual void v8()=0;
virtual void v9()=0;
virtual void v10()=0;
virtual void v11()=0;
virtual void v12()=0;
virtual void v13()=0;
virtual void v14()=0;
virtual void v15()=0;
virtual void v16()=0;
virtual void v17()=0;
virtual void v18()=0;
virtual void v19()=0;
virtual void v20()=0;
virtual void v21()=0;
virtual void v22()=0;
virtual void v23()=0;
virtual void v24()=0;
virtual void v25()=0;
virtual void v26()=0;
virtual void v27()=0;
virtual void v28()=0;
virtual void v29()=0;
virtual bool Fallback(V&)=0;Owner* owner;V TransformPosition(Request*,const unsigned&,int,int,int);
};
V Parameters::TransformPosition(Request* r,const unsigned&a,int b,int c,int d){V p;if(!r->Get(p,a,b,c,d)&&!Fallback(p)){p.z=0;p.y=0;p.x=0;}if(owner)p=owner->Transform(p);return V(p.x,p.y,p.z);}
}