// CTCObstacle::~CTCObstacle() -- this class's vtable (0x0123E28C); when the steering agent at +0xc exists: it is destroyed (~CSteeringAgent @ 0x00A1C950) and freed, the object at +0x10 released through its virtual slot 23 with 1, and both pointers cleared; then the base destructor (0x00686830) as the tail jump
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D95EB, length 62 bytes.
// Leaf: ??1CTCObstacle@@QAE@XZ
// Shape: the size-optimised module (`pop ecx`, `and dword ptr [..],0`), a non-virtual destructor in a polymorphic class, the agent in edi, `jmp` into the base destructor.
#pragma optimize("s", on)
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CSteeringAgent { public:
    unsigned char m_body[4];
    ~CSteeringAgent();                             // 0x00A1C950
};
struct CReleasable_004d95eb {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void Release(long flags);              // slot 23 (+0x5c)
};
struct CTCBase_004d95eb {
    virtual void v00();
    unsigned char _pad04[8];
    ~CTCBase_004d95eb();                           // 0x00686830 (labelled ~CChunkedFileChunk)
};

class CTCObstacle : public CTCBase_004d95eb { public:           // vtable 0x0123E28C
    CSteeringAgent*       m_pAgent;                // +0x0c
    CReleasable_004d95eb* m_pTarget;               // +0x10
    ~CTCObstacle();
};

CTCObstacle::~CTCObstacle()
{
    CSteeringAgent* agent = m_pAgent;                // held in edi across the two calls
    if (agent) {
        agent->~CSteeringAgent();
        operator delete(agent);
        if (m_pTarget)
            m_pTarget->Release(1);
        m_pAgent = 0;
        m_pTarget = 0;
    }
}