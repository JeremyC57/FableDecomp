// CTCCarriedActionUseFood::OnActionUse(CThing& user) -- base OnActionUse; eating applies the food's bonus-item def to the user's hero stats, plays the user's "EAT_FOOD" sound and kills the food
// Grade: DIFFER(ours v retail) -- the escaped def out pointer (homed in the dead user slot) is re-read into esi at the end of each path in retail (before the ApplyBonus push, on the skip path, after the sound block) and kept for the release; ours reads it into ecx for the push and once before Kill
// Leaf: ?OnActionUse@CTCCarriedActionUseFood@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EDA90, length 239 bytes (+ int3 padding). Vtable slot 28 ("vfunc_28"), `ret 4`.
// Callees: CTCActionUseBase::OnActionUse @ 0x007EAB90 (qualified; manifest "$E2"), CDefClassBase::GetSubDef<CBonusItemDef> @ 0x007E1D30
// (on the food's def, +0x70), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface: CTCHeroStats
// type 4, the sound emitter type 0x10), CTCHeroStats::ApplyBonusItemEffects(const CBonusItemDef&) @ 0x0057C4F1, CCharString(const
// char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, the emitter's PlaySound(const CCharString&, bool, long, long, long)
// @ 0x0066BD00, CThing::Kill(bool) @ 0x004C9B80, the def's virtual Destroy (slot 1). Literal "EAT_FOOD" 0x012715F4.
// Shape: the GetSubDef out pointer is a function-scope CDefPointer (homed by retail in the dead `user` slot, re-read at each use
// and released by its destructor); the sound name is a named local in the sound block.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CTCBaseInterface;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CBonusItemDef : CDefPointeeBase {
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CBonusItemDef>* out) const;   // 0x007E1D30
};
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
    unsigned char _pad4c[0x70 - 0x4c];
    const CDefClassBase* PDef;                  // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool immediately);                // 0x004C9B80
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CTCHeroStats {
public:
    void ApplyBonusItemEffects(const CBonusItemDef& def);   // 0x0057C4F1
};
class CTCSoundEmitter {                         // interface type 0x10
public:
    unsigned long PlaySound(const CCharString& name, bool positional, long a, long b, long c);   // 0x0066BD00
};
class CTCActionUseBase {
public:
    virtual ~CTCActionUseBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    virtual void OnActionUse(CThing& user);     // 0x007EAB90
};
class CTCCarriedActionUseFood : public CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);
};

void CTCCarriedActionUseFood::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    const CDefClassBase* thingDef = ParentThing->PDef;
    CDefPointer<CBonusItemDef> def;
    CTCHeroStats* stats;
    if (thingDef->GetSubDef(&def) && user.GetInterface(&stats, 4))
        stats->ApplyBonusItemEffects(*def.Object);
    CTCSoundEmitter* sound;
    if (user.GetInterface(&sound, 0x10)) {
        CCharString name("EAT_FOOD");
        sound->PlaySound(name, false, 0, 0, -1);
    }
    CBonusItemDef* object = def.Object;
    ParentThing->Kill(true);
    if (object) {
        if (--object->RefCount == 0)
            object->Destroy();
    }
}
