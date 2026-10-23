struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bab18;
void __fastcall GlobalMethodTailThunk_01229440()
{
    g_AutoTinyThunkObject_013bab18.Invoke();
}