// 00cbc940 Row00cbc940::vector<Element>::_M_insert_overflow (STLport; catalog: CScriptThing_VectorReallocate, but element starts with a CCharString, not the CScriptThing vptr)
// Flags: /O2 /Oy /W3 
// Leaf: _M_insert_overflow
#include <stdlib.h>
#include <new>
namespace Row00cbc940 {
// Layout: oracle uses a 12-byte STLport vector; element stride 12 with counted pointer at final dword.
struct CCharString {void*p;CCharString();CCharString(const char*);CCharString(const CCharString&);~CCharString();}; struct Ref{int refs;};struct Element{CCharString text;unsigned a;Ref*ref;Element():a(0),ref(0){}__forceinline Element(const Element&v):text(v.text),a(v.a),ref(v.ref){if(ref)++ref->refs;}};
struct __false_type {__false_type(){}};
Element* __fastcall __uninitialized_copy(Element*,Element*,Element*,const bool&);
Element* __fastcall __uninitialized_fill_n(Element*,unsigned,const Element&,const bool&);
void __fastcall _Destroy(Element*,Element*,const bool&);
__forceinline const unsigned& max(const unsigned&a,const unsigned&b){return a<b?b:a;}
template<class T> struct vector {T*_M_start,*_M_finish,*_M_end_of_storage;void _M_insert_overflow(T*,const T&,const __false_type&,unsigned,bool);};
template<class T> void vector<T>::_M_insert_overflow(T*pos,const T&value,const __false_type&,unsigned n,bool atEnd) {
 unsigned old=_M_finish-_M_start; unsigned capacity=old+max(old,n);
 T* buffer=capacity?(T*)malloc(capacity*sizeof(T)):0;
 T* finish=__uninitialized_copy(_M_start,pos,buffer,atEnd);
 if(n==1){if(finish)new(finish)T(value);++finish;}
 else finish=__uninitialized_fill_n(finish,n,value,atEnd);
 if(!atEnd)finish=__uninitialized_copy(pos,_M_finish,finish,atEnd);
 _Destroy(_M_start,_M_finish,atEnd);if(_M_start)free(_M_start);
 _M_start=buffer;_M_finish=finish;_M_end_of_storage=buffer+capacity;
}
template struct vector<Element>;
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
static StringData* block(const CCharString& s){return (StringData*)s.p;}
static const char* chars(const CCharString& s){return s.p?block(s)->Text:"";}
CCharString::CCharString():p(0){++liveStrings;++constructedStrings;}
CCharString::CCharString(const char* value):p(0){++liveStrings;++constructedStrings;if(!value||!value[0])return;StringData* data=new StringData;data->Length=(unsigned)strlen(value);data->Capacity=data->Length+1;data->UseFastExtend=0;data->Refs=1;data->Text=new char[data->Capacity];strcpy(data->Text,value);p=data;++liveBlocks;++liveTexts;}
CCharString::CCharString(const CCharString& value):p(value.p){++liveStrings;++constructedStrings;if(p)++block(*this)->Refs;}
CCharString::~CCharString(){if(p){StringData* data=block(*this);if(--data->Refs<=0){if(data->Text){delete[] data->Text;data->Text=0;--liveTexts;}data->Length=0;data->Capacity&=0x80000000u;delete data;--liveBlocks;}p=0;}--liveStrings;++destroyedStrings;}
static void __fastcall deletePayload(void* receiver){Payload* p=(Payload*)receiver;assert(p&&p->ID<4&&p->TextBlock&&p->TextBlock->Refs>0);++payloadDeletes[p->ID];++callbackCount;--livePayloads;delete p;}
Element* __fastcall __uninitialized_copy(Element* _M_start,Element* _M_finish,Element* out,const bool&){for(;_M_start!=_M_finish;++_M_start,++out)if(out)new(out)Element(*_M_start);return out;}
Element* __fastcall __uninitialized_fill_n(Element* out,unsigned count,const Element& value,const bool&){for(;count;--count,++out)if(out)new(out)Element(value);return out;}
void __fastcall _Destroy(Element* _M_start,Element* _M_finish,const bool&){for(;_M_start!=_M_finish;++_M_start){RefControl* control=(RefControl*)_M_start->ref;if(control){++controlReleases;--control->refs;if(control->refs==0){assert(control->DeleteFunc&&control->Data&&(unsigned)control->Data==_M_start->a);control->DeleteFunc(control->Data);delete control;--liveControls;++controlDeletes;}}_M_start->a=0;_M_start->ref=0;assert(!_M_start->a&&!_M_start->ref);_M_start->~Element();++destroyedElements;}}
static Element* create(const char* text,unsigned id){Element* e=(Element*)malloc(sizeof(Element));assert(e);new(e)Element;e->text.~CCharString();new(&e->text)CCharString(text);Payload* payload=new Payload;payload->ID=id;payload->TextBlock=block(e->text);RefControl* control=new RefControl;control->refs=1;control->DeleteFunc=deletePayload;control->Data=payload;e->a=(unsigned)payload;e->ref=control;++liveControls;++livePayloads;return e;}
static void dispose(Element* e){bool tag=false;_Destroy(e,e+1,tag);free(e);}
static vector<Element> emptyVector(){Element* raw=(Element*)malloc(sizeof(Element));assert(raw);vector<Element> v;v._M_start=v._M_finish=v._M_end_of_storage=raw;return v;}
static void clear(vector<Element>& v){bool tag=false;_Destroy(v._M_start,v._M_finish,tag);free(v._M_start);v._M_start=v._M_finish=v._M_end_of_storage=0;}
static void contents(const vector<Element>& v,const Element& seed,unsigned n){assert((unsigned)(v._M_finish-v._M_start)==n);for(unsigned i=0;i<n;++i){assert(v._M_start[i].text.p==seed.text.p);assert(v._M_start[i].a==seed.a&&v._M_start[i].ref==seed.ref);assert(!strcmp(chars(v._M_start[i].text),chars(seed.text)));}}
}
int main(){using namespace Row00cbc940;assert(sizeof(StringData)==17&&offsetof(StringData,Refs)==13);assert(sizeof(RefControl)==12&&offsetof(RefControl,DeleteFunc)==4&&offsetof(RefControl,Data)==8);assert(sizeof(Element)==12);
/* All actual shared copies use constructed objects and32-bit references. */
{CCharString nullText((const char*)0),emptyText("");CCharString nullCopy(nullText);assert(!nullText.p&&!emptyText.p&&!nullCopy.p);CCharString shared("shared");assert(block(shared)->UseFastExtend==0);CCharString* copies[300];for(int i=0;i<300;++i){copies[i]=new CCharString(shared);assert(copies[i]->p==shared.p);}assert(block(shared)->Refs==301);for(int j=0;j<45;++j)delete copies[j];assert(block(shared)->Refs==256);delete copies[45];assert(block(shared)->Refs==255);for(int k=46;k<300;++k)delete copies[k];assert(block(shared)->Refs==1&&liveBlocks==1&&liveTexts==1);}
assert(!liveStrings&&!liveBlocks&&!liveTexts);
Element* seed=create("alpha",1);StringData* data=block(seed->text);RefControl* control=(RefControl*)seed->ref;assert(data->Refs==1&&control->refs==1&&!data->UseFastExtend);vector<Element> v=emptyVector();__false_type tag;
v._M_insert_overflow(v._M_start,*seed,tag,1,false);contents(v,*seed,1);assert(data->Refs==2&&control->refs==2);
v._M_insert_overflow(v._M_start,*seed,tag,2,false);contents(v,*seed,3);assert(data->Refs==4&&control->refs==4);
v._M_insert_overflow(v._M_start+1,v._M_start[2],tag,1,false);contents(v,*seed,4);assert(data->Refs==5&&control->refs==5);
v._M_insert_overflow(v._M_start+2,v._M_start[0],tag,300,false);contents(v,*seed,304);assert(data->Refs==305&&control->refs==305&&!callbackCount);
v._M_insert_overflow(v._M_finish,v._M_start[7],tag,0,true);contents(v,*seed,304);assert(v._M_end_of_storage-v._M_start==608&&data->Refs==305&&control->refs==305&&!callbackCount);
v._M_insert_overflow(v._M_finish,v._M_start[7],tag,1,true);contents(v,*seed,305);assert(data->Refs==306&&control->refs==306);
clear(v);assert(data->Refs==1&&control->refs==1&&!callbackCount&&liveControls==1&&livePayloads==1);dispose(seed);assert(payloadDeletes[1]==1&&callbackCount==1&&controlDeletes==1&&!liveStrings&&!liveBlocks&&!liveTexts&&!liveControls&&!livePayloads);
/* A distinct control releases only at zero; a null control releases nothing. */
Element* beta=create("beta",2);Element* betaCopy=(Element*)malloc(sizeof(Element));assert(betaCopy);new(betaCopy)Element(*beta);assert(betaCopy->text.p==beta->text.p&&betaCopy->ref==beta->ref&&beta->ref->refs==2&&block(beta->text)->Refs==2);dispose(beta);assert(!payloadDeletes[2]&&liveControls==1&&livePayloads==1&&betaCopy->ref->refs==1);dispose(betaCopy);assert(payloadDeletes[2]==1&&callbackCount==2&&controlDeletes==2);
Element* nullElement=(Element*)malloc(sizeof(Element));assert(nullElement);new(nullElement)Element;assert(!nullElement->text.p&&!nullElement->ref&&!nullElement->a);int releasesBefore=controlReleases;dispose(nullElement);assert(controlReleases==releasesBefore&&callbackCount==2);
assert(!liveStrings&&!liveBlocks&&!liveTexts&&!liveControls&&!livePayloads&&constructedStrings==destroyedStrings&&controlDeletes==callbackCount&&destroyedElements>600);puts("00cbc940_TEST PASS");return 0;}