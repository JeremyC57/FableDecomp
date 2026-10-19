// CMiniMapDisplay::ClearPrimitiveHandles @ 0x00A46D90 (retail; catalog: CMiniMapDisplay::ClearPrimitiveHandles, 107 bytes)
// Tears down two inline handle vectors (vecA at +0x10 first, then vecB at +0x0): each element (0x14 bytes)
// has a CCharString at +0x0 destroyed with ~CCharString @ 0x0099EAE0, the element block is released with
// free (MSVCR71 thunk 0x00BFEA14; STLport's allocator), and the vector's own leading CCharString word is
// destroyed with the same ~CCharString. vecB's leading string sits at the object's +0x0, so its destructor
// call runs on this. __fastcall, this in ecx.
#pragma optimize("t", on)
struct CCharString {
    void* PStringData;
    ~CCharString();                 // 0x0099EAE0
};
struct Handle {
    CCharString Name;               // +0x00
    unsigned char pad[0x10];
};
struct HandleVec {
    CCharString Name;               // +0x00
    Handle* first;                  // +0x04
    Handle* last;                   // +0x08
    void*   end;                    // +0x0C
};
struct CMiniMapDisplay {
    HandleVec vecB;                 // +0x00
    HandleVec vecA;                 // +0x10
};
extern "C" void __cdecl free(void* block);   // MSVCR71 import thunk 0x00BFEA14
static void ClearVec(HandleVec* v)
{
    Handle* last = v->last;
    for (Handle* p = v->first; p != last; ++p)
        p->Name.~CCharString();
    if (v->first)
        free(v->first);
    v->Name.~CCharString();
}
void __fastcall CMiniMapDisplay_ClearPrimitiveHandles(CMiniMapDisplay* self)
{
    ClearVec(&self->vecA);
    Handle* last2 = self->vecB.last;
    for (Handle* p = self->vecB.first; p != last2; ++p)
        p->Name.~CCharString();
    if (self->vecB.first)
        free(self->vecB.first);
    self->vecB.Name.~CCharString();
}
