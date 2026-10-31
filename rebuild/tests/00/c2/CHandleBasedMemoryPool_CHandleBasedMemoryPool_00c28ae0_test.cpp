// Behaviour test for 00c28ae0: src/00c28ae0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CHandleBasedMemoryPool_012b4098::CHandleBasedMemoryPool_012b4098()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CHandleBasedMemoryPool_012b4098::CHandleBasedMemoryPool_012b4098() -- pool ctor: zero the buffer/size/count words +4..+0x18, the default failure handler at +0x34, clear the 256 free lists (ClearFreeLists), an empty handle vector at +0x440, FirstUnusedHandle = -1, the name string from the nil string 0x0139C900, then point FailureHandler at the default handler and set the four defrag/size defaults
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C28AE0, length 158 bytes. Callees: ClearFreeLists @ 0x00C28200 (rep stosd of 0x100 dwords at +0x3c).
// Label: the catalog's "CSoundDef::CSoundDef" is propagated. The class is this copy of CHandleBasedMemoryPool_012b4098 (0x00C27EF0..0x00C28A80):
//   the callee 0x00C28200 is CHandleBasedMemoryPool_012b4098::ClearFreeLists (labels_chunk223_corrections), the landed neighbours
//   Lock 0x00C283C0 / FreeHandle 0x00C28610 / InternalAllocateHandle 0x00C28A80 / FindLargestFreeArea 0x00C28050 read the same
//   retail layout (free lists +0x3c, handle table +0x440, FirstUnusedHandle +0x44c), and the dtor 0x00C287E0 (landed as
//   "CHandleBasedMemoryPool_012b4098") stores the same vtable 0x012B4098.
// Layout: local retail view. Retail provably differs from engine/CHandleBasedMemoryPool.h (Ego_r: FailureHandler +0x34,
//   FreeLists +0x38, FirstUnusedHandle +0x448, Name +0x44c): this ctor stores a vptr at +0x34 (an embedded default handler,
//   vtable 0x012B37FC), sets +0x38 = &(+0x34), and ClearFreeLists zeroes +0x3c..+0x43c; +0x44c = -1 and +0x450 = the nil string.
//   The +0x20..+0x2c words are named by offset (their Ego_r names do not fit the dword stores).
// Shape: retail opens the /EHsc frame (and reserves the `this` home slot) but stores no EH state: ClearFreeLists() is non-throwing
// (declared throw(); it is a bare rep stosd) and the only throwing call, an inline vector reserve(0) whose
// `capacity() < 0` unsigned test folds away, disappears after the frame is laid out. A throwing ClearFreeLists() stores states
// and homes `this`; a non-throwing ClearFreeLists() alone drops the frame; a same-TU ClearFreeLists() body changes the scheduling.
// The identity of the folded call is not recoverable from the bytes; reserve(0) on the empty +0x440 vector is a stand-in.
// Leaf: ??0CHandleBasedMemoryPool
// Flags: /EHsc
void __cdecl operator delete(void* block) throw();       // 0x00BFE9BC

class CFailedAllocationHandler { public:          // embedded at +0x34, vtable 0x012B37FC
    virtual void v00();
    ~CFailedAllocationHandler() { }
};

struct CHandleVector {                  // +0x440: {begin, end, capacity}
    void* m_pBegin;
    void* m_pEnd;
    void* m_pCapacity;
    CHandleVector() : m_pBegin(0), m_pEnd(0), m_pCapacity(0) {}
    void Reallocate(unsigned long bytes);
    void reserve(unsigned long bytes) { if ((unsigned long)((char*)m_pCapacity - (char*)m_pBegin) < bytes) Reallocate(bytes); }
    ~CHandleVector()
    {
        if (m_pBegin)
            operator delete(m_pBegin);
        m_pBegin = 0;
        m_pEnd = 0;
        m_pCapacity = 0;
    }
};

extern long g_pchNil;                      // 0x0139C900 (the nil string block)

struct CPoolName {                  // +0x450
    long m_value;
    CPoolName() : m_value(g_pchNil) {}
    ~CPoolName();                          // 0x00C0BA40
};

