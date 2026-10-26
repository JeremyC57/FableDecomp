// _global::UninitCopy_CProng_006f10f0(const CProng* first, const CProng* last, CProng* result, const __false_type&) -- STLport __uninitialized_copy<CDefendingCombatantInfo::CProng*>: placement copy-construct 28-byte combat-wheel prongs, return the advanced result
// Leaf: UninitCopy_CProng_006f10f0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F10F0, length 131 bytes (+ int3 padding), `ret 8`.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90.
// Vtable: CIntelligentPointer<CThingCreatureBase> 0x0123E7D8 (stored after the base constructor).
// Call sites: 0x006F3F67 and 0x006F3FB2 (vector<CProng> helpers); byte-identical to this chunk's 0x006F14A0 (the copy the
// landed vector<CProng>::operator= @ 0x006F3C20 calls) -- a separate COMDAT copy.
// Layout (Ego_r.pdb CDefendingCombatantInfo::CProng, 28 bytes; landed CProng::SetPOccupyingAttacker @ 0x006F17A0):
//   Blocked bool +0x00, Prong C3DVector +0x01 (byte-packed: the C3DVector record has alignment 1), ProngBlockageTimer
//   +0x10, POccupier CIntelligentPointer<CThingCreatureBase> +0x14.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` (`__fastcall`: first in ecx, last in
// edx -- both spilled with result to the frame -- result + the tag reference on the stack). Placement new (null test on
// the destination) of CProng's copy constructor: the bool byte, the packed vector's three words at +1/+5/+9, the timer,
// then the pointer's copy (CBaseIntelligentPointer() + the derived vtable store + SetPItem(src.GetPItem())). The advanced
// result is returned (kept in the result argument slot across the calls).
// Label correction: the catalog's `_Copy_impl<NPlayerTargeting::CTargetPoint*,...>` is propagated.
inline void* operator new(unsigned int, void* p) { return p; }

class CBaseObject;
class CThingCreatureBase;

struct CBaseIntelligentPointer {
    void* PData;                                 // +0x04
    CBaseIntelligentPointer();                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();          // 0x00A01C10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x0123E7D8 for CThingCreatureBase
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

#pragma pack(push, 1)
class C3DVector { public:                               // byte-packed record (the PDB places CProng::Prong at +1)
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)

struct CProng {                                  // 0x1c bytes
    bool                                    Blocked;              // +0x00
    C3DVector                               Prong;                // +0x01
    long                                    ProngBlockageTimer;   // +0x10
    CIntelligentPointer<CThingCreatureBase> POccupier;            // +0x14
};

struct __false_type { __false_type() { } };

CProng* __fastcall UninitCopy_CProng_006f10f0(const CProng* first, const CProng* last, CProng* result, const __false_type&)
{
    CProng* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CProng(*first);
    return cur;
}