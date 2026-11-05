#include <cstdio>

int __fastcall CInputTypeMouseWheelMovementDownEvent_GetType(void *event);

int __fastcall CInputTypeMouseWheelMovementDownEvent_GetType(void *event)
{
    (void)event;
    return 8;
}

int main()
{
    void *obj = (void *)0x12345678;
    int r = CInputTypeMouseWheelMovementDownEvent_GetType(obj);
    if (r != 8) { printf("FAIL got %d\n", r); return 1; }
    printf("OK_0x004457b0 GetType=%d\n", r);
    return 0;
}