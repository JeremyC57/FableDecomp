// CLocalDetailCacheMap::CQuadTreeElement::CQuadTreeElement(CLocalDetailCacheMap*,long,long,long,long) -- initialize a local-detail tree node
// Retail: 0x00BDDF30,119 bytes,thiscall
// Leaf: ??0CQuadTreeElement@CLocalDetailCacheMap@@QAE@PAV1@JJJJ@Z
#define FABLE_RETAIL_CLOCALDETAILCACHEMAP_QUADTREE
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CLocalDetailCacheMap.h"
CLocalDetailCacheMap::CQuadTreeElement::CQuadTreeElement(CLocalDetailCacheMap* map,long mapx,long mapy,long width,long height)
    : Map(map)
{
    Children[0]=0;
    Children[1]=0;
    Children[2]=0;
    Children[3]=0;
    ObjectCacheGroupCollections=0;
    RenderUpdateNeeded=false;
    MapX=mapx;
    MapY=mapy;
    Width=width;
    Height=height;
    RenderUpdateNeeded=false;
    Loading=false;
    StaticContentsLoaded=false;
    HasDynamicAreasFlag=false;
    MaxFadeDistance=0.0f;
    PrimitiveTypeMask=0;
    BoundingSphere.Center.Initialise(0.0f,0.0f,0.0f);
    BoundingSphere.Radius=0.0f;
    FileBlockPos=0;
    FileBlockSize=0;
    OffsetIntoFileBlock=0;
    PrimitiveTypeMask=0;
}