// CAIStateGroup_BirdFlyToFood_00c48430::Update(void*) -- EH-framed: a 0x40-byte action request built on the stack (type 5, three words zeroed, its intelligent pointer at +0x14 built out of line @ 0x00C167B0 at state 0), the slot at +0x3c filled through the out-of-line accessor (@ 0x00C47EF0), then the request issued through the group's forwarder (@ 0x00C47F00, which retargets to the member at +0x18) and the pointer destroyed (@ 0x00C167C0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C48430, oracle length 126 bytes. Built under /EHsc.
// The request's inline constructor stores (type and zeros) are hoisted above the pointer's
// constructor call; the accessors return references into the record (the union-like slots differ
// per request type: 0x00C47EC0 +0x20, 0x00C47ED0 +0x24, 0x00C47EE0 +0x34, 0x00C47EF0 +0x3c). The
// manifest label is propagated; the class is named by address.
struct CBaseIntelligentPointer_00c48430 {       // +0x14 of the request, 8 bytes
    unsigned char _body[8];
    CBaseIntelligentPointer_00c48430();        // 0x00C167B0
    ~CBaseIntelligentPointer_00c48430();       // 0x00C167C0
};

struct SPair_00c48430 { void* m_first; void* m_second; };

struct SActionRequest_00c48430 {               // 0x40-byte request record built on the stack
    long m_type;                           // +0
    long m_4;
    long m_8;
    long m_c;
    long m_10;
    CBaseIntelligentPointer_00c48430 m_thing;  // +0x14
    long m_1c;
    void* m_target20;                      // +0x20
    void* m_24;
    long m_28, m_2c, m_30;
    SPair_00c48430 m_pair34;                   // +0x34
    void* m_target3c;                      // +0x3c
    SActionRequest_00c48430(long type) : m_type(type), m_8(0), m_c(0), m_10(0), m_thing() { }
    void*& Target20();                     // 0x00C47EC0 (`lea eax,[ecx+20h]`)
    SPair_00c48430& Pair34();                  // 0x00C47EE0 (`lea eax,[ecx+34h]`)
    void*& Target3c();                     // 0x00C47EF0 (`lea eax,[ecx+3Ch]`)
};

struct CAIStateGroup_BirdFlyToFood_00c48430 {
    void Issue(SActionRequest_00c48430* request);   // 0x00C47F00 (forwards to the member at +0x18)
    void Update(void* target);
};

void CAIStateGroup_BirdFlyToFood_00c48430::Update(void* target)
{
    SActionRequest_00c48430 req(5);
    req.Target3c() = target;
    Issue(&req);
}