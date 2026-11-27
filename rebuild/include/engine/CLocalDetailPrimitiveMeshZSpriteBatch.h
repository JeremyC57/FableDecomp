#ifdef FABLE_RETAIL_LOCAL_DETAIL_RENDER_PASS29
#ifndef FABLE_ENGINE_CLOCALDETAILPRIMITIVEMESHZSPRITEBATCH_H
#define FABLE_ENGINE_CLOCALDETAILPRIMITIVEMESHZSPRITEBATCH_H
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CMovableResource.h"
#include "engine/C3DBoundingBox.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CMatrix3x4.h"
class C3DMeshInfo;
class CLocalDetailCacheMap;
class CEngineCamera;
class CDataInputStream;
namespace NEnginePrimitiveManagerRepeatedZSprites { struct CObjectEntry; }
enum LOCAL_DETAIL_PRIMITIVE_TYPE;
class CLocalDetailObjectCollectionType : public CBaseClassNonCopyable {
public:
    const C3DMeshInfo* PeekMesh() const { return Mesh; }
    const C3DMeshInfo* PeekShadowMesh() const { return ShadowMesh; }
    float PeekFadeStartBeforeVideoOptionModifier() const { return PlainDataBlock.FadeStart; }
    float PeekFadeEndBeforeVideoOptionModifier() const { return PlainDataBlock.FadeEnd; }
    long PeekAlphaRef() const { return PlainDataBlock.AlphaRef; }
    float PeekAlphaMipBias() const { return PlainDataBlock.AlphaMipBias; }
    float PeekZSpriteFadeStart() const { return PlainDataBlock.ZSpriteFadeStart; }
    float PeekZSpriteFadeEnd() const { return PlainDataBlock.ZSpriteFadeEnd; }
    bool PeekAlphaIsBoolean() const { return PlainDataBlock.AlphaIsBoolean; }
    float PeekReceivesShadows() const { return PlainDataBlock.ReceivesShadows; }

private:
    const C3DMeshInfo *Mesh, *ZSpriteMesh, *ShadowMesh;
    long MeshPolyCount, MeshBankIndex, ZSpriteMeshBankIndex, ShadowMeshBankIndex;
    struct {
        float FadeStart, FadeEnd;
        long AlphaRef;
        float AlphaMipBias, WindSkewConstantFactor, WindSkewRandomFactor, WindSkewSpeedFactor;
        float ZSpriteFadeStart, ZSpriteFadeEnd;
        LOCAL_DETAIL_PRIMITIVE_TYPE PrimitiveType;
        long CacheGroup;
        bool AlphaIsBoolean, LandscapeNormalLighting, HasWindSkew, CastsShadows, ReceivesShadows;
    } PlainDataBlock;
};
class CLocalDetailPrimitiveBase : public CMovableResource {
public:
    virtual void RenderUpdate(const CEngineCamera&,bool,unsigned long,const CLocalDetailObjectCollectionType*,float);
    virtual void AddObjectsToPrimitiveRenderer() = 0;
    virtual bool NeedsRenderUpdate();
    virtual void OnPostRelocation();
    virtual bool Load(CDataInputStream&) = 0;
    virtual LOCAL_DETAIL_PRIMITIVE_TYPE GetType() const = 0;
    CLocalDetailPrimitiveBase* Next;
    bool ClipPrimitive(const CLocalDetailObjectCollectionType*,const CEngineCamera&,bool,unsigned long);
protected:
    C3DBoundingBox BoundingBox;
    C3DBoundingSphere BoundingSphere;
    const CLocalDetailObjectCollectionType* Type;
    CLocalDetailCacheMap* Map;
};
class CLocalDetailPrimitiveMeshZSpriteBatch : public CLocalDetailPrimitiveBase {
public:
    virtual void RenderUpdate(const CEngineCamera&,bool,unsigned long,const CLocalDetailObjectCollectionType*,float);
private:
    class CObject {
    public:
        CMatrix3x4 Matrix;
        float Scale;
        C3DBoundingSphere BoundingSphere;
        CEnginePrimitiveHandle PrimitiveHandle;
    };
    CObject* Objects;
    NEnginePrimitiveManagerRepeatedZSprites::CObjectEntry* ZSpriteEntries;
    long ObjectCount;
    bool HasMeshPrimitiveHandles;
    CEnginePrimitiveHandle ZSpriteHandle;
};
FABLE_STATIC_ASSERT(sizeof(CLocalDetailPrimitiveBase)==0x38);
FABLE_STATIC_ASSERT(sizeof(CLocalDetailPrimitiveMeshZSpriteBatch)==0x50);
#endif
#endif
