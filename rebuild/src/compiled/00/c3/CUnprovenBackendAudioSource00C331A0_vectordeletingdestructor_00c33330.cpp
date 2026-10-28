// CUnprovenBackendAudioSource_00C331A0::vector deleting destructor(flags) -- destroy the object or array and optionally release its allocation.
// Leaf: ??_ECUnprovenBackendAudioSource_00C331A0@@UAEPAXI@Z
// Flags: /EHsc
// Identity: descriptive owner, proven compiler-helper role; Own vector deleting target flags2 array arm calls compiler EH-vector destructor00bfeed8 with exact stride64, own plain destructor00c331a0, array cookie count and adjusted allocation pointer; flags1 invokes original custom deallocator00c3c370. Exact target is compiler generated from ordinary virtual destructor plus array new; no hand-written flags/iteration wrapper. Complete original object5868 -> CPooledNode8778 chain proven node full dtor4cf40 final5868store. Additional counted-node5860/RefCount0c -> audio source4f74/seven-word descriptor10..28/Flags2c -> backend source4f84/token30..3f. Counted dtor3c5c0 tailcallsnode; resource33120 cleanup3c450 then counted dtor; backend331a0 cleanup3c720 then resource33120. Descriptor is not WAVEFORMATEX; Format full32-bit incl80000000 and Channels+4, Bytes+c, Data+18 are own observed. Token handle/flags proven OpenAL helper4d9b0.
// Deallocation: scalar and array class operators inline to the same observed custom allocator routine00c3c370; FreeBlock_00c3c370 is descriptive. This avoids giving one retail callee four incompatible class-operator identities. Original class ownership of allocator APIs remains unproven.
// Layout: complete observed bases and typed data preserve retail size. Every unrecovered library type/member name explicit; ordinary C++ new[] makes the actual helper materialize. No applicable shared engine header.
#ifndef _MSC_VER
#define __forceinline inline
#endif
void FreeBlock_00c3c370(void*);
class CUnprovenLibraryObject_012B5868{public:virtual ~CUnprovenLibraryObject_012B5868();};
class CPooledNode:public CUnprovenLibraryObject_012B5868{public:CPooledNode* Prev;CPooledNode* Next;CPooledNode();virtual ~CPooledNode();};
class CUnprovenCountedPooledNode_00C3C590:public CPooledNode{public:unsigned int RefCount;CUnprovenCountedPooledNode_00C3C590();virtual ~CUnprovenCountedPooledNode_00C3C590();virtual void Cleanup();};
struct CUnprovenAudioDescriptor28{unsigned int Format,Channels,Rate,Bytes,Unproven10,Unproven14;void* Data;};
class CUnprovenAudioSource:public CUnprovenCountedPooledNode_00C3C590{public:CUnprovenAudioDescriptor28 Descriptor;unsigned int Flags;CUnprovenAudioSource();virtual ~CUnprovenAudioSource();virtual void Cleanup();virtual int Load(const CUnprovenAudioDescriptor28*);virtual int Read(void*);};
struct CUnprovenAudioBackendToken16{unsigned int Handle,Unproven04,Flags,Unproven0C;};
class CUnprovenBackendAudioSource_00C331A0:public CUnprovenAudioSource{public:CUnprovenAudioBackendToken16 BackendToken;CUnprovenBackendAudioSource_00C331A0();virtual ~CUnprovenBackendAudioSource_00C331A0();virtual void Cleanup();virtual int Load(const CUnprovenAudioDescriptor28*);static __forceinline void operator delete(void* p){FreeBlock_00c3c370(p);}static __forceinline void operator delete[](void* p){FreeBlock_00c3c370(p);}};
#ifdef _MSC_VER
typedef char ExactDescriptor[sizeof(CUnprovenAudioDescriptor28)==28?1:-1];typedef char ExactSource[sizeof(CUnprovenAudioSource)==48?1:-1];typedef char ExactBackendSource[sizeof(CUnprovenBackendAudioSource_00C331A0)==64?1:-1];
#endif
CUnprovenBackendAudioSource_00C331A0* ForceArray_00c33330(unsigned int n){return new CUnprovenBackendAudioSource_00C331A0[n];}