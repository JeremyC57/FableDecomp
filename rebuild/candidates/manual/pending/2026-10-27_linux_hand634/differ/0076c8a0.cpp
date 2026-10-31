// CTCEnemy::CTCEnemy(CThing& thing) -- the enemy/ally component: not friends with everything, followers' enemy proxy enabled, empty enemy and ally lists (self-linked header nodes), then the faction named by the thing's CEnemyDef sub-def
// Grade: DIFFER(254v254) -- register swap only: retail keeps the thing def in ebp and the enemy def in edi, ours the reverse (same instructions, same order, same release order)
// Leaf: ??0CTCEnemy@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076C8A0, 254 bytes (+ int3 padding), `ret 4`, no EH frame (the TU has no /EHsc).
// Identity: Ego_r.pdb ??0CTCEnemy@@QAE@AAVCThing@@@Z (the parked source took CThing*); CTCBase(CThing&) ??0CTCBase@@QAE@AAVCThing@@@Z,
//   the protected const GetDefinitionManager / GetWorld, CDefinitionManager::GetDef<CThingDef>(long, CDefPointer<const CThingDef>*) const,
//   CDefClassBase::GetSubDef<CEnemyDef>, ?GetDefNameFromGlobalIndex@CDefinitionManager@@QBE?AVCDefString@@J@Z,
//   ?GetPFaction@CFactionManager@@QBEPAVCFaction@@ABVCCharString@@@Z, ?SetFaction@CTCEnemy@@QAEXABVCCharString@@@Z (inline here).
// Callees: CTCBase::CTCBase 0x00686800, malloc 0x00BFEA0E (the two list header nodes), CTCBase::GetDefinitionManager 0x006869D0 (x2),
//   GetDef<T> 0x0042AF3C, GetSubDef<CEnemyDef> 0x006C7720, GetDefNameFromGlobalIndex 0x009ACCC0, CDefStringTable::GetString
//   0x009D49B0 on g_DefStringTable 0x013CA828 (CDefString's conversion operator), CTCBase::GetWorld 0x00686A80,
//   CFactionManager::GetPFaction 0x006B93D0, ~CCharString 0x0099EAE0, the defs' Delete (slot 1).
// Shape (chunk 634 retest, the brief's "real fix"): the shared retail def model (FABLE_RETAIL_DEF_CLASS_BASE, engine/CDefPointer.h
//   with its ENullDef constructor, engine/CSubDefClassBase.h, engine/CParentDefClassBase.h) replaces the parked stand-ins: two
//   CDefPointer<const T> locals released by their destructors, and the real GetDef<T> / GetSubDef<CEnemyDef> bodies as same-TU
//   noinline definitions (out of line in retail, not graded) instead of the invented FindDef_* externs. The bytes are the parked
//   form's exactly (the same ebp/edi swap): this source is now landable as a model, the residue unchanged. Declaring the enemy def
//   first keeps the swap and moves its zero store (and flips the release order); a named `found` bool adds `cmp al,bl`.
// Layout: Ego_r.pdb CTCEnemy with retail 4-byte STLport list heads: FriendsWithEverythingFlag +0x0c, EnableFollowersEnemyProxy +0x0d,
//   EnemyThings +0x10, AllyThings +0x14, PFaction +0x18; vtable 0x0126A47C; CThing def index +0x8c; CEnemyDef Faction +0x28;
//   CWorld PFactionManager +0x54. CTCBase is modelled locally (engine/CTCBase.h is flattened: no virtuals).
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"

extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

class CDefStringTable {
public:
    CCharString GetString(long index) const;    // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828
class CDefString {
public:
    long TablePos;
    operator CCharString() const { return g_DefStringTable.GetString(TablePos); }
};

class CThingDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
};
class CEnemyDef : public CSubDefClassBase {
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CEnemyDef", -1); }
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    long Faction;                               // +0x28
};

class CDefinitionManager {
    friend class CDefClassBase;
public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;
    CDefString GetDefNameFromGlobalIndex(long index) const;          // 0x009ACCC0
protected:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
class CGameDefinitionManager : public CDefinitionManager { };

#pragma auto_inline(off)
template <class T>
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{                                               // 0x0042AF3C (same-TU body; not graded)
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (def.Object) {
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CEnemyDef>(CDefPointer<const CEnemyDef>* out) const
{                                               // 0x006C7720 (same-TU body; not graded)
    const CSubDefInfo* info = GetPSubDefInfo(CEnemyDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)

class CFaction;
class CFactionManager {
public:
    CFaction* GetPFaction(const CCharString& name) const;   // 0x006B93D0
};
class CWorld {
public:
    unsigned char _pad00[0x54];
    CFactionManager* PFactionManager_ptr;       // +0x54 scoped_ptr<CFactionManager>::ptr
};
class CThing {
public:
    unsigned char  _pad00[0x8c];
    unsigned short DefGlobalIndex;              // +0x8c
};

class CTCBase {
public:
    CTCBase(CThing& thing);                     // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
protected:
    const CGameDefinitionManager& GetDefinitionManager() const; // 0x006869D0
    CWorld& GetWorld() const;                   // 0x00686A80
};

struct SThingListNode { SThingListNode* _M_next; SThingListNode* _M_prev; void* _M_vptr; void* _M_item; };
struct CThingList {
    SThingListNode* _M_node;
    CThingList() : _M_node(0)
    {
        SThingListNode* node = (SThingListNode*)malloc(sizeof(SThingListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};

class CTCEnemy : public CTCBase {               // vtable 0x0126A47C
public:
    CTCEnemy(CThing& thing);
    virtual ~CTCEnemy();
    void SetFaction(const CCharString& name)
    {
        CFactionManager* manager = GetWorld().PFactionManager_ptr;
        PFaction = manager->GetPFaction(name);
    }
    bool       FriendsWithEverythingFlag;       // +0x0c
    bool       EnableFollowersEnemyProxy;       // +0x0d
    CThingList EnemyThings;                     // +0x10
    CThingList AllyThings;                      // +0x14
    CFaction*  PFaction;                        // +0x18
};

CTCEnemy::CTCEnemy(CThing& thing)
    : CTCBase(thing)
    , FriendsWithEverythingFlag(false)
    , EnableFollowersEnemyProxy(true)
    , PFaction(0)
{
    CDefPointer<const CThingDef> thingDef;
    GetDefinitionManager().GetDef(ParentThing->DefGlobalIndex, &thingDef);
    CDefPointer<const CEnemyDef> enemyDef;
    if (thingDef->GetSubDef(&enemyDef))
        SetFaction(GetDefinitionManager().GetDefNameFromGlobalIndex(enemyDef->Faction));
}
