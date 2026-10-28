#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
extern char* _afxPchNil;typedef void(__fastcall* AssertHook)(const char*);extern AssertHook g_pfnAssertHook;
class CString{public:char* m_pchData;CString():m_pchData(_afxPchNil){}~CString();CString& __fastcall operator<<(const char*);CString& __fastcall operator<<(long);};struct CDebugBreak{CDebugBreak();};
enum CUnprovenStaticQueueWord_00C49E30{CUnprovenStaticQueueWord_00C49E30_UnprovenMin=-2147483647-1,CUnprovenStaticQueueWord_00C49E30_UnprovenMax=2147483647};
template<class T>class CUnprovenStaticQueueInterface{public:virtual T Take()=0;virtual void Put(const T&)=0;virtual unsigned int Count()=0;virtual int IsFull()=0;};
template<class T>class PKCStaticQueue:public CUnprovenStaticQueueInterface<T>{public:unsigned int Capacity;T* Entries;unsigned int Used,WriteIndex;virtual T Take();virtual void Put(const T&);virtual unsigned int Count(){return Used;}virtual int IsFull(){return Used>=Capacity;}
#ifdef _MSC_VER
__declspec(noinline)
#endif
int OldestIndex();};
template<class T>int PKCStaticQueue<T>::OldestIndex(){int index=(int)(WriteIndex-Used);if(index<0)index+=Capacity;return index;}
template<class T>void PKCStaticQueue<T>::Put(const T& input){if(Used>=Capacity){{CString text;text << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCStaticQueue.h" << "(" << (long)57 << ") : " << "This queue is full!" << "\n";if(g_pfnAssertHook)g_pfnAssertHook(text.m_pchData);}CDebugBreak();}Entries[WriteIndex]=input;WriteIndex=(WriteIndex+1)%Capacity;++Used;}
template void PKCStaticQueue<CUnprovenStaticQueueWord_00C49E30>::Put(const CUnprovenStaticQueueWord_00C49E30&);
template<class T>T PKCStaticQueue<T>::Take(){return (T)0;}
char* _afxPchNil=(char*)"";AssertHook g_pfnAssertHook;static int bad,logged,destroyed,broken;static char text[512];CString& CString::operator<<(const char* p){if(m_pchData==_afxPchNil)text[0]=0;strcat(text,p);m_pchData=text;return *this;}CString& CString::operator<<(long n){char b[32];sprintf(b,"%ld",n);return (*this)<<b;}CString::~CString(){++destroyed;}CDebugBreak::CDebugBreak(){if(destroyed!=broken+1)bad=1;++broken;}static void __fastcall Log(const char* p){++logged;if(strcmp(p,"d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCStaticQueue.h(57) : This queue is full!\n"))bad=1;}int main(){PKCStaticQueue<CUnprovenStaticQueueWord_00C49E30> queue;CUnprovenStaticQueueWord_00C49E30 values[3],input;for(int i=0;i<3;++i)values[i]=(CUnprovenStaticQueueWord_00C49E30)(100+i);input=(CUnprovenStaticQueueWord_00C49E30)900;queue.Capacity=3;queue.Entries=values;queue.Used=0;queue.WriteIndex=1;g_pfnAssertHook=Log;queue.Put(input);if(values[1]!=900||queue.WriteIndex!=2||queue.Used!=1)goto fail;queue.Put(input);if(queue.WriteIndex||queue.Used!=2)goto fail;if(logged||destroyed||broken||bad)goto fail;queue.Used=3;queue.WriteIndex=0;queue.Put(input);if(queue.Used!=4||queue.WriteIndex!=1||values[0]!=900)goto fail;if(logged!=1||destroyed!=1||broken!=1||bad)goto fail;g_pfnAssertHook=0;queue.Used=3;queue.Put(input);if(logged!=1||destroyed!=2||broken!=2||bad)goto fail;puts("00c49e30_TEST PASS");return 0;fail:puts("00c49e30_TEST FAIL");return 1;}