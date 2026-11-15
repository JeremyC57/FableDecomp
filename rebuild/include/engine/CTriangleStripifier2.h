#ifndef FABLE_CTRIANGLESTRIPIFIER2_H
#define FABLE_CTRIANGLESTRIPIFIER2_H
#include "engine/CArray.h"
class C3DTriangle2;
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
