struct CWideString
{
    unsigned long value;
    void CopyConstruct_0099ec30(CWideString* source);
};

struct VectorPushBackOverlay
{
    unsigned char unused[4];
    CWideString* finish;
    CWideString* end_of_storage;

    void InsertRange_00433530(
        CWideString* position,
        CWideString* value,
        CWideString** value_copy,
        int count,
        int copies);
};

void __fastcall push_back(VectorPushBackOverlay* self, void*, CWideString* value)
{
    CWideString* finish = self->finish;
    if (finish != self->end_of_storage) {
        if (finish != 0) {
            finish->CopyConstruct_0099ec30(value);
        }
        self->finish = (CWideString*)((unsigned char*)self->finish + 4);
        return;
    }
    self->InsertRange_00433530(finish, value, &value, 1, 1);
}