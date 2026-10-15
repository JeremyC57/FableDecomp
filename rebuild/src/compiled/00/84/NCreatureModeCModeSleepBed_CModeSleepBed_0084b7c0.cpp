// NCreatureMode::CModeSleepBed::CModeSleepBed(CThing* thing, const CCharString& sleepAnimName, const CCharString& group) -- CModeBase(thing), own vtable, the animation-group filter, the sleep animation name, a null remove-action pointer
// Leaf: ??0CModeSleepBed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084B7C0, length 59 bytes.
// Callees: CModeBase::CModeBase(CThing*) @ 0x0073EAC0, CAnimFilterOfGroup::CAnimFilterOfGroup(const CCharString&)
// @ 0x0073E8F0, CCharString::CCharString(const CCharString&) @ 0x0099EC30 (all as in landed sources).
// Identity: vtable 0x01275760 = NCreatureMode::CModeSleepBed (rtti-vtables.tsv); the manifest's
// "CTCCarriedActionUseTrophy::CTCCarriedActionUseTrophy" label is propagated and rejected.
// Layout: Ego_r.pdb CModeSleepBed (0x34 bytes): CModeBase 0x1c, Filter +0x1c (CAnimFilterOfGroup, 0xc),
// SleepAnimName +0x28, PRemoveAction +0x2c (CCountedPointer<CCreatureActionBase>, two zeroed words).
// Shape: the third argument feeds the filter and the second the name (argument order fixed by the
// `[esp+10h]` / `[esp+0Ch]` loads); init-list order = declaration order; the counted pointer's own
// inline constructor zeroes both words through the shared zero register.
struct CThing;
struct SCountBlock;
struct CCreatureActionBase;

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};

struct CAnimFilterOfGroup {                         // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                            // +4
    unsigned long GroupCRC;                         // +8
    CAnimFilterOfGroup(const CCharString& group);   // 0x0073E8F0
};

template <class T> struct CCountedPointer {
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

struct CModeBase {
    virtual void v00();
    unsigned char _body[0x18];                      // ParentThing, WeaponBasedFilter, FramesBeforeRemovingSelf, Active
    CModeBase(CThing* thing);                       // 0x0073EAC0
};

namespace NCreatureMode {

struct CModeSleepBed : CModeBase {                  // vtable 0x01275760
    CAnimFilterOfGroup                    Filter;          // +0x1c
    CCharString                           SleepAnimName;   // +0x28
    CCountedPointer<CCreatureActionBase>  PRemoveAction;   // +0x2c
    virtual void v00();
    CModeSleepBed(CThing* thing, const CCharString& sleepAnimName, const CCharString& group);
};

CModeSleepBed::CModeSleepBed(CThing* thing, const CCharString& sleepAnimName, const CCharString& group)
    : CModeBase(thing)
    , Filter(group)
    , SleepAnimName(sleepAnimName)
{
}

}