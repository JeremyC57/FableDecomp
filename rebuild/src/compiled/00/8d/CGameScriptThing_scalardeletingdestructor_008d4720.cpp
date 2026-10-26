class CGameScriptThing { public:
    void* vftable;
};

void __fastcall CGameScriptThing_dtor(CGameScriptThing* thing);
void operator delete(void*);

void* __fastcall CGameScriptThing_scalar_deleting_destructor(CGameScriptThing* thing, int edx_unused, unsigned int flags)
{
    CGameScriptThing_dtor(thing);
    if (flags & 1)
        operator delete(thing);
    return thing;
}