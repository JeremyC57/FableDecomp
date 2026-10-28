// CListenerState::vector deleting destructor(flags) -- destroy the object or array and optionally release its allocation.
// Leaf: ??_ECListenerState@@UAEPAXI@Z
// Flags: /EHsc
// Identity: descriptive owner, proven compiler-helper role; Own vector deleting target flags2 array arm calls compiler EH-vector destructor00bfeed8 with exact stride128, own plain destructor00c3cec0, array cookie count and adjusted allocation pointer; flags1 invokes original custom deallocator00c3c370. Exact target is compiler generated from ordinary virtual destructor plus array new; no hand-written flags/iteration wrapper. Complete original object5868 -> CPooledNode8778 chain proven node full dtor4cf40 final5868store. Listener constructor3cea0 callsnode; dtor3cec0 cleanup40800 thennode4cf40. Full original128-byte typed listener model shared with N310/N311; custom class deallocation declarations bind actual common free address. Inactive original allocator member ownership/name not claimed.
// Deallocation: scalar and array class operators inline to the same observed custom allocator routine00c3c370; FreeBlock_00c3c370 is descriptive. This avoids giving one retail callee four incompatible class-operator identities. Original class ownership of allocator APIs remains unproven.
// Layout: complete observed bases and typed data preserve retail size. Every unrecovered library type/member name explicit; ordinary C++ new[] makes the actual helper materialize. No applicable shared engine header.
#ifndef _MSC_VER
#define __forceinline inline
#endif
void FreeBlock_00c3c370(void*);
class CUnprovenLibraryObject_012B5868{public:virtual ~CUnprovenLibraryObject_012B5868();};
class CPooledNode:public CUnprovenLibraryObject_012B5868{public:CPooledNode* Prev;CPooledNode* Next;CPooledNode();virtual ~CPooledNode();};
class CUnprovenFloat3_00C40970{public:float X,Y,Z;};
class CListenerState:public CPooledNode{public:CUnprovenFloat3_00C40970 Vector0C,Vector18;int Integer24,Integer28,Integer2C;float Float30,Float34,Float38;int Integer3C;float Scalar40;int Integer44;float Scalar48,Scalar4C,Scalar50,Scalar54,Scalar58,Scalar5C,Scalar60,Scalar64;int Integer68,Integer6C;unsigned int UnprovenWord70;int Integer74;unsigned long DirtyFlags;void* EffectHandle;CListenerState();virtual ~CListenerState();void SetVector0C(const CUnprovenFloat3_00C40970*);void SetVector18(const CUnprovenFloat3_00C40970*);void SetInteger44(int);void SetInteger6C(int);static __forceinline void operator delete(void* p){FreeBlock_00c3c370(p);}static __forceinline void operator delete[](void* p){FreeBlock_00c3c370(p);}};
#ifdef _MSC_VER
typedef char NodeSize[sizeof(CPooledNode)==12?1:-1];typedef char ParameterSize[sizeof(CListenerState)==128?1:-1];
#endif
CListenerState* ForceArray_00c3d0f0(unsigned int n){return new CListenerState[n];}