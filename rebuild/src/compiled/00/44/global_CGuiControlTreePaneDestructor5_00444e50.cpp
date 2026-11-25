// Naming (NF451, 2026-11-25; r450b done through the shared header): callee-only respell of the destructor stand-in call to
//   0x00442280 ??1CStatBase@NPlayerGui@@QAE@XZ (FableWin.pdb only, r449c best guess), a qualified destructor call on
//   engine/NPlayerGui_CStatBase.h (its add-only no-containers view, so no STLport and no new .drectve token); was ?BaseDestructor@GuiControlTreePaneOverlay5@@QAEXXZ.
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_GUIMODE
#define FABLE_RETAIL_NPLAYERGUI_CSTATBASE_NO_CONTAINERS
#include "engine/NPlayerGui_CStatBase.h"                // ~CStatBase 0x00442280 (FableWin.pdb ??1CStatBase@NPlayerGui@@QAE@XZ)
#include <stdlib.h>

void __cdecl operator delete(void* memory);

struct GuiSortStateOverlay
{
    void SortTreeRecursively_0043fd80();
};

struct GuiTreeStorageOverlay5
{
    void* memory;
    void SortTreeRecursively();
};

struct GuiControlTreePaneOverlay5
{
    unsigned char unused[0x64];
    GuiTreeStorageOverlay5 tree;
    GuiSortStateOverlay state;

};

void __fastcall CGuiControlTreePane_Destructor_5(GuiControlTreePaneOverlay5* self)
{
    if (self != 0) {
        self->state.SortTreeRecursively_0043fd80();
        self->tree.SortTreeRecursively();
        void* memory = self->tree.memory;
        if (memory != 0) {
            free(memory);
        }
        NPlayerGui::CStatBase* base = reinterpret_cast<NPlayerGui::CStatBase*>(self);
        base->NPlayerGui::CStatBase::~CStatBase();
        operator delete(self);
    }
}