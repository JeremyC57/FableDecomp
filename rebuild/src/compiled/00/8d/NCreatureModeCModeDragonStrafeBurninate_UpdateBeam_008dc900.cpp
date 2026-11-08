// NCreatureMode::CModeDragonStrafeBurninate::UpdateBeam(float progress) -- while the flame is active and the def's
// explode interval has passed since the last explosion: place a point along the flame line by how far progress is between
// the start and stop percentages, and when that point is on the world map drop it to the ground, create a
// DRAGON_BURNINATE_EXPLOSION object there, register it as a dependent of the dragon (type-0x41 component) and restamp
// the explosion frame
// Leaf: UpdateBeam
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DC900, 0x1F6 bytes (padded to 512), `ret 4`.
// Callees: the dragon def getter 0x008E3300 (GetSubDef-shaped, out CDefPointer), the world-frame getter 0x0049D870
//   (__fastcall), CCharString 0x0099EBF0 / ~ 0x0099EAE0, CTCGraphicAppearance::GetDummyObject 0x004C08C0,
//   CModeBase::GetDisplayEngine 0x0073A680 (its virtual slots 4 and 3, each with the CWorldMap at +0x28),
//   CWorldMap::IsPosInMap 0x004FC600 / GetHeightZBelowPos 0x0063E480, CGameDefinitionManager::Get 0x0044C6B0 (static
//   __fastcall) / GetDefGlobalIndexFromName 0x009AD410, the object Create 0x0080C460 (__fastcall), CVectorMap::LowerBound
//   0x0040F020 (inline lookup 0x41), AddDependentThing 0x0080CBF0; def release via slot 1.
// Layout: local model; the mode is the Ego_r PDB layout 4 bytes lower (LastExplodeFrame +0xA0, FlameStartPos +0xA4,
//   FlameEndPos +0xB0, FlameActive +0xBC, FlameStartPercentage +0xC0, FlameStopPercentage +0xC4); the def's explode
//   interval at +0x58.
// Shape: same-TU out-only stand-in for the dragon def getter (the out slot stays private, so the name temp reuses it);
//   both world maps named in locals before the IsPosInMap / GetHeightZBelowPos calls (retail evaluates `this` first);
//   the dependents component through the out-parameter GetInterface ternary (`mov eax,[eax+4] ; push ; mov ecx,eax`).
#include "fable_compiler.h"
#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Forward, Up; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefVirtual {
    virtual ~CDefVirtual();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
struct CDragonDef : CDefBase {
    unsigned char _pad08[0x58 - 0x08];
    long          ExplodeInterval;              // +0x58
};
struct CDragonDefPointer {
    CDragonDef* m_p;
    CDragonDefPointer() : m_p(0) { }
};
class CDefClassBase : public CDefBase { public:
    bool GetDragonDef(CDragonDefPointer* out);  // 0x008E3300
};
void* FindDef_CDefClassBase_GetDragonDef(const void* owner);   // stand-in callee
// Same-TU stand-in for the GetSubDef<CDragonDef> instantiation (0x008E3300, a template member compiled in the caller's
// TU): it writes the out slot only, so the def pointer's address does not escape.
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetDragonDef(CDragonDefPointer* out)
{
    void* def = FindDef_CDefClassBase_GetDragonDef(this);
    if (!def)
        return false;
    out->m_p = (CDragonDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString* name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
class CWorldMap { public:
    bool IsPosInMap(const C3DVector* position);             // 0x004FC600
    float GetHeightZBelowPos(const C3DVector* position);    // 0x0063E480
};
struct CWorldHolder { unsigned char _pad[0x28]; CWorldMap* PWorldMap; };
class CDisplayEngine { public:
    virtual void _v0(); virtual void _v1(); virtual void _v2();
    virtual CWorldHolder* GetWorldB();          // slot 3
    virtual CWorldHolder* GetWorldA();          // slot 4
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing;
struct CTCDependents {
    void AddDependentThing(CThing* thing);      // 0x0080CBF0
};
class CThing { public:
    unsigned char         _pad0[0x20];
    unsigned long         m_presentBits[9];     // +0x20
    CInterfaceMap         m_interfaces;         // +0x44
    unsigned char         _pad54[0x10];
    CTCGraphicAppearance* PGraphic;             // +0x64
    unsigned char         _pad68[0x70 - 0x68];
    CDefClassBase*        PDef;                 // +0x70
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CDefinitionManager { public:   // the PDB base the accessor belongs to (empty here: offset 0)
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410 (Ego_r.pdb CDefinitionManager, const)
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                    // 0x0044C6B0
};
class CWorld { public: static long __fastcall GetFrame(); };                                        // 0x0049D870
CThing* __fastcall CreateExplosionObject(long defIndex, const C3DVector* position);   // 0x0080C460
namespace NCreatureMode {
class CModeDragonStrafeBurninate {
public:
    void*         _vptr;                        // +0x00
    CThing*       Owner;                        // +0x04
    unsigned char _pad08[0xa0 - 0x08];
    long          LastExplodeFrame;             // +0xA0
    C3DVector     FlameStartPos;                // +0xA4
    C3DVector     FlameEndPos;                  // +0xB0
    bool          FlameActive;                  // +0xBC
    float         FlameStartPercentage;         // +0xC0
    float         FlameStopPercentage;          // +0xC4
    CDisplayEngine* GetDisplayEngine();         // 0x0073A680
    void UpdateBeam(float progress);
};
void CModeDragonStrafeBurninate::UpdateBeam(float progress)
{
    if (!FlameActive)
        return;
    CDragonDefPointer slot;
    Owner->PDef->GetDragonDef(&slot);
    CDragonDef* def = slot.m_p;
    if (CWorld::GetFrame() - LastExplodeFrame > def->ExplodeInterval) {
        float fraction = (progress - FlameStartPercentage) / (FlameStopPercentage - FlameStartPercentage);
        C3DVector mouth;
        CRightHandedSet orientation;
        {
            CCharString dummy("DRAGON_MOUTH_DUMMY");
            Owner->PGraphic->GetDummyObject(&dummy, 0, 0, &mouth, &orientation, 1.0f);
        }
        C3DVector position;
        position = (FlameEndPos - FlameStartPos) * fraction + FlameStartPos;
        CWorldMap* mapA = GetDisplayEngine()->GetWorldA()->PWorldMap;
        if (mapA->IsPosInMap(&position)) {
            CWorldMap* mapB = GetDisplayEngine()->GetWorldB()->PWorldMap;
            position.Z = mapB->GetHeightZBelowPos(&position);
            long index;
            {
                CCharString name("DRAGON_BURNINATE_EXPLOSION");
                index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
            }
            CThing* explosion = CreateExplosionObject(index, &position);
            if (explosion) {
                CTCDependents* dependents;
                if (explosion->GetInterface(&dependents, 0x41))
                    dependents->AddDependentThing(Owner);
                LastExplodeFrame = CWorld::GetFrame();
            }
        }
    }
    if (--def->m_refCount == 0)
        def->Destroy();
}
}