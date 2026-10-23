struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013badd8;
void __fastcall GlobalMethodTailThunk_01229b40()
{
    g_AutoTinyThunkObject_013badd8.Invoke();
}