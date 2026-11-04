#ifndef FABLE_ENGINE_NUISYSTEM_CMORPHINGSPRITE_H
#define FABLE_ENGINE_NUISYSTEM_CMORPHINGSPRITE_H
// NUISystem::CMorphingSprite (new, chunk 752): RTTI .?AVCMorphingSprite@NUISystem@@, single direct base NUISystem::CSprite
// (rtti_bases.tsv). Vtables 0x0124A49C (+0), 0x0124A474 (+4, CObserver: slot 0 the ??_E adjustor 0x00555120), 0x0124A46C
// (+0x18, CActionParam: slot 0 the adjustor 0x00555130). Overrides (primary vtable): 1 Update 0x00554150, 2 Draw 0x00554740,
// 43 Initialise 0x005540F0, 58 ProcessChangeState (MAE) 0x005546F0, 138 InternalChanged 0x00554110, 145 SetGraphicIndex
// 0x00555100 (stores +0x198); new virtuals after CSprite's 148: 149 GetGRaphicIndices 0x00555110 (`lea eax,[ecx+18Ch]`),
// 150 UpdateMorph(float) (MAE) 0x00554140 (`ret 4`), 151 ChangeGraphic(unsigned long, float) (MAE) 0x00554130 (`ret 8`).
// Name set: Ego_r.pdb ??0CMorphingSprite@NUISystem@@QAE@J@Z, ??1..UAE@XZ, ?ChangeGraphic@..MAEXKM@Z, ?Draw@..UAEXAAVCIEngine@@
// PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z, GetDataFromDef, ?GetGRaphicIndices@..UAEAAV?$map@KK..., Initialise,
// InternalChanged, ProcessChangeState (MAE), SetGraphicIndex, Update, ?UpdateMorph@..MAEXM@Z.
// Layout: Ego_r.pdb CMorphingSprite (0x1D8) minus 0x30 (retail CSprite 0x184) and the STLport list (4, PDB 0xC): RenderCount
// +0x184, Handles +0x188, GraphicIndices +0x18C, SpriteGraphicIndex +0x198 (the landed ctor 0x005545D0 and SetGraphicIndex).
#include "engine/NUISystem_CSprite.h"
#include "stlport/fable_map.h"
namespace NUISystem {
class CMorphingSprite : public CSprite {
public:
    CMorphingSprite(long);                                               // 0x005545D0
    CMorphingSprite(const CMorphingSprite&);
    virtual ~CMorphingSprite();                                          // 0x005541E0
    virtual void Initialise();
    virtual void Update(float);
    virtual void Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*);
    virtual bool InternalChanged();
    virtual void SetGraphicIndex(unsigned long);
    virtual std::map<unsigned long,unsigned long>& GetGRaphicIndices(); // slot 149
    void GetDataFromDef();
protected:
    virtual void ProcessChangeState();
    virtual void UpdateMorph(float);                                     // slot 150
    virtual void ChangeGraphic(unsigned long,float);                     // slot 151
public:
    long RenderCount;                                                    // +0x184
    std::list<CEnginePrimitiveHandle*> Handles;                          // +0x188
    std::map<unsigned long,unsigned long> GraphicIndices;                // +0x18c
    unsigned long SpriteGraphicIndex;                                    // +0x198
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CMorphingSprite) == 0x19c);
#endif
