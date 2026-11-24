#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_CROSS_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_DOT_ORIGINAL_SCOPE_PASS23
#define FABLE_RETAIL_GFFASTONEOVERSQRT_PORTABLE_BITS_PASS26
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#include "engine/C3DVector.h"
#include "fable_assert.h"
unsigned long GFastOneOverSqrtTable[128];
const float DELTA_NOTIONAL_ZERO=0.0001f;
namespace QuadricOptimiserInternals {
static void check(bool x){if(!x)exit(1);}
struct CIndexList {
 std::set<unsigned long> values;
 std::set<unsigned long>::iterator Begin(){return values.begin();}
 std::set<unsigned long>::iterator End(){return values.end();}
 CIndexList GetSymmetricDifference(const CIndexList& other)const{
  CIndexList out;std::set_symmetric_difference(values.begin(),values.end(),other.values.begin(),other.values.end(),std::inserter(out.values,out.values.begin()));return out;
 }
};
struct COptimisedVertex{C3DVector Position;CIndexList FaceList;};
struct CSubContraction{unsigned long Index1,Index2;};
struct COptimisedFace{
 bool Exists;unsigned long idx[3];
 unsigned long GetA()const{return idx[0];}unsigned long GetB()const{return idx[1];}unsigned long GetC()const{return idx[2];}
 unsigned long GetVertexIndex(unsigned long i)const{check(i<3);return idx[i];}
 long FindVertex(unsigned long id)const{for(int i=0;i<3;++i)if(idx[i]==id)return i;return -1;}
};
template<class T>struct Collection{
 std::vector<T> data;int reads;Collection():reads(0){}
 T& At(unsigned int index){check(index<data.size());++reads;return data[index];}
};
struct COptimisedPrimitive{Collection<COptimisedVertex> Vertices;Collection<COptimisedFace> Faces;bool DetectFoldOver(const CSubContraction& sub_contraction,const C3DVector& position);};
bool COptimisedPrimitive::DetectFoldOver(const CSubContraction& sub_contraction,const C3DVector& position)
{
    const COptimisedVertex& merged_vertex=Vertices.At(sub_contraction.Index1);
    const COptimisedVertex& remove_vertex=Vertices.At(sub_contraction.Index2);
    CIndexList facelist=merged_vertex.FaceList.GetSymmetricDifference(remove_vertex.FaceList);
    std::set<unsigned long>::iterator itr;
    for (itr=facelist.Begin(); itr!=facelist.End(); ++itr) {
        unsigned long face_index=*itr;
        const COptimisedFace& face=Faces.At(face_index);
        if (!face.Exists) continue;
        unsigned long center_vertex=face.FindVertex(sub_contraction.Index1);
        if (center_vertex==static_cast<unsigned long>(-1))
            center_vertex=face.FindVertex(sub_contraction.Index2);
        const COptimisedVertex& vertex_a=Vertices.At(face.GetA());
        const COptimisedVertex& vertex_b=Vertices.At(face.GetB());
        const COptimisedVertex& vertex_c=Vertices.At(face.GetC());
        C3DVector normal=(vertex_c.Position-vertex_a.Position)^(vertex_b.Position-vertex_a.Position);
        if (normal.IsNonZero()) normal.Normalise();
        COptimisedVertex& edge_vertex_1=Vertices.At(face.GetVertexIndex((center_vertex+1)%3));
        COptimisedVertex& edge_vertex_2=Vertices.At(face.GetVertexIndex((center_vertex+2)%3));
        C3DVector vect12=edge_vertex_2.Position-edge_vertex_1.Position;
        C3DVector edge_normal=normal^vect12;
        if(edge_normal.IsNonZero()) edge_normal.Normalise();
        // Donor +41B and +42D evaluate position first, then the first edge vertex.
        if (edge_normal.Dot(position) > edge_normal.Dot(edge_vertex_1.Position))
            return true;
    }
    return false;
}
}

using namespace QuadricOptimiserInternals;
static void cases(){
 for(int reverse=0;reverse<2;++reverse)for(int common=0;common<2;++common)
 for(int live=0;live<2;++live)for(int degenerate=0;degenerate<2;++degenerate)for(int side=0;side<3;++side){
  COptimisedPrimitive g;g.Vertices.data.resize(4);CSubContraction sub;sub.Index1=reverse?3:0;sub.Index2=reverse?0:3;
  g.Vertices.data[0].Position=C3DVector(0,0,0);g.Vertices.data[1].Position=C3DVector(2,0,0);
  g.Vertices.data[2].Position=degenerate?C3DVector(2,0,0):C3DVector(0,2,0);g.Vertices.data[3].Position=C3DVector(0,0,0);
  COptimisedFace f;f.Exists=live!=0;f.idx[0]=0;f.idx[1]=1;f.idx[2]=2;g.Faces.data.push_back(f);
  g.Vertices.data[0].FaceList.values.insert(0);if(common)g.Vertices.data[3].FaceList.values.insert(0);
  bool answer=g.DetectFoldOver(sub,C3DVector((float)side,1,8));
  check(answer==(side==2&&live&&!common&&!degenerate));check(g.Faces.reads==(common?0:1));
 }
 for(int center=0;center<3;++center){
  COptimisedPrimitive g;g.Vertices.data.resize(4);CSubContraction sub;sub.Index1=0;sub.Index2=3;
  g.Vertices.data[0].Position=C3DVector(0,0,0);g.Vertices.data[1].Position=C3DVector(2,0,0);
  g.Vertices.data[2].Position=C3DVector(0,2,0);g.Vertices.data[3].Position=C3DVector(0,0,0);
  COptimisedFace f;f.Exists=false;f.idx[center]=0;f.idx[(center+1)%3]=1;f.idx[(center+2)%3]=2;
  g.Faces.data.push_back(f);f.Exists=true;g.Faces.data.push_back(f);g.Faces.data.push_back(f);
  for(int i=0;i<3;++i)g.Vertices.data[0].FaceList.values.insert(i);
  check(g.DetectFoldOver(sub,C3DVector(2,1,-8)));check(g.Faces.reads==2);
 }
}
int main(){cases();puts("00acd0e0_TEST PASS");return 0;}