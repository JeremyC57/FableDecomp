// CVtable_012b401c::PostRequest(unsigned char flag, PKCRequest* request) -- record flag in the request (+0x76) and push the request onto the worker queue at +0xC
// Leaf: ?PostRequest@CVtable_012b401c@@UAEXEPAUPKCRequest@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C26A60, thiscall, `ret 8`.
// Identity (descriptive): slot 0 of the non-RTTI vtable 0x012B401C, stored only by the landed constructor
//   CVtable_012b401c::CVtable_012b401c 0x00C26A30 (an inner link at +4, the worker built over it at +0xC by 0x00C26C10,
//   the owner at +0x3C). libpk has no PDB or RTTI, so the class is named by its vtable.
// Callees: the worker queue's push @ 0x00C26CD0 (EH-framed: under its scoped lock the request is appended to the vector at
//   +0x18 and the semaphore at +0x28 released; not landed).
// Shape: the flag is a byte argument normalised to bool and stored as a short member, `xor edx,edx ; test al,al ; setne dl ; mov word ptr [eax+76h],dx`.
struct PKCRequest {
    unsigned char _pad00[0x76];
    short m_flag;                                 // +0x76
};

struct CWorkerQueue_00c26cd0 {                  // the worker at +0xC (0x30 bytes)
    unsigned char _body[0x30];
    void Push(PKCRequest* request);               // 0x00C26CD0
};

class CVtable_012b401c {                        // vtable 0x012B401C
public:
    virtual void PostRequest(unsigned char flag, PKCRequest* request);   // slot 0
    void*                   m_link[2];          // +0x04 the inner link {vptr, owner}
    CWorkerQueue_00c26cd0 m_worker;             // +0x0c
    void*                   m_pOwner;           // +0x3c
};

void CVtable_012b401c::PostRequest(unsigned char flag, PKCRequest* request)
{
    request->m_flag = flag != 0;
    m_worker.Push(request);
}