// CMeshBaseWithStrip_00baa380::`scalar deleting destructor'(unsigned int) -- destroy the member at +0xac out of line, the CEngineInternalPrimitiveMeshBase member at +0 destroyed out of line, then the flag-tested operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAA380.
// Callees: the member destructor @ 0x00BAA2B0 (CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip), the base destructor @ 0x00BEBC50 (CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase), operator delete @ 0x00BFE9BC. Shape: no vtable store (the class is
// non-polymorphic: the polymorphic object at +0 is a member, so VC7.1 emits the non-virtual deleting destructor); the member at +0xac is destroyed first. The chunk label
// "CEngineSubPrimitiveText `scalar deleting destructor'" is propagated; the class is named by address.
class CPatchTesselationEdgeStrip { public:
    unsigned char _body[0x4];
    ~CPatchTesselationEdgeStrip();                                  // 0x00BAA2B0
};

class CEngineInternalPrimitiveMeshBase { public:                                  // polymorphic base of 0xac bytes
    virtual void v00();
    unsigned char _pad[0xa8];
    virtual ~CEngineInternalPrimitiveMeshBase();                                   // 0x00BEBC50
};

struct CMeshBaseWithStrip_00baa380 {                                   // non-polymorphic: the polymorphic CEngineInternalPrimitiveMeshBase is its FIRST MEMBER, not a base (no vtable store, members destroyed in reverse order)
    CEngineInternalPrimitiveMeshBase m_base;                                 // +0x00
    CPatchTesselationEdgeStrip m_member;                            // +0xac
    ~CMeshBaseWithStrip_00baa380() { }
};

// The delete-expression that makes the compiler emit the (non-virtual) scalar deleting destructor.
void Destroy_00baa380(CMeshBaseWithStrip_00baa380* object)
{
    delete object;
}