#ifndef FABLE_ENGINE_CENGINELANDSCAPEMESHBUILDER_H
#define FABLE_ENGINE_CENGINELANDSCAPEMESHBUILDER_H
// Hand-written (chunk 723): CEngineLandscapeMeshBuilder : CBaseClassNonCopyable : CBaseClass (RTTI). Retail size 0x488 (vptr +
// CIndexBuffer* SharedIndexBuffers[17][17]; the Ego_r.pdb tail past +0x488 is absent in TLC: CEngineLandscapeRenderer::LODZBandsZ
// follows at +0x498 = MeshBuilder +0x10 + 0x488). Out-of-line virtual destructor 0x00BE2B20 (Ego_r.pdb ??1CEngineLandscapeMeshBuilder@@UAE@XZ).
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
class CIndexBuffer;
#ifdef FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_MAPPINGBLEND
class C3DVector;
enum LANDSCAPE_TEXTURE_MAPPING_DIRECTION { };                         // W4LANDSCAPE_TEXTURE_MAPPING_DIRECTION@@ (global; enumerators not needed here)
#endif
class CEngineLandscapeMeshBuilder : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_CTOR
    // Retail opt-in (wfE7, row 0x00B69000): Ego_r.pdb / FableWin.pdb ??0CEngineLandscapeMeshBuilder@@QAE@XZ (retail 0x00BE32A0, called by
    // the CEngineLandscapeRenderer ctor). Declaration only.
    CEngineLandscapeMeshBuilder();                                       // 0x00BE32A0
#endif
    virtual ~CEngineLandscapeMeshBuilder();                              // 0x00BE2B20
#ifdef FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_MAPPINGBLEND
    // Retail opt-in (chunk 1023, row 0x00B67270 CEngineLandscapeRenderer::BuildBlendingTables): Ego_r.pdb
    // ?GetMappingDirectionBlend@CEngineLandscapeMeshBuilder@@QAEMW4LANDSCAPE_TEXTURE_MAPPING_DIRECTION@@ABVC3DVector@@@Z, out of line in
    // retail at 0x00BE2B70 (`push normal ; push direction ; mov ecx,[renderer+10h] lea` at 0x00B673B0, float in st0). Declaration only.
    float GetMappingDirectionBlend(LANDSCAPE_TEXTURE_MAPPING_DIRECTION direction, const C3DVector& normal);   // 0x00BE2B70
#endif
#ifdef FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_MAPPINGDIRNORMALS
    // Retail opt-in (hand2b, row 0x00BE2B70 GetMappingDirectionBlend): Ego_r.pdb / FableWin.pdb
    // ?MappingDirNormals@CEngineLandscapeMeshBuilder@@0PAVC3DVector@@A (private static array, dynamic initializer in FableWin);
    // retail 0x0139C338, indexed `lea eax,[esi+esi*2] ; mov ecx,[eax*4+139C338h]` at 0x00BE2B78. Declaration only (bound size unknown).
private:
    static C3DVector MappingDirNormals[];                                // 0x0139C338
public:
#endif
#ifdef FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_GENERATIONTABLES
    // Retail opt-in (egod2a, row 0x00BE2CD0): Ego_r.pdb ?BuildSharedIndexBufferGenerationTables@CEngineLandscapeMeshBuilder@@QAEXPAVC2DCoordI@@PAG1JJAAJ@Z
    // (public; parameter names from Ego_d.pdb). Called by BuildSharedIndexBuffers 0x00BE3110. Declaration only.
    void BuildSharedIndexBufferGenerationTables(class C2DCoordI* verts, unsigned short* tri_striped_indicies, unsigned short* tri_list_indicies,
                                                long width, long height, long& tri_striped_index_count);   // 0x00BE2CD0
#endif
    CIndexBuffer* SharedIndexBuffers[17][17];                            // +0x04
};
FABLE_STATIC_ASSERT_AT(24, sizeof(CEngineLandscapeMeshBuilder) == 0x488);

#endif // FABLE_ENGINE_CENGINELANDSCAPEMESHBUILDER_H
