// QuadricOptimiserInternals::COptimisedPrimitive::PerformSubContraction(const CSubContraction& sub_contraction, const CVertexPairContraction& contraction) -- merge the second vertex into the first and update faces, ownership and weighted attributes.
// Leaf: ?PerformSubContraction@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXABVCSubContraction@2@ABVCVertexPairContraction@2@@Z
// Grade: DIFFER(1293v1264) -- pair-contraction loop lifetimes: retail iterates the pair map without the CContractionReference copy/destructor Codex's by-value pair adds; binding the pair by reference, advancing with ++itr and the if/else cost max bring 1367 down to 1293, but register roles for the two vertex records (ebx/edi swapped from the first existence test on) and the counted-pointer assignment boundaries still differ; 13 shapes tried. Graded in Codex's header context
// Flags: /Gr
#define FABLE_RETAIL_QUADRIC_SUB_CONTRACTION_RETRY
#define FABLE_RETAIL_CARRAY_GEOMETRY
#define FABLE_RETAIL_VECTOR_GEOMETRY
#define FABLE_RETAIL_COPTIMISEDPRIMITIVE
#include "engine/COptimisedPrimitive.h"
#include "engine/QuadricOptimiserBodies.h"
namespace QuadricOptimiserInternals {
void COptimisedVertex::RemovePairContraction(unsigned long other_vertex){
 TPairContractionMap::iterator found=PairContractions.find(other_vertex);
 PairContractions.erase(found);
}
void COptimisedPrimitive::PerformSubContraction(const CSubContraction& sub_contraction,const CVertexPairContraction& contraction){
 unsigned long index1=sub_contraction.Index1,index2=sub_contraction.Index2;
 if(index1==index2)return;
 COptimisedVertex& vertex1=Vertices[index1];
 COptimisedVertex& vertex2=Vertices[index2];
 if(!vertex1.Exists||!vertex2.Exists||!vertex1.HasPairContraction(index2)||!vertex2.HasPairContraction(index1))return;
 const CIndexList& affected_faces=vertex2.FaceList;
 CIndexList::TConstIterator face_itr;
 for(face_itr=affected_faces.Begin();face_itr!=affected_faces.End();++face_itr){
  unsigned long f=*face_itr;
  COptimisedFace& face=Faces[f];
  if(face.Exists){face.RemapIndex(index2,index1);if(face.IsDegenerate())face.Exists=false;}
 }
 CIndexList facelist=vertex1.FaceList.GetUnion(vertex2.FaceList);
 vertex1.FaceList.Clear();
 for(face_itr=facelist.Begin();face_itr!=facelist.End();++face_itr){
  unsigned long face_index=*face_itr;
  if(Faces[face_index].Exists)vertex1.FaceList.AddIndex(face_index);
 }
 vertex1.RemovePairContraction(index2);
 vertex2.RemovePairContraction(index1);
 COptimisedVertex::TPairContractionMap::iterator itr=vertex2.PairContractions.begin();
 while(itr!=vertex2.PairContractions.end()){
  std::pair<const unsigned long,CContractionReference>& contr_pair=*itr;
  unsigned long other_index=contr_pair.first;
  COptimisedVertex& other_vertex=Vertices[other_index];
  CContractionReference& local_contraction=contr_pair.second;
  CContractionReference& merged_contraction=vertex1.PairContractions[other_index];
  if(merged_contraction.Contraction.IsNull()){
   local_contraction.Contraction->RemapIndex(index2,index1);
   merged_contraction=local_contraction;
  }else if(local_contraction.Contraction->Exists){
   local_contraction.Contraction->Exists=false;
   VertexPairContractions.erase(local_contraction.Contraction->ListIterator);
  }
  other_vertex.UpdatePairContractions(other_index,*this);
  ++itr;
 }
 float weight2=contraction.Weight2;
 float weight1=1.0f-weight2;
 vertex1.Position=contraction.ResultingPosition;
 vertex1.TextureCoord=vertex1.TextureCoord*weight1+vertex2.TextureCoord*weight2;
 vertex1.Normal=vertex1.Normal*weight1+vertex2.Normal*weight2;
 if(vertex1.Normal.IsNonZero())vertex1.Normal.Normalise();
 vertex1.PositionError+=vertex2.PositionError;
 vertex1.PositionError*=0.5;
 vertex1.InterpolateGroups(vertex2,contraction.Weight2);
 if(contraction.Unsplit)ComputeVertexCost(vertex1);
 else if(vertex2.Cost>vertex1.Cost)vertex1.Cost=vertex2.Cost;
 vertex2.Kill();
}
}
