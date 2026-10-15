// CBaseObject::ScalarDeletingDestructor(unsigned int flags) -- deleting destructor: detach the object from its intelligent-pointer record (free the record when no pointer still references it), fall into CBaseClass, free the object when bit 0 is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A4D0, length 76 bytes (`ret 4`, eax = this). Callee: operator delete
// @ 0x00BFE9BC (the record, then the object).
// The manifest label (CObjectTypeCollection::`vector_deleting_destructor') is propagated and wrong: the body stores
// vtable 0x0129A7D8 = CBaseObject (slot 0 of that vtable is this address) and then 0x01231710 = CBaseClass
// (rtti-vtables.tsv). Layout from the PDB: CBaseObject = vptr + IntelligentPointer (CBaseObjectPointer*) at +4;
// CBaseObjectPointer = {CBaseObject* object, unsigned long refcount}. engine/CBaseObject.h models the vptr as a plain
// `__vftable` word, so the polymorphic class is modelled locally (as the landed copy constructor @ 0x0099A390 does).
// Shape: the `??_G` of a class whose inline destructor is `if (IntelligentPointer) { IntelligentPointer->object = 0;
// if (IntelligentPointer->refcount == 0) delete IntelligentPointer; IntelligentPointer = 0; }` -- the member is
// re-read after the store through it, the record (no destructor) is freed without a null test; the CBaseClass
// destructor is inline and empty (its vtable store only); `__forceinline` on the virtual destructor is what inlines it
// into the `??_G` (plain inline leaves a `call ??1`); the `new CBaseObject` factory pulls the vtable and the
// deleting destructor into the translation unit.
// Leaf: ??_GCBaseObject
class CBaseObject;

struct CBaseObjectPointer {
    CBaseObject*  object;                          // +0x00
    unsigned long refcount;                        // +0x04
};

class CBaseClass {                                 // vtable 0x01231710
public:
    virtual ~CBaseClass() {}
};

class CBaseObject : public CBaseClass {            // vtable 0x0129A7D8
public:
    CBaseObjectPointer* IntelligentPointer;        // +0x04

    CBaseObject() : IntelligentPointer(0) {}
    __forceinline virtual ~CBaseObject()
    {
        if (IntelligentPointer) {
            IntelligentPointer->object = 0;
            if (IntelligentPointer->refcount == 0)
                delete IntelligentPointer;
            IntelligentPointer = 0;
        }
    }
};

CBaseObject* MakeBaseObject()
{
    return new CBaseObject;
}