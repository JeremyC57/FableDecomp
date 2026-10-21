// CFrontendGameComponent::ShowMoreWorlds() -- page the front-end worlds list forward by 8, wrapping to the start
// Leaf: ?ShowMoreWorlds@CFrontendGameComponent@@AAEXXZ
// Retail: Fable.exe @ 0x004946C0, 69 bytes, `ret` (thiscall, no args); Ego_r.pdb ?ShowMoreWorlds@CFrontendGameComponent@@AAEXXZ;
// catalog label ShowMoreProfiles is wrong: it pages WorldNames.
// Body: if more than 8 names, advance WorldNamesOffset by 8 and reset it to 0 once it passes the end (unsigned compares, the
// size re-read after the store).
// Layout: retail CFrontendGameComponent -- WorldNames (12-byte STLport vector<CWideString>) +0x1e30, WorldNamesOffset +0x1e3c
// (Ego_r.pdb +0x1e38 / +0x1e48, its vectors are 16 bytes); same block as ChooseSaveNumber 0x00494800; no engine/ header.
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
    unsigned char            _pad0000[0x1e30];
    std::vector<CWideString> WorldNames;                  // +0x1e30
    long                     WorldNamesOffset;            // +0x1e3c
private:
    void ShowMoreWorlds();
    friend struct CFrontendGameComponent_Test;           // behaviour-test hook
};
void CFrontendGameComponent::ShowMoreWorlds()
{
    if (WorldNames.size() > 8)
    {
        WorldNamesOffset += 8;
        if ((unsigned long)WorldNamesOffset > WorldNames.size())
            WorldNamesOffset = 0;
    }
}