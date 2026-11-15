// QuadricOptimiserInternals::COptimisedPrimitive::Initialise(const COptimisationParameters& parameters) -- set up the quadric optimiser: per-vertex face lists, adjacency and segments, the plane / crease / border quadrics of every vertex, the initial vertex costs, then every edge and same-position vertex pair as a contraction in the cost-ordered list, registered with both of its vertices
// Grade: DIFFER(7240v7256) -- structure equal (66 calls, same blocks); residue: register/slot choices (this in ebx in passes 0-1, edge-loop new ptr in a slot, filter-loop zero register), pass-5 IsNonZero product order (Z,Y,X), scaling_squared store placement, loop-alignment padding
// Leaf: ?Initialise@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXABVCOptimisationParameters@2@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ACA830, 7256 bytes, thiscall `ret 4`, aligned frame 0xD4, no EH.
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_DECAL_HEAP
#define FABLE_RETAIL_STLP_MULTIMAP_BEGIN_END
#define FABLE_RETAIL_STLP_TREE_INSERT_EQUAL_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
#define FABLE_RETAIL_QUADRIC_CUNIQUEVERTEX
#define FABLE_RETAIL_QUADRIC_COPTIMISEDVERTEX_GROUPS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDFACE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDEDGE
#define FABLE_RETAIL_QUADRIC_CVERTEXPAIRCONTRACTION
#define FABLE_RETAIL_CQUADRIC_MEMBERS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_METHODS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_INITIALISE
#include <new>
#include "stlport/fable_map.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/QuadricOptimiserTypes.h"
#include "engine/COptimisedPrimitive.h"

