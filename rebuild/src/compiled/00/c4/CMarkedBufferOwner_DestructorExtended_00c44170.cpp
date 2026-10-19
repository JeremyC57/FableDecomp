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