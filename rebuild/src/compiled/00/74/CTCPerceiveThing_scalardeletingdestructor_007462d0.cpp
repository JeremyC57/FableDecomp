struct CTCPerceiveThing {
    void* vftable;
};

void __fastcall CTCPerceiveThing_dtor(CTCPerceiveThing* thing);
void operator delete(void*);

void* __fastcall CTCPerceiveThing_scalar_deleting_destructor(CTCPerceiveThing* thing, int edx_unused, unsigned int flags)
{
    CTCPerceiveThing_dtor(thing);
    if (flags & 1)
        operator delete(thing);
    return thing;
}