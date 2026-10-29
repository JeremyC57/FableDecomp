// NTCScriptedControl::CActionTurnToFacePosition::Update() -- make the thing look at the stored position and finish once it lies inside the view cone (or at once when the thing has no look component)
// Leaf: ?Update@CActionTurnToFacePosition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00714CC0, 100 bytes (+12 int3 padding); vtable 0x01265534 slot 1.
// FableWin.pdb: ?Update@CActionTurnToFacePosition@NTCScriptedControl@@UAEXXZ; ctor
//          ??0CActionTurnToFacePosition@NTCScriptedControl@@QAE@ABVC3DVector@@M@Z (position, angle).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x43 = CTCLook: bit 3 of the bitset byte
//          +0x28), CTCLook::LookAtPos @ 0x00753E90 (priority 0x21), CTCLook::IsPosInViewRange @ 0x007534B0 -- the same
//          calls and model as the landed CActionFollowCreature::Update @ 0x00713EE0.
// Layout: CActionBase {vptr, PTCScript +4, Activated +8, Terminated +9}; Position (C3DVector) +0x0C, Angle (float) +0x18;
//          CTCScriptedControl ParentThing +4 (CTCBase); CThing UsingComponents +0x20, Components +0x44.
// Shape: no look component branches straight to the Terminated store; the in-view result also lands on it.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);          // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    void*         __vftable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];              // +0x20
    CInterfaceMap Components;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCLook { public:                                   // interface 0x43
    void LookAtPos(const C3DVector* position, long priority);           // 0x00753E90
    bool IsPosInViewRange(const C3DVector& pos, float angle, bool a, bool b);   // 0x007534B0
};
class CTCScriptedControl { public:
    void*   __vftable;
    CThing* ParentThing;                           // +0x04 (CTCBase)
};

namespace NTCScriptedControl {
class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                         // slot 1
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
};

class CActionTurnToFacePosition : public CActionBase {
public:
    virtual void Update();
    C3DVector Position;                            // +0x0C
    float     Angle;                               // +0x18
};

void CActionTurnToFacePosition::Update()
{
    CTCLook* look;
    if (PTCScript->ParentThing->GetInterface(&look, 0x43)) {
        look->LookAtPos(&Position, 0x21);
        if (look->IsPosInViewRange(Position, Angle, false, false))
            Terminated = true;
    } else
        Terminated = true;
}
}