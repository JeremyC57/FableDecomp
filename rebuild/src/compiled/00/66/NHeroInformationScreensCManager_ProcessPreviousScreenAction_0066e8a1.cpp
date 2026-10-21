// NHeroInformationScreens::CManager::ProcessPreviousScreenAction(long number_of_columns, long number_of_screens) -- scroll every column back one screen, skipping screens that refuse
// Leaf: ?ProcessPreviousScreenAction@CManager@NHeroInformationScreens@@QAEXJJ@Z
// Retail: Fable.exe @ 0x0066E8A1, 77 bytes, `ret 8`; one caller 0x0066E933; Ego_r.pdb
// ?ProcessPreviousScreenAction@CManager@NHeroInformationScreens@@QAEXJJ@Z.
// Body: if (--HighlightedScreen < 0) { HighlightedScreen = 0; for each column i: do { --ScrollBarIndices[i], wrapping
// below 0 by + number_of_screens } while (!GetScreenAtScrollIndex(i) (0x0066E7E1)->virtual slot 3 (+0x0c, a bool query)) }.
// Layout: retail NHeroInformationScreens::CManager (as 0x0066EA46 / 0x0066E941): ScrollBarIndices +0x1c (12-byte STLport
// vector<long>), HighlightedScreen +0x30 (Ego_r.pdb +0x24 / +0x3c with 16-byte vectors).
// Shape: size TU (unaligned entry); the element re-read after the decrement (the vector may alias).
#pragma optimize("s", on)
class CWorld;
namespace NHeroInformationScreens {
class CBase {
public:
    virtual ~CBase();
    virtual void _slot01() const; virtual void _slot02() const;
    virtual bool IsSelectable() const;                   // slot 3 (+0x0c), name not recovered
};
template <class T> class CVector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    T& operator[](unsigned int n) { return _M_start[n]; }
};
class CManager {
public:
    CWorld*               World;                         // +0x00
    CVector<const CBase*> Screens;                       // +0x04
    CVector<const CBase*> InventoryScreens;              // +0x10
    CVector<long>         ScrollBarIndices;              // +0x1c
    long                  CurrentScreen;                 // +0x28
    long                  LastScreen;                    // +0x2c
    long                  HighlightedScreen;             // +0x30
    const CBase* GetScreenAtScrollIndex(long index);     // 0x0066E7E1
    void ProcessPreviousScreenAction(long number_of_columns, long number_of_screens);
};
void CManager::ProcessPreviousScreenAction(long number_of_columns, long number_of_screens)
{
    if (--HighlightedScreen < 0)
    {
        HighlightedScreen = 0;
        for (long i = 0; i < number_of_columns; ++i)
        {
            do
            {
                --ScrollBarIndices[i];
                if (ScrollBarIndices[i] < 0)
                    ScrollBarIndices[i] += number_of_screens;
            } while (!GetScreenAtScrollIndex(i)->IsSelectable());
        }
    }
}
}