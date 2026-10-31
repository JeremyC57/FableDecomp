// CAIStateGroup_HandyMan::Update() -- while the villager is in its work hours and the work spot is alive, walk to the spot and use it; then terminate
// Leaf: ?Update@CAIStateGroup_HandyMan@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091B190, 332 bytes, thiscall, no EH frame.
// Identity: vtable 0x01289CA0 (RTTI CAIStateGroup_HandyMan) slot 12 (slot 11 IsRunnable 0x0091B170); Ego_r.pdb name set
//   ?Update@CAIStateGroup_HandyMan@@UAEXXZ. RTTI base list: CAIStateGroup_HandyMan : CAIStateGroup_WorkBase<CAIStateGroupBase>
//   : CAIStateGroupBase : CBaseClass (read from the class hierarchy descriptor behind the vtable).
// Callees (PDB spellings): CThing::GetPos 0x004C73D0; CTCVillageMember::GetPVillage 0x007EE540; CTCVillage::IsWorkTime
//   0x00658B20 (reads CVillageDef +0x64/+0x70; 0x00658B6B is IsCookTime, +0x74.. -- the parked file's IsCookTime spelling
//   was wrong); CAIStateGroupBase::AIAction_MoveToPos 0x0097EC00; CCreatureAction_UseObject(CThingCreatureBase&, CThing&,
//   bool) 0x0069F230; CAIStateGroupBase::AIAction_PerformActionAndWait 0x0097DB70; ~CBaseIntelligentPointer 0x00A01C10
//   (the action's PUsedThing +0xA8); ~CCreatureActionBase 0x00693EF0; CAIStateGroupBase::Terminate 0x008FCF40.
// Shape: the spot position copied once; IsInWorkHours (PDB CAIStateGroup_WorkBase<CAIStateGroupBase>::IsInWorkHours,
//   inline: GetTC<CTCVillageMember> 0x23 with the vector-map lower_bound inlined, GetPVillage, IsWorkTime) and the spot's
//   IS_THING_ALIVE are SEPARATE `if (...) { Terminate(); return; }` statements; the UseObject result is a named bool
//   (retail's `setne bl`) tested before the final Terminate. 1 lives in ebx (bitset mask, Dead bit, the `true` pushes).
// Levers (chunk 604): (1) the real class chain (CAIStateGroupBase::PBrain member + inline GetCreature(), as the landed
//   BuyFromSeller 0x00919AE0 model) instead of the flat header + pad-cast brain fixed the loop-head brain/creature/bitset
//   chain (ecx/eax/edx) and the UseObject argument loads; (2) splitting `!IsInWorkHours() || !alive` into two early-return
//   statements fixed the this/end callee-saved pair (edi/ebp; the one-`||` form gives ebp/edi): RELOCATION_MATCH.
// Layout: engine/CAIStateGroup_HandyMan.h and engine/CAIStateGroupBase.h are flat generated headers (vptr as a data member,
//   no base chain), so the class chain is a local retail model (header debt, NOTES.md); CAIBrain from engine/CAIBrain.h
//   (Creature +0x18); CThing: UsingComponents bitset<274> +0x20, Components CVectorMap +0x44, Dead bit 0 of +0x91 (retail
//   STLport offsets, as the landed BuyFromSeller model). CCreatureAction_UseObject is 0xE0 bytes in retail (PUsedThing +0xA8).
#include "engine/CAIBrain.h"
#include "engine/CBaseClass.h"
#include "engine/CTCBase.h"                            // vptr +0, ParentThing +4 (flat generated header)
#include <stddef.h>

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: __forceinline allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    __forceinline _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    __forceinline iterator begin() { return this->_M_start; }
    __forceinline iterator end() { return this->_M_finish; }
};
template <size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    __forceinline static size_t _S_whichword(size_t __pos) { return __pos / 32; }
    __forceinline static _WordT _S_maskbit(size_t __pos) { return 1UL << (__pos % 32); }
    __forceinline _WordT _M_getword(size_t __pos) const { return _M_w[_S_whichword(__pos)]; }
};
template <size_t _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    __forceinline bool operator[](size_t __pos) const { return _Unchecked_test(__pos); }
    __forceinline bool _Unchecked_test(size_t __pos) const { return (this->_M_getword(__pos) & this->_S_maskbit(__pos)) != 0; }
};

}   // namespace std

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    __forceinline const C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    __forceinline float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
};

