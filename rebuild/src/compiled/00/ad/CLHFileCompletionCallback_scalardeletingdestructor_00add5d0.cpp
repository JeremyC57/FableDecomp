struct CLHFileCompletionCallback {
    void* vftable;
};

void __fastcall CLHFileCompletionCallback_dtor(CLHFileCompletionCallback* callback);
void operator delete(void*);

void* __fastcall CLHFileCompletionCallback_scalar_deleting_destructor(CLHFileCompletionCallback* callback, int edx_unused, unsigned int flags)
{
    CLHFileCompletionCallback_dtor(callback);
    if (flags & 1)
        operator delete(callback);
    return callback;
}