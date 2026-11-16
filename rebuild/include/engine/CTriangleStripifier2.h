#ifndef FABLE_CTRIANGLESTRIPIFIER2_H
#define FABLE_CTRIANGLESTRIPIFIER2_H
#include "engine/CArray.h"
class C3DTriangle2;
#ifdef FABLE_RETAIL_PASS22_TRIANGLE_ORIGINAL_COMPLETE_DECLARATIONS
// Opt-in (Codex pass 22, row 0x00F3E120): complete original signatures; inverse overload-group order is supported
// by measured VC71 LF_METHODLIST emission, not literal textual source proof.
// Generated constructors, destructors and assignments stay implicit.
// Also row 0x00B6EDF0 (acctC wfE6; it defined FABLE_RETAIL_CTRIANGLESTRIPIFIER2 for this class shape, a name chunk 730 already uses for
// the members below): Fable.exe RTTI CTriangleStripifier2 : CTriangleStripifierBase : CBaseClassNonCopyable : CBaseClass, no data
// (4 bytes); vtable 0x012A331C: slot 0 ??_G 0x00B7AE00 (no vtable stores, so both destructors are the implicit ones), slot 1 the
// C3DTriangle2 GenerateTriangleListAndStrip 0x00F3F3A0; the index-array overload is non-virtual (0x00F3EA50; caller 0x00B6EDF0).
#include "engine/CBaseClassNonCopyable.h"
class CTriangleStripifierBase : public CBaseClassNonCopyable {
public:
 virtual void GenerateTriangleListAndStrip(const CArray<C3DTriangle2>&,CArray<C3DTriangle2>&,CArray<unsigned short>&)=0;
};
class CTriangleStripifier2 : public CTriangleStripifierBase {
public:
 virtual void GenerateTriangleListAndStrip(const CArray<C3DTriangle2>&,CArray<C3DTriangle2>&,CArray<unsigned short>&);
 void GenerateTriangleListAndStrip(CArray<unsigned short>&,CArray<unsigned short>&,CArray<unsigned short>&);
 void ReorderTriangleListInPlace(unsigned short *,long);
private:
 void BuildVertexAndPolyMaps(const CArray<unsigned short>&,CArray<CArray<long> >&,CArray<CArray<unsigned short> >&);
 void GetMeshIslandSubsection(const CArray<unsigned short>&,const CArray<bool>&,const CArray<CArray<long> >&,CArray<unsigned short>&,CArray<long>&);
 void BuildRings(long,const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<CArray<unsigned short> >&,CArray<unsigned short>&,CArray<unsigned short>&,CArray<CArray<unsigned short> >&);
 void BuildCacheOptimizedVertexList(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<CArray<unsigned short> >&,const CArray<unsigned short>&,const CArray<CArray<unsigned short> >&,CArray<unsigned short>&);
 long CalcSectionPolyMask(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<unsigned short>&,CArray<bool>&);
 float CalcSectionScore(const CArray<unsigned short>&,const CArray<unsigned short>&,const CArray<CArray<long> >&);
 long FindBestStartVertex(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<CArray<unsigned short> >&);
 void FillInPolygons(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<unsigned short>&,const CArray<bool>&,CArray<long>&,CArray<unsigned short>&,bool);
 void FillInTriStrip(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<unsigned short>&,const CArray<bool>&,CArray<unsigned short>&,bool,bool&);
 void CheckForReshadedVerts(CArray<unsigned short>&);
 void StripUnusedVerts(CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<bool>&);
};
#else
// Layout: original PDB base and derived records each have size four, no data.
class CTriangleStripifierBase{
public:
 virtual void GenerateTriangleListAndStrip(const CArray<C3DTriangle2>&,CArray<C3DTriangle2>&,CArray<unsigned short>&)=0;
 virtual ~CTriangleStripifierBase(){}
};
class CTriangleStripifier2:public CTriangleStripifierBase{
public:
 virtual void GenerateTriangleListAndStrip(const CArray<C3DTriangle2>&,CArray<C3DTriangle2>&,CArray<unsigned short>&);
 virtual ~CTriangleStripifier2(){}
#ifdef FABLE_RETAIL_CTRIANGLESTRIPIFIER2
 // Retail branch (chunk 730): the rest of the PDB member set, in Ego_r.pdb / FableWin.pdb spelling (both name sets carry every
 // one): the public index-list overload ?GenerateTriangleListAndStrip@CTriangleStripifier2@@QAEXAAV?$CArray@G@@00@Z (retail
 // 0x00F3EA50) and ReorderTriangleListInPlace (QAEXPAGJ@Z, 0x00F3F5A0), and the private (AAE) helpers with their retail
 // addresses: StripUnusedVerts 0x00F3B970, BuildCacheOptimizedVertexList 0x00F3BA70, FillInTriStrip 0x00F3BDD0,
 // FillInPolygons 0x00F3C990, CalcSectionPolyMask 0x00F3CF50, CheckForReshadedVerts 0x00F3D200, GetMeshIslandSubsection
 // 0x00F3D320, CalcSectionScore 0x00F3D830, BuildRings 0x00F3D980, FindBestStartVertex 0x00F3E120, BuildVertexAndPolyMaps
 // 0x00F3E6B0 (retail order = donor order except CalcSectionPolyMask; addresses from the catalog and the call sites in
 // 0x00F3EA50 / 0x00F3E120 / 0x00F3D980, signatures from the demangled PDB names).
 void GenerateTriangleListAndStrip(CArray<unsigned short>&,CArray<unsigned short>&,CArray<unsigned short>&);
 void ReorderTriangleListInPlace(unsigned short*,long);
#endif
private:
 void FillInPolygons(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<unsigned short>&,const CArray<bool>&,CArray<long>&,CArray<unsigned short>&,bool);
 void GetMeshIslandSubsection(const CArray<unsigned short>&,const CArray<bool>&,const CArray<CArray<long> >&,CArray<unsigned short>&,CArray<long>&);
#ifdef FABLE_RETAIL_CTRIANGLESTRIPIFIER2
 void StripUnusedVerts(CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<bool>&);
 long CalcSectionPolyMask(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<unsigned short>&,CArray<bool>&);
 void BuildCacheOptimizedVertexList(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<CArray<unsigned short> >&,const CArray<unsigned short>&,const CArray<CArray<unsigned short> >&,CArray<unsigned short>&);
 void FillInTriStrip(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<unsigned short>&,const CArray<bool>&,CArray<unsigned short>&,bool,bool&);
 void CheckForReshadedVerts(CArray<unsigned short>&);
 float CalcSectionScore(const CArray<unsigned short>&,const CArray<unsigned short>&,const CArray<CArray<long> >&);
 void BuildVertexAndPolyMaps(const CArray<unsigned short>&,CArray<CArray<long> >&,CArray<CArray<unsigned short> >&);
 void BuildRings(long,const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<CArray<unsigned short> >&,CArray<unsigned short>&,CArray<unsigned short>&,CArray<CArray<unsigned short> >&);
 long FindBestStartVertex(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<CArray<unsigned short> >&);
#endif
};
#endif
#endif
