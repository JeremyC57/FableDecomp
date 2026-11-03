#ifndef FABLE_ENGINE_CRENDERMANAGER_H
#define FABLE_ENGINE_CRENDERMANAGER_H
#include "engine/CRenderManager2D.h"
// Layout: original sole base, no additional members; native size3ea4.
class CRenderManager:public CRenderManager2D {public:CRenderManager(CDisplayManager&,const CWideString&,unsigned long);virtual ~CRenderManager();};
FABLE_STATIC_ASSERT(sizeof(CRenderManager)==0x3ea4);
#endif
