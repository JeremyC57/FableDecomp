class CTCCreatureNavigation { public:
    char pad[0x50];
    int cur;
};
extern "C" int helperA(void);
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
int __fastcall CTCCreatureNavigation_IsCurrentlyNavigating(CTCCreatureNavigation* navigation)
{
    if (navigation->cur >= 0) {
        helperA();
        if (CWorld::GetFrame() - navigation->cur <= 1)
            return 1;
    }
    return 0;
}