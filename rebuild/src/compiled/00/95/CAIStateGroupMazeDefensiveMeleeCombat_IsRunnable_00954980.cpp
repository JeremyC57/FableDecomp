// CAIStateGroup_MazeDefensiveMeleeCombat::IsRunnable() -- runnable when the creature's targeting component has a live target; adopt it as PTargetThing
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- HasTC + the ternary GetTC model into an UNINITIALISED local (the miss arm reads the key slot): both arms load the targeting component straight into ecx
// Leaf: ?IsRunnable@CAIStateGroup_MazeDefensiveMeleeCombat@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00954980, length 157 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (the const instantiation of the inlined interface lookup, type 8,
// tested as `mov ecx,[eax+20h] ; test ch,1`), the component's virtual slot 29 (+0x74, the current target),
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10.
// Vtable 0x01238C6C = CIntelligentPointer<CThing>.
//
// Shape: the const out-parameter GetInterface with its result ignored (the component lands straight in ecx for
// the virtual call; the miss arm reads the never-written local from the key slot), an inline `thing && !dead`
// test on the returned pointer, then `PTargetThing = CIntelligentPointer<CThing>(target)` through the stack
// temporary (whose 8-byte slot also homes the lookup key) and `return true`; the shared `xor al,al` tail last.
// The component class is not identified (interface type 8): descriptive names. The engine header
// CAIStateGroup_MazeDefensiveMeleeCombat.h is flattened data-only: local model (PDB: LastHealFrame +0x20).
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;     // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];                 // +0x4c..+0x90
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline T* GetTC(long wanted)        // the landed 0x00584157 lookup: the entry pick as one ternary, the component read once
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || wanted < it->first) ? map->last : it)->second;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};

struct CTCTargeting {                           // interface type 8 (descriptive)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetTarget();                // slot 29 (+0x74)
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    virtual bool IsRunnable();
};

struct CAIStateGroup_MazeDefensiveMeleeCombat : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTargetThing;   // +0x14
    long _base1c;                               // +0x1c
    long LastHealFrame;                         // +0x20
    virtual bool IsRunnable();
};

bool CAIStateGroup_MazeDefensiveMeleeCombat::IsRunnable()
{
    CTCTargeting* targeting;
    if (PBrain->PCreature->HasTC(8))
        targeting = PBrain->PCreature->GetTC<CTCTargeting>(8);
    CThing* target = targeting->GetTarget();
    if (target && !(target->m_flags91 & 1)) {
        PTargetThing = CIntelligentPointer<CThing>(target);
        return true;
    }
    return false;
}