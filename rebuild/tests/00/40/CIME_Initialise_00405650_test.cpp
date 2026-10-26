#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
#define private public
// CIME::Initialise(HWND__*) -- reset IME, install native callbacks and configure appearance/screen dimensions
// Retail00405650,206 bytes. Actual fable_ime.h layout retained, including the full64-byte Appearance.
// Appearance.symbolFont is at+16 (=this+1c), distinct from CIME.editBox_ at4c.
// Native nonvirtual Reset method missing from actual header uses a field-free derived method adapter.
// No CSystemManager/CDisplayManager headers exist, including quarantine.
// GetRenderTargetDimensions uses explicit genuine hidden-result pointer projection of009BEDC0;
// native caller pushes8-byte result address; callee writes X,Y, returns output pointer in EAX and ret4.
#pragma once

#pragma once

// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#ifndef _MSC_VER
#define FABLE_STATIC_ASSERT(expression)
#else
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_abi_assert_, __LINE__)[(expression) ? 1 : -1]
#endif


struct HWND__;
class CPersistContext;

namespace NUISystem
{
    class CIME
    {
    public:
        static void FABLE_FASTCALL LoadIMESettings(
            CPersistContext& context);
        void Initialise(HWND__* window);

    private:
        bool enableIme_;
        fable_u8 padding01_[3];
        HWND__* window_;
        bool initialised_;
        fable_u8 padding09_[3];
        fable_u8 appearance_[0x40];
        void* editBox_;
        bool cancelOnOkClick_;
        fable_u8 padding51_[3];
    };
}

FABLE_STATIC_ASSERT(sizeof(NUISystem::CIME) == 0x54);

#pragma once

#include <stddef.h>



class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#pragma pack(push, 1)
class CCharStringData
{ public:
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#pragma pack(pop)

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    long GetLength() const;
    CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    bool operator!=(const wchar_t* other) const;

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

private:
    explicit CWideString(const CCharString& text);
    void UnassignString();
    CWideStringData* AllocStringData(
        const wchar_t* text,
        long length);

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, long length);
    ~CCharString();
    operator const char*() const;
    CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

private:
    void UnassignString();
    CCharStringData* AllocStringData(
        const char* text,
        long length);

    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);

// Retail folds equivalent narrow/wide string lifetime helpers. These counters
// remain separate because their addresses and object families are distinct.
extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];

FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);

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


