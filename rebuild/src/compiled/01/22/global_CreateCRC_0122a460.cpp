struct AutoTinyReverseLoopTarget
{
    unsigned char storage[40];
    void Invoke();
};
extern AutoTinyReverseLoopTarget g_AutoTinyReverseLoopObjects_013bb768[79];
void __fastcall CreateCRC()
{
    AutoTinyReverseLoopTarget* cursor =
        g_AutoTinyReverseLoopObjects_013bb768 + 79;
    for (int remaining = 79; remaining != 0; --remaining)
    {
        --cursor;
        cursor->Invoke();
    }
}