#include <cstdio>
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


namespace NHeroInformationScreens {
CBase::~CBase() {}
void CBase::_slot01() const {} void CBase::_slot02() const {}
bool CBase::IsSelectable() const { return true; }
class CTestScreen : public CBase { public: bool Ok; virtual bool IsSelectable() const { return Ok; } };
static CTestScreen g_screens[4];
const CBase* CManager::GetScreenAtScrollIndex(long index) { return &g_screens[ScrollBarIndices[index]]; }
}
using namespace NHeroInformationScreens;
int main()
{
    g_screens[0].Ok = true; g_screens[1].Ok = false; g_screens[2].Ok = true; g_screens[3].Ok = false;
    static CManager m; static long idx[2] = { 0, 3 };
    m.ScrollBarIndices._M_start = idx; m.ScrollBarIndices._M_finish = idx + 2;
    m.HighlightedScreen = 1;
    m.ProcessPreviousScreenAction(2, 4);
    bool ok = m.HighlightedScreen == 0 && idx[0] == 0 && idx[1] == 3;
    m.ProcessPreviousScreenAction(2, 4);
    ok = ok && m.HighlightedScreen == 0 && idx[0] == 2 && idx[1] == 2;
    m.ProcessPreviousScreenAction(1, 4);
    ok = ok && idx[0] == 0 && idx[1] == 2;
    if (ok) std::printf("0066e8a1_TEST PASS\n");
    return 0;
}