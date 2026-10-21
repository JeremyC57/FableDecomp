// CFrontendGameComponent::GetMaxVisibleSaveNumber() -- how many save names fit on screen from the scroll offset: at most 8
// Leaf: ?GetMaxVisibleSaveNumber@CFrontendGameComponent@@QBEJXZ
// Retail: Fable.exe @ 0x00494770, 53 bytes; Ego_r.pdb ?GetMaxVisibleSaveNumber@CFrontendGameComponent@@ABEJXZ; callers 0x004968F2, 0x0049694E, 0x0049695B, 0x004969A7.
// Catalog label GetMaxVisibleProfileNumber is wrong: this reads SaveNames / SaveNamesOffset. Body: SaveNames.size() - SaveNamesOffset, compared unsigned (`ja`) with 8; the
// difference is recomputed for the return (the `size - offset <= 8 ? size - offset : 8` shape).
// Layout: retail CFrontendGameComponent -- SaveNames (12-byte STLport vector<CWideString>) +1e1c, SaveNamesOffset +1e28
// (Ego_r.pdb 0x1e20/0x1e30); no engine/ header.
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
    unsigned char            _pad0000[0x1e1c];
    std::vector<CWideString> SaveNames;          // +0x1e1c
    long                     SaveNamesOffset;    // +0x1e28
    long GetMaxVisibleSaveNumber() const;
};
long CFrontendGameComponent::GetMaxVisibleSaveNumber() const
{
    if (SaveNames.size() - SaveNamesOffset <= 8)
        return SaveNames.size() - SaveNamesOffset;
    return 8;
}