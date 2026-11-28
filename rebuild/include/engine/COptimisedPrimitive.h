#ifndef FABLE_ENGINE_COPTIMISEDPRIMITIVE_H
#define FABLE_ENGINE_COPTIMISEDPRIMITIVE_H
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
#include "engine/C3DVector.h"
class CChunkedFileChunk;
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_CTOR
class C3DMeshFilePrimitiveChunk;
#endif
namespace QuadricOptimiserInternals {
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_OPTIMALPLACEMENT
class CQuadric;
#endif
class CUniqueVertex;class COptimisedVertex;class COptimisedFace;class COptimisedEdge;class CVertexPairContraction;
class COptimisationParameters {public:float Scaling,EdgeWeight,EdgeCost,GroupWeight,TextureWeight,TexturePenalty,UnsplitTextureWeight,UnsplitNormalWeight,HighPenalty;unsigned long MaximumBonesPerVertex;};
class COptimisedPrimitive {
private:
 long MaterialIndex;
 CArray<CUniqueVertex> UniqueVertices;
 CArray<COptimisedVertex> Vertices;
 CArray<COptimisedFace> Faces;
 CArray<COptimisedEdge> Edges;
 std::multimap<float,CCountedPointer<CVertexPairContraction> > VertexPairContractions;
 long SegmentCount;CArray<float> SegmentSizes;CArray<CCountedPointer<CChunkedFileChunk> > SubChunksToKeep;
 COptimisationParameters Parameters;
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_OPTIMALPLACEMENT
 // Retail opt-in (Codex pass 24): the original private member, retail 0x00AC7750.
 C3DVector ComputeOptimalVertexPlacement(const CQuadric&,const C3DVector&,const C3DVector&);
#endif
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_SUBCOST
 // Retail opt-in (Codex pass 17): exact original private nonconst method.
 float ComputeSubContractionCost(const CSubContraction&,const C3DVector&);
#endif
 C3DVector MakeOrthogonal(const C3DVector& vect,const C3DVector& normal)const;
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_JOINADJACENTFACES
 // Retail opt-in (row 0x00AC8AE0): Ego_r.pdb 0x00C61CA0 / FableWin.pdb ?JoinAdjacentFaces@COptimisedPrimitive@QuadricOptimiserInternals@@AAE_NKK@Z
 // (private, non-const). Declaration only; no layout change.
 bool JoinAdjacentFaces(unsigned long face_a,unsigned long face_b);                                 // 0x00AC8AE0
#endif
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_METHODS
 // Retail opt-in (chunk Q15, rows 0x00AC7CB0 / 0x00AC7300 / 0x00ACD960): Ego_r.pdb name set of the class (access and const from the
 // decorations: ABE = private const, AAE = private, QAE = public). Declarations only; no layout change.
 C3DVector GetFaceNormal(const COptimisedFace& face)const;                                          // 0x00AC7CB0
 C3DVector FindTextureTangent(const C3DVector& p0,const C3DVector& p1,const C3DVector& p2,float t0,float t1,float t2); // 0x00AC7300
 void FindFaceTextureTangents(COptimisedFace& face);                                                // 0x00AC7E10
 void PerformContraction(CVertexPairContraction& contraction);                                      // 0x00ACD840
 bool DetectFoldOver(const CVertexPairContraction& contraction);                                    // 0x00ACD900
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_DETECTFOLDOVER_SUB
 // Retail opt-in (egod e02, row 0x00ACD960): Ego_r.pdb 0x00C65BB0 ?DetectFoldOver@COptimisedPrimitive@QuadricOptimiserInternals@@AAE_NABVCSubContraction@2@ABVC3DVector@@@Z
 // (private overload; retail 0x00ACD0E0, called per sub-contraction by DetectFoldOver(const CVertexPairContraction&) 0x00ACD92A). Declaration only.
 bool DetectFoldOver(const CSubContraction& sub_contraction,const C3DVector& position);              // 0x00ACD0E0
#endif
 void PopFrontContraction();
 void UpdateCost(const CCountedPointer<CVertexPairContraction>& contraction);
public:
 void Optimise(float max_cost);                                                                     // 0x00ACD960
private:
#endif
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_CTOR
 // Retail opt-in (row 0x00ACDB80): Ego_r.pdb ??0COptimisedPrimitive@QuadricOptimiserInternals@@QAE@AAVC3DMeshFilePrimitiveChunk@@@Z
 // (public) and FableWin.pdb inline ?GetVertex@COptimisedPrimitive@QuadricOptimiserInternals@@QAEAAVCOptimisedVertex@2@K@Z (fwsize
 // SAME 12i/28b). Needs the complete COptimisedVertex (engine/QuadricOptimiserTypes.h first). No layout change.
public:
#ifdef FABLE_RETAIL_QUADRIC_MESH_CTOR_PASS30
 ~COptimisedPrimitive();
#endif
 COptimisedPrimitive(C3DMeshFilePrimitiveChunk& chunk);                                             // 0x00ACDB80
#ifdef FABLE_RETAIL_QUADRIC_COPY_TO_PASS30
 void CopyTo(C3DMeshFilePrimitiveChunk& destination); // 00ACC490.
#endif
 COptimisedVertex& GetVertex(unsigned long index){return Vertices[index];}
private:
#endif
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_INITIALISE
 // Retail opt-in (row 0x00ACA830): Ego_r.pdb ?Initialise@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXABVCOptimisationParameters@2@@Z
 // (public) and the private members it calls (Ego_r / FableWin names, AAE = private): FindAdjacentFaces 0x00AC9A60, PaintSegments
 // 0x00AC8540, MeasureSegments 0x00AC9D50, ComputeVertexCost 0x00AC8C80, ComputeContraction 0x00AC92C0, ComputeSelfContraction
 // 0x00AC7EB0; FableWin.pdb inline ?UniqueToActualIndex@COptimisedPrimitive@QuadricOptimiserInternals@@ABEJKABVCOptimisedFace@2@@Z
 // (expanded four times in 0x00ACA830: a k < 3 loop on Vertices[face.VertexIndices[k]].UniqueIndex, else -1). Needs the complete
 // COptimisedVertex / COptimisedFace (engine/QuadricOptimiserTypes.h first). No layout change.
public:
 void Initialise(const COptimisationParameters& parameters);                                        // 0x00ACA830
private:
 void FindAdjacentFaces();                                                                          // 0x00AC9A60
 void PaintSegments();                                                                              // 0x00AC8540
 void MeasureSegments();                                                                            // 0x00AC9D50
 void ComputeVertexCost(COptimisedVertex& vertex);                                                  // 0x00AC8C80
 void ComputeContraction(CVertexPairContraction& contraction);                                      // 0x00AC92C0
 void ComputeSelfContraction(CVertexPairContraction& contraction);                                  // 0x00AC7EB0
 long UniqueToActualIndex(unsigned long unique_index,const COptimisedFace& face)const{
  for(unsigned long k=0;k<3;k++){
   unsigned long index=face.VertexIndices[k];
   if(Vertices[index].UniqueIndex==unique_index)return index;
  }
  return -1;
 }
#endif
};
}
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof COptimisedPrimitive = 0xa0.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof COptimisedPrimitive = 0x84.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
struct COptimisedPrimitive {
    long          MaterialIndex;                             // +0x00
    unsigned char UniqueVertices[0xc];                       // +0x04 CArray<QuadricOptimiserInternals::CUniqueVertex>
    unsigned char Vertices[0xc];                             // +0x10 CArray<QuadricOptimiserInternals::COptimisedVertex>
    unsigned char Faces[0xc];                                // +0x1c CArray<QuadricOptimiserInternals::COptimisedFace>
    unsigned char Edges[0xc];                                // +0x28 CArray<QuadricOptimiserInternals::COptimisedEdge>
    unsigned char VertexPairContractions[0xc];               // +0x34 multimap<float,CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction>,std::less<float>,std::allocator<std::pair<float_const_,CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction>_>_>_>
    long          SegmentCount;                              // +0x40
    unsigned char SegmentSizes[0xc];                         // +0x44 CArray<float>
    unsigned char SubChunksToKeep[0xc];                      // +0x50 CArray<CCountedPointer<CChunkedFileChunk>_>
    float         Parameters_Scaling;                        // +0x5c COptimisationParameters::Scaling
    float         Parameters_EdgeWeight;                     // +0x60 COptimisationParameters::EdgeWeight
    float         Parameters_EdgeCost;                       // +0x64 COptimisationParameters::EdgeCost
    float         Parameters_GroupWeight;                    // +0x68 COptimisationParameters::GroupWeight
    float         Parameters_TextureWeight;                  // +0x6c COptimisationParameters::TextureWeight
    float         Parameters_TexturePenalty;                 // +0x70 COptimisationParameters::TexturePenalty
    float         Parameters_UnsplitTextureWeight;           // +0x74 COptimisationParameters::UnsplitTextureWeight
    float         Parameters_UnsplitNormalWeight;            // +0x78 COptimisationParameters::UnsplitNormalWeight
    float         Parameters_HighPenalty;                    // +0x7c COptimisationParameters::HighPenalty
    unsigned long Parameters_MaximumBonesPerVertex;          // +0x80 COptimisationParameters::MaximumBonesPerVertex
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(82, sizeof(COptimisedPrimitive) == 0x84);
FABLE_STATIC_ASSERT_AT(83, offsetof(COptimisedPrimitive, MaterialIndex) == 0x0);
FABLE_STATIC_ASSERT_AT(84, offsetof(COptimisedPrimitive, UniqueVertices) == 0x4);
FABLE_STATIC_ASSERT_AT(85, offsetof(COptimisedPrimitive, Vertices) == 0x10);
FABLE_STATIC_ASSERT_AT(86, offsetof(COptimisedPrimitive, Faces) == 0x1c);
FABLE_STATIC_ASSERT_AT(87, offsetof(COptimisedPrimitive, Edges) == 0x28);
FABLE_STATIC_ASSERT_AT(88, offsetof(COptimisedPrimitive, VertexPairContractions) == 0x34);
FABLE_STATIC_ASSERT_AT(89, offsetof(COptimisedPrimitive, SegmentCount) == 0x40);
FABLE_STATIC_ASSERT_AT(90, offsetof(COptimisedPrimitive, SegmentSizes) == 0x44);
FABLE_STATIC_ASSERT_AT(91, offsetof(COptimisedPrimitive, SubChunksToKeep) == 0x50);
FABLE_STATIC_ASSERT_AT(92, offsetof(COptimisedPrimitive, Parameters_Scaling) == 0x5c);
FABLE_STATIC_ASSERT_AT(93, offsetof(COptimisedPrimitive, Parameters_EdgeWeight) == 0x60);
FABLE_STATIC_ASSERT_AT(94, offsetof(COptimisedPrimitive, Parameters_EdgeCost) == 0x64);
FABLE_STATIC_ASSERT_AT(95, offsetof(COptimisedPrimitive, Parameters_GroupWeight) == 0x68);
FABLE_STATIC_ASSERT_AT(96, offsetof(COptimisedPrimitive, Parameters_TextureWeight) == 0x6c);
FABLE_STATIC_ASSERT_AT(97, offsetof(COptimisedPrimitive, Parameters_TexturePenalty) == 0x70);
FABLE_STATIC_ASSERT_AT(98, offsetof(COptimisedPrimitive, Parameters_UnsplitTextureWeight) == 0x74);
FABLE_STATIC_ASSERT_AT(99, offsetof(COptimisedPrimitive, Parameters_UnsplitNormalWeight) == 0x78);
FABLE_STATIC_ASSERT_AT(100, offsetof(COptimisedPrimitive, Parameters_HighPenalty) == 0x7c);
FABLE_STATIC_ASSERT_AT(101, offsetof(COptimisedPrimitive, Parameters_MaximumBonesPerVertex) == 0x80);

#endif
#endif // FABLE_ENGINE_COPTIMISEDPRIMITIVE_H
