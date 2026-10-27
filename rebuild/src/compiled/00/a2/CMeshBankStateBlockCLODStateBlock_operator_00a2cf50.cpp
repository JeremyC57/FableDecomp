// CMeshBankStateBlock::CLODStateBlock::operator=(const CMeshBankStateBlock::CLODStateBlock& other) -- memberwise assignment: the leading part through its out-of-line operator=, then the counted pointer (release the old block, adopt, add-ref)
// Leaf: ??4CLODStateBlock@CMeshBankStateBlock@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a2cf50, 80 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ??4CLODStateBlock@CMeshBankStateBlock@@QAEAAV01@ABV01@@Z; its caller CMeshBankStateBlock::Set 0x00A2D6C0 (landed) assigns the block at +0x14 (Ego_r.pdb CMeshBankStateBlock::LODState); Ego_r.pdb members BoneLODs (map<CCharString, long>, the out-of-line operator= 0x00432B38) and MinXYZRange (CCountedPointer<C3DBoundingBox>, retail +0x0C).
// Callees: map<CCharString, long>::operator= 0x00432B38, operator delete 0x00BFE9BC (the old count block); CCountedPointer's operator= inline.
// Shape (family L088): the compiler-generated copy assignment, written out (same body) so the translation unit emits it.
class CCPPointerInfo {                                     // 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;                 // the source's fields are read first
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
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
class CCharString { public: void* PStringData; };
class C3DBoundingBox;
namespace std {
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                                // STLport 4.6, 12 bytes
public:
    void* _M_t[3];
    map& operator=(const map& __x);                        // 0x00432B38 for map<CCharString, long>
};
}
class CMeshBankStateBlock {
public:
    class CLODStateBlock {
    public:
        std::map<CCharString, long>     BoneLODs;          // +0x00
        CCountedPointer<C3DBoundingBox> MinXYZRange;       // +0x0C
        CLODStateBlock& operator=(const CLODStateBlock& other);
    };
};
CMeshBankStateBlock::CLODStateBlock& CMeshBankStateBlock::CLODStateBlock::operator=(const CLODStateBlock& other)
{
    BoneLODs = other.BoneLODs;
    MinXYZRange = other.MinXYZRange;
    return *this;
}