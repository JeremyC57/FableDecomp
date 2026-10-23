struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b0c;
void __fastcall GlobalMethodTailThunk_01228eba()
{
    g_AutoTinyThunkObject_013b8b0c.Invoke();
}