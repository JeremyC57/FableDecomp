struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac80;
void __fastcall GlobalMethodTailThunk_01229790()
{
    g_AutoTinyThunkObject_013bac80.Invoke();
}