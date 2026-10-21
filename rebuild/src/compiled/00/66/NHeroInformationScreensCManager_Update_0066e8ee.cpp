// NHeroInformationScreens::CManager::Update() -- when the highlighted position changed, scroll the columns forwards or backwards (wrapping at either end)
// Leaf: ?Update@CManager@NHeroInformationScreens@@QAEXXZ
// Retail: Fable.exe @ 0x0066E8EE, 83 bytes, plain `ret`; Ego_r.pdb ?Update@CManager@NHeroInformationScreens@@QAEXXZ.
// Body: columns = ScrollBarIndices.size(), screens = InventoryScreens.size() (both up front); if CurrentScreen (+0x28) !=
// LastScreen (+0x2c): wrap-around forwards (Last >= screens, Current == 0) -> ProcessNextScreenAction (0x0066E845);
// wrap-around backwards (Last == 0, Current >= screens) -> ProcessPreviousScreenAction (0x0066E8A1); otherwise by the sign
// of Current - Last; then LastScreen = CurrentScreen.
// Layout: retail NHeroInformationScreens::CManager (as 0x0066EA46 / 0x0066E8A1): InventoryScreens +0x10,
// ScrollBarIndices +0x1c (12-byte STLport vectors), CurrentScreen +0x28, LastScreen +0x2c.
// Shape: size TU (unaligned entry).
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
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
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
    void ProcessNextScreenAction(long number_of_columns, long number_of_screens);       // 0x0066E845
    void ProcessPreviousScreenAction(long number_of_columns, long number_of_screens);   // 0x0066E8A1
    void Update();
};
void CManager::Update()
{
    const long columns = ScrollBarIndices.size();
    const long screens = InventoryScreens.size();
    if (CurrentScreen != LastScreen)
    {
        if (LastScreen >= screens && CurrentScreen == 0)
            ProcessNextScreenAction(columns, screens);
        else if (LastScreen == 0 && CurrentScreen >= screens)
            ProcessPreviousScreenAction(columns, screens);
        else if (CurrentScreen > LastScreen)
            ProcessNextScreenAction(columns, screens);
        else if (CurrentScreen < LastScreen)
            ProcessPreviousScreenAction(columns, screens);
        LastScreen = CurrentScreen;
    }
}
}