template <class K, class V> class CKeyPairCompareLess { };
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V, class TCompare> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::iterator iterator;
    CArray<value_type> Data;
    TCompare           Compare;
    bool               Dirty;
    iterator LowerBound(const K& key)                   // 0x0040F020
    {
        long count = Data._M_finish - Data._M_start;
        iterator first = Data._M_start;
        while (count > 0) {
            long half = count / 2;
            iterator middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
};

enum ETCInterfaceType { };
// CBaseClass: engine/CBaseClass.h
class CBaseObject : public CBaseClass { };
class CClass : public CBaseObject { };
class CTCBase;

class CThing : public CClass {
public:
    unsigned char      _pad04[0x14];
    unsigned __int64   UniqueID;                        // +0x18
    std::bitset<274>   UsingComponents;                 // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char      _pad54[0x3c];                    // +0x54 UpdateComponentsList .. CurrentMapNumber
    signed char        PlayerNumber;                    // +0x90
    unsigned char      Dead : 1;                        // +0x91
    bool IsDead() const { return Dead; }                // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
    __forceinline bool HasTC(ETCInterfaceType type) const { return UsingComponents[type]; }
    template <class T> __forceinline bool GetTC(T** out)
    {
        bool has = HasTC((ETCInterfaceType)T::INTERFACE_TYPE);
        if (has) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator it = map->LowerBound(type);
            *out = (T*)((it == map->Data.end() || T::INTERFACE_TYPE < it->first) ? map->Data.end() : it)->second;
        }
        return has;
    }
    const C3DVector& GetPos() const;                    // 0x004C73D0
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingObject : public CThingGameObject { };
class CThingCreatureBase : public CThingGameObject { };
class CThingAICreature : public CThingCreatureBase { };

class CTCVillage : public CTCBase {
public:
    bool IsWorkTime() const;                            // 0x00658B20
};
class CTCVillageMember : public CTCBase {
public:
    enum { INTERFACE_TYPE = 0x23 };
    CTCVillage* GetPVillage() const;                    // 0x007EE540
};

class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

// Layout: engine/CCreatureActionBase.h is the flat generated form (vptr as a data member, no virtual dtor); the call needs the polymorphic base with the virtual ~CCreatureActionBase, so it is modelled here.
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();                     // 0x00693EF0
private:
    unsigned char _body[0xa4];                          // +0x04
};
class CCreatureAction_UseObject : public CCreatureActionBase {
public:
    CCreatureAction_UseObject(CThingCreatureBase& creature, CThing& usedThing, bool allowTurn);   // 0x0069F230
private:
    CIntelligentPointer<CThing> PUsedThing;             // +0xA8
    void*         PreviousPhysicsComponent;             // +0xB0
    unsigned char Dummy[0x18];                          // +0xB4 CRightHandedSet
    C3DVector     DummyPos;                             // +0xCC
    float         Proximity;                            // +0xD8
    bool          UsingDummy;                           // +0xDC
    bool          MovingToDummy;                        // +0xDD
    bool          TurningToDummy;                       // +0xDE
    bool          AllowTurn;                            // +0xDF
};

enum EAIActionReturn { };
enum EAIActionRepeat { };
enum EMovementType { };
enum EScriptAIPriority { };
class CCharStringData;
class CCharString { public: CCharStringData* PStringData; };
class CAIStateGroupBase : public CBaseClass {
public:
    CAIBrain*         PBrain;                           // +0x04
    EScriptAIPriority ScriptAIPriority;                 // +0x08
    CCharString       Name;                             // +0x0C
    bool              Terminated;                       // +0x10
    unsigned char     Priority;                         // +0x11
    bool              Enabled;                          // +0x12
    void Terminate();                                   // 0x008FCF40
protected:
    __forceinline CThingAICreature& GetCreature() const { return *PBrain->Creature; }
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& arrived, EMovementType movement,
        bool a, bool b, const CThing* thing, bool c, float d);   // 0x0097EC00
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait);   // 0x0097DB70
};

template <class TBase> class CAIStateGroup_WorkBase : public TBase {
protected:
    __forceinline bool IsInWorkHours() const
    {
        CTCVillageMember* member;
        if (this->GetCreature().GetTC(&member)) {
            CTCVillage* village = member->GetPVillage();
            if (village && village->IsWorkTime())
                return true;
        }
        return false;
    }
};

class CAIStateGroup_HandyMan : public CAIStateGroup_WorkBase<CAIStateGroupBase> {
public:
    CThingObject* WorkSpot;                             // +0x14
    virtual void Update();
};

void CAIStateGroup_HandyMan::Update()
{
    C3DVector spot = WorkSpot->GetPos();
    EAIActionReturn result;
    do {
        if (!IsInWorkHours()) {
            Terminate();
            return;
        }
        if (!WorkSpot || WorkSpot->IsDead()) {
            Terminate();
            return;
        }
        bool arrived = false;
        result = AIAction_MoveToPos((EAIActionRepeat)2, spot, arrived, (EMovementType)3, true, true, 0, true, 0.0f);
    } while (result == 4);
    if (result == 2) {
        bool failed = AIAction_PerformActionAndWait(CCreatureAction_UseObject(GetCreature(), *WorkSpot, true), false) != 2;
        if (failed) {
            Terminate();
            return;
        }
    }
    Terminate();
}