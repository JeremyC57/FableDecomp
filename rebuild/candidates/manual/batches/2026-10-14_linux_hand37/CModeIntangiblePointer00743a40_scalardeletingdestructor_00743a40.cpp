// CModeIntangiblePointer_00743a40::`scalar deleting destructor'(unsigned int) -- destroy the member at +0x98 out of line, the CModeIntangible member at +0 destroyed out of line, then the flag-tested operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00743A40.
// Callees: the member destructor @ 0x00A01C10 (CBaseIntelligentPointer::~CBaseIntelligentPointer), the base destructor @ 0x00743000 (CModeIntangible::~CModeIntangible), operator delete @ 0x00BFE9BC. Shape: no vtable store (the class is
// non-polymorphic: the polymorphic object at +0 is a member, so VC7.1 emits the non-virtual deleting destructor); the member at +0x98 is destroyed first. The chunk label
// "CEngineSubPrimitiveText `scalar deleting destructor'" is propagated; the class is named by address.
struct CBaseIntelligentPointer {
    unsigned char _body[0x8];
    ~CBaseIntelligentPointer();                                  // 0x00A01C10
};

struct CModeIntangible {                                  // polymorphic base of 0x98 bytes
    virtual void v00();
    unsigned char _pad[0x94];
    ~CModeIntangible();                                   // 0x00743000
};

struct CModeIntangiblePointer_00743a40 {                                   // non-polymorphic: the polymorphic CModeIntangible is its FIRST MEMBER, not a base (no vtable store, members destroyed in reverse order)
    CModeIntangible m_base;                                 // +0x00
    CBaseIntelligentPointer m_member;                            // +0x98
    ~CModeIntangiblePointer_00743a40() { }
};

// The delete-expression that makes the compiler emit the (non-virtual) scalar deleting destructor.
void Destroy_00743a40(CModeIntangiblePointer_00743a40* object)
{
    delete object;
}