class CHandleBasedMemoryPool_012b4098 { public:              // vtable 0x012B4098
    virtual void v00();
    void* Buffer;                          // +0x04
    void* UnalignedBuffer;                 // +0x08
    long  PoolSize;                        // +0x0c
    long  AllocatedSize;                   // +0x10
    long  NumAllocations;                  // +0x14
    long  LockCount;                       // +0x18
    long  LargestFreeArea;                 // +0x1c
    long m_20, m_24, m_28, m_2c;           // +0x20 .. +0x2c
    long m_30;
    CFailedAllocationHandler  DefaultFailureHandler;        // +0x34
    CFailedAllocationHandler* FailureHandler;       // +0x38
    unsigned long       FreeLists[0x100];    // +0x3c
    long                m_43c;             // +0x43c
    CHandleVector    Handles;          // +0x440
    long                FirstUnusedHandle;             // +0x44c
    CPoolName    Name;          // +0x450
    void ClearFreeLists() throw();         // 0x00C28200 (rep stosd over FreeLists)
    CHandleBasedMemoryPool_012b4098();
    ~CHandleBasedMemoryPool_012b4098();
};

CHandleBasedMemoryPool_012b4098::CHandleBasedMemoryPool_012b4098()
    : Buffer(0), UnalignedBuffer(0), PoolSize(0), AllocatedSize(0), NumAllocations(0), LockCount(0),
      FailureHandler(0),
      FirstUnusedHandle(-1)
{
    ClearFreeLists();
    Handles.reserve(0);
    FailureHandler = &DefaultFailureHandler;
    m_20 = 0x80000;
    m_24 = 0x100000;
    m_28 = 1;
    m_2c = 0x10;
}

// ---- stubs and driver ----
#include <new>
long g_pchNil = 0x5151;
static int g_clears = 0, g_reallocs = 0, g_nameDtors = 0;
void CHandleBasedMemoryPool_012b4098::ClearFreeLists() throw() { ++g_clears; for (int i = 0; i < 0x100; ++i) FreeLists[i] = 0; }
void CHandleVector::Reallocate(unsigned long) { ++g_reallocs; }
void CFailedAllocationHandler::v00() { }
void CHandleBasedMemoryPool_012b4098::v00() { }
CPoolName::~CPoolName() { ++g_nameDtors; }
void __cdecl operator delete(void* block) throw() { (void)block; }
int main()
{
    static double buf[(sizeof(CHandleBasedMemoryPool_012b4098) + 7) / 8];
    memset(buf, 0xcd, sizeof(buf));
    CHandleBasedMemoryPool_012b4098* p = new (buf) CHandleBasedMemoryPool_012b4098();
    CHECK(p == (CHandleBasedMemoryPool_012b4098*)buf);
    CHECK(sizeof(CHandleBasedMemoryPool_012b4098) == 0x454);
    CHECK(p->Buffer == 0 && p->UnalignedBuffer == 0 && p->PoolSize == 0 && p->AllocatedSize == 0 && p->NumAllocations == 0 && p->LockCount == 0);
    CHECK(g_clears == 1); CHECK(g_reallocs == 0);
    CHECK(p->FreeLists[0] == 0 && p->FreeLists[255] == 0);
    CHECK(p->FailureHandler == &p->DefaultFailureHandler);
    CHECK((char*)&p->DefaultFailureHandler - (char*)p == 0x34);
    CHECK(p->Handles.m_pBegin == 0 && p->Handles.m_pEnd == 0 && p->Handles.m_pCapacity == 0);
    CHECK(p->FirstUnusedHandle == -1);
    CHECK(p->Name.m_value == 0x5151);
    CHECK(p->m_20 == 0x80000 && p->m_24 == 0x100000 && p->m_28 == 1 && p->m_2c == 0x10);
    CHECK(g_nameDtors == 0);
    if (fails == 0) printf("00c28ae0_TEST PASS\n"); else printf("00c28ae0_TEST FAIL\n");
    return fails;
}
