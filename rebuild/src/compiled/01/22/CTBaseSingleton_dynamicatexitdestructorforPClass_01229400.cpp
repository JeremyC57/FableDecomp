// `dynamic atexit destructor for 'CTBaseSingleton<CSkeletalMorphResourceManager>::PClass'' -- releases the singleton's static counted pointer at exit: the inline ~CCountedPointer (ReleaseData)
// Leaf: _$E2
// Symbol: VC7.1 names the atexit destructor of a template static data member's definition with the TU-local `_$E2` (its
//   dynamic initializer is `_$E1`), not a public ??__F name, so the function has no linkable name in retail either.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01229400, oracle length 57 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe).
//
// Retail bytes:  mov eax,[0x13bab14] ; push esi ; xor esi,esi ; cmp eax,esi ; je Z ; dec dword ptr [eax] ; mov eax,[0x13bab14] ; cmp [eax],esi ; jne Z ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[0x13bab14] ; push eax ; call operator delete ; add esp,4 ; Z: mov [0x13bab10],esi ; mov [0x13bab14],esi ; pop esi ; ret
// The static CCountedPointer {Data @ 0x013BAB10, Info @ 0x013BAB14} is CTBaseSingleton<CSkeletalMorphResourceManager>::PClass
// (NF41 entry e167; the landed CWorld::Uninitialise 0x004A2E90 reaches it through CTBaseSingleton::Get()). Its dynamic initializer
// 0x0121E180 (`push 01229400h ; call atexit`) registers this function, so the row is the compiler-generated atexit destructor of
// the static's definition: the inline ~CCountedPointer decrements the info block, destroys the object through the block's
// function pointer and frees the block (operator delete @ 0x00BFE9BC) on the last reference, then nulls both words.
// NF52 (round 7): was a made-up Release_013bab10() over local SCountBlock / CCountedPointerStatic models and the made-up global
//   g_pointer_013bab10; now the definition of the shared engine/CTBaseSingleton.h static (engine/CCountedPointer.h), explicitly
//   instantiated for CSkeletalMorphResourceManager.
#include "engine/CTBaseSingleton.h"
class CSkeletalMorphResourceManager;

template<class T> CCountedPointer<T> CTBaseSingleton<T>::PClass;
template CCountedPointer<CSkeletalMorphResourceManager> CTBaseSingleton<CSkeletalMorphResourceManager>::PClass;
