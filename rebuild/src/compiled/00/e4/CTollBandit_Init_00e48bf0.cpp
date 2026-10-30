// CTollBandit::Init() -- reset the toll state, take the pay-me distance from the script def, find the bandit escape marker, then put the bandit in the friendly bandit faction and stop it wanting money
// Leaf: ?Init@CTollBandit@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E48BF0, length 271 bytes (+ int3 padding).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, ~CBaseClass @ 0x0099A2E0, operator
// delete @ 0x00BFE9BC, CGameScriptInterface slots 72 GetThingWithScriptName (+0x120), 601 EntitySetInFaction (+0x964) and
// 478 EntitySetThingAsWantingMoney (+0x778) (ghidra_out/cgsi_slots_aligned.tsv).
// Globals: pGlobalScriptDef @ 0x0143E90C (TollBanditPayMeDistance at +0x414); strings 0x012E4DA4 "M_BanditEscapePoint",
// 0x012D218C "FACTION_BANDITS_FRIENDLY".
// Layout: Ego_r.pdb CTollBandit (= retail): Game +0x04, Me +0x08 (the entity-script base, as the landed CM_TollBandit::Main
// 0x00E48A30), ParentClass +0x14, PMasterData +0x18, TollState +0x1c, TollBanditOnlyCollectsToll +0x20,
// KilledReluctantTrader +0x21, TollBanditPayMeDistance +0x24, BanditEscapeMarker +0x28 (CScriptThing).
// Shape: the marker is assigned from the GetThingWithScriptName temporary through the inline CScriptThing / CCountedPointer
// operator= (the source's fields read first, the old block released, the new one add-ref'd), then the temporary's
// inline destructor (vtable store, release, zeroing, ~CBaseClass) and the name's; both names are implicit conversions.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    ~CScriptThing() {}
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const; virtual void s006() const;
    virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const;
    virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const;
    virtual void s019() const; virtual void s020() const; virtual void s021() const; virtual void s022() const; virtual void s023() const; virtual void s024() const;
    virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const; virtual void s030() const;
    virtual void s031() const; virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const;
    virtual void s037() const; virtual void s038() const; virtual void s039() const; virtual void s040() const; virtual void s041() const; virtual void s042() const;
    virtual void s043() const; virtual void s044() const; virtual void s045() const; virtual void s046() const; virtual void s047() const; virtual void s048() const;
    virtual void s049() const; virtual void s050() const; virtual void s051() const; virtual void s052() const; virtual void s053() const; virtual void s054() const;
    virtual void s055() const; virtual void s056() const; virtual void s057() const; virtual void s058() const; virtual void s059() const; virtual void s060() const;
    virtual void s061() const; virtual void s062() const; virtual void s063() const; virtual void s064() const; virtual void s065() const; virtual void s066() const;
    virtual void s067() const; virtual void s068() const; virtual void s069() const; virtual void s070() const; virtual void s071() const; virtual void s072() const;
    virtual void s073() const; virtual void s074() const; virtual void s075() const;
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad[0x414];
    long          TollBanditPayMeDistance;               // +0x414
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
extern const char g_strBanditEscapePoint[];              // 0x012E4DA4 "M_BanditEscapePoint"
extern const char g_strFactionBanditsFriendly[];         // 0x012D218C "FACTION_BANDITS_FRIENDLY"

class CV_BanditTollScript;
class CQ_SunnyvaleMasterData;
class CTollBandit {
public:
    void*                   __vftable;                   // +0x00
    CGameScriptInterface*   Game;                        // +0x04
    CScriptThing            Me;                          // +0x08
    CV_BanditTollScript*    ParentClass;                 // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                 // +0x18
    long                    TollState;                   // +0x1c
    bool                    TollBanditOnlyCollectsToll;  // +0x20
    bool                    KilledReluctantTrader;       // +0x21
    long                    TollBanditPayMeDistance;     // +0x24
    CScriptThing            BanditEscapeMarker;          // +0x28
    void Init();
};

void CTollBandit::Init()
{
    TollState = 1;
    TollBanditOnlyCollectsToll = false;
    KilledReluctantTrader = false;
    TollBanditPayMeDistance = pGlobalScriptDef->TollBanditPayMeDistance;
    BanditEscapeMarker = Game->GetThingWithScriptName(g_strBanditEscapePoint);
    Game->EntitySetInFaction(Me, g_strFactionBanditsFriendly);
    Game->EntitySetThingAsWantingMoney(Me, 0);
}