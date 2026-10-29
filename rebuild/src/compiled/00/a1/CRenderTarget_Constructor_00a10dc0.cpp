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

    __forceinline CSurface()
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
        const CSurface& primary,
        const CSurface& secondary);

    virtual void VTableAnchor();
};

CRenderTarget::CRenderTarget(
    const CSurface& primary,
    const CSurface& secondary)
{
    primary04 = primary;
    secondary14 = secondary;
}
