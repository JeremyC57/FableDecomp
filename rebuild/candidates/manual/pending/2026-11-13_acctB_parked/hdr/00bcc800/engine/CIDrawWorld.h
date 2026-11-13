#ifndef FABLE_ENGINE_CIDRAWWORLD_H
#define FABLE_ENGINE_CIDRAWWORLD_H
// New header (chunk 2031, row 0x00B65A20 CEngineSkyRenderer::RenderOuterSky): the engine's draw-side view of the game world, RTTI
// .?AVCIDrawWorld@@ (no bases; CWorld's copy at +4, vtable 0x012390DC). Slot order from the retail vtable 0x012390DC and the landed
// overrides: slot 0 DrawGetProceduralTexture 0x004AE890, slot 1 DrawGetEnvironment 0x0049E350 (`mov eax,[ecx+14h]` = CWorld +0x18
// PEnvironment), slot 2 DrawGetWorldMap 0x0049E1A0 (`[ecx+10h]` = CWorld +0x14 PWorldMap), slot 3 the scalar deleting destructor 0x004AE8A0;
// names and signatures from Ego_r.pdb / FableWin.pdb (?DrawGetEnvironment@CWorld@@UBEPBVCIDrawEnvironment@@XZ, ?DrawGetWorldMap@CWorld@@
// UBEPBVCIDrawWorldMap@@XZ, ?DrawGetProceduralTexture@CWorld@@UBEPBVCProceduralTexture@@XZ). Header debt: engine/CWorld.h's own CIDrawWorld
// (FABLE_RETAIL_CIDRAWWORLD_DEFINED) lists slot 1 / slot 2 the other way round; this header defines the guard so a TU that includes both
// keeps the retail order.
#ifndef FABLE_RETAIL_CIDRAWWORLD_DEFINED
#define FABLE_RETAIL_CIDRAWWORLD_DEFINED
class CProceduralTexture;
class CIDrawEnvironment;
class CIDrawWorldMap;
class CIDrawWorld {
public:
    virtual const CProceduralTexture* DrawGetProceduralTexture() const = 0;   // slot 0
    virtual const CIDrawEnvironment* DrawGetEnvironment() const = 0;           // slot 1
    virtual const CIDrawWorldMap* DrawGetWorldMap() const = 0;                 // slot 2
    virtual ~CIDrawWorld();                                                    // slot 3
};
#endif
#endif // FABLE_ENGINE_CIDRAWWORLD_H
