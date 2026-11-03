#ifndef FABLE_CTRIANGLESTRIPIFIER2_H
#define FABLE_CTRIANGLESTRIPIFIER2_H
#ifdef FABLE_RETAIL_CTRIANGLESTRIPIFIER2
// Retail opt-in (chunk 723): Fable.exe RTTI CTriangleStripifier2 : CTriangleStripifierBase : CBaseClassNonCopyable : CBaseClass, no
// data (4 bytes). vtable 0x012A331C: slot 0 ??_G 0x00B7AE00 (no vtable stores, so both destructors are the implicit ones), slot 1
// the C3DTriangle2 GenerateTriangleListAndStrip 0x00F3F3A0. The index-array overload is non-virtual (FableWin.pdb
// ?GenerateTriangleListAndStrip@CTriangleStripifier2@@QAEXAAV?$CArray@G@@00@Z, 0x00F3EA50; caller 0x00B6EDF0).
#include "engine/CArray.h"
#include "engine/CBaseClassNonCopyable.h"
class C3DTriangle2;
class CTriangleStripifierBase : public CBaseClassNonCopyable {
public:
    virtual void GenerateTriangleListAndStrip(const CArray<C3DTriangle2>&, CArray<C3DTriangle2>&, CArray<unsigned short>&) = 0;
};
class CTriangleStripifier2 : public CTriangleStripifierBase {
public:
    virtual void GenerateTriangleListAndStrip(const CArray<C3DTriangle2>&, CArray<C3DTriangle2>&, CArray<unsigned short>&);   // 0x00F3F3A0
    void GenerateTriangleListAndStrip(CArray<unsigned short>& polys, CArray<unsigned short>& list, CArray<unsigned short>& strip);   // 0x00F3EA50
private:
    void FillInPolygons(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<unsigned short>&,const CArray<bool>&,CArray<long>&,CArray<unsigned short>&,bool);
    void GetMeshIslandSubsection(const CArray<unsigned short>&,const CArray<bool>&,const CArray<CArray<long> >&,CArray<unsigned short>&,CArray<long>&);
};
#else // the landed default (unchanged)
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
};
#endif // FABLE_RETAIL_CTRIANGLESTRIPIFIER2
#endif
