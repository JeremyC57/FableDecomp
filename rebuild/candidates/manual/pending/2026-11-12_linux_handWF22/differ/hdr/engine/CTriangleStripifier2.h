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
private:
 void FillInPolygons(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<unsigned short>&,const CArray<bool>&,CArray<long>&,CArray<unsigned short>&,bool);
 void GetMeshIslandSubsection(const CArray<unsigned short>&,const CArray<bool>&,const CArray<CArray<long> >&,CArray<unsigned short>&,CArray<long>&);
#ifdef FABLE_RETAIL_CTRIANGLESTRIPIFIER2_STRIPUNUSED
 void StripUnusedVerts(CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<bool>&);
#endif
#ifdef FABLE_RETAIL_CTRIANGLESTRIPIFIER2_BUILDCACHE
 void BuildCacheOptimizedVertexList(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<CArray<unsigned short> >&,const CArray<unsigned short>&,const CArray<CArray<unsigned short> >&,CArray<unsigned short>&);
#endif
};
#endif
