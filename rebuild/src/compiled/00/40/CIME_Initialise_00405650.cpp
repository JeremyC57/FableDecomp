// CIME::Initialise(HWND__*) -- reset IME, install native callbacks and configure appearance/screen dimensions
// Retail00405650,206 bytes. Actual fable_ime.h layout retained, including the full64-byte Appearance.
// Appearance.symbolFont is at+16 (=this+1c), distinct from CIME.editBox_ at4c.
// Native nonvirtual Reset method missing from actual header uses a field-free derived method adapter.
// No CSystemManager/CDisplayManager headers exist, including quarantine.
// GetRenderTargetDimensions uses explicit genuine hidden-result pointer projection of009BEDC0;
// native caller pushes8-byte result address; callee writes X,Y, returns output pointer in EAX and ret4.
#include "fable_ime.h"
#include "fable_string.h"
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
extern void (FABLE_FASTCALL *g_ImeDrawRect)(int,int,int,int,unsigned long); //0143CD64
extern void* (FABLE_FASTCALL *g_ImeMalloc)(unsigned int); //0143E514
extern void (FABLE_FASTCALL *g_ImeFree)(void*); //0143CF08
extern void (FABLE_FASTCALL *g_ImeDrawFans)(const IMEUI_VERTEX*,unsigned int); //0143CDFC
extern void (FABLE_FASTCALL *g_ImeOnChar)(wchar_t); //0143E630
void FABLE_FASTCALL Callback_DrawRect(int,int,int,int,unsigned long); //00404D50
void* FABLE_FASTCALL Callback_malloc(unsigned int); //00404A60, genuine malloc importBFEA0E
void FABLE_FASTCALL Callback_free(void*); //00404A70, freeBFEA14
void FABLE_FASTCALL Callback_DrawFans(const IMEUI_VERTEX*,unsigned int); //00404E10
void FABLE_FASTCALL Callback_OnChar(wchar_t); //00404F00
bool FABLE_FASTCALL ImeUI_Initialize(HWND__*,bool); //00CB5180
void FABLE_FASTCALL ImeUi_EnableIme(bool); //00CB5130
void FABLE_FASTCALL ImeUi_FinalizeString(bool); //00CB2C60
struct IMEUI_APPEARANCE;
void FABLE_FASTCALL ImeUi_SetAppearance(const IMEUI_APPEARANCE*); //00CB2E20
void FABLE_FASTCALL ImeUi_SetScreenDimension(unsigned int,unsigned int); //00CB2EE0
class C2DExtentsI { public: fable_i32 X,Y; };
struct CDisplayManagerNative {
 C2DExtentsI* GetRenderTargetDimensions(C2DExtentsI&) const; //009BEDC0 explicit hidden result ABI projection
};
struct CSystemManagerNative {
 unsigned char BeforeDisplayManager[0x60];
 CDisplayManagerNative* PDisplayManager; //PDB CCountedPointer::Data+60
};
CSystemManagerNative* FABLE_FASTCALL GFGetSystemManager(); //009A4EC0
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
  g_ImeDrawRect=Callback_DrawRect;
  g_ImeMalloc=Callback_malloc;
  g_ImeFree=Callback_free;
  g_ImeDrawFans=Callback_DrawFans;
  g_ImeOnChar=Callback_OnChar;
  window_=window;
  initialised_=ImeUI_Initialize(window_,false);
  if(initialised_) {
   reinterpret_cast<IMEUI_APPEARANCE*>(appearance_)->symbolFont=new CIMEFont;
   ImeUi_SetAppearance(reinterpret_cast<IMEUI_APPEARANCE*>(appearance_));
   C2DExtentsI dimensions;
   CSystemManagerNative* manager=GFGetSystemManager();
   manager->PDisplayManager->GetRenderTargetDimensions(dimensions);
   if(initialised_) ImeUi_SetScreenDimension(dimensions.X,dimensions.Y);
  }
  if(initialised_) {
   ImeUi_FinalizeString(false);
   ImeUi_EnableIme(false);
  }
 } else ImeUI_Initialize(window,true);
}
}