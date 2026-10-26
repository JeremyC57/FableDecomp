class CScriptGameResourceObjectScriptedThing { public:
    void* vftable;
};

void __fastcall CScriptGameResourceObjectScriptedThing_dtor(CScriptGameResourceObjectScriptedThing* thing);
void operator delete(void*);

void* __fastcall CScriptGameResourceObjectScriptedThing_scalar_deleting_destructor(CScriptGameResourceObjectScriptedThing* thing, int edx_unused, unsigned int flags)
{
    CScriptGameResourceObjectScriptedThing_dtor(thing);
    if (flags & 1)
        operator delete(thing);
    return thing;
}