// 009c7a60 NGameText::ConstructStringInternal
// Flags: /O2 /Oy /W3 /GS
// Leaf: ConstructStringInternal
#include <stdarg.h>
#include <stdlib.h>
#include <wchar.h>
namespace NGameText {
// Layout: wide strings one dword; integer conversion writes 64-byte stack buffer.
// Shape: index is initialised before the va_list and the first-argument test is the while condition itself (no
//   enclosing if); va_arg is read inside the body before ++index, while key is still alive. The default CWideString
//   constructor is only declared here (retail calls 0x0099AED0 in another TU; an in-TU body lets VC reuse ecx).
struct CWideStringData{unsigned short*text;unsigned length,capacity;int refs;};
extern int g_CWideStringInstanceCount_013BCA20;
struct CWideString {CWideStringData* data;CWideString();CWideString(const CWideString&);CWideString(const char*);CWideString(const unsigned short*);CWideString&operator+=(const CWideString&);~CWideString();void UnassignString();void __cdecl Format(const unsigned short*,...);bool Contains(const CWideString&)const;void Replace(const CWideString&,const CWideString&);};
__declspec(noinline) CWideString::CWideString(const CWideString&other):data(0){++g_CWideStringInstanceCount_013BCA20;if(&other!=this&&other.data!=data){if(data)UnassignString();if(other.data){data=other.data;++data->refs;}}}
__declspec(noinline) CWideString::~CWideString(){CWideStringData*storage=data;if(storage){--storage->refs;if(storage->refs<=0){storage=data;if(storage){if(storage->text)free(storage->text);delete storage;}}data=0;}--g_CWideStringInstanceCount_013BCA20;}
extern const unsigned short Empty[]; // retail129a8e0 UTF16 empty string
__declspec(noinline) bool CWideString::Contains(const CWideString&rhs)const{const unsigned short*needle=rhs.data?rhs.data->text:Empty;const unsigned short*haystack=data?data->text:Empty;return wcsstr((const wchar_t*)haystack,(const wchar_t*)needle)!=0;}
__declspec(noinline) CWideString __fastcall operator+(const unsigned short*left,const CWideString&right){CWideString result(left);result+=right;return result;}
void __fastcall FormatNumber_AddThousandsSeparators(int,unsigned short*);
extern const unsigned short FormatNumber[],Prefix[];
__forceinline CWideString FormatIndex(int index){CWideString n;n.Format(FormatNumber,index);return n;}
bool __cdecl ConstructStringInternal(CWideString&output,int first,...){bool result=true;int index=1;va_list args;va_start(args,first);int number=first;while(number!=-1){CWideString key=Prefix+FormatIndex(index);if(!output.Contains(key)){result=false;break;}unsigned short buffer[32];FormatNumber_AddThousandsSeparators(number,buffer);output.Replace(key,CWideString(buffer));number=va_arg(args,int);++index;}va_end(args);return result;}

}


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace NGameText {
// Fixture models owned UTF-16 strings and the ASCII numeric formatting domain used here.
#pragma pack(push,1)
typedef CWideStringData Data;
#pragma pack(pop)
CWideString::CWideString():data(0){++g_CWideStringInstanceCount_013BCA20;}
static int replacements;int g_CWideStringInstanceCount_013BCA20;
static unsigned Length(const unsigned short*p){unsigned n=0;while(p[n])++n;return n;}
static void Release(void*v){Data*d=(Data*)v;if(d&&--d->refs<=0){free(d->text);delete d;}}
static void Set(CWideString&w,const unsigned short*p){unsigned n=Length(p);Data*d=new Data;d->text=(unsigned short*)malloc((n+1)*2);assert(d->text);memcpy(d->text,p,(n+1)*2);d->length=n;d->capacity=n+1;d->refs=1;Release(w.data);w.data=d;}
const unsigned short Empty[]={0};
static const unsigned short* Text(const CWideString&w){return w.data?((Data*)w.data)->text:Empty;}
static void SetAscii(CWideString&w,const char*p){unsigned short buffer[512];unsigned n=0;for(;p[n];++n){assert(n<511&&(unsigned char)p[n]<128);buffer[n]=(unsigned char)p[n];}buffer[n]=0;Set(w,buffer);}
static bool Equal(const CWideString&w,const char*p){const unsigned short*t=Text(w);unsigned n=0;for(;p[n];++n)if(t[n]!=(unsigned char)p[n])return false;return t[n]==0;}
static bool Starts(const unsigned short*a,const unsigned short*b){for(;*b;++a,++b)if(*a!=*b)return false;return true;}
void CWideString::UnassignString(){Release(data);data=0;}CWideString::CWideString(const char*p):data(0){++g_CWideStringInstanceCount_013BCA20;SetAscii(*this,p);}

void CWideString::Replace(const CWideString&from,const CWideString&to){++replacements;const unsigned short*a=Text(*this),*f=Text(from),*t=Text(to);unsigned short buffer[512];unsigned n=0,k=Length(f);assert(k);while(*a){if(Starts(a,f)){for(unsigned j=0;t[j];++j){assert(n<511);buffer[n++]=t[j];}a+=k;}else{assert(n<511);buffer[n++]=*a++;}}buffer[n]=0;Set(*this,buffer);}
const unsigned short FormatNumber[]={37,100,0},Prefix[]={37,0};
void __cdecl CWideString::Format(const unsigned short*format,...){assert(format[0]=='%'&&format[1]=='d'&&!format[2]);va_list args;va_start(args,format);int n=va_arg(args,int);va_end(args);char buffer[64];sprintf(buffer,"%d",n);SetAscii(*this,buffer);}
CWideString::CWideString(const unsigned short*p):data(0){++g_CWideStringInstanceCount_013BCA20;Set(*this,p);}CWideString&CWideString::operator+=(const CWideString&w){unsigned short buffer[512];unsigned n=Length(Text(*this)),m=Length(Text(w));assert(n+m<512);memcpy(buffer,Text(*this),n*2);memcpy(buffer+n,Text(w),(m+1)*2);Set(*this,buffer);return *this;}
static unsigned short thousandsSeparator=',';
void __fastcall FormatNumber_AddThousandsSeparators(int n,unsigned short*p){char digits[32];sprintf(digits,"%d",n);int length=(int)strlen(digits);for(int j=0;j<=length;++j)p[j]=(unsigned char)digits[j];int separators=(length-1)/3;if(separators>0){unsigned short*dest=p+length+separators;unsigned short*src=p+length;for(int count=0;count<length;++count){*dest=*src;if(count&&count%3==0){--dest;*dest=thousandsSeparator;}--dest;--src;}}}
}
int main(){using namespace NGameText;assert(sizeof(Data)==16);
{CWideString w("%1 then %2");assert(ConstructStringInternal(w,-1)&&Equal(w,"%1 then %2")&&replacements==0);assert(ConstructStringInternal(w,7,-8,-1)&&Equal(w,"7 then -8")&&replacements==2);}
assert(!g_CWideStringInstanceCount_013BCA20);
{CWideString w("%1 %1 %2");assert(ConstructStringInternal(w,0,2147483647,-1)&&Equal(w,"0 0 2,147,483,647"));}
{CWideString w("%1 done");int before=replacements;assert(!ConstructStringInternal(w,-2147483647-1,9,-1)&&Equal(w,"-2,147,483,648 done")&&replacements==before+1);}
{CWideString w("unchanged");int before=replacements;assert(!ConstructStringInternal(w,5,-1)&&Equal(w,"unchanged")&&replacements==before);}
{CWideString w("%1");{CWideString copy(w);assert(((Data*)w.data)->refs==2);assert(ConstructStringInternal(copy,11,-1)&&Equal(copy,"11")&&Equal(w,"%1"));}assert(((Data*)w.data)->refs==1);}
{CWideString w("%1/%2");assert(ConstructStringInternal(w,1234567,-999,-1)&&Equal(w,"1,234,567/-,999"));}{CWideString w("%1");thousandsSeparator='.';assert(ConstructStringInternal(w,1234,-1)&&Equal(w,"1.234"));thousandsSeparator=',';}assert(!g_CWideStringInstanceCount_013BCA20);puts("009c7a60_TEST PASS");return 0;}