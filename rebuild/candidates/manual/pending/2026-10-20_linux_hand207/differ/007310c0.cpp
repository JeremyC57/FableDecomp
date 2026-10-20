// Grade: DIFFER(163v163) -- only the out def pointer's zero store: retail stores it after pushing its address (right before the GetSubDef call), ours before the Open(true) call (a local after Open loads the parent through eax instead)
// CTCDoor::UpdateOpenStatusTriggerTypeScript() -- when the script changed the open state, open (touching the appearance's DOOR_OPEN animation in the main mesh bank) or close the door, then latch OldOpen
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007310C0, length 163 bytes (+ int3 padding). __thiscall, `ret`.
//         Ego_r.pdb: `?UpdateOpenStatusTriggerTypeScript@CTCDoor@@IAEXXZ` (protected).
// Callees: CTCDoor::Open(bool) @ 0x00730C00, CTCDoor::Close() @ 0x00730DD0,
//          CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>*) const @ 0x0042B0A2,
//          CAnimationSet::GetAnim(const CCharString&, long, const CAnimFilterBase&) const @ 0x005DC340,
//          CTCBase::GetWorld @ 0x00686A70, the main mesh bank's GetAnimation(long) @ 0x00A242C0 (result unused);
//          global CCharString "DOOR_OPEN" @ 0x013BAD34; CAnimFilterNoGroup vtable 0x0122FFDC (base CAnimFilterBase 0x0122FFCC).
// Shape: GetSubDef's result is ignored; the sub-def is read once from the out slot into a holder that releases it with
//        `dec [edi+4] ; jne`; the filter is an inline temporary whose dtor stores the base vtable; both arms end in the
//        duplicated `OldOpen = CurrentOpen` tail.
// Layout: engine/CTCDoor.h (+0x04 `m4` is CTCBase::ParentThing); retail CThing PBaseDef.Object +0x70 (PDB +0x80);
//         CAppearanceDef::Animation +0x34 (PDB); retail CWorld::PMainMeshBank +0x3C.
#include "engine/CTCDoor.h"

class CCharString;

class CAnimFilterBase {                             // vtable 0x0122FFCC
public:
    virtual bool operator()(const class CAnimationEntry& entry) const;
    CAnimFilterBase() {}
    ~CAnimFilterBase() {}
};
class CAnimFilterNoGroup : public CAnimFilterBase { // vtable 0x0122FFDC
public:
    virtual bool operator()(const class CAnimationEntry& entry) const;
    CAnimFilterNoGroup() {}
};

class CAnimationEntry {
public:
    long BankIndex;                                 // +0x00
};

class CAnimationSet {
public:
    const CAnimationEntry& GetAnim(const CCharString& name, long n, const CAnimFilterBase& filter) const;   // 0x005DC340
};

class CAppearanceDef {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                        // +0x34
};

template<class T> class CDefPointer {               // the out slot: the def object only
public:
    T* Object;
    CDefPointer() : Object(0) {}
};

class CDefClassBase {
public:
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0042B0A2 for T = CAppearanceDef
};

struct CAppearanceDefHolder {                       // releases the sub-def read once from the out slot
    const CAppearanceDef* Def;
    CAppearanceDefHolder(const CAppearanceDef* def) : Def(def) {}
    ~CAppearanceDefHolder()
    {
        if (Def && --Def->RefCount == 0)
            Def->Destroy();
    }
};

struct C3DAnimationInfo;
struct CMeshDataBank {
    C3DAnimationInfo* GetAnimation(long index);     // 0x00A242C0
};
struct CWorld {
    unsigned char _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;                   // +0x3C
};

struct CBaseDefPointer {                           // CDefPointer<const CThingBaseDef>
    const CDefClassBase* Object;
    const CDefClassBase* operator->() const { return Object; }
};

struct CThing {
    unsigned char _pad00[0x70];
    CBaseDefPointer PBaseDef;                       // +0x70 CDefPointer<const CThingBaseDef>
};

struct CTCBase {
    CWorld* GetWorld();                             // 0x00686A70
};

extern CCharString g_CCharString_013bad34;          // 0x013BAD34 "DOOR_OPEN"

struct CTCDoor_Methods : CTCDoor {
    CThing* GetParentThing() const { return (CThing*)m4; }
    void Open(bool instant);                        // 0x00730C00
    void Close();                                   // 0x00730DD0
    void UpdateOpenStatusTriggerTypeScript();
};

void CTCDoor_Methods::UpdateOpenStatusTriggerTypeScript()
{
    if (CurrentOpen != OldOpen) {
        if (CurrentOpen) {
            CDefPointer<const CAppearanceDef> out;
            Open(true);
            GetParentThing()->PBaseDef->GetSubDef(&out);
            CAppearanceDefHolder appearance(out.Object);
            const CAnimationEntry& entry = appearance.Def->Animation.GetAnim(g_CCharString_013bad34, 0, CAnimFilterNoGroup());
            CWorld* world = ((CTCBase*)this)->GetWorld();
            world->PMainMeshBank->GetAnimation(entry.BankIndex);
        } else {
            Close();
        }
        OldOpen = CurrentOpen;
    }
}
