// NUISystem::CSlider::~CSlider() -- if the value moved, push the starting value back into every action's target and re-run the actions; delete the back/select action lists; destroy the action vector and the sprite base
// Leaf: ??1CSlider@NUISystem
// Retail 0x0054B360, 328 bytes, ret. Callees: CFrontEndManager::GetInstance 0x0041E5F2 and its ExecuteAction (slot 14,
// +0x38), list<CCountedPointer<CAction> >::clear 0x005356A0, free 0x00BFEA14, operator delete 0x00BFE9BC, the action
// range destroyer 0x0054A1C0 (__fastcall first/last + allocator reference), CSprite::~CSprite 0x0041AB40.
// Vtables 0x01248D04 / 0x01248CE0 / 0x01248CD8 at +0 / +4 / +0x18. Epsilon float 0x0129BA3C.
// Layout: the landed RevertValue 0x0054ABE0 model (retail = PDB - 0x30 up to Actions): Value +0x1A4, Move +0x1AC,
// OldValue +0x1AD, StartValue +0x1B5 (unaligned, pack(1)), Actions +0x1C0, PActionsOnBack +0x1CC, PActionsOnSelect +0x1D0.
// Shape: the C2DVector twin of the landed CTextSlider::~CTextSlider 0x00549A00. The "moved" test is `!(StartValue ==
// Value)` with an inline epsilon operator== written with `<=` (`test ah,41h ; jp` / `jnp`); a `>`/`||` operator!= gives
// `je`/`jne` instead.
#include <math.h>
extern "C" void __cdecl free(void* block);       // 0x00BFEA14
void operator delete(void* p);                   // 0x00BFE9BC
struct CCPPointerInfo { long Count; };
template <class T> struct CCountedPointer {
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    T* operator->() const { return Data; }
};
struct CActionAllocator {                        // std::allocator<CCountedPointer<CAction> > (empty)
    CActionAllocator() { }
};
namespace NUISystem { struct CAction; }
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
extern const float g_SliderValueEpsilon;         // 0x0129BA3C
#pragma pack(push, 1)
struct C2DVector {
    float X, Y;
    bool operator==(const C2DVector& v) const
    {
        return fabs(X - v.X) <= g_SliderValueEpsilon && fabs(Y - v.Y) <= g_SliderValueEpsilon;
    }
};
#pragma pack(pop)
struct CComponent;
struct ISpritePrimary {
    virtual void v000();
};
struct CObserver {
    virtual void v000();
    unsigned char _pad04[0x10];
};
struct ISpriteSecondaryB {
    virtual void v000();
};
struct CSprite : ISpritePrimary, CObserver, ISpriteSecondaryB {
    unsigned char _pad1c[0x184 - 0x1c];
    ~CSprite();                                  // 0x0041AB40
};
namespace NUISystem {
#pragma pack(push, 1)
struct CSlider : CSprite {                       // vtables 0x01248D04 / 0x01248CE0 / 0x01248CD8
    C2DVector     Dimensions;                    // +0x184
    C2DVector     Min;                           // +0x18c
    C2DVector     Max;                           // +0x194
    C2DVector     Step;                          // +0x19c
    C2DVector     Value;                         // +0x1a4
    bool          Move;                          // +0x1ac
    C2DVector     OldValue;                      // +0x1ad
    C2DVector     StartValue;                    // +0x1b5
    unsigned char _pad1bd[3];
    CRetailVector<CCountedPointer<CAction> > Actions;          // +0x1c0
    CRetailList<CCountedPointer<CAction> >*  PActionsOnBack;   // +0x1cc
    CRetailList<CCountedPointer<CAction> >*  PActionsOnSelect; // +0x1d0
    long          PDefAction;                    // +0x1d4 EActionType
    CComponent*   PLeftArrow;                    // +0x1d8
    CComponent*   PRightArrow;                   // +0x1dc
    C2DVector     PreviousValue;                 // +0x1e0
    CComponent*   PAcceptButton;                 // +0x1e8
    virtual void v000();
    ~CSlider();
};
#pragma pack(pop)
}
struct CSliderValue { unsigned char _pad[8]; float Value; };                    // +0x08
struct CSliderTarget { unsigned char _pad[8]; CSliderValue* Value; };           // +0x08
struct CSliderTargetRef { CSliderTarget* Target; };                             // +0x00
namespace NUISystem {
struct CAction {
    virtual void v00();
    CSliderTargetRef* PTarget;                   // +0x04
};
}
struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual void ExecuteAction(const CCountedPointer<NUISystem::CAction>& action);   // slot 14 (+0x38)
};
namespace NUISystem {
CSlider::~CSlider()
{
    if (!(StartValue == Value)) {
        for (unsigned int i = 0; i < Actions.size(); ++i) {
            Actions[i]->PTarget->Target->Value->Value = StartValue.X;
            CFrontEndManager::GetInstance()->ExecuteAction(Actions[i]);
        }
        Move = true;
    }
    delete PActionsOnBack;
    delete PActionsOnSelect;
}
}