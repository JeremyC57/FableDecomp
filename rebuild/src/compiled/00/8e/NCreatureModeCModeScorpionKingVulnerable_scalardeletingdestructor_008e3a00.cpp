struct CModeScorpionKingVulnerable {
    void* vftable;
};

void __fastcall CModeScorpionKingVulnerable_dtor(CModeScorpionKingVulnerable* mode);
void operator delete(void*);

void* __fastcall CModeScorpionKingVulnerable_scalar_deleting_destructor(CModeScorpionKingVulnerable* mode, int edx_unused, unsigned int flags)
{
    CModeScorpionKingVulnerable_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}