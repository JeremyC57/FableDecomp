#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_NUISYSTEM_CSPRITE_H
#define FABLE_ENGINE_NUISYSTEM_CSPRITE_H
// Retail opt-in (Codex pass 12): original PDB class, complete fields; native CChangingStateComponent base0x15c.
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CWideString.h"
#include "engine/C2DBoxF.h"
class CFontBank;class CTexture;
enum EEngineSprite2DFlag {ENGINE_2D_SPRITE_CENTRE_ON_POS=1,ENGINE_2D_SPRITE_ENABLE_FILTERING=2};
namespace NUISystem {
class CSprite: public CChangingStateComponent {
public:
CSprite(long);
CSprite(const NUISystem::CSprite &);
virtual ~CSprite();
virtual void Draw(CIEngine &,CEnginePrimitiveHandle *,long,long *,NUISystem::CComponent *);
virtual C2DVector GetSize() const;
virtual C2DVector GetDynamicSize() const;
void GetDataFromDef();
virtual void Initialise();
virtual void SetAngle(float);
virtual void SetWidth(unsigned long);
virtual void SetHeight(unsigned long);
virtual void SetGraphicIndex(unsigned long);
virtual unsigned long GetGraphicIndex();
virtual unsigned long GetWidth();
virtual unsigned long GetHeight();
unsigned long GetDefaultGraphicIndex();
void DrawWithRenderManager();
protected:
CEnginePrimitiveHandle Handle;
float Angle;
unsigned long Width;
unsigned long Height;
unsigned char NoRenderNextFrame;
EEngineSprite2DFlag Sprite2DFlag;
private:
unsigned long GraphicIndex;
public:
CCountedPointer<CTexture> PTexture;
};
FABLE_STATIC_ASSERT(sizeof(CSprite)==0x184);
}
#endif

#else

#endif
