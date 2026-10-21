// std::pair<long, CCountedPointer<CRegionConnectionNode> >::operator=(const pair&) -- the implicit copy assignment: the key, then the counted pointer (release old, share new)
// Leaf: ??4?$pair@
// Retail: Fable.exe @ 0x00448380, 78 bytes, `ret 4`, returns this; callers 0x004489F6 / 0x00448EB4 in the heap/insertion sort
// code over this 12-byte element (with 0x00448910 / 0x004488B0). Catalog label NInventory::CItem::operator= is wrong.
// Body: first = o.first; CCountedPointer::operator=: if (Info != o.Info) { release ours (if Info, --RefCount, at zero
// DeleteFunc(Data) __fastcall + operator delete(Info) 0x00BFE9BC); Data = o.Data; Info = o.Info; if (Info) ++RefCount }.
// Layout: pair {long first +0, CCountedPointer second: Data +4, Info +8}; CCPPointerInfo {RefCount, DeleteFunc, Data}.
// Shape: /O2 (16-aligned entry); the source Data / Info read into locals first (retail loads both before the compare); the compiler-generated operator=, emitted out of line by an assignment in a helper.
struct CCPPointerInfo {
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