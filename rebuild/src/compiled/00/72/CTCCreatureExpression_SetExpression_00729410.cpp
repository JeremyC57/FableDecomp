// CTCCreatureExpression::SetExpression(long expressionType, float maxWeight) -- pick a random animation for the expression type from the creature's expression def and force it as the primary expression; type 0 first restores or demotes the front state
//
// Retail 0x00729410, 134 bytes (`ret 8`). Callees: CDefClassBase::GetSubDef<CCreatureExpressionDef>
// (CDefPointer<const CCreatureExpressionDef>*) @ 0x00485815 (label "GetSubDef<CAbilityDef>",
// propagated), CTCBase::GetWorld @ 0x00686A70, CWorld::GetPRandomSeed @ 0x0049D810 (label
// "CVertexBufferWin32::DoSizeof", propagated), the def's expression map FindRandomElement(long,
// unsigned long*) @ 0x0086F510, CTCCreatureExpression::ForcePrimaryExpression(long, float) @ 0x007293A0,
// the def's virtual slot 1 (release at zero).
// Layout: ExpressionStates (one sentinel pointer) at +0x0c, CExpressionState as in 0x007293A0; the
// parent thing's def +0x70; the expression def's map at +0x38.
// Shape: the type-0 block uses the front node as a pointer local (AnimIndex == 0 -> MaxWeight = 1.0f,
// else Primary = false); the sub-def out pointer starts null; its object is read once (ebp) and released
// at the end without a null test.
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CExpressionState {
    long          AnimIndex;                        // +0x00
    float         MaxWeight;                        // +0x04
    float         Weight;                           // +0x08
    float         OldWeight;                        // +0x0c
    bool          Primary;                          // +0x10
    unsigned char Stage;                            // +0x11
};

struct CExpressionStateList {                       // STLport list<CExpressionState>: one sentinel pointer
    struct node { node* next; node* prev; CExpressionState value; };
    node* head;
    bool empty() const { return head->next == head; }
};

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +4
};

struct CExpressionMap {
    long FindRandomElement(long expressionType, unsigned long* seed);   // 0x0086F510
};

struct CCreatureExpressionDef : CDefPointeeBase {
    unsigned char   _pad08[0x38 - 0x08];
    CExpressionMap* Expressions;                    // +0x38
};

template <class T>
struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (DecRefToZero(Object->m_refCount))
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CCreatureExpressionDef>* out);   // 0x00485815
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCreatureExpressionDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef_Object;                     // +0x70
};

struct CWorld {
    unsigned long* GetPRandomSeed();                // 0x0049D810
};

struct CTCCreatureExpression {
    virtual void v00();
    CThing*              ParentThing;               // +0x04 (CTCBase)
    unsigned char        _pad08[4];
    CExpressionStateList ExpressionStates;          // +0x0c
    bool                 Blinking;                  // +0x10
    CWorld* GetWorld();                             // 0x00686A70 (CTCBase)
    void ForcePrimaryExpression(long animIndex, float maxWeight);   // 0x007293A0
    void SetExpression(long expressionType, float maxWeight);
};

void CTCCreatureExpression::SetExpression(long expressionType, float maxWeight)
{
    if (expressionType == 0 && !ExpressionStates.empty())
    {
        CExpressionStateList::node* front = ExpressionStates.head->next;
        if (front->value.AnimIndex == 0)
            front->value.MaxWeight = 1.0f;
        else
            front->value.Primary = false;
    }
    CDefPointer<CCreatureExpressionDef> def;
    ParentThing->PDef_Object->GetSubDef(&def);
    CExpressionMap* expressions = def->Expressions;
    long anim = expressions->FindRandomElement(expressionType, GetWorld()->GetPRandomSeed());
    if (anim)
        ForcePrimaryExpression(anim, maxWeight);
}