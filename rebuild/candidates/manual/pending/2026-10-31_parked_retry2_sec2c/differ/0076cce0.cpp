// Grade: DIFFER(751v754)/17 lines (was 767v754/49) -- shared def model (engine/CDefClassBase.h + CDefPointer verify text with ENullDef; region def copied from the region's CDefPointer member) plus the retail GetSubDef<CCreatureDef> body (0x00485815) as a same-TU noinline specialization: the region-def and creature-def add-ref / release blocks are now retail. Left: lea ecx v retail lea eax for the two interface-key out slots (opinion 0x4f, village 0x23; the GetInterface out-param form gives the same), a dead add eax,24h after the region-def copy (also with a non-const CDefPointer<CRegionDef> member), and 'other' in esi + the else-branch reload in the faction lookup. Needs the header ENullDef ctor (PENDING_HEADER) once matched (retry wave 2 sec2c)
// Was: DIFFER(767v754) -- frame and calls match; retail caches the region def's incremented refcount in ecx before the GuardsAttackHero test (ours interleaves), releases the creature def with a cached load/dec form in a different block order, and the faction lookup keeps 'other' in esi with a separate else-branch reload; the this-spill slot is shared with the creature def out slot
// CTCEnemy::IsEnemyOf(const CThing*) const -- decides whether the other thing is an enemy: explicit ally/enemy lists, then the village/opinion rules for the hero, then the factions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076CCE0, 752 bytes, `ret 4`, returns bool.
// Callees: _Find over the ally list @ 0x0076D810, CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50,
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0, CTCVillageMember::GetVillage() @ 0x007EE540,
//          CTCBase::GetWorld() const @ 0x00686D20, CWorld::GetCurrentRegionIndex() @ 0x004FB150, CWorld::GetRegion(long)
//          @ 0x004FC180, CThing::IsUnderScriptSupervision() const @ 0x004C7E90, CDefClassBase::GetSubDef<CCreatureDef>
//          @ 0x00485815, CTCCreatureOpinionOfHero::IsHeroOpinionEnemy() const @ 0x006827A0, CTCScriptedControl::IsActive()
//          @ 0x004C70F0, CFaction::IsEnemy(const CFaction*) @ 0x006B94C0, IsOverridingFactionEnemyOnTarget @ 0x00864B30.
// Layout: retail CTCEnemy: EnemyThings +0x10, AllyThings +0x14 (4-byte STLport list headers), PFaction +0x18.
class CThing;
class CBaseIntelligentPointer {
public:
    void* _vfptr;
    void* PData;
    CThing* GetPItem() const;                       // 0x00A01B50
};
struct CThingListNode {
    CThingListNode* Next;
    CThingListNode* Prev;
    CBaseIntelligentPointer Value;
};
struct CThingList {
    CThingListNode* Header;
};
// The out-of-line _Find builds its result iterator in the caller's slot (ecx) and returns that slot.
struct CThingListIterator {
    CThingListNode* Node;
    CThingListIterator* Find(CThingListNode* first, CThingListNode* last, const CThing* value);   // 0x0076D810
};

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#define FABLE_DEFPTR_INLINE __forceinline
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
template <class T> class CDefPointer {
public:
    CDefPointer(ENullDef) : Object(0) { }                               // PROPOSED header addition (FableWin ??0..QAE@W4ENullDef@@@Z)
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};

class CCreatureDef : public CDefClassBase {
public:
    unsigned char _pad25[0xac - 0x25];
    long Morality;                                  // +0xAC
};
class CThingDef : public CDefClassBase {
public:
};
class CCharString {
public:
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                               // 0x0099EAE0
    void* PStringData;
};
class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
// The retail GetSubDef<CCreatureDef> body, same-TU noinline (only the caller is graded).
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CCreatureDef", -1));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
class CRegionDef : public CDefClassBase {
public:
    unsigned char _pad25[0x9d - 0x25];
    bool GuardsAttackHero;                          // +0x9D
};
struct CRegion {
    unsigned char _pad00[0x24];
    CDefPointer<const CRegionDef> PDef;             // +0x24
};
class CWorld {
public:
    long GetCurrentRegionIndex();                   // 0x004FB150
    CRegion& GetRegion(long index);                 // 0x004FC180
};
extern bool g_GuardsAlwaysHostile;                  // 0x0137574F

