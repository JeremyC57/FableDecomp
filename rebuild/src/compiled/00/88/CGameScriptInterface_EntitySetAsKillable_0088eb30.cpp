// CGameScriptInterface::EntitySetAsKillable(const CScriptThing& thing, bool killable, bool b) const -- on a game object (+0x6c bit 2) stores the killable bit (+0xbc bit 1), forwards (thing, killable) to the combo-multiplier and stat-change setters (virtual slots 526/527), then SetAsGivingHeroStatChangesOnBeingHit(killable ? true : b)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088EB30, length 129 bytes.
// Callees: CScriptThing virtual slots 75 (+0x12c, IsValid) and 11 (+0x2c, GetThing), CGameScriptInterface virtual slots 526 (+0x838, EntitySetAsToAddToComboMultiplierWhenHit) and 527 (+0x83c, EntitySetAsToAddToStatChangesWhenHit),
// CThingGameObject::SetAsGivingHeroStatChangesOnBeingHit @ 0x008389C0.
// Leaf: EntitySetAsKillable
#include <string.h>
struct CThing_Retail;
class CScriptThing;
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
struct CThing_Retail;
struct CBase {
    CBase();                                        // 0x0099A2D0
    virtual ~CBase();
};
class CScriptThing : public CBase { public:                       // 12 bytes: vptr (0x01238C8C) + the thing's intelligent pointer words
     virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007();
    virtual void s008(); virtual void s009(); virtual void s010();
    virtual CThing_Retail* GetThing() const;   // slot 11 (+0x2c)
    virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019();
    virtual void s020(); virtual void s021(); virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025(); virtual void s026(); virtual void s027();
    virtual void s028(); virtual void s029(); virtual void s030(); virtual void s031(); virtual void s032(); virtual void s033(); virtual void s034(); virtual void s035();
    virtual void s036(); virtual void s037(); virtual void s038(); virtual void s039(); virtual void s040(); virtual void s041(); virtual void s042(); virtual void s043();
    virtual void s044(); virtual void s045(); virtual void s046(); virtual void s047(); virtual void s048(); virtual void s049(); virtual void s050(); virtual void s051();
    virtual void s052(); virtual void s053(); virtual void s054(); virtual void s055(); virtual void s056(); virtual void s057(); virtual void s058(); virtual void s059();
    virtual void s060(); virtual void s061(); virtual void s062(); virtual void s063(); virtual void s064(); virtual void s065(); virtual void s066(); virtual void s067();
    virtual void s068(); virtual void s069(); virtual void s070(); virtual void s071(); virtual void s072(); virtual void s073(); virtual void s074();
    virtual bool IsValid() const;   // slot 75 (+0x12c)
    void* m_pData;                                  // +0x04
    void* m_pItem;                                  // +0x08
    CScriptThing() : m_pData(0), m_pItem(0) { }
};
struct CTCBase_Retail;
struct CInterfaceMap_Retail {                       // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x004365B0 / 0x0040F020 (two copies of the same body)
};
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
    unsigned char         _pad4c[0x20];
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
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                    // 0x00449970
};
class CWorld;
struct CDefinitionManager_Retail;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntitySetAsKillable(const CScriptThing& thing, bool killable, bool flag) const
{
    if (thing.IsValid()) {
        CThing_Retail* object = thing.GetThing();
        if (object->m_flags & 4) {
            object->m_givesHeroStatChangesOnHit = killable;
            EntitySetAsToAddToComboMultiplierWhenHit(thing, killable);
            EntitySetAsToAddToStatChangesWhenHit(thing, killable);
            if (killable)
                object->SetAsGivingHeroStatChangesOnBeingHit(true);
            else
                object->SetAsGivingHeroStatChangesOnBeingHit(flag);
        }
    }
}