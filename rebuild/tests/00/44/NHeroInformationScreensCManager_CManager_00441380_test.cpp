#include <cstdio>
#include <stdlib.h>
class CWorld;
namespace NHeroInformationScreens {
class CBase;
template <class T> class CVector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    ~CVector() { if (_M_start) free(_M_start); }
};
class CManager {
public:
    CWorld*               World;                         // +0x00
    CVector<const CBase*> Screens;                       // +0x04
    CVector<const CBase*> InventoryScreens;              // +0x10
    CVector<long>         ScrollBarIndices;              // +0x1c
    ~CManager();
};
CManager::~CManager()
{
}
}

#include <new>
int main()
{
    using namespace NHeroInformationScreens;
    static double mem[8];
    CManager* m = new (mem) CManager;
    m->World = 0;
    m->Screens._M_start = (const CBase**)malloc(16); m->Screens._M_finish = m->Screens._M_start;
    m->InventoryScreens._M_start = 0; m->InventoryScreens._M_finish = 0;
    m->ScrollBarIndices._M_start = (long*)malloc(40); m->ScrollBarIndices._M_finish = m->ScrollBarIndices._M_start;
    m->~CManager();
    CManager* z = new (mem) CManager;
    z->Screens._M_start = 0; z->InventoryScreens._M_start = 0; z->ScrollBarIndices._M_start = 0;
    z->~CManager();
    std::printf("00441380_TEST PASS\n");
    return 0;
}