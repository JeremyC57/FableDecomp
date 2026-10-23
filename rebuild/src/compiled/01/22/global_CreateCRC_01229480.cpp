struct AutoTinyReverseLoopTarget
{
    unsigned char storage[4];
    void Invoke();
};
extern AutoTinyReverseLoopTarget g_AutoTinyReverseLoopObjects_013bab40[16];
void __fastcall CreateCRC()
{
    AutoTinyReverseLoopTarget* cursor =
        g_AutoTinyReverseLoopObjects_013bab40 + 16;
    for (int remaining = 16; remaining != 0; --remaining)
    {
        --cursor;
        cursor->Invoke();
    }
}