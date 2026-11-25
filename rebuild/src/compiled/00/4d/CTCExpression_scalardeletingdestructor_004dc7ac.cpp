#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DC7C8 now binds its PDB spelling ??1CTCExpression@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCExpression_Destruct@@YIXPAX@Z.
class CTCExpression { public: virtual ~CTCExpression(); };
void* __fastcall CTCExpression_ScalarDeletingDestructor(void* expression, int, unsigned int flags)
{
    reinterpret_cast<CTCExpression*>(expression)->CTCExpression::~CTCExpression();
    if (flags & 1)
        operator delete(expression);
    return expression;
}