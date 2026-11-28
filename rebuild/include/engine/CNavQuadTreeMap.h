#ifndef FABLE_ENGINE_CNAVQUADTREEMAP_H
#define FABLE_ENGINE_CNAVQUADTREEMAP_H
#include "engine/CCountedPointer.h"
#include "engine/C2DBoxI.h"
#include "fable_string.h"
class CNavQuadTree;
class CNavQuadTreeMap {
public:
    CCountedPointer<CNavQuadTree> PQuadTree;
    long Index;
    C2DBoxI MapArea;
    CCharString SectionName;
};
FABLE_STATIC_ASSERT_AT(1160,sizeof(CNavQuadTreeMap)==0x20);
#endif
