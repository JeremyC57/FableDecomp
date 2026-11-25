// Naming (NF451, 2026-11-25; r450b done through the shared header): callee-only respell of the destructor stand-in call to
//   0x00442280 ??1CStatBase@NPlayerGui@@QAE@XZ (FableWin.pdb only, r449c best guess), a qualified destructor call on
//   engine/NPlayerGui_CStatBase.h (its add-only no-containers view, so no STLport and no new .drectve token); was ?BaseDestructor@GuiControlTreePaneOverlay@@QAEXXZ.
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_GUIMODE
#define FABLE_RETAIL_NPLAYERGUI_CSTATBASE_NO_CONTAINERS
#include "engine/NPlayerGui_CStatBase.h"                // ~CStatBase 0x00442280 (FableWin.pdb ??1CStatBase@NPlayerGui@@QAE@XZ)
#include <stdlib.h>

void __cdecl operator delete(void* memory);

struct GuiTreeStorageOverlay
{
    void* memory;
    void SortTreeRecursively();
};

struct GuiControlTreePaneOverlay
{
    unsigned char unused[0x64];
    GuiTreeStorageOverlay tree;

    GuiControlTreePaneOverlay* CGuiControlTreePane_Destructor_2(unsigned char flags);
};

GuiControlTreePaneOverlay* GuiControlTreePaneOverlay::CGuiControlTreePane_Destructor_2(unsigned char flags)
{
    tree.SortTreeRecursively();
    void* memory = tree.memory;
    if (memory != 0) {
        free(memory);
    }
    reinterpret_cast<NPlayerGui::CStatBase*>(this)->NPlayerGui::CStatBase::~CStatBase();
    if ((flags & 1) != 0) {
        operator delete(this);
    }
    return this;
}