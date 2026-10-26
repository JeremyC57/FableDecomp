// Behaviour test for 0x0098F570 std::less<CThreadedFileManager::CPendingObjectSortKey>::operator():
// order pending loads by the file's physical sort key, then the file position, then the object address
#include <stdio.h>
class CThreadedFile { public:
    unsigned char _pad00[0x14];
    unsigned long PhysicalSortKey;
};
class CThreadedFileObjectDesc { public:
    void*         PDst;
    unsigned long FilePos;
    unsigned char _pad08[0x10];
};
class CThreadedFileManager {
public:
    class CFileObject { public:
        CThreadedFileObjectDesc Desc;
        unsigned long  UID;
        long           LoadingStatus;
        void*          Overlapped;
        CThreadedFileManager* PManager;
        CThreadedFile* PFile;
    };
    class CPendingObjectSortKey {
    public:
        CFileObject* Object;
        bool operator<(const CPendingObjectSortKey& o) const
        {
            unsigned long mySortKey = Object->PFile->PhysicalSortKey;
            unsigned long otherSortKey = o.Object->PFile->PhysicalSortKey;
            if (mySortKey < otherSortKey)
                return true;
            if (mySortKey > otherSortKey)
                return false;
            if (Object->Desc.FilePos < o.Object->Desc.FilePos)
                return true;
            if (Object->Desc.FilePos > o.Object->Desc.FilePos)
                return false;
            return Object < o.Object;
        }
    };
};
namespace std {
template <class _Arg1, class _Arg2, class _Result>
struct binary_function {
    typedef _Arg1   first_argument_type;
    typedef _Arg2   second_argument_type;
    typedef _Result result_type;
};
template <class _Tp>
struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}
typedef CThreadedFileManager::CFileObject CObj;
typedef CThreadedFileManager::CPendingObjectSortKey CKey;
static CThreadedFile gFiles[2];
static CObj gObjs[4];
int main()
{
    gFiles[0].PhysicalSortKey = 10;
    gFiles[1].PhysicalSortKey = 20;
    gObjs[0].PFile = &gFiles[1]; gObjs[0].Desc.FilePos = 0;     // later file, earliest position
    gObjs[1].PFile = &gFiles[0]; gObjs[1].Desc.FilePos = 500;   // earlier file
    gObjs[2].PFile = &gFiles[0]; gObjs[2].Desc.FilePos = 100;   // earlier file, earlier position
    gObjs[3].PFile = &gFiles[0]; gObjs[3].Desc.FilePos = 100;   // same file+position as [2]: address breaks the tie
    CKey k0, k1, k2, k3;
    k0.Object = &gObjs[0]; k1.Object = &gObjs[1]; k2.Object = &gObjs[2]; k3.Object = &gObjs[3];
    std::less<CKey> lt;
    bool ok = true;
    ok = ok && lt(k1, k0) && !lt(k0, k1);        // sort key wins over file position
    ok = ok && lt(k2, k1) && !lt(k1, k2);        // same sort key: file position
    ok = ok && lt(k2, k3) && !lt(k3, k2);        // full tie: object address (gObjs[2] < gObjs[3])
    ok = ok && !lt(k2, k2);
    if (ok) { printf("0098f570_TEST PASS\n"); return 0; }
    printf("0098f570_TEST FAIL\n");
    return 1;
}