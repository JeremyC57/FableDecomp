// Leaf: ??0CRenderTarget@@QAE@AAVCSurface@@0@Z
#include "fable_compiler.h"
class CBaseClass {
public:
    CBaseClass();
    virtual void VTableAnchor();
};

class CSurface
{ public:
    void* resource04;
    unsigned long resourceState08;
    unsigned long PAllocatedMemory;

    FABLE_FORCEINLINE CSurface()
        : resource04(0),
          resourceState08(0)
    {
    }

    virtual void VTableAnchor();
    CSurface& operator=(const CSurface& source);
};

class CRenderTarget : public CBaseClass
{ public:
    CSurface primary04;
    CSurface secondary14;

    CRenderTarget(
        CSurface& primary,
        CSurface& secondary);

    virtual void VTableAnchor();
};

CRenderTarget::CRenderTarget(
    CSurface& primary,
    CSurface& secondary)
{
    primary04 = primary;
    secondary14 = secondary;
}
