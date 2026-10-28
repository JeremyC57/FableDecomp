// PKCStaticQueue<CUnprovenStaticQueueWord_00C4BB20>::Take() -- take the oldest ring entry and report an invalid queue state.
// Leaf: ?Take@?$PKCStaticQueue@W4CUnprovenStaticQueueWord_00C4BB20@@@@UAE?AW4CUnprovenStaticQueueWord_00C4BB20@@XZ
// Flags: /EHsc
// Identity: probable template owner from own assertion filename; Own filename PKCStaticQueue.h line67 and exact message Nothing in this queue - you should have checked first!. Concrete four-slot table012b8364 has Take/Put/Count/IsFull, no virtual destructor; separate four-pure interface012b8374 and constructor establish actual polymorphic base. Original constructor allocates capacity*4 and stores Capacity4/Entries8/Used12/WriteIndex16. Real OldestIndex00c4bf80 computes signed WriteIndex-Used, adds Capacity if negative. Actual element identity unknown; explicit unknown scalar32 representation uses a full-range enum projection, retaining proven raw EAX return ABI; neither original element kind nor enumerator identities are claimed.
// Layout: genuine four-slot abstract queue interface and real typed ring storage. Unknown original library base/element/method names remain descriptive projections; no applicable shared engine header. Full CString diagnostic lifetime and unmodified debug-break ordering preserved.
#ifndef _MSC_VER
#define __fastcall
#endif
extern char* _afxPchNil;typedef void(__fastcall* AssertHook)(const char*);extern AssertHook g_pfnAssertHook;
class CString{public:char* m_pchData;CString():m_pchData(_afxPchNil){}~CString();CString& __fastcall operator<<(const char*);CString& __fastcall operator<<(long);};struct CDebugBreak{CDebugBreak();};
enum CUnprovenStaticQueueWord_00C4BB20{CUnprovenStaticQueueWord_00C4BB20_UnprovenMin=-2147483647-1,CUnprovenStaticQueueWord_00C4BB20_UnprovenMax=2147483647};
template<class T>class CUnprovenStaticQueueInterface{public:virtual T Take()=0;virtual void Put(const T&)=0;virtual unsigned int Count()=0;virtual int IsFull()=0;};
template<class T>class PKCStaticQueue:public CUnprovenStaticQueueInterface<T>{public:unsigned int Capacity;T* Entries;unsigned int Used,WriteIndex;virtual T Take();virtual void Put(const T&);virtual unsigned int Count(){return Used;}virtual int IsFull(){return Used>=Capacity;}
#ifdef _MSC_VER
__declspec(noinline)
#endif
int OldestIndex();};
template<class T>int PKCStaticQueue<T>::OldestIndex(){int index=(int)(WriteIndex-Used);if(index<0)index+=Capacity;return index;}
template<class T>T PKCStaticQueue<T>::Take(){if(!Used){{CString text;text << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCStaticQueue.h" << "(" << (long)67 << ") : " << "Nothing in this queue - you should have checked first!" << "\n";if(g_pfnAssertHook)g_pfnAssertHook(text.m_pchData);}CDebugBreak();}int index=OldestIndex();--Used;return Entries[index];}
template CUnprovenStaticQueueWord_00C4BB20 PKCStaticQueue<CUnprovenStaticQueueWord_00C4BB20>::Take();