namespace QuadricOptimiserInternals {

typedef std::map<unsigned long,CContractionReference> CPairContractionMap;

void CQuadric::Clear()
{
    AA = 0.0;
    AB = 0.0;
    AC = 0.0;
    AD = 0.0;
    BB = 0.0;
    BC = 0.0;
    BD = 0.0;
    CC = 0.0;
    CD = 0.0;
    DD = 0.0;
    Weight = 0.0;
}

void CQuadric::operator+=(const CQuadric& other)
{
    AA += other.AA;
    AB += other.AB;
    AC += other.AC;
    AD += other.AD;
    BB += other.BB;
    BC += other.BC;
    BD += other.BD;
    CC += other.CC;
    CD += other.CD;
    DD += other.DD;
    Weight += other.Weight;
}

void CQuadric::operator*=(double scale)
{
    AA *= scale;
    AB *= scale;
    AC *= scale;
    AD *= scale;
    BB *= scale;
    BC *= scale;
    BD *= scale;
    CC *= scale;
    CD *= scale;
    DD *= scale;
    Weight *= scale;
}

void COptimisedPrimitive::Initialise(const COptimisationParameters& parameters)
{
    Parameters = parameters;
    float scaling = parameters.Scaling;
    float scaling_squared = scaling * scaling;

    for (unsigned long i = 0; i < Vertices.size(); i++) {
        COptimisedVertex& vertex = Vertices[i];
        vertex.PositionError.Clear();
        vertex.Cost = 0.0f;
    }

    for (unsigned long i = 0; i < Faces.size(); i++) {
        COptimisedFace& face = Faces[i];
        COptimisedVertex& vertex0 = Vertices[face.VertexIndices[0]];
        COptimisedVertex& vertex1 = Vertices[face.VertexIndices[1]];
        COptimisedVertex& vertex2 = Vertices[face.VertexIndices[2]];
        vertex0.FaceList.AddIndex(i);
        vertex1.FaceList.AddIndex(i);
        vertex2.FaceList.AddIndex(i);
        UniqueVertices[vertex0.UniqueIndex].FaceList.AddIndex(i);
        UniqueVertices[vertex1.UniqueIndex].FaceList.AddIndex(i);
        UniqueVertices[vertex2.UniqueIndex].FaceList.AddIndex(i);
    }

    FindAdjacentFaces();
    PaintSegments();
    MeasureSegments();

    CQuadric quadric;
    for (unsigned long i = 0; i < Faces.size(); i++) {
        COptimisedFace& face = Faces[i];
        COptimisedVertex& vertex0 = Vertices[face.VertexIndices[0]];
        COptimisedVertex& vertex1 = Vertices[face.VertexIndices[1]];
        COptimisedVertex& vertex2 = Vertices[face.VertexIndices[2]];
        C3DVector edge0 = vertex1.Position - vertex0.Position;
        float length0 = (float)sqrt(edge0.Y * edge0.Y + edge0.Z * edge0.Z + edge0.X * edge0.X);
        C3DVector edge1 = vertex2.Position - vertex1.Position;
        float length1 = (float)sqrt(edge1.Y * edge1.Y + edge1.Z * edge1.Z + edge1.X * edge1.X);
        C3DVector edge2 = vertex0.Position - vertex2.Position;
        float length2 = (float)sqrt(edge2.Y * edge2.Y + edge2.Z * edge2.Z + edge2.X * edge2.X);
        float longest = ((length0 > length1 ? length0 : length1) > length2) ? (length0 > length1 ? length0 : length1) : length2;
        C3DVector normal = GetFaceNormal(face);
        if (normal.IsNonZero()) {
            normal.Normalise();
            quadric.Initialise(normal, vertex2.Position, 1.0f);
            quadric *= longest * longest * 0.5f;
            vertex0.PositionError += quadric;
            vertex1.PositionError += quadric;
            vertex2.PositionError += quadric;
        }
    }

    for (unsigned long i = 0; i < Vertices.size(); i++) {
        COptimisedVertex& vertex = Vertices[i];
        if (vertex.PositionError.Weight > 0.0)
            vertex.PositionError *= scaling_squared / vertex.PositionError.Weight;
    }

    for (unsigned long i = 0; i < Faces.size(); i++) {
        COptimisedFace& face = Faces[i];
        C3DVector normal = GetFaceNormal(face);
        for (unsigned long e = 0; e < 3; e++) {
            long adjacent = face.Edges[e];
            if (adjacent == -1)
                continue;
            C3DVector other_normal = GetFaceNormal(Faces[adjacent]);
            float dot = other_normal.Dot(normal);
            if (dot < 0.0f) {
                C3DVector crease_normal = other_normal + normal;
                if (crease_normal.IsNonZero()) {
                    crease_normal.Normalise();
                    COptimisedVertex& vertex_a = Vertices[face.VertexIndices[e]];
                    COptimisedVertex& vertex_b = Vertices[face.VertexIndices[(e + 1) % 3]];
                    quadric.Initialise(crease_normal, vertex_a.Position, 1.0f);
                    quadric *= -dot;
                    vertex_a.PositionError += quadric;
                    vertex_b.PositionError += quadric;
                }
            }
        }
    }

    for (unsigned long i = 0; i < Faces.size(); i++) {
        COptimisedFace& face = Faces[i];
        C3DVector normal = GetFaceNormal(face);
        for (unsigned long e = 0; e < 3; e++) {
            if (face.Edges[e] != -1)
                continue;
            COptimisedVertex& vertex_a = Vertices[face.VertexIndices[e]];
            COptimisedVertex& vertex_b = Vertices[face.VertexIndices[(e + 1) % 3]];
            C3DVector edge = vertex_b.Position - vertex_a.Position;
            C3DVector edge_normal = normal ^ edge;
            if (edge_normal.IsNonZero())
                edge_normal.Normalise();
            quadric.Initialise(edge_normal, vertex_a.Position, 0.0f);
            quadric *= edge.GetSquaredMagnitude() * parameters.EdgeWeight * scaling_squared;
            vertex_a.PositionError += quadric;
            vertex_b.PositionError += quadric;
            vertex_a.Cost += parameters.EdgeCost;
            vertex_b.Cost += parameters.EdgeCost;
        }
    }

    for (unsigned long i = 0; i < Faces.size(); i++)
        FindFaceTextureTangents(Faces[i]);

    for (unsigned long i = 0; i < Vertices.size(); i++)
        ComputeVertexCost(Vertices[i]);

    std::map<std::pair<long,long>,CCountedPointer<CVertexPairContraction> > contractions;
    for (unsigned long i = 0; i < Edges.size(); i++) {
        COptimisedEdge& edge = Edges[i];
        unsigned long unique1 = edge.UniqueVertex1;
        unsigned long unique2 = edge.UniqueVertex2;
        if (unique1 > unique2) {
            unsigned long temp = unique1;
            unique1 = unique2;
            unique2 = temp;
        }
        CCountedPointer<CVertexPairContraction>& contraction = contractions[std::pair<long,long>(unique1, unique2)];
        if (contraction.IsNull())
            contraction.Reset(new CVertexPairContraction);
        const COptimisedFace& face_a = Faces[edge.FaceA];
        CSubContraction sub_a;
        sub_a.Index1 = UniqueToActualIndex(unique1, face_a);
        sub_a.Index2 = UniqueToActualIndex(unique2, face_a);
        contraction->SubContractions.push_back(sub_a);
        const COptimisedFace& face_b = Faces[edge.FaceB];
        CSubContraction sub_b;
        sub_b.Index1 = UniqueToActualIndex(unique1, face_b);
        sub_b.Index2 = UniqueToActualIndex(unique2, face_b);
        if (sub_a.Index1 != sub_b.Index1 || sub_a.Index2 != sub_b.Index2)
            contraction->SubContractions.push_back(sub_b);
    }

    for (std::map<std::pair<long,long>,CCountedPointer<CVertexPairContraction> >::iterator it = contractions.begin(); it != contractions.end(); ++it) {
        std::set<long> vertices1;
        std::set<long> vertices2;
        unsigned long count = it->second->SubContractions.size();
        for (unsigned long s = 0; s < count; s++) {
            const CSubContraction& sub = it->second->SubContractions[s];
            vertices1.insert(sub.Index1);
            vertices2.insert(sub.Index2);
        }
        if (vertices1.size() == count && vertices2.size() == count) {
            ComputeContraction(*it->second);
            VertexPairContractions.insert(std::pair<const float,CCountedPointer<CVertexPairContraction> >(it->second->Cost, it->second));
        }
    }

    for (unsigned long u = 0; u < UniqueVertices.size(); u++) {
        CIndexList& vertex_indices = UniqueVertices[u].VertexIndices;
        if (vertex_indices.Indices.size() > 1) {
            for (std::set<unsigned long>::iterator it1 = vertex_indices.Indices.begin(); it1 != vertex_indices.Indices.end(); ++it1) {
                unsigned long vertex1 = *it1;
                std::set<unsigned long>::iterator it2 = it1;
                for (++it2; it2 != vertex_indices.Indices.end(); ++it2) {
                    unsigned long vertex2 = *it2;
                    CCountedPointer<CVertexPairContraction> contraction;
                    contraction.Reset(new CVertexPairContraction);
                    contraction->Unsplit = true;
                    CSubContraction sub;
                    sub.Index1 = vertex1;
                    sub.Index2 = vertex2;
                    contraction->SubContractions.push_back(sub);
                    ComputeSelfContraction(*contraction);
                    VertexPairContractions.insert(std::pair<const float,CCountedPointer<CVertexPairContraction> >(contraction->Cost, contraction));
                }
            }
        }
    }

    for (std::multimap<float,CCountedPointer<CVertexPairContraction> >::iterator it = VertexPairContractions.begin(); it != VertexPairContractions.end(); ++it) {
        CCountedPointer<CVertexPairContraction> contraction = it->second;
        unsigned long count = contraction->SubContractions.size();
        for (unsigned long s = 0; s < count; s++) {
            const CSubContraction& sub = contraction->SubContractions[s];
            COptimisedVertex& vertex1 = Vertices[sub.Index1];
            COptimisedVertex& vertex2 = Vertices[sub.Index2];
            reinterpret_cast<CPairContractionMap&>(vertex1.PairContractions)[sub.Index2] = CContractionReference(contraction, s);
            reinterpret_cast<CPairContractionMap&>(vertex2.PairContractions)[sub.Index1] = CContractionReference(contraction, s);
        }
        contraction->ListIterator = it;
    }
}

}
