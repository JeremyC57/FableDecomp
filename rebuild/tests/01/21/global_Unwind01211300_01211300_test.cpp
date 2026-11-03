// agent680 behaviour fixture for 00c0ffe0: the src destructor body pasted unchanged; libpk callees stubbed.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#endif
static unsigned long g_now;static int leaves,breaks,hooks;static char g_text[256];
unsigned long __stdcall GetTickCount(){return g_now;}
char empty_string[1]={0};char* _afxPchNil=empty_string;
unsigned long g_maxProtectionTime;
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
PFNASSERTHOOK g_pfnAssertHook;
class CString{public:char* m_pchData;CString():m_pchData(_afxPchNil){}~CString(){if(m_pchData!=_afxPchNil)free(m_pchData);}
 CString& __fastcall operator<<(const char* text){size_t n=strlen(m_pchData),k=strlen(text);char*p=(char*)malloc(n+k+1);memcpy(p,m_pchData,n);memcpy(p+n,text,k+1);if(m_pchData!=_afxPchNil)free(m_pchData);m_pchData=p;return *this;}
 CString& __fastcall operator<<(long v){char b[32];sprintf(b,"%ld",v);return *this<<(const char*)b;}
 CString& __fastcall operator<<(unsigned long v){char b[32];sprintf(b,"%lu",v);return *this<<(const char*)b;}};
struct CDebugBreak{CDebugBreak(){++breaks;}};
class PKCCriticalSection{public:unsigned char m_section[0x18];void Leave(){++leaves;}};
class PKCProtectionInstance{public:PKCCriticalSection* m_pSection;unsigned long m_tick;~PKCProtectionInstance();};
static void __fastcall Hook(const char* m){++hooks;strncpy(g_text,m,255);}
PKCProtectionInstance::~PKCProtectionInstance()
{
    unsigned long now = GetTickCount();
    unsigned long elapsed = now - m_tick;
    if (elapsed > g_maxProtectionTime) {
        {
            CString line;
            line << ".\\PKCProtectionInstance.cpp" << "(" << (long)19 << ") : " << "Event time (" << elapsed
                 << " ms) over maximum (" << g_maxProtectionTime << " ms)" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    m_pSection->Leave();
}

#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d\n",__LINE__);abort();}}while(0)
int main(){int cases=0;const unsigned long ticks[]={0,1,99,100,101,5000,0xffffffffUL};const unsigned long maxs[]={0,100,0xfffffffeUL};
 for(int hk=0;hk<2;++hk)for(unsigned t=0;t<7;++t)for(unsigned m=0;m<3;++m)for(unsigned s=0;s<7;++s){
  PKCCriticalSection cs;leaves=breaks=hooks=0;g_text[0]=0;g_pfnAssertHook=hk?Hook:0;g_maxProtectionTime=maxs[m];
  {PKCProtectionInstance* p=(PKCProtectionInstance*)malloc(sizeof(PKCProtectionInstance));p->m_pSection=&cs;p->m_tick=ticks[s];g_now=ticks[t];p->~PKCProtectionInstance();free(p);}
  unsigned long el=ticks[t]-ticks[s];bool over=el>maxs[m];
  CHECK(leaves==1);CHECK(breaks==(over?1:0));CHECK(hooks==((over&&hk)?1:0));
  if(over&&hk){char want[256];sprintf(want,".\\PKCProtectionInstance.cpp(19) : Event time (%lu ms) over maximum (%lu ms)\n",el,maxs[m]);CHECK(strcmp(want,g_text)==0);}
  ++cases;}
 CHECK(cases==294);puts("01211300_TEST PASS");return 0;}