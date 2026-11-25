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
// Naming (NF451, 2026-11-25; r450b done through the shared header): this record derives from NPlayerGui::CStatBase
//   (engine/NPlayerGui_CStatBase.h, add-only no-containers view: +0x00..+0x30, the vptr and the scalar members), so the implicit
//   base-destructor call names 0x00442280 ??1CStatBase@NPlayerGui@@QAE@XZ (FableWin.pdb only, r449c best guess); was ??1CBase_00443220@@QAE@XZ.
//   The derived model pads +0x30..+0x64 itself (the base's two STLport vectors +0x30/+0x3C, left out of the view, and the
//   derived scalar members).
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_GUIMODE
#define FABLE_RETAIL_NPLAYERGUI_CSTATBASE_NO_CONTAINERS
#include "engine/NPlayerGui_CStatBase.h"                // ~CStatBase 0x00442280 (FableWin.pdb ??1CStatBase@NPlayerGui@@QAE@XZ)
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

struct __declspec(novtable) CDerived_00443220 : NPlayerGui::CStatBase {
    unsigned char _body[0x64 - 0x30];          // +0x30 (base vectors, derived scalars)
    CStringRecordList m_records;               // +0x64
    CPodList          m_entries;               // +0x68
    ~CDerived_00443220();
};

CDerived_00443220::~CDerived_00443220()
{
}