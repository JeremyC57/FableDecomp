// CFrontendGameComponent::ShowMoreSaves() -- page the front-end saves list forward by 8, wrapping to the start
// Leaf: ?ShowMoreSaves@CFrontendGameComponent@@AAEXXZ
// Retail: Fable.exe @ 0x004947B0, 69 bytes, `ret` (thiscall, no args); Ego_r.pdb ?ShowMoreSaves@CFrontendGameComponent@@AAEXXZ;
// catalog label ShowMoreProfiles is wrong: it pages SaveNames.
// Body: if more than 8 names, advance SaveNamesOffset by 8 and reset it to 0 once it passes the end (unsigned compares, the
// size re-read after the store).
// Layout: retail CFrontendGameComponent -- SaveNames (12-byte STLport vector<CWideString>) +0x1e1c, SaveNamesOffset +0x1e28
// (Ego_r.pdb +0x1e20 / +0x1e30, its vectors are 16 bytes); same block as ChooseSaveNumber 0x00494800; no engine/ header.
// Shape: /O2 (16-aligned entry).
class CWideString {
public:
    void* PData;
};
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
    std::vector<CWideString> SaveNames;                  // +0x1e1c
    long                     SaveNamesOffset;            // +0x1e28
private:
    void ShowMoreSaves();
    friend struct CFrontendGameComponent_Test;           // behaviour-test hook
};
void CFrontendGameComponent::ShowMoreSaves()
{
    if (SaveNames.size() > 8)
    {
        SaveNamesOffset += 8;
        if ((unsigned long)SaveNamesOffset > SaveNames.size())
            SaveNamesOffset = 0;
    }
}