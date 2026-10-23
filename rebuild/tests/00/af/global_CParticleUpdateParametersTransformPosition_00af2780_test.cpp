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

#include <assert.h>
#include <stdio.h>
#include <string.h>
namespace NPosition00af2780 {
static bool valid=true,complexMatrix=false,fallbackValid=false;static const unsigned*expectedKey;static int getCalls,matrixCalls,fallbackCalls;bool Request::Get(V& v,const unsigned&a,int b,int c,int d){++getCalls;assert(&a==expectedKey&&a==1&&b==2&&c==3&&d==4);if(valid)v=V(1,2,3);return valid;}
void Owner::GetMatrix(Matrix& m){++matrixCalls;memset(&m,0,sizeof(m));if(complexMatrix){for(int i=0;i<12;++i)m.m[i]=(float)(i+1);}else{m.m[0]=m.m[4]=m.m[8]=2;m.m[9]=4;}}
struct TestParameters:Parameters {void v0(){}void v1(){}void v2(){}void v3(){}void v4(){}void v5(){}void v6(){}void v7(){}void v8(){}void v9(){}void v10(){}void v11(){}void v12(){}void v13(){}void v14(){}void v15(){}void v16(){}void v17(){}void v18(){}void v19(){}void v20(){}void v21(){}void v22(){}void v23(){}void v24(){}void v25(){}void v26(){}void v27(){}void v28(){}void v29(){}bool Fallback(V& v){++fallbackCalls;if(fallbackValid)v=V(2,4,6);return fallbackValid;}};
}
int main(){using namespace NPosition00af2780;TestParameters t;Request r;Owner o;memset(&o,0,sizeof(o));t.owner=0;
unsigned key=1;expectedKey=&key;V p=t.TransformPosition(&r,key,2,3,4);assert(p.x==1&&p.y==2&&p.z==3&&getCalls==1&&fallbackCalls==0&&matrixCalls==0);
t.owner=&o;p=t.TransformPosition(&r,key,2,3,4);assert(p.x==6&&p.y==4&&p.z==6&&getCalls==2&&fallbackCalls==0&&matrixCalls==1);
complexMatrix=true;p=t.TransformPosition(&r,key,2,3,4);assert(p.x==40&&p.y==47&&p.z==54&&matrixCalls==2);
o.flags=64;p=t.TransformPosition(&r,key,2,3,4);assert(p.x==1&&p.y==2&&p.z==3&&matrixCalls==2&&fallbackCalls==0);
valid=false;p=t.TransformPosition(&r,key,2,3,4);assert(p.x==0&&p.y==0&&p.z==0&&fallbackCalls==1&&matrixCalls==2);
fallbackValid=true;p=t.TransformPosition(&r,key,2,3,4);assert(p.x==2&&p.y==4&&p.z==6&&fallbackCalls==2&&matrixCalls==2);
o.flags=0;p=t.TransformPosition(&r,key,2,3,4);assert(p.x==70&&p.y==83&&p.z==96&&fallbackCalls==3&&matrixCalls==3&&getCalls==7);
puts("00af2780_TEST PASS");return 0;}