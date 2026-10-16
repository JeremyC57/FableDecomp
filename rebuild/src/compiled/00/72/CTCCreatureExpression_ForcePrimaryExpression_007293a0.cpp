// CTCCreatureExpression::ForcePrimaryExpression(long animIndex, float maxWeight) -- make the expression the primary one: when it already is the front state just update its max weight, else demote the front state and push a new primary state for it (index 0 = none)
//
// Retail 0x007293A0, 99 bytes (`ret 8`). Callee: list<CExpressionState>::push_front(const
// CExpressionState&) @ 0x00729960 (out of line; manifest "ExpressionSet_InsertNode").
// Layout: as the landed ctor 0x00729360: ExpressionStates (STLport list, one sentinel pointer) at
// +0x0c; node {next, prev, CExpressionState +8}; Ego_r.pdb CExpressionState (0x14): AnimIndex +0,
// MaxWeight +4, Weight +8, OldWeight +0xc, Primary +0x10, Stage +0x11.
// Shape: `!empty()` then the front node as a pointer local (`head->next` re-read; a `front()` reference
// adds `add eax,8`); the new state is a 0x14-byte frame temporary
// built by an inline constructor (index, max weight, two zero floats, Primary = true, Stage = 0)
// and passed by reference to the out-of-line push_front on the list (ecx kept as `this + 0xc`).
struct CExpressionState {
    long          AnimIndex;                        // +0x00
    float         MaxWeight;                        // +0x04
    float         Weight;                           // +0x08
    float         OldWeight;                        // +0x0c
    bool          Primary;                          // +0x10
    unsigned char Stage;                            // +0x11
    CExpressionState(long animIndex, float maxWeight)
        : AnimIndex(animIndex), MaxWeight(maxWeight), Weight(0.0f), OldWeight(0.0f), Primary(true), Stage(0) {}
};

struct CExpressionStateList {                       // STLport list<CExpressionState>: one sentinel pointer
    struct node { node* next; node* prev; CExpressionState value; };
    node* head;
    bool empty() const { return head->next == head; }
    void push_front(const CExpressionState& state);   // 0x00729960
};

struct CTCCreatureExpression {
    virtual void v00();
    void*                ParentThing;               // +0x04 (CTCBase)
    unsigned char        _pad08[4];
    CExpressionStateList ExpressionStates;          // +0x0c
    bool                 Blinking;                  // +0x10
    void ForcePrimaryExpression(long animIndex, float maxWeight);
};

void CTCCreatureExpression::ForcePrimaryExpression(long animIndex, float maxWeight)
{
    if (!ExpressionStates.empty())
    {
        CExpressionStateList::node* front = ExpressionStates.head->next;
        if (front->value.AnimIndex == animIndex)
        {
            front->value.MaxWeight = maxWeight;
            return;
        }
        front->value.Primary = false;
    }
    if (animIndex)
        ExpressionStates.push_front(CExpressionState(animIndex, maxWeight));
}