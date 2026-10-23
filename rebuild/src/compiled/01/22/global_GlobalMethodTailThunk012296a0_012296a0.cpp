struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac44;
void __fastcall GlobalMethodTailThunk_012296a0()
{
    g_AutoTinyThunkObject_013bac44.Invoke();
}