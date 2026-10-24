// CCombatActionBase::CStrikeEvent::`scalar deleting destructor'(unsigned int) -- free the struck-thing list's buffer, destroy the weapon intelligent pointer and the helper-point name, and free the event itself on bit 0
// Leaf: ??_GCStrikeEvent@CCombatActionBase@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085AE90, length 57 bytes (+7 int3), thiscall `ret 4`. No call or address
//   reference reaches it in retail (an unreferenced COMDAT copy).
// Identity: the members it destroys match Ego_r.pdb CCombatActionBase::CStrikeEvent exactly -- HelperPointName (CCharString)
//   +0x04, PWeapon (CIntelligentPointer<const CThing>) +0x14, StruckThingList (vector<CThingGameObject*>, 12 bytes in the
//   retail STLport) +0x1C; three bools +0..+2 and three longs +8..+0x10 need no destruction. Neighbours are the
//   CCombatActionBase TU (SetTarget 0x0085AA90, CCountedPointer<CCombatActionBase> 0x0085A840). No vtable store: the class
//   is not polymorphic, so `??_G` is the `QAE` (non-virtual) form that VC7.1 emits for a plain `delete p`.
// Label correction: catalog "CParticleMorphs_Destroy" is a propagated label.
// Callees: free @ 0x00BFEA14 (the inline STLport vector destructor's deallocate), CBaseIntelligentPointer::
//   ~CBaseIntelligentPointer @ 0x00A01C10 (landed model 0x0085AA90), ~CCharString @ 0x0099EAE0, operator delete @ 0x00BFE9BC.
// Shape: members destroyed in reverse declaration order; the intelligent pointer's implicit destructor is the base's
//   out-of-line one (no vtable store); the `delete` helper below only makes VC emit the deleting destructor.
extern "C" void __cdecl free(void* block);

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;                                 // +0x04
    ~CBaseIntelligentPointer();                         // 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();
};
class CThing;
class CThingGameObject;
namespace std {
template <class T> class vector {                       // retail STLport vector: start / finish / end of storage
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector()
    {
        if (_M_start)
            free(_M_start);
    }
};
}

class CCombatActionBase {
public:
    class CStrikeEvent {                                // Ego_r.pdb, 0x28 bytes in retail
    public:
        bool                                Legacy;             // +0x00
        bool                                PrimaryLegacy;      // +0x01
        bool                                Weapon;             // +0x02
        CCharString                         HelperPointName;    // +0x04
        long                                StrikeEventIndex;   // +0x08
        long                                StartFrame;         // +0x0C
        long                                EndFrame;           // +0x10
        CIntelligentPointer<const CThing>   PWeapon;            // +0x14
        std::vector<CThingGameObject*>      StruckThingList;    // +0x1C
    };
};

void DeleteStrikeEvent(CCombatActionBase::CStrikeEvent* strikeEvent)
{
    delete strikeEvent;
}