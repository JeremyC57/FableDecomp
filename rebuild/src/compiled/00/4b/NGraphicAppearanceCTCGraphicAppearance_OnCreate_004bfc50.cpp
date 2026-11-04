// NGraphicAppearance::CTCGraphicAppearance::OnCreate() -- vtable slot 4: cache the owner's animation component and, when the owner's def has an appearance sub-def, take its lighting channel
// Leaf: ?OnCreate@CTCGraphicAppearance@NGraphicAppearance@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BFC50, length 144 bytes, plain `ret`.
// Name: slot 4 of the CTCGraphicAppearance vtable 0x0123986C (rtti-vtables.tsv). The Ego_r.pdb name set gives the class's
//   virtual overrides as OnCreate, OnKill, Copy, HandleMessage, BulletTimeFrameUpdate, GetClassTypeName, GetInterfaceType;
//   slot 4 of the sibling CTCBase vtables is OnCreate (landed CTCGraphicAppearanceSprite::OnCreate 0x0077C480), and the body
//   (cache a component, read the def) is creation work: ?OnCreate@CTCGraphicAppearance@NGraphicAppearance@@UAEXXZ.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (the inlined GetInterface, type 0x5A = bit 26 of the
//   bitset word +0x28), CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>*) const 0x0042B0A2
//   (Ego_r.pdb ??$GetSubDef@VCAppearanceDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCAppearanceDef@@@@@Z),
//   CTCGraphicAppearance::SetLightingChannel(const CEnginePrimitiveThemeChannelDesc&, bool, bool) 0x004BFB40 (landed),
//   the def's virtual slot 1 (release to zero).
// Layout: CTCBase ParentThing +4; PTCAnimation +0x24 (Ego_r.pdb +0x2C less the two 4-byte-smaller STLport maps); retail
//   CThing bitset +0x20, interface map +0x44, base def +0x70 (as the landed CatchMessage 0x004BFD10 of this TU); retail
//   CAppearanceDef LightingChannel +0x58 (PDB +0x5C, the def base 4 bytes smaller); CEnginePrimitiveThemeChannelDesc is
//   four bytes (ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad; Ego_r.pdb). engine/CTCBase.h declares the vptr as
//   a plain `void*`, so CTCBase is modelled locally with its real member names.
// Shape: two levers together (each alone leaves a register residue): (1) GetInterface writes a LOCAL out pointer that is
//   then assigned to the member under the found-test -- the member's address as the out pointer loads the component into
//   ecx (`mov ecx,[eax+4] ; mov [ebp+24h],ecx`), retail eax; (2) the base def read through the inline getter GetBaseDef()
//   (the lever of the landed CatchMessage 0x004BFD10). The zeroed def pointer, the lookup key and the channel desc share
//   the one `push ecx` frame word; the desc is built by its inline constructor after the pushes (Pad untouched); the def is
//   released by the pointer's inline destructor (`dec [esi+4] ; jne ; call [edx+4]`). The GetSubDef body above is a same-TU
//   stand-in: the landed 0x0042B0A2 body verbatim.
// Models: the shared retail def model (FABLE_RETAIL_DEF_CLASS_BASE: engine/CSubDefClassBase.h -> CDefClassBase.h ->
//   CDefPointeeBase.h -> CResource.h, and engine/CDefPointer.h); CAppearanceDef as the landed 0x0042B0A2 declares it, with
//   its LightingChannel; CDefinitionManager is engine/CDefinitionManager.h through the define pattern, adding the
//   protected GetPDefFromGlobalIndex overload the header lacks (as the landed GetSubDef<T> rows declare it).
class CThing;
class CTCBase { public:
    virtual void OnCreate();                                // slot 4 (+0x10); the other slots are not called here
    CThing*       ParentThing;                              // +0x04
    bool          ToBeRemoved;                              // +0x08
    bool          IsInGlobalUpdateTCs;                      // +0x09
};
class CTCAnimationBase;
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#define CDefinitionManager CDefinitionManager_PDB
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
class CDefinitionManager : public CDefinitionManager_PDB {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (protected non-const overload; hidden result pointer)
};
class CAppearanceDef : public CSubDefClassBase {                    // RTTI .?AVCAppearanceDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CAppearanceDef", -1); }   // ?StatGetTypeName@CAppearanceDef@@SI?AVCCharString@@XZ (inline)
    virtual void Transfer(CPersistContext&);                      // ?Transfer@CAppearanceDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase*);                      // ?Copy@CAppearanceDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CAppearanceDef@@UAEKXZ
    unsigned char _pad_0x25[0x58 - 0x25];
    long          LightingChannel;                                // +0x58 ELightingChannel (PDB +0x5C)
};
#pragma auto_inline(off)
// Same-TU stand-in for CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (the landed body, as the landed CatchMessage
// 0x004BFD10 of this TU carries one): VC7.1 then knows the def pointer's address does not escape. Only the target row is graded.
template <>
bool CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CAppearanceDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CThingBaseDef : public CDefClassBase { };
enum ETCInterfaceType { };
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                        // +0x20
    CInterfaceMap  m_interfaces;                            // +0x44
    unsigned char  _pad54[0x1c];
    CThingBaseDef* PBaseDef;                                // +0x70 CDefPointer<CThingBaseDef const>::Object
    CThingBaseDef* GetBaseDef() const { return PBaseDef; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0;
    unsigned char ChannelIndex1;
    unsigned char ChannelBlendFactor;
    unsigned char Pad;
    CEnginePrimitiveThemeChannelDesc(unsigned char channel)
    {
        ChannelIndex0 = channel;
        ChannelIndex1 = 0;
        ChannelBlendFactor = 0;
    }
};
namespace NGraphicAppearance {
class CTCGraphicAppearance : public CTCBase { public:
    virtual void OnCreate();                                // slot 4 (+0x10)
    unsigned char     _pad0c[0x24 - 0x0c];                  // MeshEffects map +0x0c (STLport, 0xc), TintColours ..
    CTCAnimationBase* PTCAnimation;                         // +0x24
    void SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool includeCarried, bool includeWeapons);   // 0x004BFB40
};

void CTCGraphicAppearance::OnCreate()
{
    CThing* parent = ParentThing;
    CTCAnimationBase* animation;
    if (parent->GetInterface(&animation, 0x5a))
        PTCAnimation = animation;
    CDefPointer<const CAppearanceDef> def;
    if (parent->GetBaseDef()->GetSubDef(&def))
        SetLightingChannel(CEnginePrimitiveThemeChannelDesc((unsigned char)def->LightingChannel), true, true);
}
}