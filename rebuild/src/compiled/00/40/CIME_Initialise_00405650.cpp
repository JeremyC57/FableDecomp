// CIME::Initialise(HWND__*) -- reset IME, install native callbacks and configure appearance/screen dimensions
// Leaf: ?Initialise@CIME@NUISystem@@QAEXPAUHWND__@@@Z
// Retail00405650,206 bytes. Actual fable_ime.h layout retained, including the full64-byte Appearance.
// Appearance.symbolFont is at+16 (=this+1c), distinct from CIME.editBox_ at4c.
// Native nonvirtual Reset method missing from actual header uses a field-free derived method adapter.
// Naming (NF142, 2026-11-20, q4291): the CSystemManagerNative / CDisplayManagerNative stand-ins and the local C2DExtentsI model replaced by
//   engine/CSystemManager.h, engine/CDisplayManager.h and engine/C2DExtentsI.h: GetRenderTargetDimensions 0x009BEDC0 is Ego_r.pdb 0x00B242A0 +
//   FableWin.pdb 0x03022ED0 ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ (hidden C2DExtentsI result, `ret 4`), reached through the inline CSystemManager::GetDisplayManager (PDisplayManager Data word +0x60), as GFABoxToVBox 0x009E2020.
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CIME_CALLBACKS
#include "fable_ime.h"
#include "fable_string.h"
#include "fable_compiler.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/C2DExtentsI.h"
#include "engine/CDisplayManager.h"
#include "engine/CSystemManager.h"
// Native RTTI0122DCBC: CIMEFont derives CImeUiFont_Base at0 (donor size4).
// Base has exactly five genuine virtual interface slots, no virtual destructor:
// native slot0 SetHeight,1 SetColor,2 SetPosition,3 GetTextExtent,4 DrawTextW.
class CImeUiFont_Base {
public:
    virtual void SetHeight(unsigned int)=0;
    virtual void SetColor(unsigned long)=0;
    virtual void SetPosition(int,int)=0;
    virtual void GetTextExtent(const wchar_t*,unsigned long*,unsigned long*)=0;
    virtual void DrawTextW(const wchar_t*)=0;
};
namespace NUISystem {

class CIMEFont : public CImeUiFont_Base {
public:
    virtual void SetHeight(unsigned int);
    virtual void SetColor(unsigned long);
    virtual void SetPosition(int,int);
    virtual void GetTextExtent(const wchar_t*,unsigned long*,unsigned long*);
    virtual void DrawTextW(const wchar_t*);
    fable_u32 Colour;
    float Position[2];
    fable_u32 Height;
    void GetFontNameAndScale(CCharString&,float&) const;
};

struct NativeCIME : CIME { void Reset(); }; //004051C0
}
struct IMEUI_VERTEX;
extern void (FABLE_FASTCALL *ImeUiCallback_DrawRect)(int,int,int,int,unsigned long); //0143CD64 (DXUT ImeUi.h; was g_ImeDrawRect, Codex pass 12 respell)
extern void* (FABLE_FASTCALL *ImeUiCallback_Malloc)(unsigned int); //0143E514 (DirectX SDK ImeUi.cpp; as 0x00CB2310 / 0x00CB2670)
extern void (FABLE_FASTCALL *ImeUiCallback_Free)(void*); //0143CF08 (DirectX SDK ImeUi.cpp; as 0x00CB2310)
extern void (FABLE_FASTCALL *ImeUiCallback_DrawFans)(const IMEUI_VERTEX*,unsigned int); //0143CDFC (SDK ImeUi.cpp callback pointer, chunk 685; naming wave 19)
extern void (FABLE_FASTCALL *ImeUiCallback_OnChar)(wchar_t); //0143E630 (DXUT ImeUi.h; was g_ImeOnChar, Codex pass 12 respell)
void FABLE_FASTCALL Callback_DrawRect(int,int,int,int,unsigned long); //00404D50
// NF338: Callback_malloc 0x00404A60 (malloc importBFEA0E) is the PDB public static CIME::Callback_malloc (Ego_r + FableWin ?Callback_malloc@CIME@NUISystem@@SIPAXI@Z), fable_ime.h FABLE_RETAIL_CIME_CALLBACKS
void FABLE_FASTCALL Callback_free(void*); //00404A70, freeBFEA14
// NF338: Callback_DrawFans 0x00404E10 is the PDB public static CIME::Callback_DrawFans (Ego_r + FableWin ?Callback_DrawFans@CIME@NUISystem@@SIXPBUIMEUI_VERTEX@@I@Z), same opt-in
void FABLE_FASTCALL Callback_OnChar(wchar_t); //00404F00
bool FABLE_FASTCALL ImeUi_Initialize(HWND__*,bool); //00CB5180 (FableWin.pdb ?ImeUi_Initialize@@YI_NPAUHWND__@@_N@Z; naming wave 19)
void FABLE_FASTCALL ImeUi_EnableIme(bool); //00CB5130
void FABLE_FASTCALL ImeUi_FinalizeString(bool); //00CB2C60
struct IMEUI_APPEARANCE;
void FABLE_FASTCALL ImeUi_SetAppearance(const IMEUI_APPEARANCE*); //00CB2E20
void FABLE_FASTCALL ImeUi_SetScreenDimension(unsigned int,unsigned int); //00CB2EE0
struct IMEUI_APPEARANCE {
 fable_u32 symbolColor,symbolColorOff,symbolColorText;
 fable_u8 symbolHeight,symbolTranslucence,symbolPlacement;
 CImeUiFont_Base* symbolFont;
 fable_u32 candColorBase,candColorBorder,candColorText;
 fable_u32 compColorInput,compColorTargetConv,compColorConverted,compColorTargetNotConv,compColorInputErr;
 fable_u8 compTranslucence;
 fable_u32 compColorText;
 fable_u8 caretWidth,caretYMargin;
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(IMEUI_APPEARANCE)==0x40);
FABLE_STATIC_ASSERT(offsetof(IMEUI_APPEARANCE,symbolFont)==0x10);
#endif
namespace NUISystem {
void CIME::Initialise(HWND__* window) {
 static_cast<NativeCIME*>(this)->Reset();
 if(enableIme_) {
  ImeUiCallback_DrawRect=Callback_DrawRect;
  ImeUiCallback_Malloc=Callback_malloc;
  ImeUiCallback_Free=Callback_free;
  ImeUiCallback_DrawFans=Callback_DrawFans;
  ImeUiCallback_OnChar=Callback_OnChar;
  window_=window;
  initialised_=ImeUi_Initialize(window_,false);
  if(initialised_) {
   reinterpret_cast<IMEUI_APPEARANCE*>(appearance_)->symbolFont=new CIMEFont;
   ImeUi_SetAppearance(reinterpret_cast<IMEUI_APPEARANCE*>(appearance_));
   C2DExtentsI dimensions=GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions();
   if(initialised_) ImeUi_SetScreenDimension(dimensions.X,dimensions.Y);
  }
  if(initialised_) {
   ImeUi_FinalizeString(false);
   ImeUi_EnableIme(false);
  }
 } else ImeUi_Initialize(window,true);
}
}