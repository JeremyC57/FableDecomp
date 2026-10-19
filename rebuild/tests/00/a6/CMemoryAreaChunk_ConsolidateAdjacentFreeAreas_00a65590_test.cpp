#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __int64 long long
#endif

// CMemoryAreaChunk::ConsolidateAdjacentFreeAreas -- merge physically adjacent free areas and restart the size-ordered free-list scan.
// Retail 0x00A65590, 248 bytes. No generated/quarantine CMemoryArea or CMemoryAreaChunk header exists.
// Native PDB layouts reused from independently landed Init/RemoveAreaFromFreeList.
// Globals 0x013D28D4 alignment, 0x013D28D8 shift; calls 0x00A65060 unlink and 0x00A650F0 reinsert.
#include <stddef.h>
#pragma once

// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#ifndef _MSC_VER
#define FABLE_STATIC_ASSERT(expression)
#else
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_abi_assert_, __LINE__)[(expression) ? 1 : -1]
#endif

struct CMemoryAreaChunk;
struct CMemoryAllocatorVariableSize;

// Process-wide copies of the active allocator's alignment, refreshed on entry to the allocator's
// public methods (0x013D28D4 alignment, 0x013D28D8 alignment shift).
extern unsigned long g_CurrentAlignment;        // 0x013D28D4
extern unsigned long g_CurrentAlignmentShift;   // 0x013D28D8

// Round a header size up to the current alignment: ((size + align - 1) >> shift) << shift.
#define ALIGNED_SIZE(size) ((((size) + g_CurrentAlignment - 1) >> g_CurrentAlignmentShift) << g_CurrentAlignmentShift)

struct CMemoryArea {                            // PDB: 32 bytes
    CMemoryArea*      PNextArea;                // +0x00
    CMemoryArea*      PRightChild;              // +0x04
    CMemoryArea*      PPreviousArea;            // +0x08
    CMemoryArea*      PParentArea;              // +0x0c
    CMemoryArea*      PLeftChild;               // +0x10
    CMemoryAreaChunk* PParentChunk;             // +0x14
    long              DataSize;                 // +0x18
    unsigned char     Colour;                   // +0x1c
    unsigned char     ID;                       // +0x1d
    void Init(CMemoryAreaChunk* owner,long size) {
        PNextArea=0; PRightChild=0; PPreviousArea=0; PParentArea=0; PLeftChild=0;
        Colour=0; PParentChunk=owner; DataSize=size;
    }
};

struct CMemoryAreaChunk {                       // PDB: 52 bytes
    long              TotalDataSize;            // +0x00
    long              NoFreeAreas;              // +0x04
    long              NoAllocatedAreas;         // +0x08
    long              FreeDataSize;             // +0x0c
    long              LargestFreeAreaSize;      // +0x10
    long              SmallestFreeAreaSize;     // +0x14
    CMemoryAllocatorVariableSize* PParentAllocator;   // +0x18
    bool              Permanent;                // +0x1c
    bool              DemandPaged;              // +0x1d
    CMemoryAreaChunk* PNextChunk;               // +0x20
    CMemoryAreaChunk* PPreviousChunk;           // +0x24
    CMemoryArea*      PFirstFreeArea;           // +0x28
    CMemoryArea*      PLastFreeArea;            // +0x2c
    CMemoryArea*      PFirstAllocatedArea;      // +0x30
    void ConsolidateAdjacentFreeAreas(CMemoryArea* area);
    void RemoveAreaFromFreeList(CMemoryArea* area); // 0x00A65060
    void InsertAreaIntoFreeList(CMemoryArea* area);   // 0x00A650F0
};


FABLE_STATIC_ASSERT(sizeof(CMemoryArea)==32);
FABLE_STATIC_ASSERT(offsetof(CMemoryArea,DataSize)==0x18);
FABLE_STATIC_ASSERT(sizeof(CMemoryAreaChunk)==52);
FABLE_STATIC_ASSERT(offsetof(CMemoryAreaChunk,PFirstFreeArea)==0x28);
void CMemoryAreaChunk::ConsolidateAdjacentFreeAreas(CMemoryArea* input) {
    CMemoryArea* current=PFirstFreeArea;
    CMemoryArea* area=input;
    while(current) {
        CMemoryArea* next=current->PNextArea;
        if((char*)area+area->DataSize+ALIGNED_SIZE(sizeof(CMemoryArea))==(char*)current) {
            RemoveAreaFromFreeList(area);
            RemoveAreaFromFreeList(current);
            area->Init(this, area->DataSize+current->DataSize+ALIGNED_SIZE(sizeof(CMemoryArea)));
            InsertAreaIntoFreeList(area);
            next=PFirstFreeArea;
        } else if((char*)current+current->DataSize+ALIGNED_SIZE(sizeof(CMemoryArea))==(char*)area) {
            RemoveAreaFromFreeList(area);
            RemoveAreaFromFreeList(current);
            current->Init(this, area->DataSize+current->DataSize+ALIGNED_SIZE(sizeof(CMemoryArea)));
            InsertAreaIntoFreeList(current);
            area=current;
            next=PFirstFreeArea;
        }
        current=next;
    }
}



