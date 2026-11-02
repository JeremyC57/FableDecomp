// CCreatureAction_CastStrengthMultiStrikeProper::FinishAction() -- kill every dependent thing that is still alive, then the combat-action FinishAction
// Leaf: ?FinishAction@CCreatureAction_CastStrengthMultiStrikeProper@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E63B0, 69 bytes, thiscall, tail `jmp CCombatActionBase::FinishAction`.
// Identity (proven): the only reference is slot 4 of vtable 0x012877A4 (RTTI .?AVCCreatureAction_CastStrengthMultiStrikeProper@@
//   : CCombatActionBase : CCreatureActionBase : CBaseObject : CBaseClass); slot 4 is FinishAction in CCreatureActionBase 0x01259D24
//   (?FinishAction@CCreatureActionBase@@UAEXXZ 0x00694430) and CCombatActionBase 0x01259E44 (0x00859A20, the tail). Ego_r (0x0090E0C0)
//   and FableWin (0x02AE4C30) both spell ?FinishAction@CCreatureAction_CastStrengthMultiStrikeProper@@UAEXXZ (public virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (per element), CThing::Kill(bool) 0x004C9B80 (both PDBs ?Kill@CThing@@QAEX_N@Z),
//   CCombatActionBase::FinishAction 0x00859A20 (Ego_r / FableWin ?FinishAction@CCombatActionBase@@MAEXXZ, protected).
// Layout: the loop walks CCreatureActionBase::DependentThings (+0x9C, STLport vector<CIntelligentPointer<CThing> >, 8-byte elements:
//   the Ego_r member at engine/CCreatureActionBase.h's retail offset; FableWin's protected GetDependentThings() returns it); the
//   dead test is CThing::Dead (+0x91 bit 0, Ego_r +0xA1 = 0x10 above retail) through the inline IsDead() const (FableWin).
// Shape: `for (iterator it = begin(); it != end(); ++it)` with end() re-read after each body (the call may change the vector);
//   the null test and the dead test skip the Kill(true) call.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                                 // CBaseObject : CBaseClass (IntelligentPointer +4)

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x0
    _Tp* _M_finish;                                             // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
}   // namespace std

#define CClass CClassFields                                     // engine/CClass.h is flattened (vptr as data): layout-rename include
#include "engine/CClass.h"
#undef CClass

class CClass : public CBaseObject {                             // RTTI CClass : CBaseObject; fields as engine/CClass.h
public:
    unsigned char ClassListPos[0x4];                            // +0x08 list<CClass*>::iterator
    unsigned char TypeListPos[0x4];                             // +0x0c list<CClass*>::iterator
    unsigned char Type;                                         // +0x10
};

class CThing : public CClass {                                  // RTTI CThing : CClass
public:
    unsigned char _pad14[0x91 - 0x14];
    unsigned char Dead : 1;                                     // +0x91 bit 0
    bool IsDead() const { return Dead; }                        // FableWin ?IsDead@CThing@@QBE_NXZ (inline)
    void Kill(bool);                                            // 0x004C9B80
};
class CThingCreatureBase : public CThing { };

class CBaseIntelligentPointer {                                 // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                              // 0x00A01B50
private:
    CBaseObjectPointer* PData;                                  // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CCreatureActionBase : public CBaseObject {                // vtable 0x01259D24
public:
    virtual void EditUpdate(float);                             // slot 2  0x00692DB0
    virtual void PerformAction() = 0;                           // slot 3
    virtual void FinishAction();                                // slot 4  0x00694430
    CIntelligentPointer<CThingCreatureBase> PCreature;          // +0x08
    unsigned char _pad10[0x9c - 0x10];                          // +0x10 TotalTime .. CutSceneEvents
    std::vector<CIntelligentPointer<CThing> > DependentThings;  // +0x9c
};

class CCombatActionBase : public CCreatureActionBase {          // vtable 0x01259E44, retail 0x114 bytes
protected:
    virtual void FinishAction();                                // slot 4  0x00859A20
public:
    unsigned char _padA8[0x114 - 0xa8];
};

class CCreatureAction_CastStrengthMultiStrikeProper : public CCombatActionBase {   // vtable 0x012877A4
public:
    virtual void FinishAction();
    long SpellLevel;                                            // +0x114
};

void CCreatureAction_CastStrengthMultiStrikeProper::FinishAction()
{
    for (std::vector<CIntelligentPointer<CThing> >::iterator it = DependentThings.begin(); it != DependentThings.end(); ++it) {
        CThing* thing = it->GetPItem();
        if (thing && !thing->IsDead())
            thing->Kill(true);
    }
    CCombatActionBase::FinishAction();
}