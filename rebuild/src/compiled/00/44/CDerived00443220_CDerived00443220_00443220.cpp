// CDerived_00443220::~CDerived_00443220() -- destroy the POD list at +0x68 (out-of-line list destructor), then the record list at +0x64 (its inline destructor: clear() then free the head node), then the base destructor as the tail jump
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00443220, length 46 bytes.
// Leaf: ??1CDerived_00443220
// Callees: the POD list's destructor @ 0x0043FD80 (free every node, relink and free the head), the record list's clear()
// @ 0x004407B0 (each node's CWideString at node+0x18 destroyed, node freed, head relinked), free @ 0x00BFEA14, the base
// destructor @ 0x00442280 (destroys two vectors of 8-byte polymorphic records at +0x30 / +0x3c through slot 0 with 0).
// The chunk label "CTCAssassinRush::FinishMove" is propagated over several unrelated bodies (0x00443190..0x00443220) and
// rejected: no caller or address reference to this body exists in the image, so its class is named by address.
// Shape: no vtable store of its own, so the most-derived class is `__declspec(novtable)` with an empty destructor; the
// members die in reverse order. The +0x64 list's destructor is inline (`clear(); if (m_head) free(m_head);` -- the head
// null test and free inline after the out-of-line clear), the +0x68 list's is out of line; the base destructor is a `jmp`.
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct SListNode;

struct CStringRecordList {                     // VC6-era std::list whose values hold a CWideString at +0x10
    SListNode* m_head;
    void clear();                              // 0x004407B0
    ~CStringRecordList() { clear(); if (m_head) free(m_head); }
};

namespace N_0043fd80 { struct CPodList {                              // VC6-era std::list of PODs, destructor out of line
    SListNode* m_head;
    ~CPodList();                               // 0x0043FD80
}; } using N_0043fd80::CPodList;

struct CBase_00443220 {
    virtual void v00();
    unsigned char _body[0x60];                 // +0x04..+0x64
    ~CBase_00443220();                         // 0x00442280
};

struct __declspec(novtable) CDerived_00443220 : CBase_00443220 {
    CStringRecordList m_records;               // +0x64
    CPodList          m_entries;               // +0x68
    ~CDerived_00443220();
};

CDerived_00443220::~CDerived_00443220()
{
}