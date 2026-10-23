struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacb8;
void __fastcall GlobalMethodTailThunk_01229870()
{
    g_AutoTinyThunkObject_013bacb8.Invoke();
}