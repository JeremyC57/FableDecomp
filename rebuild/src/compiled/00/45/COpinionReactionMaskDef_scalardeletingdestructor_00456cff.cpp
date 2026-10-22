#pragma optimize("s",on)
struct Member { int pad[16]; };
struct COpinionReactionMaskDef {
    char pad[0x3c];
    Member m;
};

extern "C" void __fastcall Member_dtor(Member* def);
extern "C" void __fastcall Base_dtor(COpinionReactionMaskDef* def);
extern "C" void __cdecl op_delete(void* p);

void* __fastcall COpinionReactionMaskDef_scalar_deleting_destructor(COpinionReactionMaskDef* def, int, unsigned int flags)
{
    Member_dtor(&def->m);
    Base_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}