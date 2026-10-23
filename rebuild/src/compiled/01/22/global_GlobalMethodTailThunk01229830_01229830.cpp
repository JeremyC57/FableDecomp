struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baca8;
void __fastcall GlobalMethodTailThunk_01229830()
{
    g_AutoTinyThunkObject_013baca8.Invoke();
}