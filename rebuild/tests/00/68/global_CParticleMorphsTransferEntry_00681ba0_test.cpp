// 00681ba0 CParticleMorphs_TransferEntry
// Flags: /O2 /Oy /W3
// Leaf: Transfer
// Shape: the write/read choice is a named bool (gives retail's write-first block order); the default argument of Write is
//   DefaultString() (default ctor into the inline local, copy ctor into the result, dtor) so all three strings home in the
//   dead ctx/name/arr argument slots and no frame is allocated.
#include <new>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);

// Layout: 00681ba0 string data has buffer +0 and length +4; context mode +0x18.
// The CCharString empty text 0x0129AAF4 is isolated so it does not rebind the pooled "" another landed file names EmptyString.
namespace NCharStringPool {extern const char EmptyString[];} using NCharStringPool::EmptyString;
#pragma pack(push,1)
struct Data {char* text;int size;unsigned capacity;bool fast;int refs;};
#pragma pack(pop)
class CCharString { public: Data* data;CCharString();CCharString(const char*,long=-1);CCharString(const CCharString&);~CCharString();void AppendCharacter(char);const char* Text()const{return !data?EmptyString:data->text;}int Size()const{return data?data->size:0;} CCharString Mutate();};
Data* MakeData(const char*,int);Data* ShareData(Data*);void DropData(Data*);

class CPersistContext { public:char pad[24];int mode;void Write(const char*,CCharString&,CCharString&);void Read(const char*,CCharString&);};
namespace std {template<class T>struct vector {T* first;T* last;T* end;};}
typedef std::vector<bool> BoolVector;
__forceinline CCharString DefaultString(){CCharString s;return s;}
struct CPersistTraits {void Transfer(CPersistContext*,CCharString*,BoolVector*);};
void CPersistTraits::Transfer(CPersistContext* ctx,CCharString* name,BoolVector* arr){
 bool writing=ctx->mode==1||ctx->mode==3;if(writing){CCharString value("");for(bool* p=arr->first,*end=arr->last;p!=end;++p)value.AppendCharacter(*p?'T':'F');const char* key=name->Text();ctx->Write(key,value,DefaultString());return;}
{CCharString value("");ctx->Read(name->Text(),value);if(value.data&&value.data->size>0&&value.data->size==arr->last-arr->first){const char* p=value.data->text;int n=arr->last-arr->first;for(int i=0;i<n;++i)arr->first[i]=p[i]=='T';}}

}


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma auto_inline(off)
CCharString::CCharString():data(0){}
CCharString::CCharString(const char* p,long n):data(MakeData(p,n)){}
CCharString::CCharString(const CCharString& x):data(x.data){if(data)++data->refs;}
CCharString::~CCharString(){DropData(data);data=0;}
#pragma auto_inline(on)
const char NCharStringPool::EmptyString[]=""; static char saved[32];static const char* incoming="";
Data* MakeData(const char* p,int n){if(n<0)n=strlen(p);Data* d=new Data;d->size=n;d->capacity=n+1;d->fast=true;d->refs=1;d->text=new char[n+1];memcpy(d->text,p,n);d->text[n]=0;return d;}
Data* ShareData(Data* p){if(p)++p->refs;return p;}
void DropData(Data* p){if(p&&--p->refs<=0){delete[]p->text;delete p;}}
CCharString CCharString::Mutate(){return *this;}
void CCharString::AppendCharacter(char c){int n=data->size;char* p=new char[n+2];memcpy(p,data->text,n);p[n]=c;p[n+1]=0;delete[]data->text;data->text=p;++data->size;}
void CPersistContext::Write(const char* k,CCharString& v,CCharString& d){assert(strcmp(k,"flags")==0);assert(strcmp(d.Text(),"")==0);strcpy(saved,v.Text());}
void CPersistContext::Read(const char* k,CCharString& v){assert(strcmp(k,"flags")==0);for(const char* p=incoming;*p;++p)v.AppendCharacter(*p);}

int main(){CPersistTraits t;CPersistContext c;memset(&c,0,sizeof(c));CCharString key("flags");bool b[3]={true,false,true};BoolVector a={b,b+3,b+3};c.mode=1;t.Transfer(&c,&key,&a);assert(strcmp(saved,"TFT")==0);c.mode=3;t.Transfer(&c,&key,&a);assert(strcmp(saved,"TFT")==0);c.mode=0;incoming="FTF";t.Transfer(&c,&key,&a);assert(!b[0]&&b[1]&&!b[2]);incoming="T";t.Transfer(&c,&key,&a);assert(!b[0]&&b[1]&&!b[2]);a.last=b;c.mode=1;t.Transfer(&c,&key,&a);assert(!saved[0]);puts("00681ba0_TEST PASS");return 0;}