#pragma optimize("s", on)
// Leaf: OnReadFinished
struct AutoTinyActiveFile
{
    void* resource;
    void* reader;
};
extern void __fastcall AutoTinyFinishRead_004da636(void* resource, void* reader, char* status);
extern void __cdecl AutoTinyReleaseResource(void* resource);
void __fastcall OnReadFinished(AutoTinyActiveFile* self)
{
    char status;
    AutoTinyFinishRead_004da636(self->resource, self->reader, &status);
    if (self->resource != 0)
        AutoTinyReleaseResource(self->resource);
}