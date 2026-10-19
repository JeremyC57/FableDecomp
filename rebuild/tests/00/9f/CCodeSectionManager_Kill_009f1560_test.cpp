#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __declspec(x)
#define __cdecl
#endif
// Replace only the external deallocation dependency so the fixture can inspect its sentinel.
#define free ObserveFree
// CCodeSectionManager::~CCodeSectionManager() -- destroy the section and name maps, then the singleton base.
// Retail 009F1560,185 bytes; called by deleting destructor009F1540 (RTTI0129C764 CCodeSectionManager).
// Final vptr0129C75C belongs to CTBaseSingleton. No shared headers exist for either class.
// Donor map fields ShortNameToHandle/LongNameToHandle/SectionDesc are at+4/+14/+24 with16-byte headers;
// retail uses12-byte STLport headers at+4/+10/+1c. The last header word is unused here.
// Leaf: ??1CCodeSectionManager
extern "C" void __cdecl free(void*);
struct CSectionMapNode { long Color; CSectionMapNode* Parent; CSectionMapNode* Left; CSectionMapNode* Right; };
struct CNameToHandleMap {
 CSectionMapNode* Head; unsigned long Size; unsigned char OpaquePolicy[4];
 void Erase(CSectionMapNode*); // 00579435: recursive node destruction
 ~CNameToHandleMap() {
  if(Size) { Erase(Head->Parent); Head->Left=Head; Head->Parent=0; Head->Right=Head; Size=0; }
  if(Head) free(Head); // 00BFEA14
 }
};
struct CSectionDescriptorMap {
 CSectionMapNode* Head; unsigned long Size; unsigned char OpaquePolicy[4];
 void Erase(CSectionMapNode*); // 009F0D40: recursive section-descriptor destruction
 ~CSectionDescriptorMap() {
  if(Size) { Erase(Head->Parent); Head->Left=Head; Head->Parent=0; Head->Right=Head; Size=0; }
  if(Head) free(Head); // 00BFEA14
 }
};
struct CTBaseSingleton { virtual ~CTBaseSingleton() {} };
struct __declspec(novtable) CCodeSectionManager : CTBaseSingleton {
 CNameToHandleMap ShortNameToHandle; // +04
 CNameToHandleMap LongNameToHandle; // +10
 CSectionDescriptorMap SectionDesc; // +1c
 ~CCodeSectionManager();
};
CCodeSectionManager::~CCodeSectionManager() {}
#undef free
static CCodeSectionManager* active;
static CSectionMapNode* heads[3];
static CSectionMapNode* roots[3];
static void* maps[3];
static unsigned long* sizes[3];
static int state[3],events[8],eventCount;
static bool bad;
static void record(int e) { if(eventCount<8) events[eventCount++]=e; else bad=true; }
static void erase(int which,void* receiver,CSectionMapNode* root) {
 if(receiver!=maps[which] || root!=roots[which] || state[which]!=2 || *sizes[which]!=unsigned(2+which)) bad=true;
 record(10+which);
}
void CNameToHandleMap::Erase(CSectionMapNode* root) {
 int which=this==&active->ShortNameToHandle?0:1;
 erase(which,this,root);
}
void CSectionDescriptorMap::Erase(CSectionMapNode* root) { erase(2,this,root); }
extern "C" void __cdecl ObserveFree(void* p) {
 int which=0; while(which<3 && p!=heads[which]) ++which;
 if(which==3) {bad=true;return;}
 CSectionMapNode* h=heads[which];
 if(!state[which] || *sizes[which] || h->Parent || h->Left!=h || h->Right!=h) bad=true;
 record(20+which);
}
struct TestManager : CCodeSectionManager { virtual ~TestManager() {} };
static bool run(int a,int b,int c) {
 union Storage { long double align; unsigned char bytes[sizeof(TestManager)]; } storage;
 TestManager* full=new(storage.bytes) TestManager;
 active=full;
 CTBaseSingleton base;
 void* expectedVptr; void* initialVptr; void* finalVptr;
 std::memcpy(&expectedVptr,&base,sizeof(expectedVptr));
 std::memcpy(&initialVptr,storage.bytes,sizeof(initialVptr));
 if(initialVptr==expectedVptr) return false;
 CSectionMapNode sentinel[3],root[3];
 state[0]=a;state[1]=b;state[2]=c;
 maps[0]=&active->ShortNameToHandle;maps[1]=&active->LongNameToHandle;maps[2]=&active->SectionDesc;
 sizes[0]=&active->ShortNameToHandle.Size;sizes[1]=&active->LongNameToHandle.Size;sizes[2]=&active->SectionDesc.Size;
 for(int i=0;i<3;++i) {
  heads[i]=state[i]?sentinel+i:0; roots[i]=root+i;
  sentinel[i].Color=1; sentinel[i].Parent=state[i]==2?roots[i]:0;
  sentinel[i].Left=sentinel[i].Right=state[i]==2?roots[i]:sentinel+i;
  *sizes[i]=state[i]==2?2+i:0;
 }
 active->ShortNameToHandle.Head=heads[0];active->LongNameToHandle.Head=heads[1];active->SectionDesc.Head=heads[2];
 eventCount=0;bad=false;
 active->CCodeSectionManager::~CCodeSectionManager();
 std::memcpy(&finalVptr,storage.bytes,sizeof(finalVptr));
 if(bad || finalVptr!=expectedVptr) return false;
 int at=0;
 for(int i=2;i>=0;--i) {
  if(state[i]==2) { if(at>=eventCount || events[at++]!=10+i) return false; }
  if(state[i]) { if(at>=eventCount || events[at++]!=20+i) return false; }
 }
 return at==eventCount;
}
int main() {
 for(int a=0;a<3;++a) for(int b=0;b<3;++b) for(int c=0;c<3;++c)
  if(!run(a,b,c)) { std::printf("009f1560_TEST FAIL %d%d%d\n",a,b,c); return 1; }
 std::printf("009f1560_TEST PASS\n");return 0;
}