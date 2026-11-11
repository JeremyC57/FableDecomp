// CTCVillage::GetNormalisedTimeLeftOnFines() -- for the village region's fines: the longest time left, in frames, before a guard forgets one of the hero's recorded crimes, as a fraction of that crime's knowledge life (0 when the region has no fines)
// Leaf: ?GetNormalisedTimeLeftOnFines@CTCVillage@@QAEMXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad24[0x20] removed.
// Naming wave 35c: the exact PDB symbol (Ego_r.pdb + FableWin.pdb); the catalog carries the name; body unchanged.
// Naming wave 18: the interface-presence stand-in CBitset is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065A28F, length 397 bytes (`leave`, plain `ret`): frame-pointer size TU,
//   `#pragma optimize("y", off)` + `#pragma optimize("s", on)` as the landed sibling GetRegionLeavingDurationForCurrentCrimes
//   0x0065A5BB.
// Callees: the CTCBase global-manager getters 0x00686D30 (+0x28 of 0x013B8A1C, the world map), 0x00686D60 (+0x1C, the
//   definition manager) and 0x00686A70 (+0x24, the world) -- all propagated "CViewBase::GetDisplayEngine"; CThing::
//   GetPos 0x004C73D0; the world map's slot 16 (+0x40, the map index at a position); GetRegionNumberMapIsIn
//   0x004FC190 and CWorldMap::GetRegion 0x004FB490 (propagated "CTCInventoryBase::GetCategoryWithIndex");
//   CWorld::GetCurrentPlayer 0x0049D9D0; CPlayer::GetPControlledCreature 0x00487DC0; std::bitset<>::test 0x00410DE0;
//   CVectorMap LowerBound 0x0040F020; CTCHeroOpinionDeedLog::GetDeedFromHandle 0x006C58F0 / GetDeedEffectsDef
//   0x006C5630; STLport tree Successor 0x004292C0; the current-frame function 0x0049D870; g_framesPerSecond 0x01375550.
// Layout: retail CTCVillage PlayersCrimes +0x64 and FramePlayerLastSeenByGuard +0x80 (PDB +0xDC/+0x10C less the 4-byte
//   STLport lists and the 12-byte set); the region +0x24 CDefPointer whose def +0x9D bool enables fines (descriptive);
//   COpinionDeedEffectsDef KnowledgeLifeInSeconds +0x3c (as the sibling).
// Shape: a named `CRegion* region` local, then `CDefPointer def = region->PDef` (raw-object ctor: null-tested add-ref)
//   and `if (def.Object && def->HasFines)` (a null def returns 0 unreleased; the region def releases with `--count == 0`,
//   the deed effects def with DecRef/GetRefCount and a cleared pointer); the per-crime frame-rate and current-frame reads
//   go through a manager getter whose result is discarded (static members reached through `GetX()->`); the time left is
//   `knowledge - GetWorld()->GetFramesSince(FramePlayerLastSeenByGuard)` (an inline static, so the getter call precedes
//   the field load); the effects def is a named local released after the comparison.
// Name fix (agentNF5 2026-11-10): CTCHeroOpinionDeedLog::GetDeedFromHandle 0x006C58F0 is const and returns a const pointer (?GetDeedFromHandle@CTCHeroOpinionDeedLog@@QBEPBVCOpinionDeed@@J@Z, Ego_r.pdb / FableWin.pdb).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
#include "engine/C3DVector.h"

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    std::bitset<274>       m_present;                    // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    FABLE_FORCEINLINE CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return it->second;
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CThingPlayerCreature : public CThing { };

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CWorld { public:
    const CPlayer* PeekMyPlayer() const;          // 0x0049D9D0 Ego_r.pdb / FableWin.pdb ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ
    static long __fastcall GetFrame();   // 0x0049D870
    static long GetFramesSince(long frame) { return GetFrame() - frame; }
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static; was the stand-in g_framesPerSecond)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};

