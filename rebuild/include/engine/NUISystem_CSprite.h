#ifndef FABLE_ENGINE_NUISYSTEM_CSPRITE_H
#define FABLE_ENGINE_NUISYSTEM_CSPRITE_H
// NUISystem::CSprite (new, chunk 753): RTTI .?AVCSprite@NUISystem@@, single direct base CChangingStateComponent
// (rtti_bases: CUIInterface@0, CObserver@4, CActionParam@0x18). Vtables 0x0122F5D4 (+0, 149 slots), 0x0122F5AC (+4),
// 0x0122F5A4 (+0x18). Overrides read from the primary vtable: 0 ??_G 0x0041C760, 2 Draw 0x0041AFA0, 23 GetSize 0x0041A850,
// 43 Initialise 0x0041A8D0, 111 GetDynamicSize 0x0041C660. New virtuals (bodies read from retail), in the order the FableWin.pdb
// donor emits the inline ones (SetAngle, SetWidth, SetHeight, GetGraphicIndex, GetWidth, GetHeight; SetGraphicIndex is out of line):
//   142 SetAngle 0x0041C680 (+0x164), 143 SetWidth 0x0041C690 (+0x168), 144 SetHeight 0x0041C6A0 (+0x16C),
//   145 SetGraphicIndex 0x0041B550 (stores +0x178, then reloads the texture), 146 GetGraphicIndex 0x0041C6B0 (+0x178),
//   147 GetWidth 0x0041C6C0 and 148 GetHeight 0x0041C6D0: the component's Size (+0xCC / +0xD0) converted through __ftol2.
// Name set: Ego_r.pdb / FableWin.pdb (??0..QAE@J@Z, ??0..QAE@ABV01@@Z, ??1..UAE@XZ, Draw, DrawWithRenderManager,
// GetDataFromDef, GetDefaultGraphicIndex, GetDynamicSize, GetGraphicIndex UAEKXZ, GetHeight UAEKXZ, GetSize, GetWidth UAEKXZ,
// Initialise, SetAngle UAEXM@Z, SetGraphicIndex UAEXK@Z, SetHeight UAEXK@Z, SetWidth UAEXK@Z).
// Layout: Ego_r.pdb CSprite (sizeof 0x1B4) minus 0x30 (the retail CChangingStateComponent is 0x15C): Handle +0x15C,
// Angle +0x164, Width +0x168, Height +0x16C, NoRenderNextFrame +0x170, Sprite2DFlag +0x174, GraphicIndex +0x178, PTexture
// +0x17C (the landed ~CSprite 0x0041AB40 and the slot bodies above); sizeof 0x184.
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/CEnginePrimitiveHandle.h"
class CTexture;
class CIEngine;
enum EEngineSprite2DFlag;
namespace NUISystem {
class CSprite : public CChangingStateComponent {
public:
    CSprite(long);
    CSprite(const CSprite&);
    virtual ~CSprite();
    virtual void Initialise();
    virtual void Draw(CIEngine&, CEnginePrimitiveHandle*, long, long*, CComponent*);
    virtual C2DVector GetSize() const;
    virtual C2DVector GetDynamicSize() const;
    virtual void SetAngle(float angle) { Angle = angle; }                          // slot 142
    virtual void SetWidth(unsigned long width) { Width = width; }                  // slot 143
    virtual void SetHeight(unsigned long height) { Height = height; }              // slot 144
    virtual void SetGraphicIndex(unsigned long);                                   // slot 145
    virtual unsigned long GetGraphicIndex() { return GraphicIndex; }               // slot 146
    virtual unsigned long GetWidth() { return (unsigned long)Size.X; }             // slot 147
    virtual unsigned long GetHeight() { return (unsigned long)Size.Y; }            // slot 148
    void GetDataFromDef();
    void DrawWithRenderManager();
    unsigned long GetDefaultGraphicIndex();
protected:
    CEnginePrimitiveHandle Handle;                                                 // +0x15c
    float Angle;                                                                   // +0x164
    unsigned long Width;                                                           // +0x168
    unsigned long Height;                                                          // +0x16c
    unsigned char NoRenderNextFrame;                                               // +0x170
    EEngineSprite2DFlag Sprite2DFlag;                                              // +0x174
    unsigned long GraphicIndex;                                                    // +0x178
#ifdef FABLE_RETAIL_CSPRITE_PUBLIC_TEXTURE
    // Retail opt-in (chunk 795, row 0x0055FEC0 CTCHeroOnlineScoreboard::ChooseAClan): that row assigns the clan logo straight into
    // the sprite's texture (inline CCountedPointer::operator= on +0x17C / +0x180, 0x00560350..0x0056039D); no PDB accessor is bound,
    // so the member is reachable here. Access only: no layout change.
public:
#endif
    CCountedPointer<CTexture> PTexture;                                            // +0x17c
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CSprite) == 0x184);
#endif
