// Grade: DIFFER(698v698) -- header-moved (engine/CDefPointer.h verify block, CDefClassBase) with the proven callee CTCHeroOpinionDeedLog::PeekPDeedEffectsDef / COpinionDeedEffectsDef::CrimeSeverity: same residue, the hidden-return CDefPointer homed in the dead deed slot (retail the key-0x23 word)
// Tried: def at function scope (adds a 6th frame word, 0x18 frame), block-scoped def (parameter slot), block around the whole tail (p2 slot), const& temporary binding, block from the shop lookup on.
// CTCShopKeeper::WitnessedCrime(COpinionDeed* deed, long criminal) -- a shopkeeper who saw a crime in (or near) his shop raises the alarm or shouts for the guards, then reports it to the village
// Leaf: WitnessedCrime
// Retail: Fable.exe (TLC, VC7.1) @ 0x007993F0, 704 bytes, `ret 8`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, the village-member village getter @ 0x007EE540
//   (catalog "IS_THING_ALIVE", fastcall), CTCShopKeeper::GetShopTC @ 0x00798F50, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//   CThing::GetPosition @ 0x004C73D0, CThingBuilding::IsPointInside @ 0x0082D000, CTCBase::GetThingSearchTools @ 0x00686D90,
//   CThingSearchTools::GetPFirstThing<CThingFilter_HasTC, CThingAICreature> @ 0x00799C40, CTCShop::SetAlarm @ 0x00773C90,
//   CTCTalk::StopCurrentSpeech @ 0x006E2E60, CTCTalk::PlayAISpeech @ 0x006E3B30, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//   the village criminal test @ 0x0065914B (catalog "StartRead"), CTCVillage::ReportCrime @ 0x0065CB6B,
//   CTCCreatureOpinionOfHero::KnowsOfDeed @ 0x00681AE0 / InformOfDeed @ 0x00683590, CTCBase::GetPlayerManager @ 0x00686D40,
//   CPlayerManager::GetMainPlayer @ 0x00449970, the player character getter @ 0x00487DC0 (catalog `_Cons_val`), the
//   interface-0x51 deed-definition getter @ 0x006C5630 (catalog "GetFloorMaterial"), CTCVillage::RecalculateSecuritySweep @ 0x00659C32.
// Float pool: 10.0f @ 0x0126C128; "TEXT_AI_CRIME_RUN_TO_GUARDS" @ 0x0126C23C.
// Layout: retail CTCShop StockState +0x0c, StallLike +0x50 (Ego_r.pdb +0x5c less three 12-byte vectors); CThing type byte +0x10,
//   interface bitset +0x20 / map +0x44, life flags +0x91; the shopkeeper creature's building pointer +0x170.
#include <stddef.h>
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
#include <math.h>

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    CBaseObject* PData;
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};

struct CThing {
    unsigned char _pad00[0x10];
    unsigned char ThingType;                            // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char LifeFlags;                            // +0x91
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T>
    __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T>
    __forceinline void GetInterface(long type, T** out)
    {
        if (HasTC(type))
            *out = GetTC<T>(type);
    }
};
struct CThingBuilding : CThing {
    bool IsPointInside(const C3DVector& pos);           // 0x0082D000
};
struct CThingShopKeeper : CThing {
    unsigned char _pad94[0x170 - 0x94];
    CBaseIntelligentPointer PBuilding;                  // +0x170
    CThingBuilding* GetBuilding() const { return (CThingBuilding*)PBuilding.GetPItem(); }
};

