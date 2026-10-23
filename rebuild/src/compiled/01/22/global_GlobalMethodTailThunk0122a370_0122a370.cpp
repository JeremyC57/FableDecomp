struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6dc;
void __fastcall GlobalMethodTailThunk_0122a370()
{
    g_AutoTinyThunkObject_013bb6dc.Invoke();
}