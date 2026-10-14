// CAIStateGroup_BirdFlyToFood_00c484b0::Update(void*, void*, const C3DVector*, void*) -- EH-framed: a 0x40-byte action request built on the stack (type 1, three words zeroed, its intelligent pointer at +0x14 built out of line @ 0x00C167B0 at state 0), the food record viewed from +4 (accessor @ 0x00C47EB0) filled: thing, pointer set out of line (@ 0x00C16970), position assigned memberwise over the zeroed words, target; then the request issued (@ 0x00C47F00) and the pointer destroyed (@ 0x00C167C0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C484B0, oracle length 170 bytes. Built under /EHsc.
// The request's inline constructor stores (type and zeros) are hoisted above the pointer's
// constructor call; the accessors return references into the record (the union-like slots differ
// per request type: 0x00C47EC0 +0x20, 0x00C47ED0 +0x24, 0x00C47EE0 +0x34, 0x00C47EF0 +0x3c). The
// manifest label is propagated; the class is named by address.
struct CBaseIntelligentPointer_00c484b0 {       // +0x14 of the request, 8 bytes
    unsigned char _body[8];
    CBaseIntelligentPointer_00c484b0();        // 0x00C167B0
    ~CBaseIntelligentPointer_00c484b0();       // 0x00C167C0
    void Set(void* item);                      // 0x00C16970
};

struct SPair_00c484b0 { void* m_first; void* m_second; };

struct C3DVector_00c484b0 {
    float x, y, z;
    C3DVector_00c484b0(const C3DVector_00c484b0& other) : x(other.x), y(other.y), z(other.z) { }
};

struct SFoodTarget_00c484b0 {              // the view of the request from +4: thing, position (the zeroed words), pointer, target
    void* m_pThing;                        // +0 (= +4)
    C3DVector_00c484b0 m_position;         // +4 (= +8..+0x10, the zeroed words)
    CBaseIntelligentPointer_00c484b0 m_pointer;   // +0x10 (= +0x14)
    void* m_pTarget;                       // +0x18 (= +0x1c)
};

struct SActionRequest_00c484b0 {               // 0x40-byte request record built on the stack
    long m_type;                           // +0
    long m_4;
    long m_8;
    long m_c;
    long m_10;
    CBaseIntelligentPointer_00c484b0 m_thing;  // +0x14
    long m_1c;
    void* m_target20;                      // +0x20
    void* m_24;
    long m_28, m_2c, m_30;
    SPair_00c484b0 m_pair34;                   // +0x34
    void* m_target3c;                      // +0x3c
    SActionRequest_00c484b0(long type) : m_type(type), m_8(0), m_c(0), m_10(0), m_thing() { }
    SFoodTarget_00c484b0& Food();          // 0x00C47EB0 (`lea eax,[ecx+4]`)
    void*& Target20();                     // 0x00C47EC0 (`lea eax,[ecx+20h]`)
    SPair_00c484b0& Pair34();                  // 0x00C47EE0 (`lea eax,[ecx+34h]`)
    void*& Target3c();                     // 0x00C47EF0 (`lea eax,[ecx+3Ch]`)
};

struct CAIStateGroup_BirdFlyToFood_00c484b0 {
    void Issue(SActionRequest_00c484b0* request);   // 0x00C47F00 (forwards to the member at +0x18)
    void Update(void* thing, void* item, const C3DVector_00c484b0* position, void* target);
};

void CAIStateGroup_BirdFlyToFood_00c484b0::Update(void* thing, void* item, const C3DVector_00c484b0* position, void* target)
{
    SActionRequest_00c484b0 req(1);
    SFoodTarget_00c484b0& food = req.Food();
    food.m_pThing = thing;
    food.m_pointer.Set(item);
    food.m_position = *position;
    food.m_pTarget = target;
    Issue(&req);
}
