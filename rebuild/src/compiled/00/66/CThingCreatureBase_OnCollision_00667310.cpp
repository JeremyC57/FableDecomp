// CThingCreatureBase::OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other) -- base game-object collision handling; a creature in mode 0x42 hitting a surface that is not floor-like (normal . up < 0.866) gets knocked down; then (both alive) the creature's hit-on-collision component handles the hit
// Leaf: ?OnCollision@CThingCreatureBase
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00667310, 307 bytes (+13 int3), `ret 10h`. CThingCreatureBase vtable 0x0125AC0C slot 41.
// Callees: CThingGameObject::OnCollision @ 0x00839080, CInterfaceMap::LowerBound const @ 0x004365B0 / non-const @ 0x0040F020,
//   CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 (manifest "CPixelShader::CountReferences" is propagated; see
//   labels_chunk196_corrections), the mode's virtual slot 1 (its type), the CCombatAction_GenericStrikeResponseKnockdown
//   ctor @ 0x008679C0, CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CBaseIntelligentPointer @ 0x00A01C10,
//   ~CCharString @ 0x0099EAE0, ~CCombatActionBase @ 0x0062E900, CTCHitOnCollision::OnCollision @ 0x007C81F0.
//   Float pool: 0.0f @ 0x0122DEDC, 0.866f @ 0x0125B168.
// Layout: retail CThing (local model as in the landed sibling CThingAICreature::OnCollision 0x00832840): interface bitset
//   +0x20 (TC 0x31 = word +0x24 bit 17, TC 0xC8 = word +0x38 bit 8), CVectorMap +0x44, Dead = +0x91 bit 0. The knockdown
//   action is a local model: retail CCombatActionBase is 0x114 bytes, then AnimName (CCharString) +0x114 and PThingThatHit
//   (CIntelligentPointer) +0x118 (the PDB's base is 0x18 bytes larger); 0x13c bytes in all (the PDB packs DirectionOfHit unaligned after GotPointOfHit).
// Shape: the mode test and the knockdown live inside HasTC(0x31) with the const GetTC; the slope test is an inline dot
//   product with the constant up vector (the compiler keeps (Y + X) * 0 + Z); the knockdown action is a temporary passed
//   straight to SetCurrentAction, destroyed by its implicit (inline) destructor.
// Name fix (agentNF6 2026-11-10): CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 is ?GetCurrentMode@CTCCreatureModeManager@@QBEAAVCModeBase@1@XZ (Ego_r.pdb / FableWin.pdb; engine/CTCCreatureModeManager.h): const, returns a reference to the nested CTCCreatureModeManager::CModeBase.
#include "fable_compiler.h"
#include "engine/C3DVector.h"
enum ECollideType { COLLIDE_NONE = 0, COLLIDE_LANDSCAPE = 1 };
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing;

class CTCCreatureModeManager { public:
    class CModeBase { public:
    virtual void v00();
    virtual long GetType() const;               // slot 1
};                 // CTCCreatureModeManager::CModeBase (Ego_r.pdb / FableWin.pdb nested class)
    CModeBase& GetCurrentMode() const;      // 0x0073A6E0
};
class CTCHitOnCollision { public:
    void OnCollision(C3DVector* position, C3DVector* normal, CThing* other);   // 0x007C81F0
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Dead : 1;                     // +0x91 bit 0
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return (T*)it->second;
    }
    template<class T> FABLE_FORCEINLINE const T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return (const T*)it->second;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};

class CThingGameObject : public CThing { public:
    void OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other);     // 0x00839080
};

class CCharString { public:
    void* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThingCreatureBase;
class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0x114 - 4];
    virtual ~CCombatActionBase();                       // 0x0062E900
};
class CCombatAction_GenericStrikeResponseKnockdown : public CCombatActionBase { public:
    CCharString             AnimName;           // +0x114
    CBaseIntelligentPointer PThingThatHit;      // +0x118
    unsigned char           _hitInfo[0x1c];     // +0x120 PointOfHit, GotPointOfHit, DirectionOfHit, flags (PDB packing; size 0x13c)
    CCombatAction_GenericStrikeResponseKnockdown(CThingCreatureBase* creature, CThing* hitter, C3DVector* point,
                                                 C3DVector* direction);   // 0x008679C0
};

class CThingCreatureBase : public CThingGameObject { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
    void OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other);
};

void CThingCreatureBase::OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other)
{
    CThingGameObject::OnCollision(type, position, normal, other);
    if (HasTC(0x31)) {
        const CThingCreatureBase* self = this;
        if (self->GetTC<CTCCreatureModeManager>(0x31)->GetCurrentMode().GetType() == 0x42
            && normal->Dot(C3DVector(0.0f, 0.0f, 1.0f)) < 0.866f)
            SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdown(this, other, position, normal));
    }
    if (!Dead && other && !other->Dead && HasTC(0xC8))
        GetTC<CTCHitOnCollision>(0xC8)->OnCollision(position, normal, other);
}