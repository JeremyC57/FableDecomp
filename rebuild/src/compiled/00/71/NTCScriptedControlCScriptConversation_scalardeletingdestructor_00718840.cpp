struct CScriptConversation {
    void* vftable;
};

void __fastcall CScriptConversation_dtor(CScriptConversation* conversation);
void operator delete(void*);

void* __fastcall CScriptConversation_scalar_deleting_destructor(CScriptConversation* conversation, int edx_unused, unsigned int flags)
{
    CScriptConversation_dtor(conversation);
    if (flags & 1)
        operator delete(conversation);
    return conversation;
}