#include <assert.h>
#include <stdio.h>
namespace NUISystem {
void CIMEFont::SetHeight(unsigned int h) {Height=h;}
void CIMEFont::SetColor(unsigned long c) {Colour=(fable_u32)c;}
void CIMEFont::SetPosition(int x,int y) {Position[0]=(float)x;Position[1]=(float)y;}
void CIMEFont::GetTextExtent(const wchar_t*,unsigned long*,unsigned long*) {}
void CIMEFont::DrawTextW(const wchar_t*) {}
}
void (FABLE_FASTCALL *g_ImeDrawRect)(int,int,int,int,unsigned long);
void* (FABLE_FASTCALL *g_ImeMalloc)(unsigned int);
void (FABLE_FASTCALL *g_ImeFree)(void*);
void (FABLE_FASTCALL *g_ImeDrawFans)(const IMEUI_VERTEX*,unsigned int);
void (FABLE_FASTCALL *g_ImeOnChar)(wchar_t);
void FABLE_FASTCALL Callback_DrawRect(int,int,int,int,unsigned long) {}
void* FABLE_FASTCALL Callback_malloc(unsigned int) {return 0;}
void FABLE_FASTCALL Callback_free(void*) {}
void FABLE_FASTCALL Callback_DrawFans(const IMEUI_VERTEX*,unsigned int) {}
void FABLE_FASTCALL Callback_OnChar(wchar_t) {}
static int resets,initializes,appearanceCalls,systemCalls,dimensionsCalls,screenCalls,compositionCalls,enableCalls;
static bool success,disableDuringDimensions;
static HWND__* observedWindow;
static bool observedDisable;
static NUISystem::CIME* active;
static CSystemManagerNative systemManager;
static CDisplayManagerNative displayManager;
static IMEUI_APPEARANCE* observedAppearance;
namespace NUISystem { void NativeCIME::Reset() { ++resets; } }
bool FABLE_FASTCALL ImeUI_Initialize(HWND__* w,bool disabled) {
 ++initializes;observedWindow=w;observedDisable=disabled;return success;
}
void FABLE_FASTCALL ImeUi_EnableIme(bool b) {assert(!b);++enableCalls;}
void FABLE_FASTCALL ImeUi_FinalizeString(bool b) {assert(!b);++compositionCalls;}
void FABLE_FASTCALL ImeUi_SetAppearance(const IMEUI_APPEARANCE* a) {
 ++appearanceCalls;observedAppearance=(IMEUI_APPEARANCE*)a;
 assert(observedAppearance->symbolFont);
}
void FABLE_FASTCALL ImeUi_SetScreenDimension(unsigned int x,unsigned int y) { assert(x==1920 && y==1080);++screenCalls; }
CSystemManagerNative* FABLE_FASTCALL GFGetSystemManager() { ++systemCalls;return &systemManager; }
C2DExtentsI* CDisplayManagerNative::GetRenderTargetDimensions(C2DExtentsI& d) const {
 assert(this==&displayManager);++dimensionsCalls;d.X=1920;d.Y=1080;
 if(disableDuringDimensions) active->initialised_=false;
 return &d;
}
static void resetCounts() {resets=initializes=appearanceCalls=systemCalls=dimensionsCalls=screenCalls=compositionCalls=enableCalls=0;}
int main() {
 systemManager.PDisplayManager=&displayManager;
 NUISystem::NativeCIME ime;active=&ime;
 assert((void*)&ime==(void*)static_cast<NUISystem::CIME*>(&ime));
 HWND__* window=reinterpret_cast<HWND__*>(&ime);
 for(int test=0;test<4;++test) {
  resetCounts();ime.enableIme_=test!=0;ime.initialised_=false;ime.window_=0;
  success=test!=1;disableDuringDimensions=test==3;
  ime.Initialise(window);
  assert(resets==1 && initializes==1 && observedWindow==window && observedDisable==(test==0));
  if(test==0) assert(ime.window_==0 && !ime.initialised_ && !appearanceCalls && !enableCalls);
  if(test==1) assert(ime.window_==window && !ime.initialised_ && !appearanceCalls && !enableCalls);
  if(test>=2) {
   assert(ime.window_==window && appearanceCalls==1 && systemCalls==1 && dimensionsCalls==1);
   assert(observedAppearance==(IMEUI_APPEARANCE*)ime.appearance_);
   assert(g_ImeDrawRect==Callback_DrawRect && g_ImeMalloc==Callback_malloc && g_ImeFree==Callback_free);
   assert(g_ImeDrawFans==Callback_DrawFans && g_ImeOnChar==Callback_OnChar);
   assert(screenCalls==(test==2) && compositionCalls==(test==2) && enableCalls==(test==2));
   assert(ime.initialised_==(test==2));
   CImeUiFont_Base* baseFont=observedAppearance->symbolFont;
   NUISystem::CIMEFont* concreteFont=static_cast<NUISystem::CIMEFont*>(baseFont);
   assert((void*)baseFont==(void*)concreteFont);
   baseFont->SetHeight(17);baseFont->SetColor(0x123456);baseFont->SetPosition(10,20);
   assert(concreteFont->Height==17 && concreteFont->Colour==0x123456 && concreteFont->Position[0]==10 && concreteFont->Position[1]==20);
   delete concreteFont;
  }
 }
 puts("00405650_TEST PASS");return 0;
}

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CImeUiFont_Base)==4);
typedef void (CImeUiFont_Base::*ImeHeightMember)(unsigned int);
FABLE_STATIC_ASSERT(sizeof(ImeHeightMember)==4);
#endif