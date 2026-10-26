// CAnimationInterpolatorOwner_00708a30::~CAnimationInterpolatorOwner_00708a30() -- implicit destructor of a four-member aggregate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00708A30, length 58 bytes.
// Callees: the counted object's virtual slot 1 (`call [eax+4]`, Delete) when the release at +0x80
// drops the count to zero, C3DAnimationInterpolator::~C3DAnimationInterpolator @ 0x004C2E90
// (+0x6c), CBaseClass::~CBaseClass @ 0x0099A2E0 twice (the CBaseClass members at +0x38 and +0x1c,
// the second as the tail jump).
// The manifest's "~CAIStateGroup_SummonerCharge" label is propagated. The function stores no
// vtable and ends in a member destructor, so the class is a non-polymorphic aggregate whose
// implicit destructor is declared and defined out of line (the TU's largest symbol); members are
// destroyed in reverse declaration order. The class is named by what it owns.
struct CCountedObject {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]`
    long m_refCount;                            // +0x04
};

struct CCountedPointer {
    CCountedObject* m_pObject;                  // +0x00
    ~CCountedPointer()
    {
        CCountedObject* object = m_pObject;     // named: retail never re-reads the member
        if (object != 0) {
            if (--object->m_refCount == 0)
                object->Delete();
            m_pObject = 0;
        }
    }
};

class CBaseClass { public:
    virtual void v00();
    virtual ~CBaseClass();                              // 0x0099A2E0
};

class C3DAnimationInterpolator { public:
    unsigned char _body[0x14];
    ~C3DAnimationInterpolator();                // 0x004C2E90
};

struct CAnimationInterpolatorOwner_00708a30 {
    unsigned char _pad0[0x1c];                  // +0x00..+0x1b
    CBaseClass m_first;                         // +0x1c
    unsigned char _pad1[0x18];                  // +0x20..+0x37
    CBaseClass m_second;                        // +0x38
    unsigned char _pad2[0x30];                  // +0x3c..+0x6b
    C3DAnimationInterpolator m_interpolator;    // +0x6c
    CCountedPointer m_pAnimation;               // +0x80
    ~CAnimationInterpolatorOwner_00708a30();
};

CAnimationInterpolatorOwner_00708a30::~CAnimationInterpolatorOwner_00708a30()
{
}