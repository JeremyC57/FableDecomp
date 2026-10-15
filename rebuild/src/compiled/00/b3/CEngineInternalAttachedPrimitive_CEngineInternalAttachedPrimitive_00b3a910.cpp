// CEngineInternalAttachedPrimitive::CEngineInternalAttachedPrimitive() -- constructor: primitive base of type 0x2e, this vtable, empty parent / output primitive handles, attachment info with no flags and empty dummy names
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3A910, length 108 bytes (+ int3 padding).
// Callees: CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type, long flags) @ 0x00B8FF00 (type 0x2e, 0), CBaseClass::CBaseClass()
// @ 0x0099A2D0 (twice, the handles' root), CCharString::CCharString() @ 0x0099E4B0 (twice, the dummy names).
// Vtables: 0x012A1A04 CEngineInternalAttachedPrimitive, 0x0122F598 CEnginePrimitiveHandle (rtti-vtables.tsv).
// Layout = Ego_r.pdb CEngineInternalAttachedPrimitive: ParentHandle +0x80, ParentGroupIndex +0x88 (left unset), OutputPrimitive +0x8c,
// AttachmentInfo (CEnginePrimitiveAttachmentInfo: Flags, ParentDummyName, ParentDummyIndex, ChildDummyName, ChildDummyIndex) +0x94,
// ChildInverseTransform +0xa8, ChildInverseTransformValid +0xd8 (left unset).
// Shape: base call, the first handle's address formed in edi before this class's vtable store, each handle = root constructor + its
// vtable + the null primitive (the landed CEngineInternalAttachedPrimitiveInfo constructor 0x00B3AAE0 model), then the attachment info's
// inline constructor: Flags = 0 (stored before the first name's call) and the two string constructors.
// Leaf: ??0CEngineInternalAttachedPrimitive
struct CBaseClass {
    virtual ~CBaseClass();
    CBaseClass();                            // 0x0099A2D0
};

struct CEnginePrimitiveHandle : CBaseClass {
    virtual void Release();                  // own vtable 0x0122F598
    void* m_pPrimitive;                      // +0x04
    CEnginePrimitiveHandle() : CBaseClass(), m_pPrimitive(0) {}
};

class CCharString {
public:
    CCharString();                           // 0x0099E4B0
    struct CCharStringData* PStringData;
};

struct CEnginePrimitiveAttachmentInfo {
    unsigned long Flags;                     // +0x00
    CCharString   ParentDummyName;           // +0x04
    long          ParentDummyIndex;          // +0x08
    CCharString   ChildDummyName;            // +0x0c
    long          ChildDummyIndex;           // +0x10
    CEnginePrimitiveAttachmentInfo() : Flags(0) {}
};

struct CMatrix3x4 {
    float E[12];
};

struct CEngineInternalPrimitiveBase {
    virtual ~CEngineInternalPrimitiveBase();
    CEngineInternalPrimitiveBase(long type, long flags);   // 0x00B8FF00
    unsigned char _body[0x80 - 4];
};

struct CEngineInternalAttachedPrimitive : CEngineInternalPrimitiveBase {   // vtable 0x012A1A04
    CEnginePrimitiveHandle         ParentHandle;                // +0x80
    long                           ParentGroupIndex;            // +0x88
    CEnginePrimitiveHandle         OutputPrimitive;             // +0x8c
    CEnginePrimitiveAttachmentInfo AttachmentInfo;              // +0x94
    CMatrix3x4                     ChildInverseTransform;       // +0xa8
    bool                           ChildInverseTransformValid;  // +0xd8
    virtual void Release();
    CEngineInternalAttachedPrimitive();
};

CEngineInternalAttachedPrimitive::CEngineInternalAttachedPrimitive()
    : CEngineInternalPrimitiveBase(0x2e, 0)
{
}