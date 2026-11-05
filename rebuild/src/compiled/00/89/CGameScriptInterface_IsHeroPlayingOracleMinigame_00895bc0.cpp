// CGameScriptInterface::IsHeroPlayingOracleMinigame() const -- true when the live hero's oracle minigame component (interface 0x107) has a game running (+0x14 non-null)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00895BC0, length 99 bytes.
// Callees: the hero thing through the player (0x00449970 on PlayerManager, then 0x00487DC0; both labels propagated), live (+0x91 bit 0 clear), the inlined CThing::GetInterface (the presence bitset at +0x20, CVectorMap::LowerBound on the map at +0x44, end or a larger key -> the end entry).
// Shape: the presence bit is the sign of the byte at +0x40 (`test cl,cl ; jns`); `xor ecx,ecx ; setne cl` for the pointer test.
// Leaf: IsHeroPlayingOracleMinigame
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
#include <string.h>
// CCharString (Ego_r.pdb layout, as the landed CCharString::operator!= 0x0099E960): a 4-byte handle on a 17-byte block.
#pragma pack(push, 1)
struct CBasicStringChar {                   // CBasicString<char>, 13 bytes
    char*         PData;                    // +0x00
    unsigned long StringLength;             // +0x04
    unsigned long DataLength : 31;          // +0x08
    bool          UseFastExtend : 1;        // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:                    // 17 bytes
    CBasicStringChar Data;                  // +0x00
    long             NoRefs;                // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;           // +0x00
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
    bool IsEmpty() const { return PStringData == 0 || PStringData->Data.StringLength == 0; }
};
struct CTCOracleMinigame_Retail {                   // interface type 0x107
    unsigned char _pad[0x14];
    void*         m_pGame;                          // +0x14
};
struct CThing_Retail;
struct CBase {
    CBase();                                        // 0x0099A2D0
    virtual ~CBase();
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap_Retail;
struct CThing_Retail {
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007();
    virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015();
    virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023();
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031();
    virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039();
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047();
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055();
    virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063();
    virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071();
    virtual void t072(); virtual void t073(); virtual void t074(); virtual void t075(); virtual void t076();
    virtual bool IsPlayer();   // slot 77 (+0x134)
    unsigned char         _pad04[0xc];              // +0x04 .. +0x10
    unsigned char         m_type;                   // +0x10
    unsigned char         _pad11[0xf];
    unsigned long         m_presentBits[9];         // +0x20 std::bitset of the interface types present
    CInterfaceMap_Retail  m_interfaces;             // +0x44 the sorted {type, component} map
    unsigned char         _pad54[0x18];
    unsigned char         m_flags;                  // +0x6c (bit 2 = game object, bit 3 = creature)
    unsigned char         _pad6d[0x24];
    unsigned char         m_dead : 1;               // +0x91 bit 0
    unsigned char         m_91_1 : 7;
    unsigned char         _pad92[0x2a];
    unsigned char         m_bc_0 : 1;               // +0xbc
    unsigned char         m_givesHeroStatChangesOnHit : 1;   // bit 1
    unsigned char         m_bc_2 : 6;
    unsigned char         _padbd[0x10c];
    unsigned char         m_1c9_0 : 3;              // +0x1c9
    unsigned char         m_marryable : 1;          //        bit 3
    unsigned char         m_marryableToHero : 1;    //        bit 4
    unsigned char         m_1c9_5 : 3;
    CScriptThing GetScriptThing();                  // 0x004C7E50 (by value through the hidden result pointer)
    CTCBase_Retail* AddTC(const CCharString& name, long a, long b);   // 0x004C9D60
    void RemoveTC(long interfaceType);              // 0x004C9840 (the label "CBaseObject::OnPropertyChanged" is propagated)
    void FinishCurrentAction();                     // 0x00662650 (CThingCreatureBase)
    void SheatheAllWeapons();                       // 0x006647F0 (CThingCreatureBase)
    void UnsheatheAllWeapons();                     // 0x00666830 (CThingCreatureBase)
    void UnsheatheMeleeWeapon();                    // 0x00666820 (CThingCreatureBase)
    void SetAsGivingHeroStatChangesOnBeingHit(bool giving);   // 0x008389C0 (CThingGameObject)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap_Retail* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap_Retail::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)((CTCBase_Retail*)it->second);
            return true;
        }
        return false;
    }
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
class CThingPlayerCreature : public CThing_Retail { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                  // 0x00487DC0 (label propagated)
    const CThingPlayerCreature* PeekPControlledCreature() const;            // 0x00487DD0 (label propagated)
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CWorld;
struct CDefinitionManager_Retail;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::IsHeroPlayingOracleMinigame() const
{
    CThing_Retail* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->m_dead) {
        CTCOracleMinigame_Retail* oracle;
        if (hero->GetInterface(&oracle, 0x107))
            return oracle->m_pGame != 0;
    }
    return false;
}