#ifndef FABLE_ENGINE_C3DBOUNDINGSPHERE_H
#define FABLE_ENGINE_C3DBOUNDINGSPHERE_H
// C3DBoundingSphere (Ego_r TPI, struct_layouts_egor.tsv): public Center +0 (C3DVector), Radius +0x0C, size 0x10.
// Default ctor FableWin ??0C3DBoundingSphere@@QAE@XZ (0x02D3EB90; inline in retail: 0x00AC2BE0 zero-fills the 16 bytes of each
// new C3DMeshPartitionBuilderElement's sphere). AccurateBuildFromPoints(const vector<C3DVector>&, float): Ego_r 0x00BFAAE0,
// out of line in retail. GetRadius: FableWin ?GetRadius@C3DBoundingSphere@@QBEMXZ (inline in retail).
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
class C3DBoundingSphere {
public:
    C3DBoundingSphere() : Center(0.0f,0.0f,0.0f), Radius(0.0f) { }
#ifdef FABLE_RETAIL_C3DBOUNDINGSPHERE
    // Retail opt-in (chunk 723c): Ego_r.pdb / FableWin.pdb ??0C3DBoundingSphere@@QAE@ABVC3DVector@@M@Z, inline in retail
    // (RenderSingleSprite 0x00BC04F0 builds the sprite's light-query sphere from WorldPos and BSphereRadius in its frame).
    C3DBoundingSphere(const C3DVector& center, float radius) : Center(center), Radius(radius) { }
#endif
    void AccurateBuildFromPoints(const std::vector<C3DVector>&,float);
    float GetRadius() const { return Radius; }
    C3DVector Center;
    float Radius;
};
#endif
