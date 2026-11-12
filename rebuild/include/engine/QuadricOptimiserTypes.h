#ifndef FABLE_ENGINE_QUADRICOPTIMISERTYPES_H
#define FABLE_ENGINE_QUADRICOPTIMISERTYPES_H
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#ifdef FABLE_RETAIL_CQUADRIC_GETINVERSEMATRIXPART
#include "engine/CMatrix3x4.h"
#endif
// Original Ego_r class records; retail three container reductions give native vertex strideB8.
namespace QuadricOptimiserInternals {
class CQuadric {public:double AA,AB,AC,AD,BB,BC,BD,CC,CD,DD,Weight;void operator+=(const CQuadric&);double EvaluateError(const C3DVector&)const;
#ifdef FABLE_RETAIL_CQUADRIC_GETINVERSEMATRIXPART
 // Retail opt-in (chunk Q14, row 0x00AC70F0): Ego_r.pdb 0x00C5F9C0 / FableWin.pdb 0x034BF810
 // ?GetInverseMatrixPart@CQuadric@QuadricOptimiserInternals@@QBE?AVCMatrix3x4@@XZ, out of line in retail at 0x00AC70F0. Declaration only.
 CMatrix3x4 GetInverseMatrixPart()const;
#endif
};
class COptimisedVertex {public:
 C3DVector Position,Normal,TextureCoord;CQuadric PositionError;float Cost;
 unsigned char FaceList[12];bool Exists;unsigned long UniqueIndex,OutputIndex;unsigned char Groups[12],PairContractions[12];
 float GetGroupDifference(const COptimisedVertex&)const;
};
class CSubContraction {public:unsigned long Index1,Index2;};
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDFACE
// Retail opt-in (chunk Q15, rows 0x00AC7CB0 / 0x00AC7300): Ego_r.pdb COptimisedFace (72 bytes, no containers, so the retail
// layout is the PDB one): VertexIndices +0, UniqueVertexIndices +0Ch, Edges +18h, UTangent +24h, VTangent +30h, Exists +3Ch,
// OutputIndex +40h, Segment +44h.
class COptimisedFace {public:
 unsigned long VertexIndices[3];unsigned long UniqueVertexIndices[3];long Edges[3];C3DVector UTangent,VTangent;bool Exists;
 unsigned long OutputIndex;long Segment;
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDFACE_HASUNIQUEVERTEX
 // Retail opt-in (row 0x00AC8AE0): FableWin.pdb ?HasUniqueVertex@COptimisedFace@QuadricOptimiserInternals@@QBE_NK@Z (Ego_r 0x00C5F860),
 // inline in retail (expanded six times in JoinAdjacentFaces; compare order U[0], U[1], U[2]).
 bool HasUniqueVertex(unsigned long unique_vertex)const{return UniqueVertexIndices[0]==unique_vertex||UniqueVertexIndices[1]==unique_vertex||UniqueVertexIndices[2]==unique_vertex;}
#endif
};
FABLE_STATIC_ASSERT(sizeof(COptimisedFace)==0x48);
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDEDGE
// Retail opt-in (row 0x00AC8AE0): Ego_r.pdb COptimisedEdge (16 bytes): FaceA +0, FaceB +4, UniqueVertex1 +8, UniqueVertex2 +0Ch.
class COptimisedEdge {public:
 unsigned long FaceA,FaceB,UniqueVertex1,UniqueVertex2;
};
FABLE_STATIC_ASSERT(sizeof(COptimisedEdge)==0x10);
#endif
#endif
FABLE_STATIC_ASSERT(sizeof(CQuadric)==0x58);
FABLE_STATIC_ASSERT(sizeof(COptimisedVertex)==0xb8);
FABLE_STATIC_ASSERT(sizeof(CSubContraction)==8);
}
#endif
