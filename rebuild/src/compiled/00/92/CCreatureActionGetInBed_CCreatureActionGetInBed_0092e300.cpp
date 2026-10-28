// CCreatureAction_GetInBed::CCreatureAction_GetInBed(CThingCreatureBase&, CThing&, const CAnimationEntry&, const CCharString&, long, long) -- base action on the given animation (speed -1, flags 0 / 1), the bed pointer, the animation's group name, the sleep animation name and the two bed animation indices
// Leaf: ??0CCreatureAction_GetInBed@@QAE@AAVCThingCreatureBase@@AAVCThing@@ABVCAnimationEntry@@ABVCCharString@@JJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092E300, 177 bytes, thiscall, `ret 18h`. Vtable 0x01291BFC.
// Identity (proven): stores vtable 0x01291BFC; `ret 18h` = six arguments = Ego_r.pdb
//   ??0CCreatureAction_GetInBed@@QAE@AAVCThingCreatureBase@@AAVCThing@@ABVCAnimationEntry@@ABVCCharString@@JJ@Z (donor
//   0x00951700, the only non-copy constructor); the members it fills are Ego_r PBed, AnimGroup, SleepAnimName, BedGetInAnim,
//   BedSleepAnim (engine/CCreatureAction_GetInBed.h retail offsets +0xA8..+0xBC).
// Callees: CCreatureActionBase::CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&,
//   long, long) 0x00693B30, CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 / SetPItem 0x00A01B90
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), CDefStringTable::GetString(long) const 0x009D49B0 on the global string
//   table 0x013CA828 (inline CDefString -> CCharString conversion of CAnimationEntry::GroupName), CCharString(const
//   CCharString&) 0x0099EC30, ~CCharString 0x0099EAE0.
// Shape: the landed BanditKing ctor base call (one zero register for 0.0f, the 0 flag and the CSoundPair temporary); the
//   group name is converted to a CCharString temporary, copy-constructed into AnimGroup and destroyed.
// Layout: CAnimationEntry (Ego_r, 16 bytes): BankIndex +0, AnimName +4, GroupName +8 (CDefString = TablePos).
class CCharString {
public:
    CCharString(const CCharString& other);                   // 0x0099EC30
    ~CCharString();                                          // 0x0099EAE0
    void* PStringData;
};

class CDefStringTable {
public:
    CCharString GetString(long table_pos) const;             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                     // 0x013CA828 (descriptive)

class CDefString {
public:
    long TablePos;
    operator CCharString() const { return g_DefStringTable.GetString(TablePos); }
};

class CAnimationEntry {
public:
    long BankIndex;                                          // +0
    CDefString AnimName;                                     // +4
    CDefString GroupName;                                    // +8
};

class CSoundPair {
public:
    CSoundPair() : FirstSound(0), LastSound(0) { }
    long FirstSound;
    long LastSound;
};

class CThing {
public:
    virtual ~CThing();
};

class CThingCreatureBase : public CThing { };

class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    void SetPItem(CBaseObject* item);                        // 0x00A01B90
    void* PData;
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    virtual ~CIntelligentPointer();
};

class CCreatureActionBase {
public:
    CCreatureActionBase(CThingCreatureBase& creature, float a, float b, const CAnimationEntry& anim, const CSoundPair& sound,
                        long c, long d);                     // 0x00693B30
    virtual ~CCreatureActionBase();
    unsigned char _body[0xa8 - 4];                           // retail sizeof(CCreatureActionBase) 0xA8
};

class CCreatureAction_GetInBed : public CCreatureActionBase {
public:
    CIntelligentPointer<CThing> PBed;                        // +0xA8
    CCharString AnimGroup;                                   // +0xB0
    CCharString SleepAnimName;                               // +0xB4
    long BedGetInAnim;                                       // +0xB8
    long BedSleepAnim;                                       // +0xBC
    CCreatureAction_GetInBed(CThingCreatureBase& creature, CThing& bed, const CAnimationEntry& anim,
                             const CCharString& sleep_anim_name, long bed_get_in_anim, long bed_sleep_anim);
    virtual ~CCreatureAction_GetInBed();
};

CCreatureAction_GetInBed::CCreatureAction_GetInBed(CThingCreatureBase& creature, CThing& bed, const CAnimationEntry& anim,
                                                   const CCharString& sleep_anim_name, long bed_get_in_anim,
                                                   long bed_sleep_anim)
    : CCreatureActionBase(creature, -1.0f, 0.0f, anim, CSoundPair(), 0, 1)
    , PBed(&bed)
    , AnimGroup(anim.GroupName)
    , SleepAnimName(sleep_anim_name)
    , BedGetInAnim(bed_get_in_anim)
    , BedSleepAnim(bed_sleep_anim)
{
}