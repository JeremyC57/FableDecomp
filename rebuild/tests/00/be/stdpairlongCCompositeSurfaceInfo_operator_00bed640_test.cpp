// behaviour test for 0x00BED640 std::pair<long, CCompositeSurfaceInfo >::operator= (model copy of the source)
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
class CCPPointerInfo {                                   // Ego_r.pdb, 12 bytes
public:
    unsigned long RefCount;                              // +0x00
    void (__fastcall* DeleteFunc)(void* data);           // +0x04
    void* Data;                                          // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCPPointerInfo* GetInfo() const { return Info; }
    void DeleteData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                ::operator delete(GetInfo());
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            DeleteData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
// CBaseClass: the polymorphic engine root (out-of-line virtual destructor 0x0099A2E0).
class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CCompositeSurfaceCache;
// Ego_r.pdb CCompositeSurfaceInfo (vtable 0x012A8154, 12 bytes): the implicit assignment copies Cache, never the vptr.
class CCompositeSurfaceInfo : public CBaseClass {
public:
    CCountedPointer<CCompositeSurfaceCache> Cache;      // +0x04
};
CBaseClass::~CBaseClass() { }

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
typedef std::pair<long, CCompositeSurfaceInfo > TEntry;
static int g_deleted = 0;
static void __fastcall DeleteIt(void* data) { ++*(int*)data; ++g_deleted; }
static CCPPointerInfo* Block(int* target, unsigned long refs)
{
    CCPPointerInfo* b = new CCPPointerInfo; b->RefCount = refs; b->DeleteFunc = DeleteIt; b->Data = target; return b;
}
int main()
{
    int fail = 0;
    int objA = 0, objB = 0;
    CCPPointerInfo* ba = Block(&objA, 1);               // only the destination holds A
    CCPPointerInfo* bb = Block(&objB, 1);               // the source holds B
    TEntry dst, src;
    dst.first = 1; dst.second.Cache.Data = 0; dst.second.Cache.Info = ba;
    src.first = 7; src.second.Cache.Data = 0; src.second.Cache.Info = bb;
    TEntry& r = (dst = src);
    if (&r != &dst) fail |= 1;
    if (dst.first != 7 || dst.second.Cache.Info != bb || bb->RefCount != 2) fail |= 2;
    if (objA != 1 || g_deleted != 1) fail |= 4;          // A's block hit zero: data deleted, block freed
    dst = src;                                           // same block: no release, no add-ref
    if (bb->RefCount != 2 || g_deleted != 1) fail |= 8;
    TEntry empty; empty.first = 3; empty.second.Cache.Data = 0; empty.second.Cache.Info = 0;
    dst = empty;                                         // release B (2 -> 1), adopt null
    if (dst.second.Cache.Info != 0 || bb->RefCount != 1 || objB != 0 || dst.first != 3) fail |= 16;
    if (fail) { std::printf("00bed640_TEST FAIL %d\n", fail); return 1; }
    std::printf("00bed640_TEST PASS\n");
    return 0;
}