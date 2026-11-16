// CTCCreatureExpression::CTCCreatureExpression(CThing&) -- CTCBase(owner), own vtable, an empty expression-state list, not blinking
// Leaf: ??0CTCCreatureExpression@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCCreatureExpression@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00729360, length 52 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is a
// propagated name), malloc @ 0x00BFEA0E (the list's sentinel node).
// Identity: the manifest label "CTCActionUseBase::CTCActionUseBase" is propagated; the vtable stored is
// 0x012665CC = CTCCreatureExpression (rtti-vtables.tsv).
//
// Layout: Ego_r.pdb CTCCreatureExpression = CTCBase 0xc, list<CExpressionState> ExpressionStates +0x0c,
// Blinking +0x18 (PDB 12-byte list). The retail list is a single sentinel-node pointer (as the landed
// list constructors, e.g. 0x0084F1F0), so Blinking sits at +0x10; the node is 0x1c bytes
// (next, prev, a 0x14-byte CExpressionState).
// Shape: base constructor, vtable, the list's inline constructor (head nulled before the malloc, the
// node linked to itself BEFORE it is stored in the head), and the `add esp,4` of the malloc delayed
// to just before the Blinking store.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

struct CExpressionState { unsigned char _body[0x14]; };

template<class T>
struct CRetailList {                                        // retail std::list: one sentinel pointer
    struct node { node* next; node* prev; T value; };
    node* head;
    CRetailList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

class CTCCreatureExpression : public CTCBase { public:              // vtable 0x012665CC
    CRetailList<CExpressionState> ExpressionStates;         // +0x0c
    bool                          Blinking;                 // +0x10
    CTCCreatureExpression(CThing& owner);
};

CTCCreatureExpression::CTCCreatureExpression(CThing& owner)
    : CTCBase(owner)
    , Blinking(false)
{
}