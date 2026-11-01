// agent680 behaviour fixture for 00c73660: src body pasted unchanged; Win32 version APIs stubbed.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __stdcall
#endif
struct _OSVERSIONINFOA { unsigned long dwOSVersionInfoSize, dwMajorVersion, dwMinorVersion, dwBuildNumber, dwPlatformId; char szCSDVersion[128]; };
static unsigned long g_ver, g_exBuild; static int exCalls;
unsigned long __stdcall GetVersion(void){return g_ver;}
int __stdcall GetVersionExA(_OSVERSIONINFOA* p){++exCalls; if(p->dwOSVersionInfoSize!=sizeof(_OSVERSIONINFOA)) abort(); p->dwBuildNumber=g_exBuild; return 1;}
unsigned char g_defStringInit; bool g_isWin32s,g_isWindows95,g_isWindows98,g_isWindows98SE,g_isWindowsMe,g_isWindowsNT3,g_isWindowsNT4,g_isWindows2000,g_isWindowsXP;
unsigned long g_windowsBuildNumber,g_windowsMajorVersion,g_windowsMinorVersion;
void DetectWindowsVersion()
{
    unsigned long version = GetVersion();
    g_windowsMajorVersion = (unsigned char)version;
    g_windowsMinorVersion = (unsigned char)((unsigned short)version >> 8);

    if (version < 0x80000000) {
        // Windows NT family
        g_windowsBuildNumber = version >> 16;
        switch (g_windowsMajorVersion) {
        case 3:
            g_isWindowsNT3 = true;
            break;
        case 4:
            g_isWindowsNT4 = true;
            break;
        case 5:
            if (g_windowsMinorVersion == 0)
                g_isWindows2000 = true;
            else if (g_windowsMinorVersion == 1)
                g_isWindowsXP = true;
            break;
        }
    } else if (g_windowsMajorVersion < 4) {
        g_windowsBuildNumber = (version >> 16) & 0x7FFF;
        g_isWin32s = true;
    } else {
        _OSVERSIONINFOA info;
        info.dwOSVersionInfoSize = sizeof(info);
        GetVersionExA(&info);
        g_windowsBuildNumber = (unsigned short)info.dwBuildNumber;
        if (g_windowsMinorVersion == 0)
            g_isWindows95 = true;
        else if (g_windowsMinorVersion <= 10 && g_windowsBuildNumber <= 2222) {
            if (g_windowsBuildNumber == 2222)
                g_isWindows98SE = true;
            else
                g_isWindows98 = true;
        } else
            g_isWindowsMe = true;
    }
    g_defStringInit = 1;
}

#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d ver %08lx\n",__LINE__,g_ver);abort();}}while(0)
static void reset(){g_defStringInit=0;g_isWin32s=g_isWindows95=g_isWindows98=g_isWindows98SE=g_isWindowsMe=g_isWindowsNT3=g_isWindowsNT4=g_isWindows2000=g_isWindowsXP=false;g_windowsBuildNumber=g_windowsMajorVersion=g_windowsMinorVersion=0xdeadbeef;exCalls=0;}
int main(){int cases=0;
 const unsigned long majors[]={2,3,4,5,6},minors[]={0,1,2,10,11,90},builds[]={0,950,1998,2222,2223,0x7fff,0xffff},exb[]={0x04000950UL,0x040A0898UL,0x040A08AEUL,0x040A08AFUL,0x045A0BB8UL};
 for(int nt=0;nt<2;++nt)for(unsigned a=0;a<5;++a)for(unsigned b=0;b<6;++b)for(unsigned c=0;c<7;++c)for(unsigned e=0;e<5;++e){
  reset();g_ver=(builds[c]<<16)|(minors[b]<<8)|majors[a]|(nt?0:0x80000000UL);g_exBuild=exb[e];
  DetectWindowsVersion();unsigned long M=majors[a],m=minors[b];
  CHECK(g_defStringInit==1);CHECK(g_windowsMajorVersion==M);CHECK(g_windowsMinorVersion==m);
  int flags=g_isWin32s+g_isWindows95+g_isWindows98+g_isWindows98SE+g_isWindowsMe+g_isWindowsNT3+g_isWindowsNT4+g_isWindows2000+g_isWindowsXP;
  if(g_ver<0x80000000UL){CHECK(exCalls==0);CHECK(g_windowsBuildNumber==builds[c]);
   bool want3=M==3,want4=M==4,w2k=M==5&&m==0,wxp=M==5&&m==1;
   CHECK(g_isWindowsNT3==want3&&g_isWindowsNT4==want4&&g_isWindows2000==w2k&&g_isWindowsXP==wxp);CHECK(flags==(int)(want3+want4+w2k+wxp));}
  else if(M<4){CHECK(exCalls==0);CHECK(g_windowsBuildNumber==((g_ver>>16)&0x7fff));CHECK(g_isWin32s&&flags==1);}
  else{CHECK(exCalls==1);unsigned long bn=exb[e]&0xffff;CHECK(g_windowsBuildNumber==bn);CHECK(flags==1);
   if(m==0)CHECK(g_isWindows95);else if(m<=10&&bn<=2222)CHECK(bn==2222?g_isWindows98SE:g_isWindows98);else CHECK(g_isWindowsMe);}
  ++cases;}
 CHECK(cases==2100);puts("00c73660_TEST PASS");return 0;}