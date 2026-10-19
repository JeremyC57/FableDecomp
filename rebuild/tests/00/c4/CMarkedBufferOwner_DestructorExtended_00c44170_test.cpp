#ifndef _MSC_VER
#define __stdcall
#define __cdecl
#endif
// CMarkedBufferOwner::Close() -- close its resource, release optional registration and owned memory.
// Retail00c44170 182bytes. Manifest Destructor_Extended is a propagated label: no vptr reset/deallocation.
// Owner IsOpen virtualslot2 audited in retail12b5a88; shared by12b5ab0/12b7b88.
// Address-named native views: no PDB header/RTTI mapping for this library.
struct CMarkedBufferOwner;struct CResourceLink;
struct CResourceLink {
 virtual ~CResourceLink();
 // Concrete native tables012b5c10/5c60/5cd0: dtor slot0; attach C596E0; stop C59710.
 virtual long AttachOwner(CMarkedBufferOwner*);
 virtual void Stop();
 virtual long DetachOwner(CMarkedBufferOwner*); // slot3, owner argument and status return audited at both call sites
};
struct CResourceManager {void Release(CResourceLink*);}; //00c4d070
extern CResourceManager* gResourceManager; //0143bb5c
struct CErrorState {void Report(long);};extern CErrorState gErrorState;//0143bb58;Report00c3d090
struct CReferenceLease {void Release();}; //00c3c5f0: decrement +c when nonzero
struct CMarkedBuffer {void* Block;void* Cursor;long Count;long Used;void* Marks;long MarkCount;
 bool Allocate(long); //00c44360, allocates12 bytes per item
 void Release(); //00c44550
};
typedef void (__stdcall* PFNMARK)(CMarkedBufferOwner*,void*,void*);
struct CRegistrationList {long Remove(long);}; //00c5ff60
struct CRegistrationContext {unsigned char ContextPrefix[0xe4];CRegistrationList* Registrations;};
extern CRegistrationContext* gRegistrationContext; //0143bb70
void __cdecl ReleaseOwnedMemory(void*); //00c3c370 calls configured free callback
struct CMarkedBufferOwner {
 virtual ~CMarkedBufferOwner(); //slot0 retail00c3cfc0
 virtual bool IsAllocated(); //slot1 retail00c3cfa0
 virtual bool IsOpen(); //slot2 retail00c3cfb0: Handle!=-1
 virtual bool Initialise(void*); //slot3 retail00c42dd0: parameter pointer, bool result
 virtual void Close(); //slot4 retail00c44170
 void* Previous;void* Next;long Handle;unsigned long Flags;
 unsigned char OwnerState[0x3c];CResourceLink* Resource;CMarkedBuffer Buffer;
 unsigned char BufferState[0xc];CReferenceLease* Lease;
 unsigned char OpenState[0x58];PFNMARK MarkCallback;void* MarkContext;
 void* StateCallback;void* StateContext;unsigned char RegistrationState[0x10];void* OwnedMemory;long RegistrationID;
 void Shutdown(); //00c43810
 void Cleanup(); //00c436c0
 bool AllocateMarks(long,PFNMARK,void*);
};

void CMarkedBufferOwner::Close(){
 if(IsOpen()){
  StateCallback=0;
  if(Buffer.Count)Shutdown();
  Cleanup();
  if(Lease){Lease->Release();Lease=0;
   if(Resource&&Resource->DetachOwner(this)){gResourceManager->Release(Resource);Resource=0;}
  }
  if(Resource){gResourceManager->Release(Resource);Resource=0;}
  Handle=-1;
 }
 if(Handle&0x2000){
  if(gRegistrationContext)gRegistrationContext->Registrations->Remove(RegistrationID);
  if(OwnedMemory){ReleaseOwnedMemory(OwnedMemory);OwnedMemory=0;}
 }
}

