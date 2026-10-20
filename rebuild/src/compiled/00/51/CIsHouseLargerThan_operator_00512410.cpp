// CIsHouseLargerThan::operator()(const CThingBuilding*, const CThingBuilding*) -- sort predicate: true when both houses have a sim definition and a's house size (+0x44) is greater than b's
// Leaf: ??R
// Retail: Fable.exe (TLC, VC7.1) @ 0x00512410, 292 bytes (`ret 8`).
// Callees: CThingBuilding::PeekPSimDefinition() 0x0082D2D0 (CDefPointer returned through the hidden pointer, four times);
//   the def's virtual slot 1 (+4) when a temporary drops the last reference.
// Shape: the `a && b` test keeps both PeekPSimDefinition temporaries alive to the end of the full expression, so VC7.1 tracks
//   them with the ebx construction flags (1, 3) and destroys them in reverse order; the second pair of temporaries lives in the
//   dead argument slots. The release is the landed CAIStateGroup_LightHomeCandle model (CRefCounted at +4, inline DecRef +
//   GetRefCount -> `add eax,4 ; dec ; test ; mov [eax]`); `Object = 0` in the destructor is retail's zero store after the first
//   released temporary of each pair (dead-store-eliminated on the second).
struct CRefCounted {                               // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CBuildingSimDefinition {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    CRefCounted Ref;                               // +0x04
    unsigned char _pad08[0x44 - 0x08];
    long HouseSize;                                // +0x44
    __forceinline void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    __forceinline ~CDefPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
    T* operator->() const { return Object; }
};
class CThingBuilding {
public:
    CDefPointer<CBuildingSimDefinition> PeekPSimDefinition() const;   // 0x0082D2D0
};
struct CIsHouseLargerThan {
    bool operator()(const CThingBuilding* a, const CThingBuilding* b) const;
};

bool CIsHouseLargerThan::operator()(const CThingBuilding* a, const CThingBuilding* b) const
{
    if (a->PeekPSimDefinition().Object && b->PeekPSimDefinition().Object)
        return a->PeekPSimDefinition()->HouseSize > b->PeekPSimDefinition()->HouseSize;
    return false;
}