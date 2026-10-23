struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6cc;
void __fastcall GlobalMethodTailThunk_0122a330()
{
    g_AutoTinyThunkObject_013bb6cc.Invoke();
}