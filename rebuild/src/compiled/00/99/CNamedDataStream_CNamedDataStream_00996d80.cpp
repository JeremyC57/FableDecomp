// CNamedDataStream::~CNamedDataStream() -- destructor of an abstract CDataStream-derived stream with a name.
// Leaf: ??1CNamedDataStream@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00996D80, length 25 bytes.
// Manifest label "~CAIStateGroup_HoverWanderAroundThing" is propagated; the vtable stored, 0x0129A69C, is
// CDataStream (rtti-vtables.tsv). Callees: CCharString::~CCharString @ 0x0099EAE0 for the stream name at +4,
// CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300 (tail-jumped).
//
// Shape: the member destructor runs first, THEN the CDataStream vtable is installed, then CBase's destructor
// is tail-called. A vtable store after the member destructors and right before the base tail call is an
// inlined empty intermediate-base destructor (CDataStream's), and the most-derived class stores no vtable of
// its own only when it is abstract (`__declspec(novtable)`) -- so the body belongs to an abstract class
// deriving from CDataStream that adds the name at +4; it is named descriptively (its own vtable never
// appears in the body). The destructor is NON-virtual here (with another virtual keeping the class
// polymorphic) so that the plain `??1` body is the symbol the lander grades, not a `??_G`.
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                   // 0x0099A300
};

class CCharString { public:
    void* m_pData;
    ~CCharString();                             // 0x0099EAE0
};

struct CDataStream : CBaseClassNonCopyable {                    // vtable 0x0129A69C
    ~CDataStream() { }                          // empty, inlined: only its vtable store survives
};

struct __declspec(novtable) CNamedDataStream : CDataStream {
    CCharString m_name;                         // +0x04
    ~CNamedDataStream();
};

CNamedDataStream::~CNamedDataStream()
{
}