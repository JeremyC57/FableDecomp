struct AutoTinyThunkTarget
// Leaf: ?GlobalMethodTailThunk_012293b0@@YIXXZ
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baafc;
void __fastcall GlobalMethodTailThunk_012293b0()
{
    g_AutoTinyThunkObject_013baafc.Invoke();
}