// behaviour test for 0x00430300
#include <stdio.h>
#include <string.h>
static int g_resource_dtors = 0;
#include <stdlib.h>
#pragma pack(push, 1)
class CResourceList;
class CResource {
public:
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0c
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
    virtual ~CResource() { ++g_resource_dtors; }
};
class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() {}
};
class CDefinitionManager;
class CDefClassBase : public CDefPointeeBase {
public:
    CDefinitionManager* PDefManager;                     // +0x1c
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
template <class T> class CMallocVector {                 // STLport vector<T> over the malloc allocator
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CMallocVector() { if (_M_start) free(_M_start); }
};
class CDefWithBuffer : public CDefClassBase {
public:
    long                Value;                           // +0x25
    CMallocVector<long> Data;                            // +0x29
};
#pragma pack(pop)
void DestroyDefWithBuffer(CDefWithBuffer* p)                 // emission helper, not a retail function
{
    p->CDefWithBuffer::~CDefWithBuffer();
}

int main()
{
    CDefWithBuffer* d = new CDefWithBuffer; d->Data._M_start = (long*)malloc(16);
    int ok = (char*)&d->Data - (char*)d == 0x29;
    DestroyDefWithBuffer(d); ok &= g_resource_dtors == 1;
    ::operator delete(d);
    CDefWithBuffer* e = new CDefWithBuffer; e->Data._M_start = 0; CResource* r = e; delete r; ok &= g_resource_dtors == 2;
    printf(ok ? "00430300_TEST PASS\n" : "00430300_TEST FAIL\n");
    return ok ? 0 : 1;
}