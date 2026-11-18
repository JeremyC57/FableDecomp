#include <stdio.h>
#include <set>
#include <vector>
#include <utility>

struct Vector {
    float Z;
    explicit Vector(float z=0):Z(z) {}
    float Dot(const Vector& other) const { return Z*other.Z; }
};
namespace QuadricOptimiserInternals {
struct CIndexList {
    std::set<unsigned long> Indices;
    CIndexList GetIntersection(const CIndexList& other) const
    {
        CIndexList result;
        for(std::set<unsigned long>::const_iterator i=Indices.begin();i!=Indices.end();++i)
            if(other.Indices.count(*i)) result.Indices.insert(*i);
        return result;
    }
};
struct COptimisedFace {
    long Edges[3];
    unsigned long UniqueVertexIndices[3];
    Vector Normal;
    COptimisedFace(unsigned long a,unsigned long b,unsigned long c,float z=1):Normal(z)
    {
        Edges[0]=Edges[1]=Edges[2]=-1;
        UniqueVertexIndices[0]=a; UniqueVertexIndices[1]=b; UniqueVertexIndices[2]=c;
    }
};
struct COptimisedVertex { CIndexList FaceList; };
struct COptimisedPrimitive {
    std::vector<std::pair<unsigned long,unsigned long> > Edges;
    std::vector<COptimisedFace> Faces;
    std::vector<COptimisedVertex> UniqueVertices;
    const Vector& GetFaceNormal(const COptimisedFace& face) const { return face.Normal; }
    void JoinAdjacentFaces(unsigned long first,unsigned long second)
    {
        long edge=(long)Edges.size();
        for(unsigned i=0;i!=3;++i)
            for(unsigned j=0;j!=3;++j)
                if(Faces[first].UniqueVertexIndices[i]==Faces[second].UniqueVertexIndices[(j+1)%3] &&
                   Faces[first].UniqueVertexIndices[(i+1)%3]==Faces[second].UniqueVertexIndices[j]) {
                    Faces[first].Edges[i]=edge; Faces[second].Edges[j]=edge;
                }
        Edges.push_back(std::make_pair(first,second));
    }
    void FindAdjacentFaces();
};

void COptimisedPrimitive::FindAdjacentFaces()
{
    Edges.clear();
    for (unsigned long c_face = 0; c_face < Faces.size(); ++c_face) {
        COptimisedFace& face = Faces[c_face];
        for (unsigned long c_side = 0; c_side < 3; ++c_side) {
            if (face.Edges[c_side] == -1) {
                unsigned long unique1 = face.UniqueVertexIndices[c_side];
                unsigned long unique2 = face.UniqueVertexIndices[(c_side + 1) % 3];
                CIndexList facelist_1(UniqueVertices[unique1].FaceList);
                CIndexList facelist_2(UniqueVertices[unique2].FaceList);
                CIndexList facelist = facelist_1.GetIntersection(facelist_2);
                for (std::set<unsigned long>::iterator face_itr = facelist.Indices.begin();
                     face_itr != facelist.Indices.end(); ++face_itr) {
                    unsigned long face_index = *face_itr;
                    if (face_index != c_face) {
                        COptimisedFace& face2 = Faces[face_index];
                        if (GetFaceNormal(face).Dot(GetFaceNormal(face2)) > -0.99f)
                            JoinAdjacentFaces(c_face, face_index);
                    }
                }
            }
        }
    }
}
}

int main()
{
    using namespace QuadricOptimiserInternals;
    COptimisedPrimitive p;
    p.UniqueVertices.resize(5);
    p.FindAdjacentFaces();
    if(!p.Edges.empty()) return 1;
    p.Faces.push_back(COptimisedFace(0,1,2));
    p.Faces.push_back(COptimisedFace(1,0,3));
    p.Faces.push_back(COptimisedFace(2,1,4,-1));
    for(unsigned long i=0;i!=3;++i)
        for(unsigned j=0;j!=3;++j)
            p.UniqueVertices[p.Faces[i].UniqueVertexIndices[j]].FaceList.Indices.insert(i);
    p.Edges.push_back(std::make_pair(4UL,4UL));
    p.FindAdjacentFaces();
    if(p.Edges.size()!=1 || p.Edges[0]!=std::make_pair(0UL,1UL) ||
        p.Faces[0].Edges[0]!=0 || p.Faces[1].Edges[0]!=0 || p.Faces[2].Edges[0]!=-1)
        return 2;
    puts("00ac9a60_TEST PASS");
    return 0;
}
