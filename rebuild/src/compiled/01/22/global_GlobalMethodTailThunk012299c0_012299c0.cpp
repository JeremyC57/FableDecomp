struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bad2c;
void __fastcall GlobalMethodTailThunk_012299c0()
{
    g_AutoTinyThunkObject_013bad2c.Invoke();
}