// CFrontendGameComponent::ChooseSaveNumber(long number) -- pick the save shown in on-screen slot `number` (1-based) as the save to load
// Leaf: ?ChooseSaveNumber@CFrontendGameComponent@@AAEXJ@Z
// Retail: Fable.exe @ 0x00494800, 64 bytes, `ret 4`; callers 0x004969B1, 0x004969FA; Ego_r.pdb
// ?ChooseSaveNumber@CFrontendGameComponent@@AAEXJ@Z.
// Body: index = SaveNamesOffset + number - 1; if (index < SaveNames.size()) (unsigned, `jae`): SaveToLoad = SaveNames[index]
// (CWideString::operator= 0x0099B7D0), then the component's own virtual at vtable +0x10 (slot 4, name not recovered).
// Layout: retail CFrontendGameComponent -- SaveNames (12-byte STLport vector<CWideString>) +0x1e1c, SaveNamesOffset +0x1e28,
// SaveToLoad +0x1e2c (Ego_r.pdb +0x1e20 / +0x1e30 / +0x1e34), as at 0x00494770; no engine/ header.
// Shape: /O2 (16-aligned entry).
class CWideString {
public:
    void* PData;
    const CWideString& operator=(const CWideString& s);        // 0x0099B7D0
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
class CFrontendGameComponent {
public:
    virtual ~CFrontendGameComponent();
    virtual void _slot01(); virtual void _slot02(); virtual void _slot03();
    virtual void _slot04();                              // vtable +0x10
    unsigned char            _pad0004[0x1e1c - 4];
    std::vector<CWideString> SaveNames;                  // +0x1e1c
    long                     SaveNamesOffset;            // +0x1e28
    CWideString              SaveToLoad;                 // +0x1e2c
private:
    void ChooseSaveNumber(long number);
    friend struct CFrontendGameComponent_Test;           // behaviour-test hook
};
void CFrontendGameComponent::ChooseSaveNumber(long number)
{
    unsigned long index = SaveNamesOffset + number - 1;
    if (index < SaveNames.size())
    {
        SaveToLoad = SaveNames[index];
        _slot04();
    }
}