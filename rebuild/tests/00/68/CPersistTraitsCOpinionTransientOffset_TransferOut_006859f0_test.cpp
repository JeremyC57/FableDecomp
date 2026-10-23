#include <stdio.h>
#include <assert.h>
#include <string.h>
// BEGIN exact source snapshot SHA256 050868420acbd83c392aed6d71d7510c0a4043df6b381d0a4335c05fe339c537
// 006859f0 CPersistTraits<COpinionTransientOffset>::TransferOut
// Flags: /O2 /Oy /Gr
// Leaf: TransferOut
// Layout: retail opinion record24 has int+0,float+4,int+8,int+12,float+16,int+20; punctuation literals at0122e020/24/28.
struct CCharString {void* data;CCharString():data(0){} CCharString(const CCharString&);~CCharString();const CCharString& operator+=(const char*);const CCharString& operator+=(const CCharString&);};
struct CPersistContext;
void __fastcall AppendIntegerText_009e2560(CPersistContext*,CCharString&,const int&);
CCharString __fastcall FormatRealText_009a08b0(int,double);
struct COpinionTransientOffset{int a;float b;int c,d;float e;int f;};
template<class T>struct CPersistTraits{void TransferOut(CPersistContext*,CCharString&,const T&);};
template<class T>void CPersistTraits<T>::TransferOut(CPersistContext* context,CCharString& text,const T& value){
 text.operator+=("(");{int number=value.a;AppendIntegerText_009e2560(context,text,number);}
 text.operator+=(",");text.operator+=(FormatRealText_009a08b0(6,value.b));
 text.operator+=(",");{int number=value.c;AppendIntegerText_009e2560(context,text,number);}
 text.operator+=(",");{int number=value.d;AppendIntegerText_009e2560(context,text,number);}
 text.operator+=(",");text.operator+=(FormatRealText_009a08b0(6,value.e));
 text.operator+=(",");{int number=value.f;AppendIntegerText_009e2560(context,text,number);}text.operator+=(")");
}
template void CPersistTraits<COpinionTransientOffset>::TransferOut(CPersistContext*,CCharString&,const COpinionTransientOffset&);

// END exact source snapshot
static char output[1024];static int used;
CCharString::CCharString(const CCharString& o):data(o.data){}CCharString::~CCharString(){}
const CCharString& CCharString::operator+=(const char* p){strcpy(output+used,p);used+=(int)strlen(p);return *this;}
const CCharString& CCharString::operator+=(const CCharString& p){return operator+=((const char*)p.data);}
void __fastcall AppendIntegerText_009e2560(CPersistContext*,CCharString& s,const int& value){char text[40];sprintf(text,"%d",value);s.operator+=(text);}
CCharString __fastcall FormatRealText_009a08b0(int digits,double value){static char buffer[40];sprintf(buffer,"%.*f",digits,value);CCharString text;text.data=buffer;return text;}
int main(){CPersistTraits<COpinionTransientOffset> traits;CCharString text;COpinionTransientOffset value;value.a=-1;value.b=0.5f;value.c=0;value.d=2;value.e=-2.0f;value.f=100;used=0;traits.TransferOut(0,text,value);assert(strcmp(output,"(-1,0.500000,0,2,-2.000000,100)")==0);value.a=0;value.b=0;value.c=0;value.d=0;value.e=0;value.f=0;used=0;traits.TransferOut(0,text,value);assert(strcmp(output,"(0,0.000000,0,0,0.000000,0)")==0);puts("006859f0_TEST PASS");return 0;}