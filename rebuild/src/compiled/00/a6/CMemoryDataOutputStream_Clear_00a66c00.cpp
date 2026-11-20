// CMemoryDataOutputStream::Clear  @ 00a66c00
// push esi; mov esi,ecx; lea ecx,[esi+0x340c]; call 0x9e4710
// mov [esi+0x3404],0; pop esi; ret
// NF136 (2026-11-20, q3887): the stand-in member struct SubBuffer (whose Clear() bound 0x009E4710 as ?Clear@SubBuffer@@QAEXXZ) replaced in place by
//   the retail member type std::list<CMaintainedEvent> (the MaintainedEventsList of CKeyboard / CMouse / CJoystick, Ego_r.pdb layouts) from
//   stlport/fable_containers.h with FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE: the call now binds STLport's _List_base<CMaintainedEvent>::clear,
//   ?clear@?$_List_base@VCMaintainedEvent@@V?$allocator@VCMaintainedEvent@@@std@@@std@@QAEXXZ (the landed 0x00A66990 / 0x00A66F80 spelling).
//   The row's own (propagated) name is unchanged: see NF136 NOTES.

#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "engine/CMaintainedEvent.h"
#include "stlport/fable_containers.h"

class CMemoryDataOutputStream { public:
    char     pad0[0x3404];   // 0..0x3403
    unsigned WritePos;       // 0x3404
    char     pad1[0x340c - 0x3408];
    std::list<CMaintainedEvent> sub;           // 0x340c
    void Clear();
};

void CMemoryDataOutputStream::Clear()
{
    this->sub.clear();
    this->WritePos = 0;
}