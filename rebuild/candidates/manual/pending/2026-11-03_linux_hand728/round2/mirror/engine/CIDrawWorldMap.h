#ifndef FABLE_ENGINE_CIDRAWWORLDMAP_H
#define FABLE_ENGINE_CIDRAWWORLDMAP_H
#include "rebuild_abi.h"
class CDrawMapInfo;class C2DBoxI;class C3DVector;
// Ego_r TPI0x27a42, original secondary interface, size4.
class CIDrawWorldMap {
public: virtual long DrawGetNoMaps() const =0;
public: virtual CDrawMapInfo DrawGetMapInfo(long) const =0;
public: virtual C2DBoxI DrawGetDimensions() const =0;
public: virtual long DrawGetGranularity() const =0;
public: virtual bool DrawIsPosInMap(const C3DVector &) const =0;
public: virtual float DrawGetSizeZAt(const C3DVector &,float) const =0;
public: virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long,const C3DVector &) const =0;
public: virtual long DrawGetMyPlayerMapNumber() const =0;
public: virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector &,const C3DVector &,C3DVector &) const =0;
public: virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector &,const C3DVector &,C3DVector &) const =0;
public: virtual long DrawGetFrame() const =0;
public: virtual ~CIDrawWorldMap();
};
FABLE_STATIC_ASSERT(sizeof(CIDrawWorldMap)==4);
#endif
