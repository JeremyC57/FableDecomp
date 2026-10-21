// CPlayerGui::SetButtonAlpha(long, bool, float) -- store a HUD button's alpha in the ability (special) or quick-access table
// Leaf: ?SetButtonAlpha@CPlayerGui@@QAEXJ_NM@Z
// Retail: Fable.exe @ 0x004382E0, 44 bytes, `ret 0Ch`; Ego_r.pdb / FableWin.pdb ?SetButtonAlpha@CPlayerGui@@QAEXJ_NM@Z.
// Callers 0x0064BDD4 / 0x0064BE12. The write-side twin of the landed CPlayerGui::GetButtonAlpha (0x00439AB0), which reads
// the same two vectors with the same `special` selector.
// Layout: retail, as landed 0x00439AB0 -- AbilityButtonsAlpha (STLport vector<float>, 12 bytes) +0x260,
// QuickAccessButtonsAlpha +0x26c; engine/CPlayerGui.h carries the Ego_r.pdb offsets (+0x26c / +0x27c, 16-byte vectors).
struct vector_float {                                  // STLport vector<float> (12 bytes)
    float* _M_start;
    float* _M_finish;
    float* _M_end_of_storage;
    float& operator[](long n) { return *(_M_start + n); }
};
struct CPlayerGui {
    unsigned char _pad000[0x260];
    vector_float  AbilityButtonsAlpha;                   // +0x260
    vector_float  QuickAccessButtonsAlpha;               // +0x26c
    void SetButtonAlpha(long button, bool special, float alpha);
};
void CPlayerGui::SetButtonAlpha(long button, bool special, float alpha)
{
    if (special)
        AbilityButtonsAlpha[button] = alpha;
    else
        QuickAccessButtonsAlpha[button] = alpha;
}