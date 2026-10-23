struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8708;
void __fastcall GlobalMethodTailThunk_01228a40()
{
    g_AutoTinyThunkObject_013b8708.Invoke();
}