struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8758;
void __fastcall GlobalMethodTailThunk_01228add()
{
    g_AutoTinyThunkObject_013b8758.Invoke();
}