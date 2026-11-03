#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_NUISYSTEM_CTEXT_H
#define FABLE_ENGINE_NUISYSTEM_CTEXT_H
// Retail opt-in (Codex pass 12): original PDB class, complete fields; native CChangingStateComponent base0x15c.
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CWideString.h"
#include "engine/C2DBoxF.h"
class CFontBank;class CTexture;
namespace NUISystem {
enum ETextAlignement {LEFT=0,CENTER=1,RIGHT=2};
class CText: public CChangingStateComponent {
public:
CText(long);
CText(const NUISystem::CText &);
virtual ~CText();
virtual void Draw(CIEngine &,CEnginePrimitiveHandle *,long,long *,NUISystem::CComponent *);
virtual const CWideString & GetValue();
virtual void SetValue(const CCharString &,bool,bool);
virtual void SetValue(const CWideString &,bool,bool);
virtual void SetValue(unsigned long,const CWideString &,bool);
void GetDataFromDef();
virtual void Initialise();
virtual float CanScroll(bool,C2DBoxF &);
virtual CCountedPointer<CFontBank> & GetFont();
virtual C2DVector GetSize() const;
virtual C2DVector GetDynamicSize() const;
virtual void MakeUnavailable();
virtual void MakeUnavailableDynamically();
virtual void MakeAvailable();
virtual void MakeAvailableDynamically();
virtual void SetTextWindow(C2DBoxF &);
virtual C2DBoxF & GetTextWindow();
virtual void ForceWidth(float);
virtual void Update(float);
virtual NUISystem::ETextAlignement GetAlignment() const;
static unsigned int __fastcall GetCharWidthW(wchar_t);
void ResetToDefaultValueWithPrefix(const CWideString &);
protected:
long FitInWindow(CWideString &,const C2DBoxF &);
CWideString Value;
CCountedPointer<CFontBank> PFont;
CEnginePrimitiveHandle Handle;
CEnginePrimitiveHandle HandleDropShadowBottomRight;
C2DBoxF Window;
bool ScaleText;
bool Unavailable;
unsigned char NoRenderNextFrame;
};
FABLE_STATIC_ASSERT(sizeof(CText)==0x18c);
}
#endif

#else

#endif
