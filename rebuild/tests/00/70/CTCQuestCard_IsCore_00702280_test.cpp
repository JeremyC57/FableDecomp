#include <cstdio>
struct Inner {
    char pad[0x58];
    char flag;
};
class CTCQuestCard { public:
    char pad0[0xc];
    Inner* inner;
    char pad1[4];
    int  FeatIndex;
};

bool __fastcall CTCQuestCard_IsCore(CTCQuestCard* self)
{
    if (self->FeatIndex <= -1) {
        if (self->inner->flag)
            return true;
    }
    return false;
}

int main()
{
    Inner innr; innr.flag = 1;
    Inner* pin = &innr;
    CTCQuestCard c; c.inner = pin;
    CTCQuestCard* pc = &c;

    c.FeatIndex = 0;   if (CTCQuestCard_IsCore(pc)) { printf("FAIL1\n"); return 1; }
    c.FeatIndex = 5;   if (CTCQuestCard_IsCore(pc)) { printf("FAIL2\n"); return 1; }

    c.FeatIndex = -1;  if (!CTCQuestCard_IsCore(pc)) { printf("FAIL3\n"); return 1; }
    c.FeatIndex = -100;if (!CTCQuestCard_IsCore(pc)) { printf("FAIL4\n"); return 1; }

    innr.flag = 0;
    c.FeatIndex = -1;  if (CTCQuestCard_IsCore(pc)) { printf("FAIL5\n"); return 1; }

    printf("OK_00702280\n");
    return 0;
}