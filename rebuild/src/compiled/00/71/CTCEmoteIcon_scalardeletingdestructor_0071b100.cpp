struct CTCEmoteIcon {
    void* vftable;
};

void __fastcall CTCEmoteIcon_dtor(CTCEmoteIcon* icon);
void operator delete(void*);

void* __fastcall CTCEmoteIcon_scalar_deleting_destructor(CTCEmoteIcon* icon, int edx_unused, unsigned int flags)
{
    CTCEmoteIcon_dtor(icon);
    if (flags & 1)
        operator delete(icon);
    return icon;
}