#include <stdio.h>
#include <stddef.h>
static bool ok=true,openValue,allocateValue;static int events[50],eventCount,bindCount;static long bindResult[4];
static CMarkedBufferOwner* observed;static CResourceLink* allocateResource;static CResourceLink* replacement;
static CResourceLink* released;static long reported,removed;static void* freed;static long allocationSize;
static void log(int v){events[eventCount++]=v;}
CResourceLink::~CResourceLink(){} long CResourceLink::AttachOwner(CMarkedBufferOwner*){return 0;}void CResourceLink::Stop(){}
long CResourceLink::DetachOwner(CMarkedBufferOwner* owner){ok=ok&&owner==observed;log(40);if(replacement){owner->Resource=replacement;replacement=0;}return bindResult[bindCount++];}
CMarkedBufferOwner::~CMarkedBufferOwner(){}bool CMarkedBufferOwner::Initialise(void*){return true;}bool CMarkedBufferOwner::IsAllocated(){return true;}bool CMarkedBufferOwner::IsOpen(){log(1);return openValue;}
void CMarkedBufferOwner::Shutdown(){log(10);}
void CMarkedBufferOwner::Cleanup(){log(20);}
void CReferenceLease::Release(){log(30);}
void CResourceManager::Release(CResourceLink* r){released=r;log(50);}
void CErrorState::Report(long e){reported=e;log(80);}
bool CMarkedBuffer::Allocate(long count){allocationSize=count;log(60);if(allocateValue)observed->Resource=allocateResource;return allocateValue;}
void CMarkedBuffer::Release(){log(70);}
long CRegistrationList::Remove(long id){removed=id;log(90);return 0;}
void __cdecl ReleaseOwnedMemory(void* p){freed=p;log(100);}
static CResourceManager manager;CResourceManager* gResourceManager=&manager;CErrorState gErrorState;
static CRegistrationList registrations;static CRegistrationContext registrationContext;CRegistrationContext* gRegistrationContext=&registrationContext;
static void __stdcall callback(CMarkedBufferOwner*,void*,void*){}
static void init(CMarkedBufferOwner& o){observed=&o;eventCount=bindCount=0;released=0;reported=0;removed=0;freed=0;replacement=0;openValue=true;allocateValue=true;allocateResource=0;allocationSize=-1;
 o.Handle=7;o.Flags=0;o.Buffer.Count=0;o.Resource=0;o.Lease=0;o.MarkCallback=0;o.MarkContext=0;o.StateCallback=&o;o.OwnedMemory=0;o.RegistrationID=19;
 registrationContext.Registrations=&registrations;gRegistrationContext=&registrationContext;for(int i=0;i<4;++i)bindResult[i]=0;
 if(sizeof(void*)==4)ok=ok&&((char*)&o.Resource-(char*)&o)==0x50&&((char*)&o.Buffer-(char*)&o)==0x54&&((char*)&o.Lease-(char*)&o)==0x78&&((char*)&o.MarkCallback-(char*)&o)==0xd4&&((char*)&o.StateCallback-(char*)&o)==0xdc&&((char*)&o.OwnedMemory-(char*)&o)==0xf4;
}
static bool sequence(const int* expected,int n){if(eventCount!=n)return false;for(int i=0;i<n;++i)if(events[i]!=expected[i])return false;return true;}

int main(){CMarkedBufferOwner o;CResourceLink r,swapped;CReferenceLease lease;int memory;
 init(o);o.Buffer.Count=2;o.Lease=&lease;o.Resource=&r;bindResult[0]=1;o.OwnedMemory=&memory;
 o.Close();int expected[]={1,10,20,30,40,50,90,100};ok=ok&&sequence(expected,8)&&o.StateCallback==0&&o.Resource==0&&o.Lease==0&&o.Handle==-1&&o.OwnedMemory==0&&released==&r&&removed==19&&freed==&memory;
 init(o);o.Lease=&lease;o.Resource=&r;bindResult[0]=0;o.Close();int kept[]={1,20,30,40,50,90};ok=ok&&sequence(kept,6)&&released==&r&&o.Resource==0;
 init(o);openValue=false;o.Handle=0;o.OwnedMemory=&memory;o.Close();int closed[]={1};ok=ok&&sequence(closed,1)&&o.StateCallback==&o&&o.OwnedMemory==&memory;
 init(o);openValue=false;o.Handle=0x2000;o.OwnedMemory=&memory;o.Close();int flagged[]={1,90,100};ok=ok&&sequence(flagged,3)&&o.Handle==0x2000&&o.StateCallback==&o&&o.OwnedMemory==0;
 init(o);openValue=false;o.Handle=0x2000;o.OwnedMemory=&memory;gRegistrationContext=0;o.Close();int absent[]={1,100};ok=ok&&sequence(absent,2)&&o.OwnedMemory==0;
 init(o);o.Lease=&lease;o.Resource=&r;replacement=&swapped;bindResult[0]=1;o.Close();ok=ok&&released==&swapped;
 printf(ok?"00c44170_TEST PASS\n":"00c44170_TEST FAIL\n");return ok?0:1;}