struct CRefCounted {
    mutable long m_count;   // agentNF16: mutable so the CDefPointer<const T> release compiles (model only; the count is the def's +4 ref count)
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;                     // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
};
class COpinionDeedEffectsDef : public CDefBase { public:
    unsigned char _pad_0x08[0x34];
    long KnowledgeLifeInSeconds;                // +0x3c
    FABLE_FORCEINLINE void Release() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
class CRegionDef : public CDefBase { public:
    unsigned char _pad_0x08[0x95];
    bool HasFines;                              // +0x9d (descriptive)
    FABLE_FORCEINLINE void Release() const
    {
        if (--m_count == 0)
            Destroy();
    }
};
#include "engine/EOpinionDeedType.h"
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(T* object) : Object(object)
    {
        if (Object)
            ++Object->m_count;
    }
    FABLE_FORCEINLINE ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
class COpinionDeed { public:
    long DeedType;
};
class CTCHeroOpinionDeedLog { public:
    const COpinionDeed* GetDeedFromHandle(long handle) const;   // 0x006C58F0
    CDefPointer<const COpinionDeedEffectsDef> PeekPDeedEffectsDef(EOpinionDeedType deedType) const;   // 0x006C5630 Ego_r/FableWin ?PeekPDeedEffectsDef@CTCHeroOpinionDeedLog@@QBE?AV?$CDefPointer@$$CBVCOpinionDeedEffectsDef@@@@W4EOpinionDeedType@@@Z (agentNF16: was GetDeedEffectsDef)
};

class CRegion { public:
    unsigned char           _pad00[0x24];
    CRegionDef*   PDef;                         // +0x24 CDefPointer<const CRegionDef>::Object
};
class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapAtPos(const C3DVector& pos);     // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long map) const;        // 0x004FC190
    CRegion& GetRegion(long region);                    // 0x004FB490
};

struct SNode {
    long   _M_color;
    SNode* _M_parent;
    SNode* _M_left;
    SNode* _M_right;
    long   _M_value_field;                      // +0x10
};
SNode* __cdecl Successor(SNode* node);          // 0x004292C0

class CTCBase { protected:
    CWorldMap& GetWorldMap();   // 0x00686D30 (Ego_r.pdb CTCBase, protected)
    CWorld& GetWorld() const;   // 0x00686A70 (Ego_r.pdb CTCBase, protected)
    const CMainGameComponent& PeekComponent() const;   // 0x00686D60 Ego_r.pdb ?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ (was CTCVillage::GetDefinitionManager)
};
class CTCVillage : public CTCBase { public:
    void*         __vftable;                    // +0x00
    CThing*       Owner;                        // +0x04
    unsigned char _pad_0x08[0x5c];
    SNode*        PlayersCrimes;                // +0x64 set<long> header node
    unsigned char _pad_0x68[0x18];
    long          FramePlayerLastSeenByGuard;   // +0x80
    float GetNormalisedTimeLeftOnFines();
};

float CTCVillage::GetNormalisedTimeLeftOnFines()
{
    CRegion* region = &GetWorldMap().GetRegion(GetWorldMap().GetRegionNumberMapIsIn(GetWorldMap().GetMapAtPos(Owner->GetPos())));
    CDefPointer<CRegionDef> def = region->PDef;
    if (def.Object && def->HasFines) {
        long longest = 0;
        long life = 1;
        CThing* hero = CMainGameComponent::Get()->World->PeekMyPlayer()->GetPControlledCreature();
        if (hero) {
            CTCHeroOpinionDeedLog* log;
            if (hero->HasTC(0x51))
                log = (CTCHeroOpinionDeedLog*)hero->GetTC(0x51);
            SNode* end = PlayersCrimes;
            for (SNode* it = end->_M_left; it != end; it = Successor(it)) {
                COpinionDeed* deed = (COpinionDeed*)log->GetDeedFromHandle(it->_M_value_field);
                if (deed) {
                    CDefPointer<const COpinionDeedEffectsDef> effects = log->PeekPDeedEffectsDef((EOpinionDeedType)(deed->DeedType));
                    long knowledge = effects->KnowledgeLifeInSeconds * PeekComponent().GetConstantFPS();
                    long left = knowledge - GetWorld().GetFramesSince(FramePlayerLastSeenByGuard);
                    if (left >= longest) {
                        life = knowledge;
                        longest = left;
                    }
                }
            }
        }
        return (float)longest / life;
    }
    return 0.0f;
}