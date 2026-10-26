#pragma optimize("s",on)
class CExpressionDef { public: int vt; };
extern void __fastcall CExpressionDef_dtor(CExpressionDef* def);
extern void __cdecl fable_operator_delete(void* p);

void* __fastcall CExpressionDef_scalar_deleting_destructor(CExpressionDef* def, int edx_unused, unsigned int flags) {
    CExpressionDef_dtor(def);
    if (flags & 1) {
        fable_operator_delete(def);
    }
    return def;
}