#include <stdlib.h>
#include <new>
#include <assert.h>
unsigned long g_CurrentAlignment=16, g_CurrentAlignmentShift=4;
static int removed,inserted;
// Dependency doubles exercise size-ordered list mutation/counters; allocator's separate tree is outside this fixture.
void CMemoryAreaChunk::RemoveAreaFromFreeList(CMemoryArea* p) {
    ++removed;
    if(p->PPreviousArea) p->PPreviousArea->PNextArea=p->PNextArea; else PFirstFreeArea=p->PNextArea;
    if(p->PNextArea) p->PNextArea->PPreviousArea=p->PPreviousArea; else PLastFreeArea=p->PPreviousArea;
    p->PNextArea=0;p->PPreviousArea=0;--NoFreeAreas;FreeDataSize-=p->DataSize;
}
void CMemoryAreaChunk::InsertAreaIntoFreeList(CMemoryArea* p) {
    ++inserted; CMemoryArea* cur=PFirstFreeArea;CMemoryArea* prev=0;
    while(cur && cur->DataSize<=p->DataSize){prev=cur;cur=cur->PNextArea;}
    p->PNextArea=cur;p->PPreviousArea=prev;
    if(prev)prev->PNextArea=p;else PFirstFreeArea=p;
    if(cur)cur->PPreviousArea=p;else PLastFreeArea=p;
    ++NoFreeAreas;FreeDataSize+=p->DataSize;
}
static void scenario(unsigned long align,unsigned long shift,int mode) {
    g_CurrentAlignment=align;g_CurrentAlignmentShift=shift;
    CMemoryAreaChunk c={0}; char* pool=(char*)malloc(2048);assert(pool);
    size_t h=ALIGNED_SIZE(sizeof(CMemoryArea));
    CMemoryArea* a=new(pool) CMemoryArea;
    CMemoryArea* b=new(pool+h+64) CMemoryArea;
    CMemoryArea* d=new(pool+2*h+64+128) CMemoryArea;
    a->Init(&c,64);b->Init(&c,128);d->Init(&c,32);
    a->ID=91;b->ID=92;d->ID=93;
    c.InsertAreaIntoFreeList(a); c.InsertAreaIntoFreeList(b);c.InsertAreaIntoFreeList(d);
    removed=inserted=0;
    c.ConsolidateAdjacentFreeAreas(mode?d:a);
    assert(c.NoFreeAreas==1&&c.PFirstFreeArea==a&&c.PLastFreeArea==a);
    assert(a->DataSize==(long)(224+2*h)&&a->PParentChunk==&c&&a->Colour==0&&a->ID==91);
    assert(!a->PNextArea&&!a->PPreviousArea&&!a->PRightChild&&!a->PLeftChild&&!a->PParentArea);
    assert(removed==4&&inserted==2&&c.FreeDataSize==a->DataSize);
    free(pool);
}
int main(){
    CMemoryAreaChunk c={0};c.ConsolidateAdjacentFreeAreas(0);assert(!removed&&!inserted);
    scenario(16,4,0);scenario(64,6,1);
    char* pool=(char*)malloc(2048); assert(pool);
    CMemoryArea* a=new(pool) CMemoryArea;CMemoryArea* b=new(pool+1024) CMemoryArea;
    c.PFirstFreeArea=0;c.PLastFreeArea=0;c.NoFreeAreas=0;c.FreeDataSize=0;
    a->Init(&c,64);b->Init(&c,64);c.InsertAreaIntoFreeList(a);c.InsertAreaIntoFreeList(b);
    removed=inserted=0;c.ConsolidateAdjacentFreeAreas(a);assert(!removed&&!inserted&&c.NoFreeAreas==2);free(pool);
    puts("00a65590_TEST PASS");return 0;
}