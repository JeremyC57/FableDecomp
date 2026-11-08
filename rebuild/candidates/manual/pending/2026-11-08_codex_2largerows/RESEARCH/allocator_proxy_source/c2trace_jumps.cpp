// Local diagnostic backend proxy. Never used to grade or ship a reconstructed row.
// Hooks follow the addresses documented in C2_ALLOCATOR_PROBE.md for RTM 3077.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned long DWORD;
typedef long LONG;
struct Context32 {
    DWORD flags, debug[6]; unsigned char floating[112];
    DWORD gs,fs,es,ds,edi,esi,ebx,edx,ecx,eax,ebp,eip,cs,eflags,esp,ss;
    unsigned char extended[512];
};
struct ExceptionRecord { DWORD code,flags; void* nested; void* address; DWORD count; DWORD info[15]; };
struct ExceptionPointers { ExceptionRecord* record; Context32* context; };
extern "C" {
__declspec(dllimport) void* __stdcall LoadLibraryA(const char*);
__declspec(dllimport) void* __stdcall GetProcAddress(void*,const char*);
__declspec(dllimport) int __stdcall VirtualProtect(void*,DWORD,DWORD,DWORD*);
__declspec(dllimport) void* __stdcall AddVectoredExceptionHandler(DWORD,LONG (__stdcall*)(ExceptionPointers*));
__declspec(dllimport) DWORD __stdcall GetCurrentProcessId();
}
struct Hook { DWORD address; unsigned char original; };
static Hook hooks[]={ {0x1077c08e,0}, {0x10783b6b,0}, {0x10782a0d,0}, {0x107834a4,0}, {0x1077fae3,0}, {0x1077fbf7,0} };
static Hook* pending=0;
static FILE* logFile=0;
static bool selected=false;
static DWORD lastCandidate=0;
extern "C" __declspec(dllexport) void __stdcall AbortCompilerPass(void* value){
    void* d=LoadLibraryA(getenv("C2_TRACE_ORIGINAL"));
    void (__stdcall* f)(void*)=(void (__stdcall*)(void*))GetProcAddress(d,"_AbortCompilerPass@4");
    if(f)f(value);
}
extern "C" int __stdcall TraceGetObjHandler(void* a,void* b,void* c){
    void* d=LoadLibraryA(getenv("C2_TRACE_ORIGINAL"));
    int (__stdcall* f)(void*,void*,void*)=(int (__stdcall*)(void*,void*,void*))GetProcAddress(d,"DllGetObjHandler");
    return f?f(a,b,c):0x80004005;
}
static DWORD read32(DWORD p){return *(DWORD*)p;}
static void candidate(DWORD c,const char* event){
    if (!c)return;
    __try {
        DWORD leaf=read32(c); unsigned char kind=*(unsigned char*)(leaf+4);
        const char* name="?";
        if(kind==4||kind==5){DWORD st=read32(leaf);if(st)name=(const char*)read32(st+4);}
        DWORD desc=read32(c+0x10);
        fprintf(logFile,"%s id=%lu name=%s kind=%u prio=%ld key=%lu spill=%ld refs=%lu reg=%lu ptr=%08lx\n",
                event,read32(c+0x1c),name?name:"?",kind,(long)read32(c+0xc),read32(c+0x44),
                (long)read32(c+0x40),read32(c+0x24),desc?read32(desc+0x1c):0,c);
    } __except(1) { fprintf(logFile,"UNREADABLE %08lx\n",c); }
}
struct SavedRegisters { DWORD edi,esi,ebp,esp,ebx,edx,ecx,eax,flags; };
extern "C" void __cdecl traceHook(unsigned index,SavedRegisters* c){
    if(index==0){
        const char* name=(const char*)read32(read32(c->eax)+4);
        const char* filter=getenv("C2_TRACE_FUNCTION");
        selected=name&&filter&&strstr(name,filter)!=0;
        if(selected)fprintf(logFile,"FUNCTION %s\n",name);
    }else if(selected){
        if(index==4 || index==5){
            DWORD candidatePointer=index==4?c->esi:c->ecx;
            fprintf(logFile,"REF kind=%s id=%lu line=%lu tuple=%lu\n",index==4?"DEF":"USE",read32(candidatePointer+0x1c),read32(0x108a9114),c->eax);
        } else if(index==1){
            fprintf(logFile,"SORTED class=%lu\n",c->ebx);
            DWORD p=read32(0x108807b4);unsigned lim=0;
            while(p&&++lim<10000){candidate(p,"CAND");p=read32(p+0x14);}
        }else if(index==2){candidate(c->ebp,"CHOOSE");}
        else{
            DWORD p=read32(c->esp+4+0x20);candidate(p,"ASSIGN");
            fprintf(logFile,"CHOSEN id=%lu reg=%lu costs=",read32(p+0x1c),c->ebx);
            for(unsigned k=1;k<=8;++k)fprintf(logFile,"%u:%ld,",k,(long)read32(0x10880680+k*4));
            fprintf(logFile,"\n");
        }
        fflush(logFile);
    }
}
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,DWORD,DWORD,DWORD);
static unsigned char originals[6][16];
static unsigned sizes[6]={5,6,5,8,10,6};
static void installHooks(){
    for(unsigned n=0;n<6;++n){
        DWORD address=hooks[n].address,old;
        unsigned char* t=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);
        unsigned char code[]={0x9c,0x60,0x54,0x68,0,0,0,0,0xb8,0,0,0,0,0xff,0xd0,0x83,0xc4,0x08,0x61,0x9d};
        *(DWORD*)(code+4)=n;*(DWORD*)(code+9)=(DWORD)&traceHook;
        memcpy(t,code,sizeof(code));
        memcpy(originals[n],(void*)address,sizes[n]);
        memcpy(t+sizeof(code),(void*)address,sizes[n]);
        // The third hook displaces one rel32 call; retain its original callee.
        if(n==2)*(DWORD*)(t+sizeof(code)+1)=0x1078368e-(DWORD)(t+sizeof(code)+5);
        unsigned k=sizeof(code)+sizes[n];t[k]=0xe9;*(DWORD*)(t+k+1)=address+sizes[n]-(DWORD)(t+k+5);
        VirtualProtect((void*)address,sizes[n],0x40,&old);
        *(unsigned char*)address=0xe9;*(DWORD*)(address+1)=(DWORD)t-address-5;
        for(k=5;k<sizes[n];++k)*(unsigned char*)(address+k)=0x90;
    }
}
extern "C" __declspec(dllexport) int __stdcall InvokeCompilerPass(void* a,void* b,void* c){
    const char* dll=getenv("C2_TRACE_ORIGINAL");const char* output=getenv("C2_TRACE_LOG");
    if(!dll||!output)return 123;
    void* original=LoadLibraryA(dll);if(!original)return 124;
    int (__stdcall* invoke)(void*,void*,void*)=(int (__stdcall*)(void*,void*,void*))GetProcAddress(original,"_InvokeCompilerPass@12");
    if(!invoke)return 125;
    logFile=fopen(output,"w");if(!logFile)return 126;
    fprintf(logFile,"RTM diagnostic proxy pid=%lu backend=%s\n",GetCurrentProcessId(),dll);
    fprintf(logFile,"BACKEND base=%08lx invoke=%08lx context_eip=%lu\n",(DWORD)original,(DWORD)invoke,(DWORD)&((Context32*)0)->eip);fflush(logFile);
    if((DWORD)original!=0x10700000){fclose(logFile);return 127;}
    if(getenv("C2_TRACE_OFF")){int result=invoke(a,b,c);fclose(logFile);return result;}
    installHooks();
    int result=invoke(a,b,c);
    for(unsigned n=0;n<6;++n)memcpy((void*)hooks[n].address,originals[n],sizes[n]);
    fclose(logFile);return result;
}
