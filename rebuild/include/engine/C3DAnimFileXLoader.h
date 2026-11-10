#ifndef FABLE_ENGINE_C3DANIMFILEXLOADER_H
#define FABLE_ENGINE_C3DANIMFILEXLOADER_H
// Retail opt-in (Codex pass19): original Ego_r/FableWin types and donor calls; native animation rows 00a24d70/00a26ad0. See NAMING_EVIDENCE.md.
#include "engine/C3DAnimFileLoader.h"
class C3DAnimFileXLoader:public C3DAnimFileLoader {
public:C3DAnimFileXLoader(const C3DAnimFileXLoader&);C3DAnimFileXLoader();virtual ~C3DAnimFileXLoader();C3DAnimFileXLoader& operator=(const C3DAnimFileXLoader&);
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileXLoader)==0x5c);
#endif