class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x10 - 4];
    unsigned char ThingType;                        // +0x10 (1 creature, 2 hero)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long PresentBits[9];                   // +0x20
    CInterfaceMap Interfaces;                       // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char Flags6C;                          // +0x6C (bit 3: can have enemies)
    unsigned char _pad6d[0x70 - 0x6d];
    CThingDef* PDef;                                // +0x70
    unsigned char _pad74[0xf4 - 0x74];
    CBaseIntelligentPointer CurrentTarget;          // +0xF4
    bool IsUnderScriptSupervision() const;          // 0x004C7E90
    __forceinline unsigned long HasTC(long type) const
    {
        return PresentBits[type >> 5] & (1UL << (type & 31));
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
class CThingAICreature : public CThing { };
bool __fastcall IsOverridingFactionEnemyOnTarget(const CThingAICreature* creature, const CThing* target);   // 0x00864B30

class CFaction {
public:
    bool IsEnemy(const CFaction* other) const;      // 0x006B94C0
};

class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                  // +0x04
    unsigned char _pad08[4];
    CWorld* GetWorld() const;                       // 0x00686D20
};
class CTCVillage : public CTCBase {
public:
    unsigned char _pad0c[0x7c - 0x0c];
    bool IsGuarded;                                 // +0x7C
};
class CTCVillageMember : public CTCBase {
public:
    CTCVillage* GetVillage() const;                 // 0x007EE540
};
class CTCCreatureOpinionOfHero : public CTCBase {
public:
    bool IsHeroOpinionEnemy() const;                // 0x006827A0
};
class CTCScriptedControl : public CTCBase {
public:
    bool IsActive() const;                          // 0x004C70F0
};

class CTCEnemy : public CTCBase {
public:
    bool FriendsWithEverythingFlag;                 // +0x0C
    bool EnableFollowersEnemyProxy;                 // +0x0D
    unsigned char _pad0e[2];
    CThingList EnemyThings;                         // +0x10
    CThingList AllyThings;                          // +0x14
    CFaction* PFaction;                             // +0x18
    bool IsEnemyOf(const CThing* other) const;
};

bool CTCEnemy::IsEnemyOf(const CThing* other) const
{
    if (other == Thing)
        return false;
    {
        CThingListNode* allies = AllyThings.Header;
        CThingListIterator found;
        if (found.Find(allies->Next, allies, other)->Node != allies)
            return false;
    }
    CThingListNode* enemies = EnemyThings.Header;
    CThingListNode* node = enemies->Next;
    while (node != enemies && node->Value.GetPItem() != other)
        node = node->Next;
    if (node != EnemyThings.Header)
        return true;
    CThing* thing = Thing;
    if (thing->Flags6C & 8) {
        CTCCreatureOpinionOfHero* opinion = 0;
        if (thing->HasTC(0x4f))
            opinion = (CTCCreatureOpinionOfHero*)thing->GetTC(0x4f);
        if (other->ThingType == 2) {
            if (thing->HasTC(0x23)) {
                CTCVillage* village = ((CTCVillageMember*)thing->GetTC(0x23))->GetVillage();
                if (village) {
                    CDefPointer<const CRegionDef> regionDef(GetWorld()->GetRegion(GetWorld()->GetCurrentRegionIndex()).PDef);
                    if (!regionDef.IsNull() && regionDef->GuardsAttackHero) {
                        if (village->IsGuarded) {
                            if (g_GuardsAlwaysHostile)
                                return true;
                        } else if (!Thing->IsUnderScriptSupervision()) {
                            CDefPointer<const CCreatureDef> creatureDef(NULL_DEF);
                            Thing->PDef->GetSubDef(&creatureDef);
                            if (creatureDef.IsNull() || creatureDef->Morality != 4)
                                return false;
                        }
                    }
                }
            }
            if (opinion && opinion->IsHeroOpinionEnemy()) {
                if (!Thing->HasTC(0x1f) || !((CTCScriptedControl*)Thing->GetTC(0x1f))->IsActive())
                    return true;
            }
            if (other == thing->CurrentTarget.GetPItem() && (!opinion || opinion->IsHeroOpinionEnemy()))
                return true;
        }
    }
    CFaction* faction = PFaction;
    if (!faction)
        return false;
    const CTCEnemy* otherEnemy = other->HasTC(0x49) ? (const CTCEnemy*)other->GetTC(0x49) : (const CTCEnemy*)other;
    CFaction* otherFaction = otherEnemy->PFaction;
    if (!otherFaction || !faction->IsEnemy(otherFaction))
        return false;
    if (Thing->ThingType != 1)
        return true;
    return !IsOverridingFactionEnemyOnTarget((const CThingAICreature*)Thing, other);
}
