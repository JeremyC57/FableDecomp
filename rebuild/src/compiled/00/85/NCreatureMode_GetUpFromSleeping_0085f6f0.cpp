// NCreatureMode::GetUpFromSleeping(CThingCreatureBase& creature, CThing& bed, const CCharString& animation, CAIBrain* brain) -- get a creature out of bed: while it is in the sleep-in-bed mode, leave that mode with a get-out-of-bed action (true); otherwise, when the creature is free to act, post the action through its brain (or set it directly without one) and return false
// Leaf: ?GetUpFromSleeping@NCreatureMode@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085F6F0, 320 bytes (+ int3 padding), /Gr __fastcall (creature in ecx, bed in edx;
//   animation and brain on the stack), `ret 8`.
// Identity: Ego_r.pdb ?GetUpFromSleeping@NCreatureMode@@YI_NAAVCThingCreatureBase@@AAVCThing@@ABVCCharString@@PAVCAIBrain@@@Z.
// Callees: CVectorMap::LowerBound const 0x004365B0 (inline GetTC 0x31 = CTCCreatureModeManager), CTCCreatureModeManager::
//   GetCurrentMode 0x0073A6E0, the mode's slot 1 (its type; 0x0D = CModeSleepBed), CCreatureAction_GetOutOfBed ctor
//   0x00860060 (landed), CModeSleepBed::RemoveModeWithAction 0x008D7250, ~CBaseIntelligentPointer 0x00A01C10 (PBed, +0xa8),
//   ~CCreatureActionBase 0x00693EF0, CThingCreatureBase::IsFreeToPerformAction 0x00662210 / SetCurrentAction 0x006644F0,
//   CBrainOutput ctor 0x00835620 / dtor 0x006D60C0, the action's slot 11 (+0x2c) Clone, CCountedPointer::Reset 0x00668D90,
//   CAIBrain::SetBrainOutput 0x0088B8F0.
// Layout: CCreatureAction_GetOutOfBed = retail CCreatureActionBase (0xa8 bytes) + PBed CIntelligentPointer<CThing> +0xa8
//   (the landed 0x00860060 model); CBrainOutput 0x48 bytes, Flags +0, PAction +0x20 (the landed 0x0097D8B0 model).
// Shape: the mode manager is an unset local filled only when the creature has interface 0x31 (the absent path reads the
//   uninitialised word that shares the lookup key slot); the sleeping branch passes a temporary action (the ctor's
//   returned `this` pushed) and destroys it inline; the other branch's named action shares that frame slot, its Clone
//   goes through a CCreatureActionBase reference (a virtual call), and both branches' implicit action destructor is
//   the member then base destructor call pair. The named action sits in the else block: at function scope it gets
//   its own 0xb0-byte slot (frame 0x1ac v retail 0xfc).
class CThing;
class CBaseObject;
class CCharString;
class CAnimationEntry;

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<long, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CCreatureActionBase { public:                            // retail 0xa8 bytes
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual CCreatureActionBase* Clone() const;         // slot 11 (+0x2c)
    unsigned char _body[0xa8 - 4];
    virtual ~CCreatureActionBase();                             // 0x00693EF0
};

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
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

class CThingCreatureBase;
class CCreatureAction_GetOutOfBed : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PBed;                   // +0xa8
    CCreatureAction_GetOutOfBed(CThingCreatureBase* owner, CThing* bed, const CCharString& animation);   // 0x00860060
};

class CThingCreatureBase {
public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((void*)((it == map->last || interfaceType < it->first) ? map->last : it)->second);
    }
    bool IsFreeToPerformAction(const CCreatureActionBase& action) const;   // 0x00662210
    bool SetCurrentAction(const CCreatureActionBase& action);              // 0x006644F0
};

struct CCountedPointerBase {                            // CCountedPointer<CCreatureActionBase> {object, block}
    void* Object;
    void* Block;
    void Reset(void* object);                           // 0x00668D90
};
class CBrainOutput { public:                                   // 0x48 bytes
    unsigned long Flags;                                // +0x00 (8: perform action)
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointerBase PAction;                        // +0x20
    unsigned char _pad28[0x48 - 0x28];
    CBrainOutput();                                     // 0x00835620
    ~CBrainOutput();                                    // 0x006D60C0
};
class CAIBrain { public:
    void SetBrainOutput(const CBrainOutput& output);    // 0x0088B8F0
};

namespace NCreatureMode {
class CModeBase {
public:
    virtual void v00();
    virtual long GetType() const;                       // slot 1
};
class CModeSleepBed : public CModeBase {
public:
    void RemoveModeWithAction(const CCreatureActionBase& action);   // 0x008D7250
};
}

class CTCCreatureModeManager { public:                         // interface 0x31
    NCreatureMode::CModeBase& GetCurrentMode() const;   // 0x0073A6E0
};

namespace NCreatureMode {
bool __fastcall GetUpFromSleeping(CThingCreatureBase& creature, CThing& bed, const CCharString& animation, CAIBrain* brain)
{
    CTCCreatureModeManager* modeManager;
    if (creature.HasTC(0x31))
        modeManager = creature.GetTC<CTCCreatureModeManager>(0x31);
    CModeBase& mode = modeManager->GetCurrentMode();
    if (mode.GetType() == 0x0d) {
        static_cast<CModeSleepBed&>(mode).RemoveModeWithAction(CCreatureAction_GetOutOfBed(&creature, &bed, animation));
        return true;
    } else {
        CCreatureAction_GetOutOfBed action(&creature, &bed, animation);
        if (creature.IsFreeToPerformAction(action)) {
            if (brain) {
                CBrainOutput output;
                output.Flags |= 8;
                const CCreatureActionBase& base = action;
                output.PAction.Reset(base.Clone());
                brain->SetBrainOutput(output);
            } else
                creature.SetCurrentAction(action);
        }
        return false;
    }
}
}