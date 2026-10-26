// CBaseObject::~CBaseObject() -- detach the intelligent-pointer block at +4 (clear its object, free it when no pointer still references it, null the member), then the inline CBaseClass destructor
// Leaf: ??1CBaseObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A430, length 56 bytes.
// Catalog label "CPhysicsMeshInfo::~CPhysicsMeshInfo" is propagated: the vtables stored here are
// CBaseObject (0x0129A7D8) and its base CBaseClass (0x01231710) per rtti-vtables.tsv.
// Callees: operator delete @ 0x00BFE9BC (`add esp,4`).
// Shape: members from engine/CBaseObject.h (IntelligentPointer at +4; CBaseObjectPointer = {object,
// refcount}), modelled locally because the class is polymorphic (the generated header flattens the
// vptr). The member is read through `this` at every use: the `object = 0` store through the block
// pointer may alias it, so the refcount test reloads `[esi+4]` (`mov eax,[esi+4]` after the store).
// The block is a plain record, so `delete` is a direct operator delete call.
class CBaseObject;
struct CBaseObjectPointer {
    CBaseObject*  object;                          // +0x00
    unsigned long refcount;                        // +0x04
};

class CBaseClass { public:                                // vtable 0x01231710
    virtual void v00();
    ~CBaseClass() { }
};

class CBaseObject : public CBaseClass { public:                  // vtable 0x0129A7D8
    CBaseObjectPointer* IntelligentPointer;        // +0x04
    virtual void v00();
    ~CBaseObject();
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