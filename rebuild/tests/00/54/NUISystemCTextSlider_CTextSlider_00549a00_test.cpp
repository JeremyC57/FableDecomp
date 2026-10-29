#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    T* operator->() const { return Data; }
};
struct CActionAllocator {                        // std::allocator<CCountedPointer<CAction> > (empty; the user ctor keeps
    CActionAllocator() { }                       //   VC7.1 from zero-filling the temporary byte)
};
namespace NUISystem { class CAction; }
void __fastcall DestroyActionRange(CCountedPointer<NUISystem::CAction>* first, CCountedPointer<NUISystem::CAction>* last,
                                   const CActionAllocator& allocator);   // 0x0054A1C0
template <class T> struct CRetailVector {        // STLport vector (12 bytes; PDB 16)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
    ~CRetailVector()
    {
        DestroyActionRange(_M_start, _M_finish, CActionAllocator());
        if (_M_start)
            free(_M_start);
    }
};
template <class T> struct CRetailList {          // STLport list: the header-node pointer
    void* _M_node;
    void clear();                                // 0x005356A0
    ~CRetailList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
struct CComponent;
namespace NUISystem {
struct CComponentInterface {
    virtual void v000();
};
struct CDrawableInterface {
    virtual void v000();
    unsigned char _pad04[0x10];
};
struct CDraggableInterface {
    virtual void v000();
};
class CDraggableInto : public CComponentInterface, public CDrawableInterface, public CDraggableInterface { public:
    unsigned char _pad1c[0x15c - 0x1c];
    ~CDraggableInto();                           // 0x0052CCF0
};
class CTextSlider : public CDraggableInto { public:            // vtables 0x01248A8C / 0x01248A68 / 0x01248A60 (.?AVCTextSlider@NUISystem@@)
    unsigned long SelectedChild;                 // +0x15c
    unsigned long PreviouslySelectedChild;       // +0x160
    unsigned long StartSelectedChild;            // +0x164
    bool Move;                                   // +0x168
    CRetailVector<CCountedPointer<CAction> > Actions;          // +0x16c
    CRetailList<CCountedPointer<CAction> >* PActionsOnBack;    // +0x178
    CRetailList<CCountedPointer<CAction> >* PActionsOnSelect;  // +0x17c
    float LastInputTime;                         // +0x180
    float InputDelay;                            // +0x184
    long PDefAction;                             // +0x188 EActionType
    CComponent* PLeftArrow;                      // +0x18c
    CComponent* PRightArrow;                     // +0x190
    unsigned long OriginalSelectedChild;         // +0x194
    CComponent* PAcceptButton;                   // +0x198
    unsigned long MaxChild;                      // +0x19c
    virtual void v000();
    ~CTextSlider();
};
}
struct CSliderValue { unsigned char _pad[8]; unsigned long Selected; };          // +0x08
struct CSliderTarget { unsigned char _pad[8]; CSliderValue* Value; };           // +0x08
struct CSliderTargetRef { CSliderTarget* Target; };                             // +0x00
namespace NUISystem {
class CAction { public:
    virtual void v00();
    CSliderTargetRef* PTarget;                   // +0x04
};
}
namespace NUISystem { class CManager { public:
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual void ExecuteAction(const CCountedPointer<NUISystem::CAction>& action);   // slot 14 (+0x38)
}; }
namespace NUISystem {
CTextSlider::~CTextSlider()
{
    if (StartSelectedChild != SelectedChild) {
        for (unsigned int i = 0; i < Actions.size(); ++i) {
            Actions[i]->PTarget->Target->Value->Selected = StartSelectedChild;
            CManager::GetInstance()->ExecuteAction(Actions[i]);
        }
        Move = true;
    }
    delete PActionsOnBack;
    delete PActionsOnSelect;
}
}
// ---- test doubles ----
static int g_destroyRanges = 0, g_clears = 0, g_executed = 0, g_baseDtors = 0;
static void* g_rangeFirst = 0;
void __fastcall DestroyActionRange(CCountedPointer<NUISystem::CAction>* first, CCountedPointer<NUISystem::CAction>*,
                                   const CActionAllocator&) { ++g_destroyRanges; g_rangeFirst = first; }
template <> void CRetailList<CCountedPointer<NUISystem::CAction> >::clear() { ++g_clears; }
namespace NUISystem {
void CComponentInterface::v000() {} void CDrawableInterface::v000() {} void CDraggableInterface::v000() {}
CDraggableInto::~CDraggableInto() { ++g_baseDtors; }
void CTextSlider::v000() {}
void CAction::v00() {}
}
static const NUISystem::CAction* g_lastExecuted = 0;
struct CTestFrontEnd : NUISystem::CManager {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {}
    virtual void ExecuteAction(const CCountedPointer<NUISystem::CAction>& a) { ++g_executed; g_lastExecuted = a.Data; }
};
static CTestFrontEnd g_fe;
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return &g_fe; }
void NUISystem::CManager::v00() {} void NUISystem::CManager::v01() {} void NUISystem::CManager::v02() {} void NUISystem::CManager::v03() {}
void NUISystem::CManager::v04() {} void NUISystem::CManager::v05() {} void NUISystem::CManager::v06() {} void NUISystem::CManager::v07() {}
void NUISystem::CManager::v08() {} void NUISystem::CManager::v09() {} void NUISystem::CManager::v10() {} void NUISystem::CManager::v11() {}
void NUISystem::CManager::v12() {} void NUISystem::CManager::v13() {}
void NUISystem::CManager::ExecuteAction(const CCountedPointer<NUISystem::CAction>&) {}
int main()
{
    using NUISystem::CTextSlider; using NUISystem::CAction;
    CSliderValue values[2]; values[0].Selected = 0; values[1].Selected = 0;
    CSliderTarget targets[2]; targets[0].Value = &values[0]; targets[1].Value = &values[1];
    CSliderTargetRef refs[2]; refs[0].Target = &targets[0]; refs[1].Target = &targets[1];
    CAction actions[2]; actions[0].PTarget = &refs[0]; actions[1].PTarget = &refs[1];
    CCountedPointer<CAction>* storage = (CCountedPointer<CAction>*)std::malloc(2 * sizeof(CCountedPointer<CAction>));
    storage[0].Data = &actions[0]; storage[0].Info = 0; storage[1].Data = &actions[1]; storage[1].Info = 0;
    CTextSlider* s = new CTextSlider;
    s->SelectedChild = 4; s->StartSelectedChild = 2; s->Move = false;
    s->Actions._M_start = storage; s->Actions._M_finish = storage + 2; s->Actions._M_end_of_storage = storage + 2;
    s->PActionsOnBack = new CRetailList<CCountedPointer<CAction> >; s->PActionsOnBack->_M_node = std::malloc(12);
    s->PActionsOnSelect = 0;
    s->~CTextSlider();
    bool ok = values[0].Selected == 2 && values[1].Selected == 2 && g_executed == 2 && g_lastExecuted == &actions[1]
        && g_clears == 1 && g_destroyRanges == 1 && g_rangeFirst == storage && g_baseDtors == 1;
    // unchanged selection: no revert, no list
    void* mem = std::malloc(sizeof(CTextSlider));
    CTextSlider* t = new (mem) CTextSlider;
    t->SelectedChild = 3; t->StartSelectedChild = 3;
    t->Actions._M_start = 0; t->Actions._M_finish = 0; t->PActionsOnBack = 0; t->PActionsOnSelect = 0;
    t->~CTextSlider();
    ok = ok && g_executed == 2 && g_clears == 1 && g_destroyRanges == 2 && g_baseDtors == 2;
    if (!ok) { std::printf("00549a00_TEST FAIL\n"); return 1; }
    std::printf("00549a00_TEST PASS\n");
    return 0;
}