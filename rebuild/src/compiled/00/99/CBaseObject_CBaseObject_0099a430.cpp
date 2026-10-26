// CBaseObject::~CBaseObject() -- detach the intelligent-pointer block at +4 (clear its object, free it when no pointer still references it, null the member), then the inline CBaseClass destructor
// Leaf: ??1CBaseObject@@MAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A430, length 56 bytes.
// Identity: the vtables stored are CBaseObject (0x0129A7D8) and its base CBaseClass (0x01231710) per rtti-vtables.tsv;
//   Ego_r.pdb (0x00B02090) and FableWin.pdb name the protected virtual destructor ??1CBaseObject@@MAE@XZ.
// Callees: operator delete @ 0x00BFE9BC (`add esp,4`).
// Shape: CBaseObjectPointer = {object, refcount} (Ego_r.pdb); the class is polymorphic, so it is modelled locally (the
// generated header flattens the vptr). The member is read through `this` at every use: the `object = 0` store through
// the block pointer may alias it, so the refcount test reloads `[esi+4]`. CBaseClass's virtual destructor is inline
// and empty (its vtable store only; out of line at 0x0099A2E0).
class CBaseObject;
class CBaseObjectPointer {                             // Ego_r.pdb, 8 bytes
public:
    CBaseObject*  object;                              // +0x00
    unsigned long refcount;                            // +0x04
};
class CBaseClass {                                     // vtable 0x01231710
public:
    virtual ~CBaseClass() { }                          // 0x0099A2E0
};
class CBaseObject : public CBaseClass {                // vtable 0x0129A7D8
protected:
    virtual ~CBaseObject();                            // 0x0099A430
public:
    CBaseObjectPointer* IntelligentPointer;            // +0x04
};

CBaseObject::~CBaseObject()
{
    if (IntelligentPointer) {
        IntelligentPointer->object = 0;
        if (IntelligentPointer->refcount == 0)
            delete IntelligentPointer;
        IntelligentPointer = 0;
    }
}
