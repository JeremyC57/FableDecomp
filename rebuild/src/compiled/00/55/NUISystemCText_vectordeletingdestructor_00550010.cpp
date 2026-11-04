// NUISystem::CText::`vector deleting destructor'(unsigned int) adjustor thunk -- this-4 (the CObserver subobject at +4; `sub ecx,4 ; jmp ??_G`)
// Leaf: ??_ECText@NUISystem@@W3AEPAXI@Z
// Flags: none
//
// Retail 0x00550010, 8 bytes: the VC7.1 adjustor thunk in slot 0 of CText's CObserver vtable 0x01249CA4 (+4); it moves
// `this` back to the full object and jumps to the deleting destructor ??_G 0x00550030 (Ego_r.pdb names the slot
// ??_ECText@NUISystem@@UAEPAXI@Z). VC7.1 emits the thunk in the TU that defines the virtual destructor (precedent:
// CScrollingViewport 0x0053FC10 / 0x0053FC20), so this source is the destructor's TU: ~CText (0x0054F6E0, landed: clear both
// primitive handles and the font, then release the def's font by name through the system font manager) defined here makes
// the compiler emit ??_G and both thunks.
// Callees: ??_GCText@NUISystem@@UAEPAXI@Z 0x00550030.
// Headers: engine/CText.h (retail branch + FABLE_RETAIL_CTEXT_LAYOUT, chunk 749), engine/CSystemManager.h (GetFontManager).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CTEXT_LAYOUT
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/CText.h"
#include "engine/CSystemManager.h"
#include "engine/CFontBank.h"
class CFontManager {                     // no shared header: only the PDB method this destructor calls
public:
    void ReleaseFont(const CCharString& name);                       // 0x009E2B40
};
namespace NUISystem {
CText::~CText()
{
    Handle.Clear();
    HandleDropShadowBottomRight.Clear();
    PFont.Reset(0);
    GFGetSystemManager()->GetFontManager()->ReleaseFont(LoadAndReturnDef()->Font.GetString());
}
}