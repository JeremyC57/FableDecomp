#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#endif
// CScriptDefinitionManager::Shutdown() -- release owned services and two counted buffers after the helper reset; retail @ 0x00C3E580, 191 bytes.
// The manifest's CQuadTreeElement destructor name is propagated: landed CScriptDefinitionManager::~CScriptDefinitionManager @ 0x00C3F380 calls this routine. The preliminary helper at 0x00C4D1F0 performs the shared four-subobject cleanup; fields at +0x58, +0x174/+0x178, +0x180/+0x184/+0x188 and +0x1d4 are inferred directly from the retail accesses.
struct IDeletingService { virtual void Destroy(unsigned long flags) = 0; };
struct ICountedBuffer {
    virtual void Release(unsigned long flags) = 0;
    virtual void v1() = 0; virtual void v2() = 0; virtual void v3() = 0;
};
struct IServiceHub { virtual void v0() = 0; virtual void v1() = 0; virtual void Shutdown() = 0; };
extern void __cdecl operator delete(void*); // 0x00BFE9BC
struct CScriptDefinitionManager {
    void* vftable;
    unsigned char pad04[0x54];
    IDeletingService* service58;
    unsigned char pad5c[0x118];
    ICountedBuffer* buffer174;
    ICountedBuffer* buffer178;
    unsigned char pad17c[4];
    IDeletingService* service180;
    IDeletingService* service184;
    IDeletingService* service188;
    unsigned char pad18c[0x48];
    IServiceHub* hub1d4;
    void Cleanup(); // 0x00C4D1F0
    void Shutdown();
};
void CScriptDefinitionManager::Shutdown() {
    Cleanup();
    if (service188) service188->Destroy(1);
    if (service58) { service58->Destroy(1); service58 = 0; }
    if (service180) { service180->Destroy(1); service180 = 0; }
    if (service184) service184->Destroy(1);
    if (buffer174) {
        if (*((unsigned int*)buffer174 - 1)) buffer174->Release(3);
        else operator delete((char*)buffer174 - 4);
        buffer174 = 0;
    }
    if (buffer178) {
        if (*((unsigned int*)buffer178 - 1)) buffer178->Release(3);
        else operator delete((char*)buffer178 - 4);
        buffer178 = 0;
    }
    if (hub1d4) { hub1d4->Shutdown(); hub1d4 = 0; }
}

static int cleanupCount, destroyCount, releaseCount, hubCount;
static int events[16], eventCount;
static void* deletedBlock;
static void event(int n) { events[eventCount++] = n; }
void __cdecl operator delete(void* p) { deletedBlock = p; event(7); }
static unsigned long releasedFlags;
void CScriptDefinitionManager::Cleanup() { ++cleanupCount; event(1); }
struct Service : IDeletingService { int* count; int id; Service(int* p,int n):count(p),id(n){} void Destroy(unsigned long flags) { if (flags == 1) { ++*count; event(id); } } };
struct Buffer : ICountedBuffer { void Release(unsigned long flags) { releasedFlags = flags; ++releaseCount; event(5+releaseCount); } void v1(){} void v2(){} void v3(){} };
struct Hub : IServiceHub { void Shutdown() { ++hubCount; event(8); } void v0(){} void v1(){} };
int main() {
    CScriptDefinitionManager manager = {};
    int a=0,b=0,c=0;
    Service s188(&a,2), s58(&b,3), s180(&c,4), s184(&destroyCount,5);
    Hub hub;
    union BufferStore { void* align; char bytes[sizeof(Buffer)+16]; } one, two;
    char* p1=(char*)((((unsigned long)(one.bytes+8)+sizeof(void*)-1)/sizeof(void*))*sizeof(void*));
    char* p2=(char*)((((unsigned long)(two.bytes+8)+sizeof(void*)-1)/sizeof(void*))*sizeof(void*));
    *((unsigned int*)p1-1)=1; *((unsigned int*)p2-1)=0;
    Buffer* b1=new(p1) Buffer; Buffer* b2=new(p2) Buffer;
    manager.service188=&s188; manager.service58=&s58;
    manager.service180=&s180; manager.service184=&s184;
    manager.buffer174=b1; manager.buffer178=b2; manager.hub1d4=&hub;
    manager.Shutdown();
    if (cleanupCount!=1 || a!=1 || b!=1 || c!=1 || destroyCount!=1 ||
        releaseCount!=1 || releasedFlags!=3 || deletedBlock!=(void*)(p2-4) || hubCount!=1 ||
        manager.service58 || manager.service180 || manager.buffer174 || manager.buffer178 || manager.hub1d4 ||
        manager.service188!=&s188 || manager.service184!=&s184 || eventCount!=8 ||
        events[0]!=1 || events[1]!=2 || events[2]!=3 || events[3]!=4 || events[4]!=5 ||
        events[5]!=6 || events[6]!=7 || events[7]!=8) {
        std::printf("00c3e580_TEST FAIL\n"); return 1;
    }
    CScriptDefinitionManager empty = {};
    empty.Shutdown();
    if (cleanupCount != 2 || eventCount != 9 || events[8] != 1) {
        std::printf("00c3e580_TEST FAIL\n"); return 1;
    }
    std::printf("00c3e580_TEST PASS\n"); return 0;
}