class CTCQuestCard { public:
    void* vftable;
};

void __fastcall CTCQuestCard_dtor(CTCQuestCard* card);
void operator delete(void*);

void* __fastcall CTCQuestCard_scalar_deleting_destructor(CTCQuestCard* card, int edx_unused, unsigned int flags)
{
    CTCQuestCard_dtor(card);
    if (flags & 1)
        operator delete(card);
    return card;
}