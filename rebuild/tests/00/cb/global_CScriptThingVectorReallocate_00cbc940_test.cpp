// 00cbc940 Row00cbc940::Vector::InsertOverflow
// Flags: /O2 /Oy /W3 
// Leaf: InsertOverflow
#include <stdlib.h>
#include <new>
namespace Row00cbc940 {
// Layout: oracle uses a 12-byte STLport vector; element stride 12 with counted pointer at final dword.
struct String {void*p;String();String(const char*);String(const String&);~String();}; struct Ref{int refs;};struct Element{String text;unsigned a;Ref*ref;Element():a(0),ref(0){}__forceinline Element(const Element&v):text(v.text),a(v.a),ref(v.ref){if(ref)++ref->refs;}};
struct Tag {Tag(){}};
Element* __fastcall Copy(Element*,Element*,Element*,const bool&);
Element* __fastcall Fill(Element*,unsigned,const Element&,const bool&);
void __fastcall Destroy(Element*,Element*,const bool&);
__forceinline const unsigned& Max(const unsigned&a,const unsigned&b){return a<b?b:a;}
template<class T> struct Vector {T*first,*last,*limit;void InsertOverflow(T*,const T&,const Tag&,unsigned,bool);};
template<class T> void Vector<T>::InsertOverflow(T*pos,const T&value,const Tag&,unsigned n,bool atEnd) {
 unsigned old=last-first; unsigned capacity=old+Max(old,n);
 T* buffer=capacity?(T*)malloc(capacity*sizeof(T)):0;
 T* finish=Copy(first,pos,buffer,atEnd);
 if(n==1){if(finish)new(finish)T(value);++finish;}
 else finish=Fill(finish,n,value,atEnd);
 if(!atEnd)finish=Copy(pos,last,finish,atEnd);
 Destroy(first,last,atEnd);if(first)free(first);
 first=buffer;last=finish;limit=buffer+capacity;
}
template struct Vector<Element>;
}
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace Row00cbc940 {
#pragma pack(push,1)
struct StringData {char* Text;unsigned Length;unsigned Capacity;unsigned char UseFastExtend;int Refs;};
#pragma pack(pop)
struct Payload {unsigned ID;StringData* TextBlock;};
typedef void (__fastcall* DeleteCallback)(void*);
struct RefControl:Ref {DeleteCallback DeleteFunc;void* Data;};
static int liveStrings,liveBlocks,liveTexts,liveControls,livePayloads,constructedStrings,destroyedStrings,destroyedElements,controlReleases,callbackCount,controlDeletes;
static unsigned payloadDeletes[4];
static StringData* block(const String& s){return (StringData*)s.p;}
static const char* chars(const String& s){return s.p?block(s)->Text:"";}
String::String():p(0){++liveStrings;++constructedStrings;}
String::String(const char* value):p(0){++liveStrings;++constructedStrings;if(!value||!value[0])return;StringData* data=new StringData;data->Length=(unsigned)strlen(value);data->Capacity=data->Length+1;data->UseFastExtend=0;data->Refs=1;data->Text=new char[data->Capacity];strcpy(data->Text,value);p=data;++liveBlocks;++liveTexts;}
String::String(const String& value):p(value.p){++liveStrings;++constructedStrings;if(p)++block(*this)->Refs;}
String::~String(){if(p){StringData* data=block(*this);if(--data->Refs<=0){if(data->Text){delete[] data->Text;data->Text=0;--liveTexts;}data->Length=0;data->Capacity&=0x80000000u;delete data;--liveBlocks;}p=0;}--liveStrings;++destroyedStrings;}
static void __fastcall deletePayload(void* receiver){Payload* p=(Payload*)receiver;assert(p&&p->ID<4&&p->TextBlock&&p->TextBlock->Refs>0);++payloadDeletes[p->ID];++callbackCount;--livePayloads;delete p;}
Element* __fastcall Copy(Element* first,Element* last,Element* out,const bool&){for(;first!=last;++first,++out)if(out)new(out)Element(*first);return out;}
Element* __fastcall Fill(Element* out,unsigned count,const Element& value,const bool&){for(;count;--count,++out)if(out)new(out)Element(value);return out;}
void __fastcall Destroy(Element* first,Element* last,const bool&){for(;first!=last;++first){RefControl* control=(RefControl*)first->ref;if(control){++controlReleases;--control->refs;if(control->refs==0){assert(control->DeleteFunc&&control->Data&&(unsigned)control->Data==first->a);control->DeleteFunc(control->Data);delete control;--liveControls;++controlDeletes;}}first->a=0;first->ref=0;assert(!first->a&&!first->ref);first->~Element();++destroyedElements;}}
static Element* create(const char* text,unsigned id){Element* e=(Element*)malloc(sizeof(Element));assert(e);new(e)Element;e->text.~String();new(&e->text)String(text);Payload* payload=new Payload;payload->ID=id;payload->TextBlock=block(e->text);RefControl* control=new RefControl;control->refs=1;control->DeleteFunc=deletePayload;control->Data=payload;e->a=(unsigned)payload;e->ref=control;++liveControls;++livePayloads;return e;}
static void dispose(Element* e){bool tag=false;Destroy(e,e+1,tag);free(e);}
static Vector<Element> emptyVector(){Element* raw=(Element*)malloc(sizeof(Element));assert(raw);Vector<Element> v;v.first=v.last=v.limit=raw;return v;}
static void clear(Vector<Element>& v){bool tag=false;Destroy(v.first,v.last,tag);free(v.first);v.first=v.last=v.limit=0;}
static void contents(const Vector<Element>& v,const Element& seed,unsigned n){assert((unsigned)(v.last-v.first)==n);for(unsigned i=0;i<n;++i){assert(v.first[i].text.p==seed.text.p);assert(v.first[i].a==seed.a&&v.first[i].ref==seed.ref);assert(!strcmp(chars(v.first[i].text),chars(seed.text)));}}
}
int main(){using namespace Row00cbc940;assert(sizeof(StringData)==17&&offsetof(StringData,Refs)==13);assert(sizeof(RefControl)==12&&offsetof(RefControl,DeleteFunc)==4&&offsetof(RefControl,Data)==8);assert(sizeof(Element)==12);
/* All actual shared copies use constructed objects and32-bit references. */
{String nullText((const char*)0),emptyText("");String nullCopy(nullText);assert(!nullText.p&&!emptyText.p&&!nullCopy.p);String shared("shared");assert(block(shared)->UseFastExtend==0);String* copies[300];for(int i=0;i<300;++i){copies[i]=new String(shared);assert(copies[i]->p==shared.p);}assert(block(shared)->Refs==301);for(int j=0;j<45;++j)delete copies[j];assert(block(shared)->Refs==256);delete copies[45];assert(block(shared)->Refs==255);for(int k=46;k<300;++k)delete copies[k];assert(block(shared)->Refs==1&&liveBlocks==1&&liveTexts==1);}
assert(!liveStrings&&!liveBlocks&&!liveTexts);
Element* seed=create("alpha",1);StringData* data=block(seed->text);RefControl* control=(RefControl*)seed->ref;assert(data->Refs==1&&control->refs==1&&!data->UseFastExtend);Vector<Element> v=emptyVector();Tag tag;
v.InsertOverflow(v.first,*seed,tag,1,false);contents(v,*seed,1);assert(data->Refs==2&&control->refs==2);
v.InsertOverflow(v.first,*seed,tag,2,false);contents(v,*seed,3);assert(data->Refs==4&&control->refs==4);
v.InsertOverflow(v.first+1,v.first[2],tag,1,false);contents(v,*seed,4);assert(data->Refs==5&&control->refs==5);
v.InsertOverflow(v.first+2,v.first[0],tag,300,false);contents(v,*seed,304);assert(data->Refs==305&&control->refs==305&&!callbackCount);
v.InsertOverflow(v.last,v.first[7],tag,0,true);contents(v,*seed,304);assert(v.limit-v.first==608&&data->Refs==305&&control->refs==305&&!callbackCount);
v.InsertOverflow(v.last,v.first[7],tag,1,true);contents(v,*seed,305);assert(data->Refs==306&&control->refs==306);
clear(v);assert(data->Refs==1&&control->refs==1&&!callbackCount&&liveControls==1&&livePayloads==1);dispose(seed);assert(payloadDeletes[1]==1&&callbackCount==1&&controlDeletes==1&&!liveStrings&&!liveBlocks&&!liveTexts&&!liveControls&&!livePayloads);
/* A distinct control releases only at zero; a null control releases nothing. */
Element* beta=create("beta",2);Element* betaCopy=(Element*)malloc(sizeof(Element));assert(betaCopy);new(betaCopy)Element(*beta);assert(betaCopy->text.p==beta->text.p&&betaCopy->ref==beta->ref&&beta->ref->refs==2&&block(beta->text)->Refs==2);dispose(beta);assert(!payloadDeletes[2]&&liveControls==1&&livePayloads==1&&betaCopy->ref->refs==1);dispose(betaCopy);assert(payloadDeletes[2]==1&&callbackCount==2&&controlDeletes==2);
Element* nullElement=(Element*)malloc(sizeof(Element));assert(nullElement);new(nullElement)Element;assert(!nullElement->text.p&&!nullElement->ref&&!nullElement->a);int releasesBefore=controlReleases;dispose(nullElement);assert(controlReleases==releasesBefore&&callbackCount==2);
assert(!liveStrings&&!liveBlocks&&!liveTexts&&!liveControls&&!livePayloads&&constructedStrings==destroyedStrings&&controlDeletes==callbackCount&&destroyedElements>600);puts("00cbc940_TEST PASS");return 0;}