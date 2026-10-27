// behaviour test for 0x00827F80 std::pair<long, CCountedPointer<CRegionConnectionNode> >::operator= (model copy of the source)
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
class CRegionConnectionNode;

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
typedef std::pair<long, CCountedPointer<CRegionConnectionNode> > TEntry;
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
    dst.first = 1; dst.second.Data = 0; dst.second.Info = ba;
    src.first = 7; src.second.Data = 0; src.second.Info = bb;
    TEntry& r = (dst = src);
    if (&r != &dst) fail |= 1;
    if (dst.first != 7 || dst.second.Info != bb || bb->RefCount != 2) fail |= 2;
    if (objA != 1 || g_deleted != 1) fail |= 4;          // A's block hit zero: data deleted, block freed
    dst = src;                                           // same block: no release, no add-ref
    if (bb->RefCount != 2 || g_deleted != 1) fail |= 8;
    TEntry empty; empty.first = 3; empty.second.Data = 0; empty.second.Info = 0;
    dst = empty;                                         // release B (2 -> 1), adopt null
    if (dst.second.Info != 0 || bb->RefCount != 1 || objB != 0 || dst.first != 3) fail |= 16;
    if (fail) { std::printf("00827f80_TEST FAIL %d\n", fail); return 1; }
    std::printf("00827f80_TEST PASS\n");
    return 0;
}