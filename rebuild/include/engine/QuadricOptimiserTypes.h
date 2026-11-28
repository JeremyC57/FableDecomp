#ifndef FABLE_ENGINE_QUADRICOPTIMISERTYPES_H
#define FABLE_ENGINE_QUADRICOPTIMISERTYPES_H
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#ifdef FABLE_RETAIL_CQUADRIC_GETINVERSEMATRIXPART
#include "engine/CMatrix3x4.h"
#endif
#ifdef FABLE_RETAIL_QUADRIC_CUNIQUEVERTEX
#include "stlport/fable_map.h"
#endif
#ifdef FABLE_RETAIL_QUADRIC_CVERTEXPAIRCONTRACTION
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
#endif
// Original Ego_r class records; retail three container reductions give native vertex strideB8.
namespace QuadricOptimiserInternals {
class CQuadric {public:double AA,AB,AC,AD,BB,BC,BD,CC,CD,DD,Weight;void operator+=(const CQuadric&);double EvaluateError(const C3DVector&)const;
#ifdef FABLE_RETAIL_CQUADRIC_GETINVERSEMATRIXPART
 // Retail opt-in (chunk Q14, row 0x00AC70F0): Ego_r.pdb 0x00C5F9C0 / FableWin.pdb 0x034BF810
 // ?GetInverseMatrixPart@CQuadric@QuadricOptimiserInternals@@QBE?AVCMatrix3x4@@XZ, out of line in retail at 0x00AC70F0. Declaration only.
 CMatrix3x4 GetInverseMatrixPart()const;
#endif
#if defined(FABLE_RETAIL_CQUADRIC_DETERMINANT_PASS27) && !defined(FABLE_RETAIL_CQUADRIC_OPTIMALPLACEMENT)
 double GetDeterminant() const {
    double det = AA * (BB * CC - BC * BC) + AB * (BC * AC - AB * CC) + AC * (AB * BC - BB * AC);
    return det;
 }
#endif
#ifdef FABLE_RETAIL_CQUADRIC_OPTIMALPLACEMENT
 // Retail opt-in (Codex pass 24): FableWin and Ego_r carry these two helpers expanded at 0x00AC77B6 and 0x00AC780E.
 double GetDeterminant() const {
    double det = AA * (BB * CC - BC * BC) + AC * (BC * AB - AC * BB) + AB * (AC * BC - CC * AB);
    return det;
 }
 C3DVector GetVectorPart() const { return C3DVector((float)AD, (float)BD, (float)CD); }
#endif
#ifdef FABLE_RETAIL_CQUADRIC_MEMBERS
 // Retail opt-in (row 0x00ACA830): Ego_r.pdb / FableWin.pdb ?Clear@CQuadric@QuadricOptimiserInternals@@QAEXXZ (0x00AC6CE0),
 // ??XCQuadric@QuadricOptimiserInternals@@QAEXN@Z (0x00AC6EF0; both same-TU out-of-line definitions that 0x00ACA830 expands) and
 // ?Initialise@CQuadric@QuadricOptimiserInternals@@QAEXABVC3DVector@@0M@Z (0x00AC7050, called). Declarations only.
 void Clear();
 void operator*=(double scale);
 void Initialise(const C3DVector& normal,const C3DVector& point,float weight);
#endif
};
#ifdef FABLE_RETAIL_QUADRIC_CUNIQUEVERTEX
// Retail opt-in (row 0x00ACDB80): Ego_r.pdb CIndexList { set<unsigned long> Indices } and CUniqueVertex { bool Exists; CIndexList
// VertexIndices, FaceList } with retail 12-byte STLport sets (record 0x1C: landed 0x00AD4C40 / ~COptimisedPrimitive 0x00AD84D0);
// FableWin.pdb ?AddIndex@CIndexList@QuadricOptimiserInternals@@QAEXK@Z (inline), ~CUniqueVertex 0x00AD4A70 out of line.
class CIndexList {public:
 std::set<unsigned long> Indices;
#ifdef FABLE_RETAIL_QUADRIC_FACE_FOLD_PASS27
 CIndexList GetSymmetricDifference(const CIndexList& other) const;
 std::set<unsigned long>::iterator Begin() const { return Indices.begin(); }
 std::set<unsigned long>::iterator End() const { return Indices.end(); }
#endif
#ifdef FABLE_RETAIL_QUADRIC_CINDEXLIST_INTERSECTION
 // Retail opt-in (Codex pass 24): both PDBs name the public const member at 0x00AC8960.
 CIndexList GetIntersection(const CIndexList& other) const;
#endif
 void AddIndex(unsigned long index){Indices.insert(index);}
};
class CUniqueVertex {public:
 bool Exists;CIndexList VertexIndices;CIndexList FaceList;
 ~CUniqueVertex();                                                                                  // 0x00AD4A70
};
#endif
class COptimisedVertex {public:
 C3DVector Position,Normal,TextureCoord;CQuadric PositionError;float Cost;
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDVERTEX_GROUPS
 // Retail opt-in (row 0x00ACDB80; needs FABLE_RETAIL_QUADRIC_CUNIQUEVERTEX): Ego_r.pdb COptimisedVertex FaceList is a CIndexList and
 // Groups a map<unsigned long,float> (same 12-byte retail slots, +0x84 / +0x9C); FableWin.pdb ctor / copy ctor / dtor (retail
 // 0x00AD7A00 / - / 0x00AD7400, out of line) and the inline ?AddGroup@COptimisedVertex@QuadricOptimiserInternals@@QAEXKM@Z.
 CIndexList FaceList;bool Exists;unsigned long UniqueIndex,OutputIndex;std::map<unsigned long,float> Groups;unsigned char PairContractions[12];
 COptimisedVertex();                                                                                // 0x00AD7A00
 COptimisedVertex(const COptimisedVertex& other);
 ~COptimisedVertex();                                                                               // 0x00AD7400
 void AddGroup(unsigned long group,float weight){Groups[group]=weight;}
#ifdef FABLE_RETAIL_QUADRIC_COPY_TO_PASS30
 void LimitGroupCount(unsigned long maximum); // 00ACA5F0.
#endif
#else
 unsigned char FaceList[12];bool Exists;unsigned long UniqueIndex,OutputIndex;unsigned char Groups[12],PairContractions[12];
#endif
 float GetGroupDifference(const COptimisedVertex&)const;
};
class CSubContraction {public:unsigned long Index1,Index2;};
#ifdef FABLE_RETAIL_QUADRIC_CVERTEXPAIRCONTRACTION
// Retail opt-in (row 0x00ACA830): Ego_r.pdb CVertexPairContraction (Exists, Unsplit, SubContractions, Weight2, Cost,
// ResultingPosition, ListIterator; retail 0x28 with a 12-byte CArray and a 4-byte multimap iterator, as the landed UpdateCost
// 0x00AC8A60 / ComputeContraction 0x00AC92C0 models) with FableWin.pdb's inline ??0CVertexPairContraction@QuadricOptimiserInternals@@QAE@XZ
// (expanded at 0x00ACB935: [0] = 1, [1] = 0, the vector words, [24h] = 0); Ego_r.pdb CContractionReference { CCountedPointer
// Contraction; unsigned long SubIndex } with the inline ??0CContractionReference@...@QAE@ABV?$CCountedPointer@...@@K@Z (0x00ACC2D0).
class CVertexPairContraction {public:
 bool Exists,Unsplit;CArray<CSubContraction> SubContractions;float Weight2,Cost;C3DVector ResultingPosition;
 std::multimap<float,CCountedPointer<CVertexPairContraction> >::iterator ListIterator;
 CVertexPairContraction():Exists(true),Unsplit(false),ListIterator(0){}
#ifdef FABLE_RETAIL_QUADRIC_CVERTEXPAIRCONTRACTION_ISILLEGAL
 // Retail opt-in (egod e02, row 0x00ACD960): Ego_r.pdb 0x00C61350 ?IsIllegal@CVertexPairContraction@QuadricOptimiserInternals@@QAE_NXZ
 // (right after CleanUp 0x00C61090; retail 0x00AC8360 calls CleanUp 0x00AC7B00 first). Out of line in retail; declaration only.
 bool IsIllegal();                                                                                  // 0x00AC8360
#endif
};
class CContractionReference {public:
 CCountedPointer<CVertexPairContraction> Contraction;unsigned long SubIndex;
 CContractionReference(){}
 CContractionReference(const CCountedPointer<CVertexPairContraction>& contraction,unsigned long sub_index):Contraction(contraction),SubIndex(sub_index){}
};
#endif
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDFACE
// Retail opt-in (chunk Q15, rows 0x00AC7CB0 / 0x00AC7300): Ego_r.pdb COptimisedFace (72 bytes, no containers, so the retail
// layout is the PDB one): VertexIndices +0, UniqueVertexIndices +0Ch, Edges +18h, UTangent +24h, VTangent +30h, Exists +3Ch,
// OutputIndex +40h, Segment +44h.
class COptimisedFace {public:
 unsigned long VertexIndices[3];unsigned long UniqueVertexIndices[3];long Edges[3];C3DVector UTangent,VTangent;bool Exists;
 unsigned long OutputIndex;long Segment;
#ifdef FABLE_RETAIL_QUADRIC_FACE_FOLD_PASS27
 unsigned long GetA() const { return VertexIndices[0]; }
 unsigned long GetB() const { return VertexIndices[1]; }
 unsigned long GetC() const { return VertexIndices[2]; }
 unsigned long GetVertexIndex(unsigned long index) const { return VertexIndices[index]; }
 long FindVertex(unsigned long vertex) const {
     for (unsigned long c=0;c<3;++c) if (VertexIndices[c]==vertex) return c;
     return -1;
 }
#endif
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDFACE_HASUNIQUEVERTEX
 // Retail opt-in (row 0x00AC8AE0): FableWin.pdb ?HasUniqueVertex@COptimisedFace@QuadricOptimiserInternals@@QBE_NK@Z (Ego_r 0x00C5F860),
 // inline in retail (expanded six times in JoinAdjacentFaces; compare order U[0], U[1], U[2]).
 bool HasUniqueVertex(unsigned long unique_vertex)const{return UniqueVertexIndices[0]==unique_vertex||UniqueVertexIndices[1]==unique_vertex||UniqueVertexIndices[2]==unique_vertex;}
#endif
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDFACE_CTOR
 // Retail opt-in (row 0x00ACDB80): FableWin.pdb ??0COptimisedFace@QuadricOptimiserInternals@@QAE@XZ (inline; expanded before the
 // vector<COptimisedFace>::_M_fill_insert 0x00AD54F0 call in 0x00ACDB80: Exists = true, Segment = -1, Edges[0..2] = -1).
 COptimisedFace():Exists(true),Segment(-1){Edges[0]=Edges[1]=Edges[2]=-1;}
#endif
};
FABLE_STATIC_ASSERT_AT(36, sizeof(COptimisedFace)==0x48);
#ifdef FABLE_RETAIL_QUADRIC_COPTIMISEDEDGE
// Retail opt-in (row 0x00AC8AE0): Ego_r.pdb COptimisedEdge (16 bytes): FaceA +0, FaceB +4, UniqueVertex1 +8, UniqueVertex2 +0Ch.
class COptimisedEdge {public:
 unsigned long FaceA,FaceB,UniqueVertex1,UniqueVertex2;
};
FABLE_STATIC_ASSERT_AT(42, sizeof(COptimisedEdge)==0x10);
#endif
#endif
FABLE_STATIC_ASSERT_AT(45, sizeof(CQuadric)==0x58);
FABLE_STATIC_ASSERT_AT(46, sizeof(COptimisedVertex)==0xb8);
FABLE_STATIC_ASSERT_AT(47, sizeof(CSubContraction)==8);
}
#endif
