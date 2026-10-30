// CAIStateGroup_HandyMan::Update() -- while the villager's village is in its work time and the work spot exists, walk to the spot and use it; then terminate
// Grade: DIFFER(332v332) IMPROVED (28 -> 20 difflines): in the inlined lower_bound, computing len before 'it = first' reproduces retail's first-in-eax copy to esi. Left: loop-head brain/creature/bitset chain eax/eax/ecx (retail ecx/eax/edx) and the UseObject argument loads (retail loads WorkSpot into ecx before the brain chain). Tried: a first local in lower_bound (=base), direct pad-cast brain reads instead of Brain() (30 lines). Next: a CAIStateGroupBase-derived model with a real PBrain member (the generated HandyMan header flattens the base).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091B190, length 332 bytes.
// Callees: CThing::GetPosition @ 0x004C73D0, the vector-map lower_bound inlined (interface 0x23 = CTCVillageMember as a
//          bitset::test byte), CTCVillageMember::GetPVillage @ 0x007EE540, CTCVillage::IsCookTime @ 0x00658B20,
//          AIAction_MoveToPos @ 0x0097EC00 (repeat 2, re-issued while it reports 4), CCreatureAction_UseObject(creature, thing,
//          bool) @ 0x0069F230, AIAction_PerformActionAndWait @ 0x0097DB70, ~CBaseIntelligentPointer @ 0x00A01C10 (the
//          action's PUsedThing +0xA8; action layout as the landed 0x0069F230, 0xE0 bytes), ~CCreatureActionBase @ 0x00693EF0,
//          Terminate @ 0x008FCF40.
// Shape: the spot position is copied once into a C3DVector local; 1 lives in ebx (also the bitset mask and the dead-bit
//          test); every failed condition breaks to the one Terminate; the action result is a named bool that is never
//          read (retail keeps its `setne bl`).
// Layout: engine/CAIStateGroup_HandyMan.h (WorkSpot +0x14) behind engine/CAIStateGroupBase.h.
#include "engine/CAIStateGroup_HandyMan.h"

extern "C" int __cdecl puts(const char* text);              // 0x00BFEAF2 (MSVCR71 import thunk)
extern "C" __declspec(noreturn) void __cdecl abort(void);   // 0x00BFEAEC
namespace std {
template <unsigned int _Nb>
class bitset {                                              // STLport 4.6 bitset<274>: 9 words, test() range-checked (no exceptions)
public:
    enum { _Words = (_Nb + 31) / 32 };
    unsigned long _M_w[_Words];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static unsigned int _S_whichbit(unsigned int __pos) { return __pos % 32; }
    static unsigned long _S_maskbit(unsigned int __pos) { return 1UL << _S_whichbit(__pos); }
    unsigned long _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
    bool _Unchecked_test(unsigned int __pos) const { return (_M_getword(__pos) & _S_maskbit(__pos)) != 0; }
    bool test(unsigned int __pos) const
    {
        if (__pos >= _Nb) {
            puts("bitset");
            abort();
        }
        return _Unchecked_test(__pos);
    }
};
}
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)        // STLport lower_bound (inlined in this TU)
    {
        int len = int(last - first);
        SEntry* it = first;
        while (len > 0) {
            int half = len / 2;
            SEntry* middle = it + half;
            if (middle->type < *type) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
};
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(const C3DVector& v) { x = v.x; y = v.y; z = v.z; }
};
struct CThing {
    unsigned char _pad0[0x20];
    std::bitset<274> UsingComponents;             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
    bool TestTC(long interfaceType) const
    {
        return UsingComponents.test(interfaceType);
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return has;
    }
};
struct CThingObject : CThing { };
struct CTCVillage {
    bool IsCookTime();                          // 0x00658B20
};
struct CTCVillageMember {                       // interface 0x23
    CTCVillage* GetPVillage() const;            // 0x007EE540
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_UseObject : CCreatureActionBase {
    CBaseIntelligentPointer PUsedThing;         // +0xa8
    void* PreviousPhysicsComponent;             // +0xb0
    unsigned char Dummy[0x18];                  // +0xb4 CRightHandedSet
    float DummyPos[3];                          // +0xcc
    long Proximity;                             // +0xd8
    bool UsingDummy;                            // +0xdc
    bool MovingToDummy;                         // +0xdd
    bool TurningToDummy;                        // +0xde
    bool AllowTurn;                             // +0xdf
    CCreatureAction_UseObject(CThing* creature, CThing* usedThing, bool allowTurn);   // 0x0069F230
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupHandyManMethods : CAIStateGroup_HandyMan {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* avoid, bool c, float speed);    // 0x0097EC00
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    void Terminate();                           // 0x008FCF40
    void Update();
};

void CAIStateGroupHandyManMethods::Update()
{
    C3DVector spot = WorkSpot->GetPosition();
    long result;
    do {
        CTCVillageMember* member;
        if (!Brain()->PCreature->GetInterfaceT(&member, 0x23))
            goto done;
        CTCVillage* village = member->GetPVillage();
        if (!village || !village->IsCookTime())
            goto done;
        if (!WorkSpot || (WorkSpot->m_flags91 & 1))
            goto done;
        bool reached = false;
        result = AIAction_MoveToPos(2, spot, &reached, 3, true, true, 0, true, 0.0f);
    } while (result == 4);
    if (result == 2) {
        bool failed = AIAction_PerformActionAndWait(&CCreatureAction_UseObject(Brain()->PCreature, WorkSpot, true), false) != 2;
        if (failed) {
            Terminate();
            return;
        }
    }
done:
    Terminate();
}
