// 009ac5d0 StringMap::Index
// Flags: /O2 /Oy /W3
// Leaf: Index
// Layout: tree header0,count4; node key+16,value+20; mapped value four WideString words and a zero dword. Retail calls confirm.
namespace N009ac5d0 {

#pragma pack(push,1)
struct Basic {char* Data;int length;unsigned capacity;unsigned char flags;int refs;bool Less(const Basic&)const;};
struct WideData {unsigned short* text;int length;unsigned capacity;int refs;};
#pragma pack(pop)
struct String {Basic* Data;String();String(const char*);String(const String&);~String();String& operator=(const String&); __forceinline bool operator<(const String& o)const {if(o.Data==Data)return false;if(!Data)return true;if(!o.Data)return false;return Data->Less(*o.Data);}};
struct Wide {WideData* Data;Wide();Wide(const unsigned short*);Wide(const Wide&);~Wide();Wide& operator=(const Wide&);};
struct Value {Wide A,B,C,D;unsigned int State;Value():State(0){}};
struct Pair {String Key;Value Mapped;Pair(const String& k,const Value& v):Key(k),Mapped(v){}};
struct Node {unsigned colour;Node* parent;Node* left;Node* right;String Key;Value Mapped;};
struct Iterator {Node* Ptr;Iterator(Node* p):Ptr(p){}__forceinline Iterator(const Iterator& o):Ptr(o.Ptr){}__forceinline bool operator==(const Iterator&o)const{return Ptr==o.Ptr;}};
struct StringMap {Node* Head;unsigned int Count;Node* LowerBound(const String&);Iterator Insert(Iterator,const Pair&);__forceinline Iterator end()const{return Iterator(Head);}Value& Index(const String&);};
Value& StringMap::Index(const String& key){
 Node* p=LowerBound(key);
 if(Iterator(p)==end() || key<p->Key) {Value value;Pair pair(key,value);Iterator result=Insert(p,pair);p=result.Ptr;}
 return p->Mapped;
}
}

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <stddef.h>
#include <new>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace N009ac5d0 {
static int stringLive,wideLive,basicBlocks,wideBlocks,wideFrees,basicDeletes,inserts;
static Basic* Make(const char*s){if(!s||!*s)return 0;Basic*d=new Basic;d->length=(int)std::strlen(s);d->capacity=d->length+1;d->flags=1;d->refs=1;d->Data=new char[d->capacity];std::strcpy(d->Data,s);++basicBlocks;return d;}
static void Clear(String&s){Basic*d=s.Data;if(d&&--d->refs<=0){delete[]d->Data;d->Data=0;d->length=0;d->capacity&=0x80000000u;assert(!d->Data&&!d->length&&!(d->capacity&0x7fffffffu));delete d;--basicBlocks;++basicDeletes;}s.Data=0;}
String::String():Data(0){++stringLive;}String::String(const char*s):Data(Make(s)){++stringLive;}
String::String(const String&o):Data(0){++stringLive;if(this!=&o&&o.Data){Data=o.Data;++Data->refs;}}String::~String(){Clear(*this);--stringLive;}
String&String::operator=(const String&o){if(this!=&o&&Data!=o.Data){Clear(*this);if(o.Data){Data=o.Data;++Data->refs;}}return *this;}
bool Basic::Less(const Basic&o)const{const signed char*a=(const signed char*)Data;const signed char*b=(const signed char*)o.Data;for(;;++a,++b){if(!*a&&!*b)return false;if(*a<*b)return true;if(*a>*b)return false;}}
static void Clear(Wide&w){WideData*d=w.Data;if(d&&--d->refs<=0){if(d->text){std::free(d->text);++wideFrees;}delete d;--wideBlocks;}w.Data=0;}
Wide::Wide():Data(0){++wideLive;}Wide::Wide(const unsigned short*s):Data(0){++wideLive;if(s&&*s){Data=new WideData;int n=0;while(s[n])++n;Data->length=n;Data->capacity=n+1;Data->refs=1;Data->text=(unsigned short*)std::malloc((n+1)*sizeof(unsigned short));assert(Data->text);for(int i=0;i<=n;++i)Data->text[i]=s[i];++wideBlocks;}}
Wide::Wide(const Wide&o):Data(0){++wideLive;if(this!=&o&&o.Data){Data=o.Data;++Data->refs;}}Wide::~Wide(){Clear(*this);--wideLive;}
Wide&Wide::operator=(const Wide&o){if(this!=&o&&Data!=o.Data){Clear(*this);if(o.Data){Data=o.Data;++Data->refs;}}return *this;}
Node*StringMap::LowerBound(const String&key){Node*best=Head;for(Node*n=Head->parent;n;){if(!(n->Key<key)){best=n;n=n->left;}else n=n->right;}return best;}
// Fixture insertion preserves owned nodes and binary-search order. The target does not observe tree colour or balancing.
Iterator StringMap::Insert(Iterator hint,const Pair&p){assert(hint.Ptr==LowerBound(p.Key));assert(!p.Mapped.A.Data&&!p.Mapped.B.Data&&!p.Mapped.C.Data&&!p.Mapped.D.Data&&!p.Mapped.State);Node*parent=Head;Node**slot=&Head->parent;while(*slot){parent=*slot;slot=p.Key<parent->Key?&parent->left:&parent->right;}Node*n=new Node;n->colour=0;n->parent=parent;n->left=n->right=0;n->Key=p.Key;n->Mapped=p.Mapped;*slot=n;++Count;++inserts;Node*lo=Head->parent;while(lo->left)lo=lo->left;Head->left=lo;Node*hi=Head->parent;while(hi->right)hi=hi->right;Head->right=hi;return Iterator(n);}
static void DeleteTree(Node*n){if(n){DeleteTree(n->left);DeleteTree(n->right);delete n;}}
}
int main(){using namespace N009ac5d0;assert(sizeof(Basic)==17&&offsetof(Basic,refs)==13&&sizeof(WideData)==16&&offsetof(WideData,refs)==12);assert(sizeof(Node)==40&&offsetof(Node,Key)==16&&offsetof(Node,Mapped)==20);{
Node head;head.parent=0;head.left=head.right=&head;StringMap map={&head,0};String key("key");Value&v=map.Index(key);assert(map.Count==1&&inserts==1&&!v.State&&key.Data->refs==2);v.State=19;assert(&map.Index(key)==&v&&map.Count==1&&v.State==19);String later("zzz"),earlier("aaa"),empty;assert(!map.Index(later).State&&!map.Index(earlier).State&&!map.Index(empty).State&&map.Count==4);assert(&map.Index(empty)==&map.LowerBound(empty)->Mapped&&map.Count==4);
char highText[]={char(0x80),0},highText2[]={char(0xff),0};String high(highText),high2(highText2),ascii("a");assert(high<high2&&high2<ascii&&!(ascii<high));map.Index(high).State=80;map.Index(high2).State=255;assert(map.Index(high).State==80&&map.Index(high2).State==255&&map.Count==6);DeleteTree(head.parent);head.parent=0;assert(key.Data->refs==1);
String shared("owned");{String cleared(shared);assert(shared.Data->refs==2);Clear(cleared);assert(!cleared.Data&&shared.Data->refs==1);}{String copies[140];for(int i=0;i<140;++i)copies[i]=shared;assert(shared.Data->refs==141);}assert(shared.Data->refs==1);{String zero("zero");zero.Data->refs=0;}unsigned short wideText[]={65,0x3a9,0};int wf=wideFrees;{Wide w(wideText);{Wide cleared(w);assert(w.Data->refs==2);Clear(cleared);assert(!cleared.Data&&w.Data->refs==1);}{Wide copies[140];for(int i=0;i<140;++i)copies[i]=w;assert(w.Data->refs==141);Wide c(w);assert(w.Data->refs==142);}assert(w.Data->refs==1&&w.Data->text[1]==0x3a9);}assert(wideFrees==wf+1);{Wide zero(wideText);zero.Data->refs=0;}assert(wideFrees==wf+2);
}assert(!stringLive&&!wideLive&&!basicBlocks&&!wideBlocks&&basicDeletes>0);std::puts("009ac5d0_TEST PASS");}