// std::pair<const CCharString, CIVCountedPointer<CAnimComponentBase> >::~pair() -- implicit destructor of the anim-component map's value: release second, then destroy first
// Leaf: ??1?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E0A80, 35 bytes, thiscall: second.Release() inline (null test, `dec [ecx+4]`,
//   virtual Delete (slot 1) at zero, `second.Object = 0`), then `jmp ~CCharString` 0x0099EAE0 on first (+0).
// Identity (element type proven, symbol tentative -- see NOTES): no call site (retail keeps unreferenced COMDATs). This TU
//   (0x005E0xxx-0x005E3xxx) instantiates STLport map<CCharString, CIVCountedPointer<CAnimComponentBase> > (Ego_r.pdb
//   map<CCharString,CIVCountedPointer<CAnimComponentBase>,less<CCharString> >, the Dinkumware _Tree::_Erase donor 0x00640AE0):
//   the landed _Rb_tree::_M_erase 0x005E1F30 inlines exactly this body for each node value (+0x10 CCharString, +0x14 counted
//   pointer), the landed _Cons_val 0x005E0F40 builds the same pair, and the 0x18-byte-node _Rb_tree ctor 0x005E0B00 sits
//   right after this row. The same TU keeps both copies of the EAnimComponent pair's destroy body (0x005E0160 / 0x005E0C00,
//   0 callers each), i.e. the implicit ~pair and STLport _Destroy<pair> are both emitted; this row, the first of the three
//   CCharString-pair copies (0x005E0A80, 0x005E0AB0, 0x005E1790), is taken as the value_type's ~pair. The only PDB pair with
//   a CCharString first and a CIVCountedPointer second is over CAnimComponentBase. The manifest "~C3DAnimationBlendState"
//   (C3DAnimationBlendState ends in ~CBaseClass, see 0x004C2DF0) is propagated.
// Callee: CCharString::~CCharString 0x0099EAE0 (??1CCharString@@QAE@XZ); CAnimComponentBase slot 1 = CIVCountedPointeeBase::Delete.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"              // vptr, slot 1 Delete() const, inline Release() (returns the new count)

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                     // 0x0099EAE0
    CCharStringData* PStringData;
};

class CAnimComponentBase;

template <class T> class CIVCountedPointer {
public:
    ~CIVCountedPointer() { Release(); }                 // inline (Ego_r/FableWin ??1?$CIVCountedPointer@...)
    void Release()                                      // inline (?Release@?$CIVCountedPointer@...@@QAEXXZ)
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointeeBase* Object;                      // Ego_r.pdb: CIVCountedPointeeBase* Object
};

namespace std {
// STLport 4.6 stl/_pair.h (the members; no constructor is needed for the destructor)
template <class _T1, class _T2>
struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
}

// Emission helper (not a retail symbol): forces the implicit destructor out of line as its own COMDAT.
void EmitAnimComponentPairDestructor(std::pair<const CCharString, CIVCountedPointer<CAnimComponentBase> >* value)
{
    value->~pair();
}