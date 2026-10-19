// CLocalDetailPrimitiveRepeatedMesh::SetupPrimitiveDesc() -- return repeated-mesh descriptor with fade, lighting flags and scaled wind.
// Native0x00bf9bc0 length254; PDB donorAAB060 returns CAddMeshDesc by value, native hidden output+ret4.
// All scalar and pointer fields verified against PDB; no generated headers exist for owner/descriptor/type.
#include "engine/C3DMeshInfo.h"
struct C4DVector;struct CSubsectionElement;
struct C3DBoundingBox { C3DBoundingBox(); // PDB02cfbc90 default constructor
 float MinX,MinY,MinZ,MaxX,MaxY,MaxZ;
 C3DBoundingBox(const C3DBoundingBox& b):MinX(b.MinX),MinY(b.MinY),MinZ(b.MinZ),MaxX(b.MaxX),MaxY(b.MaxY),MaxZ(b.MaxZ){}
};
struct C3DBoundingSphere { C3DBoundingSphere(); // PDB02d3eb90 default constructor
 float X,Y,Z,Radius;
 C3DBoundingSphere(const C3DBoundingSphere& s):X(s.X),Y(s.Y),Z(s.Z),Radius(s.Radius){}
};
struct CEngineFadeDistance { float FadeStart,FadeEnd; CEngineFadeDistance(float s,float e):FadeStart(s),FadeEnd(e){} };
struct CollectionType { void* vtable;C3DMeshInfo *Mesh,*ZSpriteMesh,*ShadowMesh;long MeshPolyCount,MeshBankIndex,ZSpriteMeshBankIndex,ShadowMeshBankIndex;
 float FadeStart,FadeEnd;long AlphaRef;float AlphaMipBias,WindSkewConstantFactor,WindSkewRandomFactor,WindSkewSpeedFactor,ZSpriteFadeStart,ZSpriteFadeEnd;long PrimitiveType,CacheGroup;
 bool AlphaIsBoolean,LandscapeNormalLighting,HasWindSkew,CastsShadows,ReceivesShadows;
};
struct CAddMeshDesc {
 const C3DMeshInfo* MeshInfo;float MaxScale;const float* VectorNormals;long ObjectCount;const CSubsectionElement* SubSections;
 C3DBoundingBox BoundingBox;C3DBoundingSphere BoundingSphere;CEngineFadeDistance FadeDist;C4DVector* ObjectOffsets;C4DVector* ObjectMatricies;
 unsigned long EnginePrimitiveFlags;long AlphaRef;float AlphaMipBias;bool Persistent;unsigned long EPFFlags;const unsigned char* WindDelayArray;float WindScaleFactor;
 __forceinline CAddMeshDesc(const C3DMeshInfo* mesh,long n,C4DVector* matrices,C4DVector* offsets,const float* normals,const CSubsectionElement* sections,const unsigned char* wind,float scale,const C3DBoundingBox& box,const C3DBoundingSphere& sphere,float windscale,const CEngineFadeDistance& fade,unsigned long flags,long alpha,float bias,bool persistent,unsigned long epf):
 MeshInfo(mesh),MaxScale(scale),VectorNormals(normals),ObjectCount(n),SubSections(sections),BoundingBox(box),BoundingSphere(sphere),FadeDist(fade),ObjectOffsets(offsets),ObjectMatricies(matrices),EnginePrimitiveFlags(flags),AlphaRef(alpha),AlphaMipBias(bias),Persistent(persistent),EPFFlags(epf),WindDelayArray(wind),WindScaleFactor(scale*windscale){}
};
struct RepeatedMeshView {
 void* vtable;void* Next;C3DBoundingBox BoundingBox;C3DBoundingSphere BoundingSphere;CollectionType* Type;void* Map;
 float MaxObjectScale;long ObjectCount,SubSectionsCount;CSubsectionElement* SubSections;C4DVector *ObjectMatricies,*ObjectOffsets;unsigned char* WindDelayArray;float* LandscapeNormalArray;
 CAddMeshDesc SetupPrimitiveDesc();
};
CAddMeshDesc RepeatedMeshView::SetupPrimitiveDesc() {
 CEngineFadeDistance fade(Type->FadeStart,Type->FadeEnd);
 unsigned long flags=0x100;if(Type->LandscapeNormalLighting) flags=0x8100;
 return CAddMeshDesc(Type->Mesh,ObjectCount,ObjectMatricies,ObjectOffsets,LandscapeNormalArray,SubSections,WindDelayArray,MaxObjectScale,BoundingBox,BoundingSphere,Type->WindSkewRandomFactor,fade,flags,Type->AlphaRef,Type->AlphaMipBias,true,1);
}