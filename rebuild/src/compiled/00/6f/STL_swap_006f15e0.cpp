// std::swap(CDefendingCombatantInfo::CProng& a, CDefendingCombatantInfo::CProng& b) -- exchange two combat-ring prongs (blocked flag, prong position, blockage timer, occupier pointer) through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F15E0, 199 bytes (+ int3 padding), /Gr `__fastcall` (a in ecx -> edi, b in edx
//   -> esi), plain `ret`; 0x1c-byte frame temporary.
// Identity: the CDefendingCombatantInfo::CProng vector TU (landed vector<CProng> copy ctor 0x006F20A0 and
//   __uninitialized_copy 0x006F14A0); callers are the out-of-line iter_swap 0x006F22F0 (a bare `jmp`) and the partition
//   step 0x006F2B86. Ego_r.pdb CProng (0x1c): bool Blocked +0, C3DVector Prong +1 (1-aligned), long ProngBlockageTimer
//   +0x10, CIntelligentPointer<CThingCreatureBase> POccupier +0x14 (vtable 0x0123E7D8).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90, dtor 0x00A01C10.
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` with CProng's implicit copy constructor and
//   assignment. The pointer's copy constructor is the out-of-line base constructor, the derived vtable store, then
//   `SetPItem(o.GetPItem())`; its assignment is `SetPItem(o.GetPItem())`; C3DVector has a memberwise user copy
//   constructor, so assignment copies it as a block through a pointer (`lea ebx,[esi+1] ; mov eax,ebx`). The
//   temporary's pointer dies through the base destructor (generated derived destructor, no vtable reset).
class CBaseObject;
class CThingCreatureBase;

#pragma pack(push, 1)
class C3DVector { public:                                      // 1-aligned in retail (CProng::Prong sits at +1)
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
#pragma pack(pop)

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* PItem;                                 // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0123E7D8 for CThingCreatureBase
public:
    virtual void v01();
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CDefendingCombatantInfo {
public:
    class CProng {                                      // Ego_r.pdb, 0x1c bytes
    public:
        bool                                    Blocked;              // +0x00
        C3DVector                               Prong;                // +0x01
        long                                    ProngBlockageTimer;   // +0x10
        CIntelligentPointer<CThingCreatureBase> POccupier;            // +0x14
    };
};

namespace std {
template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap<CDefendingCombatantInfo::CProng>(CDefendingCombatantInfo::CProng&, CDefendingCombatantInfo::CProng&);
}