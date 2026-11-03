#ifndef FABLE_ENGINE_NUISYSTEM_CSPRITE_H
#define FABLE_ENGINE_NUISYSTEM_CSPRITE_H
// Retail model (chunk 729, hand-written): NUISystem::CSprite : CChangingStateComponent (RTTI; class key V, pdb_class_keys.tsv),
// primary vtable 0x0122F5D4 (rtti-vtables.tsv). Ego_r.pdb / FableWin.pdb members: ctors (long) 0x0041B800 / copy, the virtual
// dtor, overrides Initialise / Draw / GetSize / GetDynamicSize and seven new virtuals in slots 142..148, checked against the
// retail bodies: 142 SetAngle(float) 0x0041C680 (stores +0x164), 143 SetWidth 0x0041C690 (+0x168), 144 SetHeight 0x0041C6A0
// (+0x16C), 145 SetGraphicIndex 0x0041B550 (stores +0x178, then reloads the graphic), 146 GetGraphicIndex 0x0041C6B0 (+0x178),
// 147 GetWidth 0x0041C6C0, 148 GetHeight 0x0041C6D0. Members: Ego_r.pdb CSprite (Handle +0x18C .. PTexture +0x1AC) less the
// 0x30 retail NUISystem shift (GOTCHAS 2026-10-29): Handle +0x15C, Angle +0x164, Width +0x168, Height +0x16C,
// NoRenderNextFrame +0x170, Sprite2DFlag +0x174, GraphicIndex +0x178, PTexture +0x17C; size 0x184.
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/CEnginePrimitiveHandle.h"
class CTexture;
namespace NUISystem {
class CSprite : public CChangingStateComponent {
public:
    CSprite(long);                                                   // 0x0041B800 ??0CSprite@NUISystem@@QAE@J@Z
    CSprite(const CSprite&);                                         // ??0CSprite@NUISystem@@QAE@ABV01@@Z
    virtual ~CSprite();
    virtual void Initialise();
    virtual void Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*);
    virtual C2DVector GetSize() const;
    virtual C2DVector GetDynamicSize() const;
    virtual void SetAngle(float);                                    // slot 142 0x0041C680
    virtual void SetWidth(unsigned long);                            // slot 143 0x0041C690
    virtual void SetHeight(unsigned long);                           // slot 144 0x0041C6A0
    virtual void SetGraphicIndex(unsigned long);                     // slot 145 0x0041B550
    virtual unsigned long GetGraphicIndex();                         // slot 146 0x0041C6B0
    virtual unsigned long GetWidth();                                // slot 147 0x0041C6C0
    virtual unsigned long GetHeight();                               // slot 148 0x0041C6D0
    void GetDataFromDef();
    unsigned long GetDefaultGraphicIndex();
    void DrawWithRenderManager();
protected:
    CEnginePrimitiveHandle Handle;                                   // +0x15c
    float Angle;                                                     // +0x164
    unsigned long Width;                                             // +0x168
    unsigned long Height;                                            // +0x16c
    unsigned char NoRenderNextFrame;                                 // +0x170
    long Sprite2DFlag;                                               // +0x174 EEngineSprite2DFlag
    unsigned long GraphicIndex;                                      // +0x178
    CCountedPointer<CTexture> PTexture;                              // +0x17c
};
FABLE_STATIC_ASSERT(sizeof(CSprite)==0x184);
}
#endif
