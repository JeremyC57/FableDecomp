struct CEventExpressionPerformedTo {
    void* vftable;
};

void __fastcall CEventExpressionPerformedTo_dtor(CEventExpressionPerformedTo* event);
void operator delete(void*);

void* __fastcall CEventExpressionPerformedTo_scalar_deleting_destructor(CEventExpressionPerformedTo* event, int edx_unused, unsigned int flags)
{
    CEventExpressionPerformedTo_dtor(event);
    if (flags & 1)
        operator delete(event);
    return event;
}