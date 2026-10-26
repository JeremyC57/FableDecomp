// CTCCreatureModeManager::SetupInitialMode() -- from the creature's CCreatureModeDef: enter the weapon mode when the creature carries something in its primary slot, swap an idle front mode for a random available idle mode, then enter the def's initial mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073AFC0, 271 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef<CCreatureModeDef>(CDefPointer<const CCreatureModeDef>*) @ 0x0073DBE0 (manifest
//   "GetSubDef<CAICreatureWillPowerIndicatorDef>" is a byte-twin propagation), CVectorMap<ETCInterfaceType,CTCBase*>::
//   LowerBound const @ 0x004365B0 (inlined GetInterface for type 0x46 = CTCCarrying, bit 6 of the presence word +0x28),
//   CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, AddMode @ 0x0073AE10 (x3), RemoveMode @ 0x0073ABA0, the front
//   mode's virtual slots 8 (+0x20, a bool "may be replaced by an idle mode" test) and 1 (+0x04, GetType),
//   CTCBase::GetWorld @ 0x00686A70, CWorld::GetPRandomSeed @ 0x0049D810, GFRandom(range, seed) @ 0x004CDAE0 (__fastcall),
//   the defs' virtual slot 1 (+0x04, delete at the last reference).
// Layout: CTCCreatureModeManager (Ego_r.pdb): PModes (list<CCountedPointer<CModeBase>>, retail 4-byte head) +0x0c;
//   CThing PDef +0x70, interface map +0x44; CCreatureModeDef (Ego_r.pdb 0x44): InitialCreatureMode +0x2c,
//   DefaultWeaponCreatureMode +0x30, AvailableIdleModes (vector<long>) +0x34.
// Shape: the thing's def copied into a counted holder (inline AddRef, release without a null test on the only path that
//   reaches it); the out CDefPointer is declared before the def test (its zero store sits above the `je`); the sub-def is
//   taken out of it into a holder right after GetSubDef (landed CTCChest 0x00750240 idiom); the idle-mode count is a
//   named local computed before GetWorld (the fastcall arguments would evaluate the seed first). GetSubDef needs the
//   same-TU stand-in body (out slot written inline, under auto_inline(off)): with only a declaration VC7.1 treats the
//   out pointer's address as escaping and gives the GetInterface key its own frame word (`sub esp,8`, this/holder swap
//   edi<->ebx); with the stand-in the key reuses the out pointer's `push ecx` word as in retail.
class CTCBase;
class CThing;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
class CDefPointeeBase { public:
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {          // a counted def reference released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
template <class T> struct CDefCopy {            // CDefPointer copy of a raw def: AddRef on construction
    T* Object;
    CDefCopy(T* object) : Object(object) { if (Object) ++Object->m_refCount; }
    ~CDefCopy()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
class CCreatureModeDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x2c - 0x08];
    long  InitialCreatureMode;                  // +0x2c
    long  DefaultWeaponCreatureMode;            // +0x30
    long* AvailableIdleModes_first;             // +0x34 vector<long>
    long* AvailableIdleModes_last;              // +0x38
    long* AvailableIdleModes_end;               // +0x3c
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CCreatureModeDef>* out);   // 0x0073DBE0
};
void* FindDef_CDefClassBase_GetSubDef();      // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 / 0x00744ED0 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCreatureModeDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CTCCarrying { public:
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                        // +0x70
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline T* GetTC(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
class CWorld { public:
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0

struct CModeBase {
    virtual void v00();
    virtual long GetType();                     // slot 1 (+0x04)
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual bool CanBeReplacedByIdleMode();     // slot 8 (+0x20)
};
struct SModeNode {                              // list node: next, prev, CCountedPointer<CModeBase>
    SModeNode* m_pNext;
    SModeNode* m_pPrev;
    CModeBase* m_pMode;
    void*      m_pModeInfo;
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCCreatureModeManager : public CTCBase { public:
    CThing*    ParentThing;                     // +0x04 (CTCBase)
    unsigned char _pad08[4];
    SModeNode* PModes;                          // +0x0c list head (the sentinel)
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
    bool AddMode(unsigned long mode);           // 0x0073AE10
    CModeBase* FrontMode() const { return PModes->m_pNext->m_pMode; }
    void SetupInitialMode();
};

void CTCCreatureModeManager::SetupInitialMode()
{
    CDefCopy<CDefClassBase> thingDef(ParentThing->PDef);
    CDefPointer<CCreatureModeDef> out;
    if (thingDef.Object) {
        bool found = thingDef->GetSubDef(&out);
        CDefHolder<CCreatureModeDef> def(out.Object);
        if (found) {
            if (ParentThing->HasTC(0x46)) {
                if (ParentThing->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot())
                    AddMode(def->DefaultWeaponCreatureMode);
            }
            if (FrontMode()->CanBeReplacedByIdleMode() && def->AvailableIdleModes_first != def->AvailableIdleModes_last) {
                RemoveMode(FrontMode()->GetType());
                unsigned long count = def->AvailableIdleModes_last - def->AvailableIdleModes_first;
                AddMode(def->AvailableIdleModes_first[GFRandom(count, GetWorld().GetPRandomSeed())]);
            }
            if (def->InitialCreatureMode)
                AddMode(def->InitialCreatureMode);
        }
    }
}