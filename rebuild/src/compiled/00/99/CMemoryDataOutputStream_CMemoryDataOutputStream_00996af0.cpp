// CMemoryDataOutputStream::CMemoryDataOutputStream() -- the CDataOutputStream base inline (CBaseClassNonCopyable @ 0x0099A2F0, its vtable 0x0129A6E0, its line terminator CCharString at +4 built from "\r\n" @ 0x012397F8 with CCharString(const char*, long) @ 0x0099EBF0), this class's vtable (0x0129A788), then the four dwords +8..+0x14 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00996AF0, length 64 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CCharString::CCharString(const char*, long) @ 0x0099EBF0. Shape: `this` in esi, the base vtable stored before the string constructor call, one zero register, `ret`.
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
};
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length);    // 0x0099EBF0
};

class CDataOutputStream : public CBaseClassNonCopyable { public:   // vtable 0x0129A6E0
    virtual void v01();
    CCharString m_lineTerminator;                  // +0x04
    CDataOutputStream() : CBaseClassNonCopyable(), m_lineTerminator("\r\n", -1) { }
};

struct SMemoryOutputState_00996af0 {              // +8..+0x14: an embedded object with an inline constructor (its stores follow the vtable store)
    unsigned char* m_pFirst;                       // +0x08
    unsigned char* m_pLast;                        // +0x0c
    long           m_10;                           // +0x10
    unsigned long  m_position;                     // +0x14
    SMemoryOutputState_00996af0() : m_pFirst(0), m_pLast(0), m_10(0), m_position(0) { }
};

class CMemoryDataOutputStream : public CDataOutputStream { public:   // vtable 0x0129A788
    virtual void v01();
    SMemoryOutputState_00996af0 m_state;           // +0x08
    CMemoryDataOutputStream();
};

CMemoryDataOutputStream::CMemoryDataOutputStream()
    : CDataOutputStream(), m_state()
{
}