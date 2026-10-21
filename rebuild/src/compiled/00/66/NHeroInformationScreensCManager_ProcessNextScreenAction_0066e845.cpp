// NHeroInformationScreens::CManager::ProcessNextScreenAction(long no_slots, long no_screens) -- move the highlight down; past the last slot, scroll every slot to its next displayable screen
// Leaf: ?ProcessNextScreenAction@CManager@NHeroInformationScreens@@QAEXJJ@Z
// Retail: Fable.exe @ 0x0066E845, 92 bytes, `ret 8`; Ego_r.pdb ?ProcessNextScreenAction@CManager@NHeroInformationScreens@@QAEXJJ@Z.
// Body: ++HighlightedScreen (+0x30); if it passes no_slots - 1 it is clamped there and, for each slot i, ScrollBarIndices[i]
// (+0x1c) is advanced (wrapping by no_screens) until GetScreenAtScrollIndex(i) (0x0066E7E1) answers its virtual slot 3
// (+0x0c) true.
// Layout: retail NHeroInformationScreens::CManager as modelled for 0x0066E7E1 / 0x0066EA46 (12-byte STLport vectors:
// ScrollBarIndices +0x1c, HighlightedScreen +0x30). The CBase slot-3 name is not confirmed.
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
class CWorld;
template <class T> class vector {                        // STLport vector<T>
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
namespace NHeroInformationScreens {
class CBase {
public:
    virtual void _v00() const;
    virtual void _v01() const;
    virtual void _v02() const;
    virtual bool IsDisplayable() const;                  // slot 3 (+0x0c, name not confirmed)
};
class CManager {
public:
    CWorld*              World;                          // +0x00
    vector<const CBase*> Screens;                        // +0x04
    vector<const CBase*> InventoryScreens;               // +0x10
    vector<long>         ScrollBarIndices;               // +0x1c
    long                 CurrentScreen;                  // +0x28
    long                 LastScreen;                     // +0x2c
    long                 HighlightedScreen;              // +0x30
    const CBase* GetScreenAtScrollIndex(long scroll_index);   // 0x0066E7E1
    void ProcessNextScreenAction(long no_slots, long no_screens);
};
void CManager::ProcessNextScreenAction(long no_slots, long no_screens)
{
    ++HighlightedScreen;
    if (HighlightedScreen > no_slots - 1)
    {
        HighlightedScreen = no_slots - 1;
        for (long i = 0; i < no_slots; ++i)
        {
            do
            {
                ++ScrollBarIndices[i];
                if (ScrollBarIndices[i] > no_screens - 1)
                    ScrollBarIndices[i] -= no_screens;
            } while (!GetScreenAtScrollIndex(i)->IsDisplayable());
        }
    }
}
}