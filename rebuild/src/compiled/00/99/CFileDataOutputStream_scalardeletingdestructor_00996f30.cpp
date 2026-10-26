// CFileDataOutputStream::`scalar deleting destructor'(unsigned int) -- stores the CFileDataOutputStream vtable, destroys the string at +4, the inlined CDataStream destructor stores its vtable, then CBase::~CBase and a free on bit 0
// Leaf: ??_GCFileDataOutputStream
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00996F30, length 52 bytes (vtable 0x0129A6F8 slot 0).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (manifest "_Cons_val<...>", propagated), CBase::~CBase
// @ 0x0099A300, operator delete @ 0x00BFE9BC.
// The catalog label "CThingTrackNode::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the CFileDataOutputStream vtable 0x0129A6F8.
// Layout: engine/CFileDataOutputStream.h is a flat struct whose +4 word is a "retail-only long"; the
// destructor shows that word is a CCharString (the stream's file name), so the class is modelled
// locally over the same offsets (vptr, name +4, File +8).
// Shape: two vtable stores = the derived store, then the inline (empty) destructor of the polymorphic
// intermediate CDataStream (vtable 0x0129A69C) after the member destructor, then the root base's
// out-of-line destructor.
class CAFile;

class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};

namespace N_0099a300 { struct CBase {                                     // root polymorphic base
    CBase();
    virtual ~CBase();                              // 0x0099A300
}; } using N_0099a300::CBase;

struct CDataStream : CBase {                       // vtable 0x0129A69C
    CDataStream() { }
    virtual ~CDataStream() { }
};

struct CFileDataOutputStream : CDataStream {       // vtable 0x0129A6F8
    CCharString m_fileName;                        // +0x04
    CAFile*     File;                              // +0x08
    CFileDataOutputStream() : File(0) { }
    __forceinline virtual ~CFileDataOutputStream() { }
};

CFileDataOutputStream* Create_CFileDataOutputStream()
{
    return new CFileDataOutputStream;
}