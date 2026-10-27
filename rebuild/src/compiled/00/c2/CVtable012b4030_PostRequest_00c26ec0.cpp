// CVtable_012b4030::PostRequest(unsigned char flag, PKCStreamRequest* request) -- record flag in the request (+0x16) and push the request onto the worker queue at +0xC
// Leaf: ?PostRequest@CVtable_012b4030@@UAEXEPAUPKCStreamRequest@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C26EC0, thiscall, `ret 8`.
// Identity (descriptive): slot 0 of the non-RTTI vtable 0x012B4030, stored only by the landed constructor
//   CVtable_012b4030::CVtable_012b4030 0x00C26E90 (an inner link at +4, the worker built over it at +0xC by 0x00C27070,
//   the owner at +0x3C). libpk has no PDB or RTTI, so the class is named by its vtable.
// Callees: the worker queue's push @ 0x00C27130 (EH-framed: under its scoped lock the request is appended to the vector at
//   +0x18 and the semaphore at +0x28 released; not landed).
// Shape: the flag is a byte argument normalised to bool and stored as a bool member, `test al,al ; setne dl ; mov byte ptr [eax+16h],dl`.
struct PKCStreamRequest {
    unsigned char _pad00[0x16];
    bool m_flag;                                 // +0x16
};

struct CWorkerQueue_00c27130 {                  // the worker at +0xC (0x30 bytes)
    unsigned char _body[0x30];
    void Push(PKCStreamRequest* request);               // 0x00C27130
};

class CVtable_012b4030 {                        // vtable 0x012B4030
public:
    virtual void PostRequest(unsigned char flag, PKCStreamRequest* request);   // slot 0
    void*                   m_link[2];          // +0x04 the inner link {vptr, owner}
    CWorkerQueue_00c27130 m_worker;             // +0x0c
    void*                   m_pOwner;           // +0x3c
};

void CVtable_012b4030::PostRequest(unsigned char flag, PKCStreamRequest* request)
{
    request->m_flag = flag != 0;
    m_worker.Push(request);
}