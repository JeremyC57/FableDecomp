// CAIStateGroup_HoverHornetQueen::WaitWhileBabiesFight() -- keep out of the way while the queen is still runnable, the boss still has living babies and is not ready for a phase change
// Grade: RELOCATION_MATCH 307/307
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095C0B0, 307 bytes. Ego_r.pdb: ?WaitWhileBabiesFight@CAIStateGroup_HoverHornetQueen@@IAE?AW4EAIActionReturn@@XZ.
// Callees: CVectorMap::LowerBound 0x0040F020, CBaseIntelligentPointer::GetPItem 0x00A01B50, CAIStateGroupBase::IsStopping
// 0x008FD010 (PDB IBE_NXZ), CTCBoss::CountLivingHelpers 0x007AEEC0 (PDB QAEJXZ), CTCBoss::IsReadyForPhaseChange 0x007AEE90,
// KeepOutOfWay 0x0095B6F0 (result ignored); returns 2.
// Shape: the loop condition is IsStillRunnable (0x0095A420) inlined -- the landed body, whose NAMED reference to
// CircleMarkers[0] gives the marker esi and keeps the loop-carried boss in edi (IS_THING_ALIVE(CircleMarkers[0]) rotates
// them). The 0xad (CTCBoss) lookup runs twice: before the loop through the out-of-line LowerBound (key and result share one
// frame word), and in the loop (the inlined CountBabies, PDB ?CountBabies@...@@IAEJXZ) as an in-place STLport __lower_bound
// whose miss path keeps the previous boss value (edi seeded from the first lookup).
// The engine header CAIStateGroup_HoverHornetQueen.h flattens the vptr and the vectors to byte arrays: local model with the retail offsets
// (retail 12-byte STLport vectors put CircleMarkers at +0x50; the PDB's 16-byte vectors would put it at +0x54).
// Leaf: WaitWhileBabiesFight@CAIStateGroup_HoverHornetQueen
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "fable_compiler.h"
class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    template<class T> FABLE_FORCEINLINE void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
        }
    }
    template<class T> FABLE_FORCEINLINE void GetInterfaceInline(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            CInterfaceMap::SEntry* last = m_interfaces.last;
            CInterfaceMap::SEntry* it = m_interfaces.first;
            long len = last - it;
            while (len > 0) {
                long half = len / 2;
                if (it[half].first < interfaceType) {
                    it += half + 1;
                    len = len - half - 1;
                }
                else
                    len = half;
            }
            *ppOut = (T*)((it == last || interfaceType < it->first) ? last : it)->second;
        }
    }
};
class CThingCreatureBase : public CThing { public: };
class CTCBoss { public:                         // interface 0xad
    long CountLivingHelpers();                  // 0x007AEEC0
    bool IsReadyForPhaseChange() const;         // 0x007AEE90
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer { public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;              // 0x00A01B50
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1);
}
struct CThingPointerVector {
    CIntelligentPointer<CThing>* _M_start;
    CIntelligentPointer<CThing>* _M_finish;
    CIntelligentPointer<CThing>* _M_end_of_storage;
    CIntelligentPointer<CThing>* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    CIntelligentPointer<CThing>& operator[](unsigned long n) const { return *(begin() + n); }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
enum EAIActionReturn { };
#include "engine/C3DVector.h"
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated, Priority, Enabled, _pad13;
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
protected:
    bool IsStopping() const;                    // 0x008FD010
};
struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTargetThing;   // +0x14
};
class CAIStateGroup_HoverHornetQueen : public CAIStateGroup_EnemyCreatureBase {
public:
    C3DVector StartPos;                         // +0x1c
    C3DVector ArenaCentrePos;                   // +0x28
    bool bOneTime;                              // +0x34
    CThing* PEmitter;                           // +0x38
    CThing* PEmitter2;                          // +0x3c
    CThingPointerVector Things;                 // +0x40
    long CurrentCirclePoint;                    // +0x4c
    CThingPointerVector CircleMarkers;          // +0x50
protected:
    inline bool IsStillRunnable()
    {
        if (!IS_THING_ALIVE(PTargetThing))
            return false;
        if (CircleMarkers.size() > 0) {
            CIntelligentPointer<CThing>& marker = CircleMarkers[0];
            if (marker.GetPItem() == 0)
                return false;
            if (marker.GetPItem()->IsDead())
                return false;
        }
        if (IsStopping())
            return false;
        return true;
    }
    EAIActionReturn KeepOutOfWay();             // 0x0095B6F0
    EAIActionReturn WaitWhileBabiesFight();
};

EAIActionReturn CAIStateGroup_HoverHornetQueen::WaitWhileBabiesFight()
{
    CTCBoss* boss;
    GetCreature()->GetInterface(&boss, 0xad);
    CTCBoss* current = boss;
    while (IsStillRunnable()) {
        GetCreature()->GetInterfaceInline(&current, 0xad);
        if (current->CountLivingHelpers() <= 0 || boss->IsReadyForPhaseChange())
            break;
        KeepOutOfWay();
    }
    return (EAIActionReturn)2;
}