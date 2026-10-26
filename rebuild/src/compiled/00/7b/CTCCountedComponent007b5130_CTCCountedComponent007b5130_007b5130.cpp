// CTCCountedComponent_007b5130::~CTCCountedComponent_007b5130() -- destroy the member at +0x14, release the counted pointer at +0xc, then ~CTCBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B5130, length 43 bytes.
// Callees: the +0x14 member's destructor @ 0x007B4D60 (the manifest label of that address, "CBankFileAsyncData::FreeBuffer", is propagated), the counted
// object's virtual slot 1 (`call [eax+4]`, Delete) when the release at +0xc drops the count to zero, CTCBase::~CTCBase @ 0x00686830 (tail jump).
// The chunk label is propagated; no vtable of this class is stored before the members go (abstract novtable component class), named by what it owns.
struct CCountedObject {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1
    long m_refCount;                            // +4
};
struct CCountedPointer {
    CCountedObject* m_pObject;
    ~CCountedPointer()
    {
        CCountedObject* object = m_pObject;
        if (object != 0) {
            if (--object->m_refCount == 0)
                object->Delete();
            m_pObject = 0;
        }
    }
};
struct CBuffer_007b4d60 {                       // the +0x14 member: destroyed out of line
    unsigned char _body[0x10];
    ~CBuffer_007b4d60();                        // 0x007B4D60
};
class CTCBase { public:
    virtual void v00();
    unsigned char _pad[8];                      // +4..+0xb
    virtual ~CTCBase();                                 // 0x00686830
};

struct __declspec(novtable) CTCCountedComponent_007b5130 : CTCBase {
    CCountedPointer m_pAction;                  // +0xc
    long m_10;
    CBuffer_007b4d60 m_buffer;                  // +0x14
    virtual void v01();
    ~CTCCountedComponent_007b5130();
};

CTCCountedComponent_007b5130::~CTCCountedComponent_007b5130()
{
}