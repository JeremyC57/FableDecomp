struct CTCCreatureNavigation {
    char pad[0x50];
    int cur;
};
extern "C" int helperA(void);
extern "C" int helperB(void);
int __fastcall CTCCreatureNavigation_IsCurrentlyNavigating(CTCCreatureNavigation* navigation)
{
    if (navigation->cur >= 0) {
        helperA();
        if (helperB() - navigation->cur <= 1)
            return 1;
    }
    return 0;
}