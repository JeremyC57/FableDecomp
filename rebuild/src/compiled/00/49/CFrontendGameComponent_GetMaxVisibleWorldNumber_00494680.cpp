// CFrontendGameComponent::GetMaxVisibleWorldNumber() -- how many world names fit on screen from the scroll offset: at most 8
// Leaf: ?GetMaxVisibleWorldNumber@CFrontendGameComponent@@QBEJXZ
// Retail: Fable.exe @ 0x00494680, 53 bytes; Ego_r.pdb ?GetMaxVisibleWorldNumber@CFrontendGameComponent@@ABEJXZ; callers 0x00496759, 0x004967B5, 0x004967C2, 0x0049680E.
// Catalog label GetMaxVisibleProfileNumber is wrong (ICF-style propagation): this reads WorldNames / WorldNamesOffset. Body: WorldNames.size() - WorldNamesOffset, compared unsigned (`ja`) with 8; the
// difference is recomputed for the return (the `size - offset <= 8 ? size - offset : 8` shape).
// Layout: retail CFrontendGameComponent -- WorldNames (12-byte STLport vector<CWideString>) +1e30, WorldNamesOffset +1e3c
// (Ego_r.pdb 0x1e38/0x1e48); no engine/ header.
// Shape: /O2.
class CWideString { public: void* PData; };
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
};
}
class CFrontendGameComponent {
public:
    unsigned char            _pad0000[0x1e30];
    std::vector<CWideString> WorldNames;         // +0x1e30
    long                     WorldNamesOffset;   // +0x1e3c
    long GetMaxVisibleWorldNumber() const;
};
long CFrontendGameComponent::GetMaxVisibleWorldNumber() const
{
    if (WorldNames.size() - WorldNamesOffset <= 8)
        return WorldNames.size() - WorldNamesOffset;
    return 8;
}