struct CTCVillage {
    void*   __vftable;
    CThing* ParentThing;                                // +0x04
    bool IsKnownCriminal(long criminal);                // 0x0065914B
    void ReportCrime(long criminal);                    // 0x0065CB6B
    void RecalculateSecuritySweep(bool now);            // 0x00659C32
};
struct CTCVillageMember {
    CTCVillage* GetVillage();                           // 0x007EE540
};
struct CTCShop {
    void*   __vftable;
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[4];
    long    StockState;                                 // +0x0c
    unsigned char _pad10[0x50 - 0x10];
    bool    StallLike;                                  // +0x50
    void SetAlarm(bool on);                             // 0x00773C90
};
struct CTCTalk {
    void StopCurrentSpeech();                           // 0x006E2E60
    long PlayAISpeech(const CCharString& text, bool a, long layer, bool b, bool c);   // 0x006E3B30
};
struct CTCCreatureOpinionOfHero {
    bool KnowsOfDeed(long criminal, long deedType, bool flag);   // 0x00681AE0
    void InformOfDeed(long criminal, bool flag);                 // 0x00683590
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

enum EOpinionDeedType { OPINION_DEED_TYPE_NONE = 0 };
class COpinionDeedEffectsDef : public CDefClassBase {      // Ego_r.pdb COpinionDeedEffectsDef (retail STLport members: 0x20 smaller)
public:
    unsigned char _pad[0x70 - sizeof(CDefClassBase)];
    long CrimeSeverity;                                 // +0x70 (ECrimeSeverity; Ego_r +0x84)
};
class CTCHeroOpinionDeedLog {                           // interface 0x51
public:
    CDefPointer<const COpinionDeedEffectsDef> PeekPDeedEffectsDef(EOpinionDeedType deed) const;   // 0x006C5630
};
struct COpinionDeed { long Type; };

struct CThingFilter_HasTC { long Type; CThingFilter_HasTC(long t) : Type(t) {} };
struct CThingAICreature;
struct CThingSearchTools {
    bool GetPFirstThing(const CThingFilter_HasTC& filter, CThingAICreature** out);   // 0x00799C40
};
struct CPlayer { CThing* GetPCharacterThing(); };       // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };    // 0x00449970

struct CTCShopKeeper {
    void*             __vftable;
    CThingShopKeeper* ParentThing;                      // +0x04
    CTCShop* GetShopTC();                               // 0x00798F50
    CThingSearchTools* GetThingSearchTools();           // 0x00686D90
    CPlayerManager* GetPlayerManager();                 // 0x00686D40
    void WitnessedCrime(COpinionDeed* deed, long criminal);
};

void CTCShopKeeper::WitnessedCrime(COpinionDeed* deed, long criminal)
{
    if (!ParentThing->HasTC(0x23))
        return;
    CTCVillage* village = ParentThing->GetTC<CTCVillageMember>(0x23)->GetVillage();
    if (!village)
        return;
    CTCShop* shop = GetShopTC();
    if (!shop)
        return;
    CThingShopKeeper* keeper = ParentThing;
    if (keeper->ThingType != 1)
        return;
    CThingBuilding* home = keeper->GetBuilding();
    if (!home || (home->LifeFlags & 1))
        return;
    if (shop->StallLike) {
        CThingBuilding* building = keeper->GetBuilding();
        const C3DVector& p = keeper->GetPosition();
        const C3DVector& b = building->GetPosition();
        C2DVector d(b.X - p.X, b.Y - p.Y);
        if (sqrtf(d.X * d.X + d.Y * d.Y) > 10.0f || shop->StockState == 1)
            return;
    } else {
        if (!keeper->GetBuilding()->IsPointInside(keeper->GetPosition()))
            return;
    }
    CThingAICreature* guard;
    if (!GetThingSearchTools()->GetPFirstThing(CThingFilter_HasTC(0xb4), &guard))
        return;
    if (ParentThing == shop->ParentThing)
        return;
    if (!shop->StallLike) {
        shop->SetAlarm(true);
    } else if (ParentThing->HasTC(0xf)) {
        CTCTalk* talk = ParentThing->GetTC<CTCTalk>(0xf);
        talk->StopCurrentSpeech();
        talk->PlayAISpeech("TEXT_AI_CRIME_RUN_TO_GUARDS", false, 1, false, true);
    }
    if (!village->IsKnownCriminal(criminal)) {
        village->ReportCrime(criminal);
        if (village->ParentThing->HasTC(0x4f)) {
            CTCCreatureOpinionOfHero* opinion = village->ParentThing->GetTC<CTCCreatureOpinionOfHero>(0x4f);
            if (!opinion->KnowsOfDeed(criminal, deed->Type, true))
                opinion->InformOfDeed(criminal, false);
        }
    }
    CTCHeroOpinionDeedLog* log;
    GetPlayerManager()->GetMainPlayer()->GetPCharacterThing()->GetInterface(0x51, &log);
    {
        CDefPointer<const COpinionDeedEffectsDef> def = log->PeekPDeedEffectsDef((EOpinionDeedType)deed->Type);
        if (def->CrimeSeverity == 2 || def->CrimeSeverity == 3)
            village->RecalculateSecuritySweep(true);
    }
}
