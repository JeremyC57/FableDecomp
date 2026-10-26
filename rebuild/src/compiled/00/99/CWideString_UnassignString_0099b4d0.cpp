class CWideStringData
{ public:
    void* buffer;
    int unused1;
    int unused2;
    int refCount;
};

class CWideString
{ public:
    CWideStringData* data;
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
void __cdecl operator delete(void* block);   // MSVCR71 import thunk 0x00BFE9BC

void __fastcall CWideString_UnassignString(CWideString* self)
{
    CWideStringData* data = self->data;
    if (data)
    {
        if (--data->refCount <= 0)
        {
            CWideStringData* p = self->data;
            if (p)
            {
                void* buf = p->buffer;
                if (buf)
                    free(buf);
                operator delete(p);
            }
        }
        self->data = 0;
    }
}