struct Inner {
    char pad[0x58];
    char flag;
};
struct CTCQuestCard {
    char pad0[0xc];
    Inner* inner;   // +0xc
    char pad1[4];   // +0x10
    int  FeatIndex;   // +0x14
};

bool __fastcall IsCore(CTCQuestCard* self)
{
    if (self->FeatIndex <= -1) {
        if (self->inner->flag)
            return true;
    }
    return false;
}