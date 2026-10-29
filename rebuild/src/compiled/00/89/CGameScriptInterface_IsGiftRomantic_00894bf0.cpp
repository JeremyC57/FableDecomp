// CGameScriptInterface::IsGiftRomantic(const CCharString& defName) const -- true when the named def has a gift sub-def of type 1 (romantic)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00894BF0, length 159 bytes.
// Callees: CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410 (on DefinitionManager +0x10), CGameDefinitionManager::Get @ 0x0044C6B0 (static), CDefinitionManager::GetDef<T> @ 0x0042AF3C,
// CDefClassBase::GetSubDef<CGiftDef> @ 0x006D3EF0, the defs' virtual slot 1 (destroy) when a release drops the count to zero.
// Shape: both defs are zeroed CDefPointer locals released by their null-tested destructors (the gift def's test is dropped on the success
// path after `return giftDef->m_giftType == 1` dereferenced it, GOTCHAS null-test rule); with the same-TU GetDef/GetSubDef stand-ins the
// class def stays in esi and both out slots share the dead argument word; the result is formed `test edx,edx ; push ebx ; sete bl`.
// Leaf: IsGiftRomantic
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
struct CDefObject_Retail {                          // a counted def object (count at +4, virtual slot 1 destroys)
    virtual void v000();
    virtual void Destroy();                         // slot 1 (+0x04)
    long m_refCount;                                // +0x04
};
struct CGiftDef_Retail : CDefObject_Retail {
    unsigned char _pad08[0x20];
    long          m_giftType;                       // +0x28 (0 friendly, 1 romantic, 2 offensive)
};
inline void ReleaseDef(CDefObject_Retail* def)    // the counted release, written by hand at each exit
{
    --def->m_refCount;
    if (def->m_refCount == 0)
        def->Destroy();
}
template<class T> struct CDefPointerSlot {          // CDefPointer<T>: zeroed, released by its destructor (null-tested)
    T* Object;
    CDefPointerSlot() : Object(0) { }
    ~CDefPointerSlot()
    {
        if (Object) {
            if (--Object->m_refCount == 0)
                Object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};
struct CDefClass_Retail : CDefObject_Retail {
    bool GetSubDef(CDefPointerSlot<CGiftDef_Retail>* out);   // 0x006D3EF0 CDefClassBase::GetSubDef<CGiftDef>
};
void* FindDef_CDefClass_Retail_GetSubDef_2();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClass_Retail::GetSubDef(CDefPointerSlot<CGiftDef_Retail>* out)
{
    void* def = FindDef_CDefClass_Retail_GetSubDef_2();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CGameDefinitionManager_Retail {
    bool GetDef(long index, CDefPointerSlot<CDefClass_Retail>* out);   // 0x0042AF3C CDefinitionManager::GetDef<T>
};
class CGameDefinitionManager : public CGameDefinitionManager_Retail { public: static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0 ?Get@CGameDefinitionManager@@SIAAV1@XZ
void* FindDef_CGameDefinitionManager_Retail_GetDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager_Retail::GetDef(long index, CDefPointerSlot<CDefClass_Retail>* out)
{
    void* def = FindDef_CGameDefinitionManager_Retail_GetDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
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
struct CPlayer {
    CThingPlayerCreature* GetPControlledCreature() const;                  // 0x00487DC0 (label propagated)
    const CThingPlayerCreature* PeekPControlledCreature() const;            // 0x00487DD0 (label propagated)
};
struct CPlayerManager_Retail {
    CPlayer* GetPlayer();                    // 0x00449970 (labelled "Catch@0040d959", propagated)
};
class CWorld;
class CDefinitionManager;
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                    World;               // +0x04 (engine/CGameScriptInterface.h)
    void*                      Component;           // +0x08
    void*                      DisplayEngine;       // +0x0c
    CDefinitionManager* DefinitionManager;   // +0x10
    CPlayerManager_Retail*     PlayerManager;       // +0x14
    bool IsGiftRomantic(const CCharString& defName) const;
};

bool CGameScriptInterface::IsGiftRomantic(const CCharString& defName) const
{
    long index = DefinitionManager->GetDefGlobalIndexFromName(defName);
    CDefPointerSlot<CDefClass_Retail> defClass;
    if (CGameDefinitionManager::Get().GetDef(index, &defClass)) {
        CDefPointerSlot<CGiftDef_Retail> giftDef;
        if (defClass->GetSubDef(&giftDef))
            return giftDef->m_giftType == 1;
    }
    return false;
}