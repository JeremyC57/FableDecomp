// NHeroInformationScreens::CManager::~CManager() -- release the ScrollBarIndices, InventoryScreens and Screens vector buffers
// Leaf: ??1CManager@NHeroInformationScreens@@QAE@XZ
// Retail: Fable.exe @ 0x00441380, 53 bytes, plain `ret`, no direct reference (reached through the CPlayerGui
// PHeroInfoScreensManager scoped_ptr teardown). Catalog label CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures
// is wrong: the body frees, in reverse member order, the _M_start buffers of three 12-byte STLport vectors at +0x1c,
// +0x10 and +0x04 (free = 0x00BFEA14, each behind a null test) -- exactly the vector members of the 0x60-byte
// Ego_r.pdb NHeroInformationScreens::CManager (World, Screens, InventoryScreens, ScrollBarIndices; the trailing
// CFadeInFadeOutBase is trivially destructible), whose ??1CManager@NHeroInformationScreens@@QAE@XZ sits beside the
// CPlayerGui code in Ego_r.pdb as this one does in retail. The only PDB struct with that vector pattern.
// Layout: retail NHeroInformationScreens::CManager -- World +0, Screens +4, InventoryScreens +0x10, ScrollBarIndices +0x1c
// (12-byte STLport vectors), as modelled at 0x0066EA85; the members past +0x28 do not reach the destructor.
// Shape: /O2.
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