#include <stdio.h>
#include <string.h>
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
template<class T>void PKCStaticQueue<T>::Put(const T&){}
char* _afxPchNil=(char*)"";AssertHook g_pfnAssertHook;static int bad,logged,destroyed,broken;static char text[512];CString& CString::operator<<(const char* p){if(m_pchData==_afxPchNil)text[0]=0;strcat(text,p);m_pchData=text;return *this;}CString& CString::operator<<(long n){char b[32];sprintf(b,"%ld",n);return (*this)<<b;}CString::~CString(){++destroyed;}CDebugBreak::CDebugBreak(){if(destroyed!=broken+1)bad=1;++broken;}static void __fastcall Log(const char* p){++logged;if(strcmp(p,"d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCStaticQueue.h(67) : Nothing in this queue - you should have checked first!\n"))bad=1;}int main(){PKCStaticQueue<CUnprovenStaticQueueWord_00C4BB20> queue;CUnprovenStaticQueueWord_00C4BB20 values[3],input;for(int i=0;i<3;++i)values[i]=(CUnprovenStaticQueueWord_00C4BB20)(100+i);input=(CUnprovenStaticQueueWord_00C4BB20)900;queue.Capacity=3;queue.Entries=values;queue.Used=2;queue.WriteIndex=1;g_pfnAssertHook=Log;if(queue.Take()!=102||queue.Used!=1||queue.WriteIndex!=1)goto fail;if(logged||destroyed||broken||bad)goto fail;queue.Used=0;queue.WriteIndex=0;if(queue.Take()!=100||queue.Used!=0xFFFFFFFFU)goto fail;if(logged!=1||destroyed!=1||broken!=1||bad)goto fail;g_pfnAssertHook=0;queue.Used=0;queue.Take();if(logged!=1||destroyed!=2||broken!=2||bad)goto fail;puts("00c4beb0_TEST PASS");return 0;fail:puts("00c4beb0_TEST FAIL");return 1;}