struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b80;
void __fastcall GlobalMethodTailThunk_01228f75()
{
    g_AutoTinyThunkObject_013b8b80.Invoke();
}