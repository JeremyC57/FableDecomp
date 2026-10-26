// CAnimComponentStrikeResponseAnim::CAnimComponentStrikeResponseAnim() -- root base count set to 1, own vtable (0x01255828), then the ResponseAnimName CCharString at +8 default-constructed (@ 0x0099E4B0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DF900, length 28 bytes.
// Callees: CCharString::CCharString() @ 0x0099E4B0.
// Layout from engine/CAnimComponentStrikeResponseAnim.h (0xc bytes): vptr, +4 unnamed, +8
// ResponseAnimName. The +4 store of 1 precedes the vtable store, so it belongs to an inlined
// base constructor (a counted root whose own vtable store is dead and dropped).
class CCharString { public:
    void* PStringData;
    CCharString();                         // 0x0099E4B0
};

class CAnimComponentBase { public:
    virtual void v00();
    unsigned long m_refCount;              // +4
    CAnimComponentBase() : m_refCount(1) { }
};

class CAnimComponentStrikeResponseAnim : public CAnimComponentBase { public:   // vtable 0x01255828
    CCharString ResponseAnimName;          // +8
    CAnimComponentStrikeResponseAnim();
};

CAnimComponentStrikeResponseAnim::CAnimComponentStrikeResponseAnim()
{
}