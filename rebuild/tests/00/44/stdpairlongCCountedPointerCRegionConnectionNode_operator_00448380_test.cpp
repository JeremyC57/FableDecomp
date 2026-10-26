#include <cstdio>
class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        T* const              data = o.Data;
        CCPPointerInfo* const info = o.Info;
        if (Info != info)
        {
            if (Info)
            {
                --Info->RefCount;
                if (Info->RefCount == 0)
                {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
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
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
typedef std::pair<long, CCountedPointer<CRegionConnectionNode> > CRegionNodeEntry;
void AssignRegionNodeEntry(CRegionNodeEntry& to, const CRegionNodeEntry& from)      // emission helper, not a retail function
{
    to = from;
}


static int g_deleted;
static void __fastcall DeleteIt(void*) { ++g_deleted; }
int main()
{
    CCPPointerInfo* a = new CCPPointerInfo; a->RefCount = 1; a->DeleteFunc = DeleteIt; a->Data = 0;
    CCPPointerInfo* b = new CCPPointerInfo; b->RefCount = 1; b->DeleteFunc = DeleteIt; b->Data = 0;
    CRegionNodeEntry x, y;
    x.first = 1; x.second.Data = (CRegionConnectionNode*)0x10; x.second.Info = a;
    y.first = 2; y.second.Data = (CRegionConnectionNode*)0x20; y.second.Info = b;
    AssignRegionNodeEntry(x, y);
    bool ok = x.first == 2 && x.second.Data == (CRegionConnectionNode*)0x20 && x.second.Info == b && b->RefCount == 2 && g_deleted == 1;
    x.first = 5; AssignRegionNodeEntry(x, y);
    ok = ok && x.first == 2 && b->RefCount == 2 && g_deleted == 1;
    CRegionNodeEntry z; z.first = 9; z.second.Data = 0; z.second.Info = 0;
    AssignRegionNodeEntry(x, z);
    ok = ok && x.second.Info == 0 && x.second.Data == 0 && b->RefCount == 1 && x.first == 9;
    if (ok) std::printf("00448380_TEST PASS\n");
    return 0;
}