// CBaseObject::ClearIntelligentPointers() -- detach the object from its intelligent-pointer block: null the block's object, free the block when no pointer references it, forget it
// Leaf: ?ClearIntelligentPointers@CBaseObject@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A3C0, length 44 bytes.
// Identity: Ego_r.pdb + FableWin.pdb ?ClearIntelligentPointers@CBaseObject@@QAEXXZ next to the CBaseObject constructors;
//   the body is exactly the middle of CBaseObject's deleting destructor 0x0099A4D0 (vtable 0x0129A7D8 slot 0) without
//   the vtable stores. No call reference was found (the destructor inlines its own copy).
// Callees: operator delete @ 0x00BFE9BC (the pointer block).
// Layout: Ego_r.pdb CBaseObject = vptr + IntelligentPointer (+0x04); CBaseObjectPointer = {object +0, refcount +4};
//   modelled locally because engine/CBaseObject.h flattens the vptr into a data word.
// Shape: the member is re-read after the store through it (the store may alias), then the block's count tested.
class CBaseObject;
class CBaseObjectPointer {                             // Ego_r.pdb, 8 bytes
public:
    CBaseObject*  object;                              // +0x00
    unsigned long refcount;                            // +0x04
};

class CBaseObject {                                    // vtable 0x0129A7D8
public:
    void ClearIntelligentPointers();                   // 0x0099A3C0
protected:
    virtual ~CBaseObject();                            // 0x0099A430
public:
    CBaseObjectPointer* IntelligentPointer;            // +0x04
};

void CBaseObject::ClearIntelligentPointers()
{
    if (IntelligentPointer) {
        IntelligentPointer->object = 0;
        if (IntelligentPointer->refcount == 0)
            delete IntelligentPointer;
        IntelligentPointer = 0;
    }
}
