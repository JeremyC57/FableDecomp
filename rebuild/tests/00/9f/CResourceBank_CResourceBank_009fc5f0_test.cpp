#include <stdio.h>
#include <string.h>
#include <new>
// CResourceBank::CResourceBank() -- construct the resident-resource list and the loading queue (each a CResourceList with a circular dummy head and an unlimited memory budget)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC5F0, length 156 bytes (+4 int3 padding).
// PDB layout: CResourceBank (0x7C) = CBaseClassNonCopyable base (vptr) + CResourceList ResourceList (+0x04) +
// CResourceList LoadingQueue (+0x40); CResourceList (0x3C, engine/CResourceList.h) = vptr, CResource Head (+0x04),
// ResourceCount, AllocatedMemory, MaximumMemory, CurrentFrame, DebugStatsFrame, UnloadDelay, UnloadedThisFrame;
// CResource (0x1C) = vptr, a count word (+0x04, starts at 1), ResourceList, PrevResource, NextResource,
// ResourceSize, LastUsedFrame.
// Vtables: CResourceBank 0x0129C814, CResourceList 0x0129C808, CResource 0x012354A4 (rtti-vtables.tsv).
// Callee: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (stores 0x0129A7C4).
// Shape: CResource derives from the polymorphic CIVCountedPointeeBase (PDB: vptr + IVRefCount at +4, ctor sets the count
// to 1), so the head's base ctor runs first and its dead vtable store is dropped; both lists are built by the same inline ctor; the count word of the dummy head is set by an inline
// base of CResource (it stores before CResource's vtable store in the second copy); the list ctor body links the
// head to itself and sets the counters in retail's store order (count, frames, memory, budget, delay, unloaded).
// Model: local polymorphic model (the generated header holds the vptr as a plain word and Head as bytes).
struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                         // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

class CResourceList;

class CIVCountedPointeeBase { public:                       // PDB: vptr + IVRefCount (+0x04)
    CIVCountedPointeeBase() : IVRefCount(1) {}
    virtual ~CIVCountedPointeeBase();
    unsigned long IVRefCount;
};

class CResource : public CIVCountedPointeeBase { public:                  // vtable 0x012354A4, 0x1C bytes
    CResource() : ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0), LastUsedFrame(0) {}
    virtual ~CResource();
    CResourceList*  ResourceList;                    // +0x08
    CResource*      PrevResource;                    // +0x0C
    CResource*      NextResource;                    // +0x10
    unsigned long   ResourceSize;                    // +0x14
    unsigned long   LastUsedFrame;                   // +0x18
};

class CResourceList { public:                               // vtable 0x0129C808, 0x3C bytes
    CResourceList()
    {
        Head.PrevResource = &Head;
        Head.NextResource = &Head;
        ResourceCount = 0;
        CurrentFrame = 0;
        DebugStatsFrame = 0;
        AllocatedMemory = 0;
        MaximumMemory = 0x7FFFFFFF;
        UnloadDelay = 1;
        UnloadedThisFrame = 0;
    }
    virtual ~CResourceList();
    CResource     Head;                              // +0x04
    unsigned long ResourceCount;                     // +0x20
    long          AllocatedMemory;                   // +0x24
    long          MaximumMemory;                     // +0x28
    unsigned long CurrentFrame;                      // +0x2C
    unsigned long DebugStatsFrame;                   // +0x30
    unsigned long UnloadDelay;                       // +0x34
    unsigned long UnloadedThisFrame;                 // +0x38
};

class CResourceBank : public CBaseClassNonCopyable { public:      // vtable 0x0129C814, 0x7C bytes
    CResourceBank();
    virtual ~CResourceBank();
    CResourceList ResourceList;                      // +0x04
    CResourceList LoadingQueue;                      // +0x40
};

CResourceBank::CResourceBank()
{
}

// ---- test ----
static int g_baseCtor;
CBaseClassNonCopyable::CBaseClassNonCopyable() { g_baseCtor++; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CIVCountedPointeeBase::~CIVCountedPointeeBase() { }
CResource::~CResource() { }
CResourceList::~CResourceList() { }
CResourceBank::~CResourceBank() { }
static int fails;
static void check(int ok, const char* what) { if (!ok) { char b[120]; _snprintf(b, sizeof b, "FAIL %s", what); puts(b); fails++; } }
static void checkList(CResourceList& l, const char* what)
{
    check(l.Head.PrevResource == &l.Head && l.Head.NextResource == &l.Head, what);
    check(l.Head.IVRefCount == 1 && l.Head.ResourceList == 0 && l.Head.ResourceSize == 0 && l.Head.LastUsedFrame == 0, what);
    check(l.ResourceCount == 0 && l.CurrentFrame == 0 && l.DebugStatsFrame == 0 && l.AllocatedMemory == 0, what);
    check(l.MaximumMemory == 0x7FFFFFFF && l.UnloadDelay == 1 && l.UnloadedThisFrame == 0, what);
}
int main()
{
    static unsigned char raw[sizeof(CResourceBank)];
    memset(raw, 0xAB, sizeof raw);
    CResourceBank* bank = new (raw) CResourceBank;
    check(g_baseCtor == 1, "base ctor");
    check(sizeof(CResourceBank) == 0x7C && (char*)&bank->LoadingQueue - (char*)bank == 0x40, "layout");
    checkList(bank->ResourceList, "resource list");
    checkList(bank->LoadingQueue, "loading queue");
    if (fails) { puts("009fc5f0_TEST FAIL"); return 1; }
    puts("009fc5f0_TEST PASS");
    return 0;
}