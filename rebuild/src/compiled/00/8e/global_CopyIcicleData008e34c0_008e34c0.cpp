// _global::Copy_IcicleData_008e34c0(const CIcicleData* first, const CIcicleData* last, CIcicleData* result, const random_access_iterator_tag&, int*) -- assign [first, last) of ice-troll icicle records onto result (two intelligent pointers, two timers, a position), return the advanced result
// Leaf: Copy_IcicleData_008e34c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E34C0, length 125 bytes (`ret 0Ch`).
// Identity: STLport `__copy<CIcicleData*>` as the landed caller NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::OnDeactivate
//   0x008D9650 declares it (its inlined vector::erase); Ego_r.pdb CIcicleData is 0x24 bytes with these members. The
//   catalog `_Move<std::pair<CIntelligentPointer<CThing const>,NStealth::CDangerStatus>*>` is propagated (that pair is 16).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (twice per element).
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   Each intelligent-pointer assignment is SetPItem(other.GetPItem()); the C3DVector block is copied through lea pointers.
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

struct CBaseObject;

struct CBaseIntelligentPointer {                // vtable +0, item handle +4
    void* PData;                                // +0x04
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CThing;

struct C3DVector { float X, Y, Z; };           // POD (implicit assignment: the pointer-register block copy)

struct CIcicleData {                            // NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData, Ego_r.pdb 0x24 bytes
    CIntelligentPointer<CThing> PEffect;        // +0x00
    CIntelligentPointer<CThing> PTrap;          // +0x08
    long                        EffectTimer;    // +0x10
    long                        TrapTimer;      // +0x14
    C3DVector                   Position;       // +0x18
};

CIcicleData* __fastcall Copy_IcicleData_008e34c0(const CIcicleData* first, const CIcicleData* last, CIcicleData* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}