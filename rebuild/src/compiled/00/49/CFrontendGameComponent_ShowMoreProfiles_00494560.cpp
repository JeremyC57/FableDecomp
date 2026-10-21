// CFrontendGameComponent::ShowMoreProfiles() -- page the front-end profiles list forward by 8, wrapping to the start
// Leaf: ?ShowMoreProfiles@CFrontendGameComponent@@AAEXXZ
// Retail: Fable.exe @ 0x00494560, 69 bytes, `ret` (thiscall, no args); Ego_r.pdb ?ShowMoreProfiles@CFrontendGameComponent@@AAEXXZ;
// ShowMoreProfiles (label correct).
// Body: if more than 8 names, advance ProfileNamesOffset by 8 and reset it to 0 once it passes the end (unsigned compares, the
// size re-read after the store).
// Layout: retail CFrontendGameComponent -- ProfileNames (12-byte STLport vector<CWideString>) +0x1e0c, ProfileNamesOffset +0x1e18
// (Ego_r.pdb +0x1e0c / +0x1e1c, its vectors are 16 bytes); same block as ChooseSaveNumber 0x00494800; no engine/ header.
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
    unsigned char            _pad0000[0x1e0c];
    std::vector<CWideString> ProfileNames;                  // +0x1e0c
    long                     ProfileNamesOffset;            // +0x1e18
private:
    void ShowMoreProfiles();
    friend struct CFrontendGameComponent_Test;           // behaviour-test hook
};
void CFrontendGameComponent::ShowMoreProfiles()
{
    if (ProfileNames.size() > 8)
    {
        ProfileNamesOffset += 8;
        if ((unsigned long)ProfileNamesOffset > ProfileNames.size())
            ProfileNamesOffset = 0;
    }
}