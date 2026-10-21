// CFrontendGameComponent::GetMaxVisibleProfileNumber() -- how many profile names fit on screen from the scroll offset: at most 8
// Leaf: ?GetMaxVisibleProfileNumber@CFrontendGameComponent@@QBEJXZ
// Retail: Fable.exe @ 0x00494520, 53 bytes; Ego_r.pdb ?GetMaxVisibleProfileNumber@CFrontendGameComponent@@ABEJXZ; callers 0x00496ABB, 0x00496B2C, 0x00496B39, 0x00496B85.
// Catalog label is right. Body: ProfileNames.size() - ProfileNamesOffset, compared unsigned (`ja`) with 8; the
// difference is recomputed for the return (the `size - offset <= 8 ? size - offset : 8` shape).
// Layout: retail CFrontendGameComponent -- ProfileNames (12-byte STLport vector<CWideString>) +1e0c, ProfileNamesOffset +1e18
// (Ego_r.pdb 0x1e0c/0x1e18); no engine/ header.
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
    unsigned char            _pad0000[0x1e0c];
    std::vector<CWideString> ProfileNames;       // +0x1e0c
    long                     ProfileNamesOffset; // +0x1e18
    long GetMaxVisibleProfileNumber() const;
};
long CFrontendGameComponent::GetMaxVisibleProfileNumber() const
{
    if (ProfileNames.size() - ProfileNamesOffset <= 8)
        return ProfileNames.size() - ProfileNamesOffset;
    return 8;
}