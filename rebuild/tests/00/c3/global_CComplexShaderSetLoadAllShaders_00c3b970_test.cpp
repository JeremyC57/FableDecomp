// 00c3b970 CComplexShaderSet_LoadAllShaders
// Flags: none beyond /O2 /Oy /W3
// Leaf: LoadAllShaders
namespace X_00c3b970 {
struct CString{char*Text;CString();~CString();};
// Layout: PKSerialiseCParser.cpp receiver is opaque; helpers transfer in either direction. Transfer helpers are descriptive placeholders identified by byte width and their distinct retail addresses c31b50/60/70/80/90.
struct CArchive{bool TransferUInt(unsigned long&);bool TransferInt(long&);bool TransferShort(unsigned short&);bool TransferFloat(float&);bool TransferString(CString&);};
// Layout: target receiver has fields at0/4/8/c/e/10/14/18/1a/1c/1e/20/22/24/28/2c. Manifest class/method is unverified; no matching active header exists.
struct CComplexShaderSet{unsigned long Field00,Field04;CString Field08;unsigned short Field0c,Field0e;unsigned long Field10;long Field14;unsigned short Field18,Field1a,Field1c,Field1e,Field20,Field22;float Field24,Field28;long Field2c;int LoadAllShaders(CArchive&);};
int CComplexShaderSet::LoadAllShaders(CArchive&r){
 if(r.TransferUInt(Field00))
 if(r.TransferUInt(Field04))
 if(r.TransferString(Field08))
 if(r.TransferShort(Field0c))
 if(r.TransferShort(Field0e))
 if(r.TransferUInt(Field10))
 if(r.TransferInt(Field14))
 if(r.TransferShort(Field18))
 if(r.TransferShort(Field1a))
 if(r.TransferShort(Field1c))
 if(r.TransferShort(Field1e))
 if(r.TransferShort(Field22))
 if(r.TransferShort(Field20))
 if(r.TransferFloat(Field24))
 if(r.TransferFloat(Field28))
 if(r.TransferInt(Field2c))
 return 1;
 return 0;
}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
namespace X_00c3b970 {
static bool writing;static unsigned char bytes[128];static unsigned size,offset,calls;static unsigned limits[16];
static bool Read(void*p,unsigned n){++calls;if(offset+n>size)return false;if(writing)memcpy(bytes+offset,p,n);else memcpy(p,bytes+offset,n);offset+=n;return true;}
CString::CString():Text(0){}CString::~CString(){free(Text);}
bool CArchive::TransferUInt(unsigned long&v){return Read(&v,4);}bool CArchive::TransferInt(long&v){return Read(&v,4);}bool CArchive::TransferShort(unsigned short&v){return Read(&v,2);}bool CArchive::TransferFloat(float&v){return Read(&v,4);}
bool CArchive::TransferString(CString&v){unsigned oldCalls=calls;unsigned long n;if(writing){n=v.Text?(unsigned long)strlen(v.Text):0;if(!Read(&n,4))return false;if(offset+n>size)return false;memcpy(bytes+offset,v.Text,n);offset+=n;calls=oldCalls+1;return true;}if(!Read(&n,4))return false;if(offset+n>size)return false;char*p=(char*)malloc(n+1);memcpy(p,bytes+offset,n);p[n]=0;offset+=n;free(v.Text);v.Text=p;calls=oldCalls+1;return true;}
static void Append(const void*p,unsigned n){memcpy(bytes+size,p,n);size+=n;}
}
int main(){using namespace X_00c3b970;unsigned long a=11,b=22,n=3,c=33;long d=-7,e=29;unsigned short shorts[]={2,4,6,8,10,12,14,16};float f=0.5f,g=2.0f;size=0;
limits[0]=size;Append(&a,4);limits[1]=size;Append(&b,4);limits[2]=size;Append(&n,4);Append("abc",3);limits[3]=size;Append(&shorts[0],2);limits[4]=size;Append(&shorts[1],2);limits[5]=size;Append(&c,4);limits[6]=size;Append(&d,4);
for(unsigned k=0;k<6;++k){limits[k+7]=size;Append(&shorts[k+2],2);}limits[13]=size;Append(&f,4);limits[14]=size;Append(&g,4);limits[15]=size;Append(&e,4);unsigned full=size;
CArchive reader;for(unsigned fail=0;fail<16;++fail){CComplexShaderSet data;size=limits[fail];offset=0;calls=0;assert(data.LoadAllShaders(reader)==0&&calls==fail+1);}
CComplexShaderSet data;size=full;offset=0;calls=0;assert(data.LoadAllShaders(reader)==1&&calls==16&&offset==full);assert(data.Field00==11&&data.Field04==22&&strcmp(data.Field08.Text,"abc")==0&&data.Field14==-7&&data.Field22==14&&data.Field20==16&&data.Field24==0.5f&&data.Field28==2.0f&&data.Field2c==29);unsigned char expected[128];memcpy(expected,bytes,full);writing=true;memset(bytes,0,sizeof(bytes));size=full;offset=0;calls=0;assert(data.LoadAllShaders(reader)==1&&calls==16&&offset==full&&memcmp(bytes,expected,full)==0);for(unsigned fail=0;fail<16;++fail){size=limits[fail];offset=0;calls=0;assert(data.LoadAllShaders(reader)==0&&calls==fail+1);}puts("00c3b970_TEST PASS");return 0;}