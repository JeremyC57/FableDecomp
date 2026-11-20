// PKCFrallocStatic<CSoundEvent>::Return(object) -- return a pool object to the free-pointer ring and report overflow.
// Leaf: ?Return@?$PKCFrallocStatic@VCSoundEvent@@@@UAEXPAVCSoundEvent@@@Z
// NF188 (2026-11-20, q12165): the element is CSoundEvent (forward-declared only; the class key V of the shared libpk/LHAudioEventPosition.h
//   and libpk/CSoundEventGroup.h). Its pool constructor 0x00C239B0 (warnings from libpk\PKCFrallocStatic.h lines 63/65) builds the
//   0xB4-byte element table with __ehvec_ctor over 0x00C21BA0 / 0x00C207F0 (CSoundEvent ctor / ~CSoundEvent: vtable 0x012B3674, which
//   0x00C207F0 binds as ??_7CSoundEvent@@6B@) and hands every element to this Return; the landed accessor 0x00C13620 is
//   PKCFrallocStatic<CSoundEvent>::Get. Was an address-named placeholder element class.
// Flags: /EHsc
// Identity: probable; own literal d:\dev\audio\ver_fable_1.5_b\libpk\PKCFrallocStatic.h line85 with Overflow; own two-slot table Return atslot0 and Take00C23BE0 atslot1 (second entry is a real dequeue method, not destructor); actual Take decrementsAvailableCount and returns pointer fromFreeList+14. Return writes incomingpointer to ring, wrapsWriteIndex moduloCapacity, incrementsAvailableCount even after warning. Original element and interface/base identity and method names unproven; explicit distinct descriptive pointerelement types, original filename establishes probable template owner.
// Layout: direct observed two-slot polymorphic pool projection; no original base is proven or invented, no fake destructor slot. Source-proven pointer storage, count/capacity/head and underlying object storage; no pointed element size accessed. All fields/base/API spellings except filename owner are descriptive. Scoped actual CString/assert-hook/debug-break family preserves EH lifetime. No shared libpk header.
#ifndef _MSC_VER
#define __fastcall
#endif
extern char* _afxPchNil;typedef void(__fastcall* AssertHook)(const char*);extern AssertHook g_pfnAssertHook;
class CString{public:char* m_pchData;CString():m_pchData(_afxPchNil){}~CString();CString& __fastcall operator<<(const char*);CString& __fastcall operator<<(long);};struct CDebugBreak{CDebugBreak();};
class CSoundEvent;template<class T>class PKCFrallocStatic{public:unsigned long Capacity,AvailableCount,WriteIndex;T* Storage;T** FreeList;virtual void Return(T*);virtual T* Take();};template<class T>void PKCFrallocStatic<T>::Return(T* object){if(AvailableCount>=Capacity){{CString text;text << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h" << "(" << (long)85 << ") : " << "Overflow" << "\n";if(g_pfnAssertHook)g_pfnAssertHook(text.m_pchData);}CDebugBreak();}FreeList[WriteIndex]=object;WriteIndex=(WriteIndex+1)%Capacity;++AvailableCount;}
template void PKCFrallocStatic<CSoundEvent>::Return(CSoundEvent*);