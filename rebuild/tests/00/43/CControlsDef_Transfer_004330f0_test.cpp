#include <cstdio>
class CActionInputControl;
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
class CPersistContext {
public:
    template <class T> void TransferVectorBinary(std::vector<T>& v);   // 0x0043318D for CActionInputControl
    void TransferBool(bool& value);                      // 0x0043314A
};
class CControlsDef {
public:
    virtual void Transfer(CPersistContext& context);     // slot 0
    unsigned char _base[0x38];                           // rest of CParentDefClassBase (0x39 bytes) and padding
    std::vector<CActionInputControl> Controls;           // +0x3c
    bool ToggleZTarget;                                  // +0x48
    bool ToggleSpells;                                   // +0x49
    bool ToggleSneak;                                    // +0x4a
    bool ToggleExpressionMenu;                           // +0x4b
    bool ToggleExpressionShift;                          // +0x4c
    bool FlourishNeedsAttackButtonHeld;                  // +0x4d
};
void CControlsDef::Transfer(CPersistContext& context)
{
    context.TransferVectorBinary(Controls);
    context.TransferBool(ToggleZTarget);
    context.TransferBool(ToggleSpells);
    context.TransferBool(ToggleSneak);
    context.TransferBool(ToggleExpressionMenu);
    context.TransferBool(ToggleExpressionShift);
    context.TransferBool(FlourishNeedsAttackButtonHeld);
}

class CActionInputControl { public: int x; };
static int g_seq[8]; static int g_n; static void* g_vec;
template <class T> void CPersistContext::TransferVectorBinary(std::vector<T>& v) { g_vec = &v; g_seq[g_n++] = -1; }
void CPersistContext::TransferBool(bool& value) { g_seq[g_n++] = (int)value; value = !value; }
int main()
{
    CControlsDef d; CPersistContext c;
    d.ToggleZTarget = true; d.ToggleSpells = false; d.ToggleSneak = true; d.ToggleExpressionMenu = false;
    d.ToggleExpressionShift = false; d.FlourishNeedsAttackButtonHeld = true;
    d.Transfer(c);
    int ok = g_n == 7 && g_vec == &d.Controls && g_seq[0] == -1 && g_seq[1] == 1 && g_seq[2] == 0 && g_seq[3] == 1 && g_seq[4] == 0
          && g_seq[5] == 0 && g_seq[6] == 1 && !d.ToggleZTarget && d.ToggleSpells && !d.FlourishNeedsAttackButtonHeld;
    printf(ok ? "004330f0_TEST PASS\n" : "004330f0_TEST FAIL\n");
    return ok ? 0 : 1;
}