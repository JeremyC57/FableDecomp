#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#endif
extern "C" void __cdecl tracked_free(void*);
#define free tracked_free
// CHeroStatsDef::~CHeroStatsDef() -- release eight owned blocks around two embedded cleanup records, then tail-call the definition base destructor; retail @ 0x0045619E, 166 bytes.
// Retail member-call receivers at +0x70/@0x004533E3 and +0xFC/@0x00453406, eight pointer loads, and the base tail-call at 0x00430300 establish this cleanup sequence. The quarantined PDB layout is not used because its members do not agree with these retail offsets.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* memory); // 0x00BFEA14
struct CDefinitionBaseModel {
    unsigned char data[0x70];
    ~CDefinitionBaseModel(); // 0x00430300
};
struct DefinitionCleanup70 {
    void* first;
    void* second;
    unsigned char pad08[0x34];
    void* pAC;
    unsigned char pad40[8];
    void* pB8;
    unsigned char pad4c[8];
    void* pC4;
    unsigned char pad58[0x28];
    void* pF0;
    unsigned char pad84[8];
    void Cleanup(); // 0x004533E3
};
struct DefinitionCleanupFC {
    void* first;
    void* second;
    unsigned char pad08[4];
    void Cleanup(); // 0x00453406
};
class CHeroStatsDef : public CDefinitionBaseModel { public:
    DefinitionCleanup70 cleanup70; // +0x70
    DefinitionCleanupFC cleanupFC; // +0xfc
    void* p108;
    unsigned char pad10c[8];
    void* p114;
    unsigned char pad118[8];
    void* p120;
    unsigned char pad124[8];
    void* p12c;
    ~CHeroStatsDef();
};
CHeroStatsDef::~CHeroStatsDef() {
    if (p12c) free(p12c);
    if (p120) free(p120);
    if (p114) free(p114);
    if (p108) free(p108);
    cleanupFC.Cleanup();
    if (cleanup70.pF0) free(cleanup70.pF0);
    if (cleanup70.pC4) free(cleanup70.pC4);
    if (cleanup70.pB8) free(cleanup70.pB8);
    if (cleanup70.pAC) free(cleanup70.pAC);
    cleanup70.Cleanup();
}

#undef free
static int events[16], count;
static void* blocks[8];
static void event(int n) { events[count++]=n; }
extern "C" void __cdecl tracked_free(void* p) {
    int i; for (i=0;i<8;++i) if (p==blocks[i]) break;
    event(i<8 ? i+1 : 99); std::free(p);
}
void DefinitionCleanupFC::Cleanup() { event(20); }
void DefinitionCleanup70::Cleanup() { event(21); }
CDefinitionBaseModel::~CDefinitionBaseModel() { event(22); }
static void* block() { return std::malloc(16); }
static int ordered(const int* expected, int n) { int i; if(count!=n) return 0; for(i=0;i<n;++i) if(events[i]!=expected[i]) return 0; return 1; }
int main() {
    union Storage { void* align; char bytes[sizeof(CHeroStatsDef)]; } storage;
    CHeroStatsDef* p=new(storage.bytes) CHeroStatsDef;
    blocks[0]=p->cleanup70.pAC=block(); blocks[1]=p->cleanup70.pB8=block();
    blocks[2]=p->cleanup70.pC4=block(); blocks[3]=p->cleanup70.pF0=block();
    blocks[4]=p->p108=block(); blocks[5]=p->p114=block();
    blocks[6]=p->p120=block(); blocks[7]=p->p12c=block();
    p->cleanupFC.first=0; p->cleanupFC.second=0; p->cleanup70.first=0; p->cleanup70.second=0;
    for (int i=0;i<8;++i) if (!blocks[i]) { std::printf("0045619e_TEST FAIL\n"); return 1; }
    p->~CHeroStatsDef();
    { const int expect[11]={8,7,6,5,20,4,3,2,1,21,22}; if(!ordered(expect,11)) { std::printf("0045619e_TEST FAIL\n"); return 1; } }
    count=0;
    union Storage emptyStorage;
    for (unsigned int j=0;j<sizeof(emptyStorage.bytes);++j) emptyStorage.bytes[j]=0;
    CHeroStatsDef* empty=new(emptyStorage.bytes) CHeroStatsDef();
    empty->~CHeroStatsDef();
    { const int expect[3]={20,21,22}; if(!ordered(expect,3)) { std::printf("0045619e_TEST FAIL\n"); return 1; } }
    std::printf("0045619e_TEST PASS\n"); return 0;
}