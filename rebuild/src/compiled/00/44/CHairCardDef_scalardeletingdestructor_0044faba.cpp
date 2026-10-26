#pragma optimize("s",on)
class CHairCardDef { public:
    void* vptr;
};
extern void __fastcall CHairCardDef_dtor(CHairCardDef* def);
extern void __cdecl engine_delete(void* p);
extern void* const CHairCardDef_vftable;

void* __fastcall CHairCardDef_scalar_deleting_destructor(CHairCardDef* def, int /*edx*/, unsigned int flags)
{
    def->vptr = (void*)&CHairCardDef_vftable;
    CHairCardDef_dtor(def);
    if (flags & 1) {
        engine_delete(def);
    }
    return def;
}