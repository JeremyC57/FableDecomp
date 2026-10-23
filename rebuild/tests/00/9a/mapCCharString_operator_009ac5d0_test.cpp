// 009ac5d0 N009ac5d0::bankMap<CCharString,CBankFileManager::CBankInfo>::operator[] (catalog: bankMap<CCharString::operator[]; bsim .69; model kept in N009ac5d0)
// Flags: /O2 /Oy /W3
// Leaf: ??A
// Layout: tree header0,count4; node key+16,value+20; mapped value four WideString words and a zero dword. Retail calls confirm.
namespace N009ac5d0 {

#pragma pack(push,1)
struct CCharStringData {char* Data;int length;unsigned capacity;unsigned char flags;int refs;bool operator<(const CCharStringData&)const;};
struct CWideStringData {unsigned short* text;int length;unsigned capacity;int refs;};
#pragma pack(pop)
struct CCharString {CCharStringData* Data;CCharString();CCharString(const char*);CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&); __forceinline bool operator<(const CCharString& o)const {if(o.Data==Data)return false;if(!Data)return true;if(!o.Data)return false;return *Data<*o.Data;}};
struct CWideString {CWideStringData* Data;CWideString();CWideString(const unsigned short*);CWideString(const CWideString&);~CWideString();CWideString& operator=(const CWideString&);};
struct CBankInfo {CWideString A,B,C,D;unsigned int State;CBankInfo():State(0){}};
struct Pair {CCharString Key;CBankInfo Mapped;Pair(const CCharString& k,const CBankInfo& v):Key(k),Mapped(v){}};
struct Node {unsigned colour;Node* parent;Node* left;Node* right;CCharString Key;CBankInfo Mapped;};
struct Iterator {Node* Ptr;Iterator(Node* p):Ptr(p){}__forceinline Iterator(const Iterator& o):Ptr(o.Ptr){}__forceinline bool operator==(const Iterator&o)const{return Ptr==o.Ptr;}};
struct map {Node* Head;unsigned int Count;Node* lower_bound(const CCharString&);Iterator insert(Iterator,const Pair&);__forceinline Iterator end()const{return Iterator(Head);}CBankInfo& operator[](const CCharString&);};
CBankInfo& map::operator[](const CCharString& key){
 Node* p=lower_bound(key);
 if(Iterator(p)==end() || key<p->Key) {CBankInfo value;Pair pair(key,value);Iterator result=insert(p,pair);p=result.Ptr;}
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
static CCharStringData* Make(const char*s){if(!s||!*s)return 0;CCharStringData*d=new CCharStringData;d->length=(int)std::strlen(s);d->capacity=d->length+1;d->flags=1;d->refs=1;d->Data=new char[d->capacity];std::strcpy(d->Data,s);++basicBlocks;return d;}
static void Clear(CCharString&s){CCharStringData*d=s.Data;if(d&&--d->refs<=0){delete[]d->Data;d->Data=0;d->length=0;d->capacity&=0x80000000u;assert(!d->Data&&!d->length&&!(d->capacity&0x7fffffffu));delete d;--basicBlocks;++basicDeletes;}s.Data=0;}
CCharString::CCharString():Data(0){++stringLive;}CCharString::CCharString(const char*s):Data(Make(s)){++stringLive;}
CCharString::CCharString(const CCharString&o):Data(0){++stringLive;if(this!=&o&&o.Data){Data=o.Data;++Data->refs;}}CCharString::~CCharString(){Clear(*this);--stringLive;}
CCharString&CCharString::operator=(const CCharString&o){if(this!=&o&&Data!=o.Data){Clear(*this);if(o.Data){Data=o.Data;++Data->refs;}}return *this;}
bool CCharStringData::operator<(const CCharStringData&o)const{const signed char*a=(const signed char*)Data;const signed char*b=(const signed char*)o.Data;for(;;++a,++b){if(!*a&&!*b)return false;if(*a<*b)return true;if(*a>*b)return false;}}
static void Clear(CWideString&w){CWideStringData*d=w.Data;if(d&&--d->refs<=0){if(d->text){std::free(d->text);++wideFrees;}delete d;--wideBlocks;}w.Data=0;}
CWideString::CWideString():Data(0){++wideLive;}CWideString::CWideString(const unsigned short*s):Data(0){++wideLive;if(s&&*s){Data=new CWideStringData;int n=0;while(s[n])++n;Data->length=n;Data->capacity=n+1;Data->refs=1;Data->text=(unsigned short*)std::malloc((n+1)*sizeof(unsigned short));assert(Data->text);for(int i=0;i<=n;++i)Data->text[i]=s[i];++wideBlocks;}}
CWideString::CWideString(const CWideString&o):Data(0){++wideLive;if(this!=&o&&o.Data){Data=o.Data;++Data->refs;}}CWideString::~CWideString(){Clear(*this);--wideLive;}
CWideString&CWideString::operator=(const CWideString&o){if(this!=&o&&Data!=o.Data){Clear(*this);if(o.Data){Data=o.Data;++Data->refs;}}return *this;}
Node*map::lower_bound(const CCharString&key){Node*best=Head;for(Node*n=Head->parent;n;){if(!(n->Key<key)){best=n;n=n->left;}else n=n->right;}return best;}
// Fixture insertion preserves owned nodes and binary-search order. The target does not observe tree colour or balancing.
Iterator map::insert(Iterator hint,const Pair&p){assert(hint.Ptr==lower_bound(p.Key));assert(!p.Mapped.A.Data&&!p.Mapped.B.Data&&!p.Mapped.C.Data&&!p.Mapped.D.Data&&!p.Mapped.State);Node*parent=Head;Node**slot=&Head->parent;while(*slot){parent=*slot;slot=p.Key<parent->Key?&parent->left:&parent->right;}Node*n=new Node;n->colour=0;n->parent=parent;n->left=n->right=0;n->Key=p.Key;n->Mapped=p.Mapped;*slot=n;++Count;++inserts;Node*lo=Head->parent;while(lo->left)lo=lo->left;Head->left=lo;Node*hi=Head->parent;while(hi->right)hi=hi->right;Head->right=hi;return Iterator(n);}
static void DeleteTree(Node*n){if(n){DeleteTree(n->left);DeleteTree(n->right);delete n;}}
}
int main(){using namespace N009ac5d0;assert(sizeof(CCharStringData)==17&&offsetof(CCharStringData,refs)==13&&sizeof(CWideStringData)==16&&offsetof(CWideStringData,refs)==12);assert(sizeof(Node)==40&&offsetof(Node,Key)==16&&offsetof(Node,Mapped)==20);{
Node head;head.parent=0;head.left=head.right=&head;map bankMap={&head,0};CCharString key("key");CBankInfo&v=bankMap[key];assert(bankMap.Count==1&&inserts==1&&!v.State&&key.Data->refs==2);v.State=19;assert(&bankMap[key]==&v&&bankMap.Count==1&&v.State==19);CCharString later("zzz"),earlier("aaa"),empty;assert(!bankMap[later].State&&!bankMap[earlier].State&&!bankMap[empty].State&&bankMap.Count==4);assert(&bankMap[empty]==&bankMap.lower_bound(empty)->Mapped&&bankMap.Count==4);
char highText[]={char(0x80),0},highText2[]={char(0xff),0};CCharString high(highText),high2(highText2),ascii("a");assert(high<high2&&high2<ascii&&!(ascii<high));bankMap[high].State=80;bankMap[high2].State=255;assert(bankMap[high].State==80&&bankMap[high2].State==255&&bankMap.Count==6);DeleteTree(head.parent);head.parent=0;assert(key.Data->refs==1);
CCharString shared("owned");{CCharString cleared(shared);assert(shared.Data->refs==2);Clear(cleared);assert(!cleared.Data&&shared.Data->refs==1);}{CCharString copies[140];for(int i=0;i<140;++i)copies[i]=shared;assert(shared.Data->refs==141);}assert(shared.Data->refs==1);{CCharString zero("zero");zero.Data->refs=0;}unsigned short wideText[]={65,0x3a9,0};int wf=wideFrees;{CWideString w(wideText);{CWideString cleared(w);assert(w.Data->refs==2);Clear(cleared);assert(!cleared.Data&&w.Data->refs==1);}{CWideString copies[140];for(int i=0;i<140;++i)copies[i]=w;assert(w.Data->refs==141);CWideString c(w);assert(w.Data->refs==142);}assert(w.Data->refs==1&&w.Data->text[1]==0x3a9);}assert(wideFrees==wf+1);{CWideString zero(wideText);zero.Data->refs=0;}assert(wideFrees==wf+2);
}assert(!stringLive&&!wideLive&&!basicBlocks&&!wideBlocks&&basicDeletes>0);std::puts("009ac5d0_TEST PASS");}