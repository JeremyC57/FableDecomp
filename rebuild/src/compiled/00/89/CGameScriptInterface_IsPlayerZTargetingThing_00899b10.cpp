// CGameScriptInterface::IsPlayerZTargetingThing(const CScriptThing&) const -- is the main player z-targeting, with its live hero's targeting component aimed at this live thing
// Leaf: ?IsPlayerZTargetingThing@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Retail: Fable.exe @ 0x00899B10, 140 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: player = PlayerManager (+0x14)->GetMainPlayer() (0x00449970); target = thing.GetPThing() (CScriptThing slot 11,
// +0x2c); if player.ZTargeting (+0x20a) and target alive (Dead +0x91 bit 0) and hero = player.GetPControlledCreature()
// (0x00487DC0, `add ecx,2Ch; jmp`, CPlayer::ControlledCreature +0x2c) alive and holding interface 8: PeekTC (const
// LowerBound 0x004365B0) and compare its slot 33 (+0x84) with target.
// Layout: engine/CPlayer.h ZTargeting +0x20a (Ego_r.pdb, no shift); CThing as the matched 0x00899E90 row; the interface-8
// class (modelled CTCTargetingBase, as 0x0089A490) and the slot-33 name are not confirmed. CScriptThing is declared
// locally with its virtuals (engine/CScriptThing.h flattens the vptr).
// Shape: /O2 (16-aligned entry); the PeekTC out-pointer form, with the end-or-mismatch choice written as one ternary
// whose ->component is stored (`*out = ((it == last || 8 < it->type) ? last : it)->component`): the if/assign form puts
// the component in eax and copies it to ecx (142 bytes); the ternary loads it straight into ecx as retail does.
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const ETCInterfaceType& type);          // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;    // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char Type;                                  // +0x10 (1 = a creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)               // ??$GetTC@V...@@@CThing@@QAE_NPAPAV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || T::INTERFACE_TYPE < it->first)
            it = map->last;
        *out = (T*)it->second;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        *out = (const T*)((it == map->last || T::INTERFACE_TYPE < it->first) ? map->last : it)->second;
        return true;
    }
};
class CScriptThing {
public:
    virtual ~CScriptThing();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual CThing* GetPThing() const;                   // slot 11 (+0x2c)
};
class CTCTargetingBase {                                // interface 8 (class not confirmed)
public:
    enum { INTERFACE_TYPE = 8 };
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTarget() const;                   // slot 33 (+0x84, name not confirmed)
};
class CThingPlayerCreature : public CThing { };
class CPlayer : public CPlayerPdb { public:
    CThingPlayerCreature* GetPControlledCreature() const; // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                    // 0x00449970
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsPlayerZTargetingThing(const CScriptThing& thing) const
{
    CPlayer& player = PlayerManager->GetMainPlayer();
    CThing* target = thing.GetPThing();
    if (player.ZTargeting && target && !target->IsDead())
    {
        CThing* hero = player.GetPControlledCreature();
        if (hero && !hero->IsDead())
        {
            const CTCTargetingBase* tc;
            if (hero->PeekTC(&tc) && tc->GetTarget() == target)
                return true;
        }
    }
    return false;
}