// std::less::operator()(const CThreadedFileManager::CPendingObjectSortKey& x, const CThreadedFileManager::CPendingObjectSortKey& y) -- less<CPendingObjectSortKey>: pending-read order: physical file sort key, then file position, then object address
// Leaf: ??R?$less@VCPendingObjectSortKey@CThreadedFileManager@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098F570, 64 bytes, `__fastcall` (the empty comparator in ecx, unused),
//   `ret 8`. No callees (the key's operator< is inline).
// Identity: STLport `less<_Tp>::operator()` (`return __x < __y;`) over CThreadedFileManager's pending-object map key,
//   emitted out of line for the map's insert/erase paths. FableWin.pdb names the inline member
//   `CPendingObjectSortKey::operator<(const CPendingObjectSortKey&) const`.
// Layout (Ego_r.pdb, as in the landed lower bound 0x009901D0): CPendingObjectSortKey = { CFileObject* Object };
//   CFileObject: Desc (CThreadedFileObjectDesc: PDst +0, FilePos +4) +0x00, PFile (CThreadedFile*) +0x28;
//   CThreadedFile::PhysicalSortKey +0x14.
// Shape: two-way unsigned compares of the physical sort keys (`jb` true / `ja` false) and the file positions, then the
//   object pointers as a `setb` bool; every exit returns a byte bool (`mov al,1` / `xor al,al`). The two physical sort
//   keys are NAMED locals: that makes VC7.1 finish x's chain (`mov ecx,[eax+28h] ; mov edx,[ecx+14h]`) before it loads
//   y (the member-expression form loads y's address first and walks x's chain through edx).
struct CThreadedFile {
    unsigned char _pad00[0x14];
    unsigned long PhysicalSortKey;             // +0x14
};
struct CThreadedFileObjectDesc {
    void*         PDst;                        // +0x00
    unsigned long FilePos;                     // +0x04
    unsigned char _pad08[0x10];
};
class CThreadedFileManager {
public:
    struct CFileObject {
        CThreadedFileObjectDesc Desc;          // +0x00
        unsigned long  UID;                    // +0x18
        long           LoadingStatus;          // +0x1C
        void*          Overlapped;             // +0x20
        CThreadedFileManager* PManager;        // +0x24
        CThreadedFile* PFile;                  // +0x28
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

template struct less<CThreadedFileManager::CPendingObjectSortKey>;
}