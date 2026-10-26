// CBaseIntelligentPointer::`scalar deleting destructor'(unsigned int) -- ~CBaseIntelligentPointer then optional operator delete
// Leaf: ??_GCBaseIntelligentPointer@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01C50, oracle length 70 bytes.
// Callees: operator delete @ 0x00BFE9BC (__cdecl, `add esp,4`) twice: the record in the inlined
// ClearData (0x00A01B20) and the object itself when bit 0 of the flags argument is set.
// Vtable slot 0 of 0x0129C95C (rtti-vtables.tsv): the compiler-generated `??_G` of a class with a
// virtual destructor (Ego_r.pdb ??_GCBaseIntelligentPointer@@UAEPAXI@Z); the destructor body (0x00A01C10) is inlined into it.
class CBaseObject;
class CBaseObjectPointer {                             // Ego_r.pdb, 8 bytes
public:
    CBaseObject*  object;                              // +0x00 the observed object, 0 once it died
    unsigned long refcount;                            // +0x04 intelligent pointers sharing this record
};
class CBaseIntelligentPointer {                        // vtable 0x0129C95C
public:
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10; `??_G` is 0x00A01C50
private:
    void ClearData() const                             // 0x00A01B20 (private const in Ego_r), inlined here
    {
        if (PData != 0) {
            PData->refcount--;
            if (PData->refcount == 0 && PData->object == 0)
                delete PData;
            PData = 0;
        }
    }
    mutable CBaseObjectPointer* PData;                 // +0x04
};

CBaseIntelligentPointer::~CBaseIntelligentPointer()
{
    ClearData();
}

void DestroyIntelligentPointer(CBaseIntelligentPointer* p)
{
    delete p;
}
