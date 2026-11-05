// Leaf: ?CEngineLandscapeMap_CanRelocateData@@YI_NPAUS@@PAX1@Z
struct S { char pad[0x5C]; unsigned char f; };
bool __fastcall CEngineLandscapeMap_CanRelocateData(S* self, void* edx, void* arg)
{
    (void)edx; (void)arg;
    return self->f == 0;
}