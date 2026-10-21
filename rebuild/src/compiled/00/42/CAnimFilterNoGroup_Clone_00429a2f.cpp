// CAnimFilterNoGroup::Clone() const -- a new copy of this (stateless) animation filter
// Leaf: Clone@CAnimFilterNoGroup
// Retail: Fable.exe @ 0x00429A2F, 22 bytes; vtable 0x0122FFDC slot 1 (RTTI CAnimFilterNoGroup : CAnimFilterBase); the object is just
// its vptr (4 bytes), so the inline constructor only stores the vtable. operator new is 0x00BFEA1A.
// Shape: size TU (`pop ecx` after the cdecl operator new instead of `add esp,4`).
#pragma optimize("s", on)
class CAnimFilterBase {
public:
    virtual ~CAnimFilterBase();                          // slot 0
    virtual CAnimFilterBase* Clone() const = 0;          // slot 1
};
class CAnimFilterNoGroup : public CAnimFilterBase {
public:
    virtual CAnimFilterBase* Clone() const;
};

CAnimFilterBase* CAnimFilterNoGroup::Clone() const
{
    return new CAnimFilterNoGroup;
}