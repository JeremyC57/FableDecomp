#ifndef FABLE_ENGINE_C3DMESHFILEXLOADER_H
#define FABLE_ENGINE_C3DMESHFILEXLOADER_H
// New header (chunk 725b): C3DMeshFileXLoader : C3DMeshFileLoader (rtti_bases.tsv), vtable 0x0129E450 (slot 0 the scalar
// deleting dtor 0x00AA7440). No members of its own (Ego_r sizeof 0xC0, as the base). Constructor ??0C3DMeshFileXLoader@@QAE@XZ
// 0x00AA5C80 registers the extended chunk prototypes; destructor ??1C3DMeshFileXLoader@@UAE@XZ (both PDBs).
#include "rebuild_abi.h"
#include "engine/C3DMeshFileLoader.h"
class C3DMeshFileXLoader : public C3DMeshFileLoader {
public:
    C3DMeshFileXLoader();
    virtual ~C3DMeshFileXLoader();
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileXLoader) == 0xC0);
#endif
