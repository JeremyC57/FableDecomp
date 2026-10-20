// Behaviour test for 007ee270: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCVillageMember::SetPVillage(CTCVillage* village) -- points the member at the village's thing (or clears it) and, for a creature owner, makes its game-state and level persistence follow whether it now belongs to a village
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
struct CBaseObject;
struct CThing {
    unsigned char _pad_0x00[0x10];
    unsigned char ThingType;                    // +0x10 (descriptive; 1 = creature)
    unsigned char _pad_0x11[0x81];
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;   // +0x92 bit 0
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;          // bit 4
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    void SetAsGameStatePersisted(bool persisted);   // 0x004C7880
};

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CBaseObject* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};

struct CTCVillage {
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
};

struct CTCVillageMember {
    void*         __vftable;                    // +0x00
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x8];
    CIntelligentPointer<CThing> PVillage;       // +0x10
    void SetPVillage(CTCVillage* village);
};

void CTCVillageMember::SetPVillage(CTCVillage* village)
{
    PVillage = CIntelligentPointer<CThing>(0);
    if (village)
        PVillage = CIntelligentPointer<CThing>(village->ParentThing);
    if (ParentThing->ThingType == 1) {
        ParentThing->SetAsGameStatePersisted(village ? true : false);
        ParentThing->LevelPersistent = village ? true : false;
    }
}

// ---- stubs and driver ----
void CBaseIntelligentPointer::v00() {}
template <class T> void CIntelligentPointer<T>::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
static int g_ipDtors;
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ipDtors; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = item; }
static int g_persistCalls; static bool g_persistArg;
void CThing::SetAsGameStatePersisted(bool persisted) { ++g_persistCalls; g_persistArg = persisted; GameStatePersisted = persisted; }

int main()
{
    static CThing owner, villageThing;
    static CTCVillage village;
    village.ParentThing = &villageThing;
    static CTCVillageMember member;
    member.ParentThing = &owner;
    owner.ThingType = 1;
    member.SetPVillage(&village);
    if (member.PVillage.m_pItem != (CBaseObject*)&villageThing || g_persistCalls != 1 || !g_persistArg || !owner.LevelPersistent || g_ipDtors != 2)
        { std::printf("007ee270_TEST FAIL (attach)\n"); return 1; }
    member.SetPVillage(0);
    if (member.PVillage.m_pItem != 0 || g_persistCalls != 2 || g_persistArg || owner.LevelPersistent || g_ipDtors != 3)
        { std::printf("007ee270_TEST FAIL (detach)\n"); return 1; }
    owner.ThingType = 2;                                         // not a creature: persistence untouched
    owner.LevelPersistent = 1;
    member.SetPVillage(&village);
    if (member.PVillage.m_pItem != (CBaseObject*)&villageThing || g_persistCalls != 2 || !owner.LevelPersistent)
        { std::printf("007ee270_TEST FAIL (object)\n"); return 1; }
    std::printf("007ee270_TEST PASS\n");
    return 0;
}
