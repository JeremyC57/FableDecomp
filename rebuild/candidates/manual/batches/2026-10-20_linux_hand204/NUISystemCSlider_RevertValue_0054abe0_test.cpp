// Behaviour test for NUISystem::CSlider::RevertValue (0x0054ABE0): the value, start and old values take the previous value, each action target takes its X and every action is executed, then Move is set.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
struct C2DVector { float X, Y; };
struct CCPPointerInfo { long Count; };
template <class T> struct CCountedPointer {
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    T* operator->() const { return Data; }
};
template <class T> struct CRetailVector {        // STLport vector (12 bytes; PDB 16)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
template <class T> struct CRetailList;
struct IPrimary_0054c050 { virtual void v00() {} };
struct CObserver {                               // the secondary base at +4 (0x14 bytes)
    virtual void a00();
    unsigned char _pad04[0x10];
};
struct ISecondaryB_0054c050 { virtual void b00(); };
struct CSprite : IPrimary_0054c050, CObserver, ISecondaryB_0054c050 {
    CSprite(void* def);                          // 0x0041B800
};
struct CComponent;
namespace NUISystem {
struct CAction;
#pragma pack(push, 1)
struct CSlider : CSprite {                       // vtables 0x01248D04 / 0x01248CE0 / 0x01248CD8
    unsigned char _pad01c[0x1a4 - 0x1c];
    C2DVector     Value;                         // +0x1a4 (PDB +0x1d4)
    bool          Move;                          // +0x1ac
    C2DVector     OldValue;                      // +0x1ad
    C2DVector     StartValue;                    // +0x1b5
    unsigned char _pad1bd[3];
    CRetailVector<CCountedPointer<CAction> > Actions;          // +0x1c0 (PDB +0x1f0)
    CRetailList<CCountedPointer<CAction> >*  PActionsOnBack;   // +0x1cc
    CRetailList<CCountedPointer<CAction> >*  PActionsOnSelect; // +0x1d0
    long          PDefAction;                    // +0x1d4 EActionType
    CComponent*   PLeftArrow;                    // +0x1d8
    CComponent*   PRightArrow;                   // +0x1dc
    C2DVector     PreviousValue;                 // +0x1e0
    CComponent*   PAcceptButton;                 // +0x1e8
    virtual void a00();
    virtual void b00();
    void RevertValue();
};
#pragma pack(pop)
}
struct CSliderValue { unsigned char _pad[8]; float Value; };                  // +0x08
struct CSliderTarget { unsigned char _pad[8]; CSliderValue* Value; };          // +0x08
struct CSliderTargetRef { CSliderTarget* Target; };                            // +0x00
namespace NUISystem {
struct CAction {
    virtual void v00() {}
    CSliderTargetRef* PTarget;                   // +0x04
};
}
struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {}
    virtual void ExecuteAction(const CCountedPointer<NUISystem::CAction>& action) = 0;   // slot 14 (+0x38)
};
namespace NUISystem {
void CSlider::RevertValue()
{
    OldValue = StartValue = Value = PreviousValue;
    for (unsigned int i = 0; i < Actions.size(); ++i) {
        Actions[i]->PTarget->Target->Value->Value = PreviousValue.X;
        CFrontEndManager::GetInstance()->ExecuteAction(Actions[i]);
    }
    Move = true;
}
}
static const void* g_executed[8]; static int g_count = 0; static float g_seen[8];
struct TestManager : CFrontEndManager {
    void ExecuteAction(const CCountedPointer<NUISystem::CAction>& a) { g_seen[g_count] = a->PTarget->Target->Value->Value; g_executed[g_count++] = &a; }
};
static TestManager g_manager;
CFrontEndManager* __fastcall CFrontEndManager::GetInstance() { return &g_manager; }
static unsigned char g_mem[sizeof(NUISystem::CSlider)];
int main()
{
    NUISystem::CSlider* s = (NUISystem::CSlider*)g_mem;
    CSliderValue values[2]; CSliderTarget targets[2]; CSliderTargetRef refs[2];
    static unsigned char actionMem[2][sizeof(NUISystem::CAction)];
    CCountedPointer<NUISystem::CAction> actions[2];
    for (int i = 0; i < 2; ++i) {
        values[i].Value = -1.0f; targets[i].Value = &values[i]; refs[i].Target = &targets[i];
        NUISystem::CAction* a = (NUISystem::CAction*)actionMem[i]; a->PTarget = &refs[i];
        actions[i].Data = a; actions[i].Info = 0;
    }
    s->Actions._M_start = actions; s->Actions._M_finish = actions + 2; s->Actions._M_end_of_storage = actions + 2;
    s->PreviousValue.X = 0.25f; s->PreviousValue.Y = 0.75f;
    s->Value.X = s->OldValue.X = s->StartValue.X = 9.0f; s->Value.Y = s->OldValue.Y = s->StartValue.Y = 9.0f; s->Move = false;
    s->RevertValue();
    bool ok = s->Value.X == 0.25f && s->Value.Y == 0.75f && s->OldValue.X == 0.25f && s->OldValue.Y == 0.75f
        && s->StartValue.X == 0.25f && s->StartValue.Y == 0.75f && s->Move
        && g_count == 2 && g_executed[0] == &actions[0] && g_executed[1] == &actions[1] && g_seen[0] == 0.25f && g_seen[1] == 0.25f
        && values[0].Value == 0.25f && values[1].Value == 0.25f;
    if (!ok) { std::printf("0054abe0_TEST FAIL\n"); return 1; }
    std::printf("0054abe0_TEST PASS\n");
    return 0;
}
