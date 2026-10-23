#pragma optimize("s",on)
struct CActiveFile { void* field0; };

extern void __fastcall CActiveFile_helper_004dbc3a(CActiveFile* self);
extern void __cdecl free_thing(void* p);

void __fastcall OnReadFinished_13(CActiveFile* self)
{
    CActiveFile_helper_004dbc3a(self);
    void* p = self->field0;
    if (p)
        free_thing(p);
}