struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b4c;
void __fastcall GlobalMethodTailThunk_01228eec()
{
    g_AutoTinyThunkObject_013b8b4c.Invoke();
}