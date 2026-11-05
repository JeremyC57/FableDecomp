// CMiniMapDisplay::RemoveAllDeadMarkers  0x0082a2a0  __fastcall(void)
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Circular doubly-linked list of marker nodes: node+0=next, node+4=prev,
// node+8 = embedded marker object.

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct Marker;               // embedded at node+8
struct Node { Node* next; Node* prev; };  // followed by Marker at +8

struct QueryResult { char pad[0x91]; unsigned char flags; };

// 0x00a01b50 : Marker::GetSomething(this=ecx) -> QueryResult*
// 0x00a01c10 : Marker::~/cleanup(this=ecx)
// 0x00bfea14 : free(void*)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class CMiniMapDisplay { public:
    char pad[0x20];
    Node* head;   // +0x20 : sentinel node pointer
};

void __fastcall CMiniMapDisplay_RemoveAllDeadMarkers(CMiniMapDisplay* self)
{
    Node* head = self->head;
    Node* n = head->next;
    while (n != self->head)
    {
        Marker* m = (Marker*)((char*)n + 8);
        QueryResult* q = ((QueryResult*)(((CBaseIntelligentPointer*)(m))->GetPItem()));
        if (q != 0 && (q->flags & 1) == 0)
        {
            n = n->next;
        }
        else
        {
            Node* nx = n->next;
            Node* pv = n->prev;
            pv->next = nx;
            nx->prev = pv;
            ((CBaseIntelligentPointer*)(m))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
            free(n);
            n = nx;
        }
    }
}