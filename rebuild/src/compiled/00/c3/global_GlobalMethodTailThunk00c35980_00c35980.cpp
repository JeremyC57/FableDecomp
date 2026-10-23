struct AutoTinyThunkTarget
{
    void Invoke_00c3d610();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject;
void __fastcall GlobalMethodTailThunk_00c35980()
{
    g_AutoTinyThunkObject.Invoke_00c3d610();
}