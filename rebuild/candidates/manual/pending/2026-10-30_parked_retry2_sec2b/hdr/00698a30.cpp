// Grade: RELOCATION_MATCH PENDING HEADER (retry wave 2 sec2): the retail GetSubDef<CEntitySoundDef> body (landed 0x0065F604) defined in the TU as a noinline explicit specialization lets VC7.1 keep the escaped handle's pointee in ecx for the closing `dec [ecx+4]; jne` release (was 239v230). Graded RELOCATION_MATCH with engine/CDefPointer.h's text inlined plus `CDefPointer(ENullDef) : Object(0) { }` (FableWin ??0?$CDefPointer@$$CBVCEntitySoundDef@@@@QAE@W4ENullDef@@@Z; probe 00698a30.verify.cpp); this file compiles once the header declares that ctor.
// PlayCameraSoundThroughObservedThing(const CThing* thing, const CCameraManagerDef& def, const CCharString& sound) -- play a camera sound on the observed thing's sound player: the camera def's entity-sound sub-def maps the name to a sample pair (played as a sample) or else to a criteria string (played as criteria)
// Leaf: ?PlayCameraSoundThroughObservedThing@?A0x
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path; it excludes the Call... wrapper.)
//
// Retail 0x00698A30, 230 bytes (+ int3 padding to 240), __fastcall `ret 4` (thing in ecx, def in edx, the sound name
//   on the stack).
// Callees: CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>*) const @ 0x0065F604 (landed),
//   the non-const CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inline GetTC of interface 0x10),
//   CSoundMap::GetPSoundPair @ 0x0065FBA4 (landed), CSoundInit::CSoundInit() @ 0x0041C930,
//   CTCSoundPlayer::PlayAttachedSample(const CSoundPair&, const CSoundInit&, bool, bool, bool) @ 0x0066B280 (landed),
//   CSoundMap::GetPCriteriaString @ 0x0069D090 (this chunk), CDefString::operator CCharString() const @ 0x00415DD0,
//   CTCSoundPlayer::PlayAttachedCriteria(const CCharString&, bool, bool, bool, long) @ 0x0066BD00 (landed),
//   ~CCharString @ 0x0099EAE0; the sub-def's virtual slot 1 (destroy) on the last release.
// Callers: nine CGameCameraManager members (0x00698B3B .. 0x00699AB0), e.g. PlayCameraSoundThroughObservedThing(const
//   CCharString&) const 0x00698B20 = `::PlayCameraSoundThroughObservedThing(ObservedThing.GetPItem(), *PDef, sound)`.
// Layout: retail CThing interface bitset +0x20 (0x10 = bit 16), interface map +0x44; Ego_r.pdb CEntitySoundDef
//   SoundMap +0x28 (CSoundMap: SoundMap +0, CriteriaMap +0x10); the def's refcount +4 behind its vptr.
// Shape: `CDefPointer<const CEntitySoundDef> soundDef;` (null, set before the thing test so the null path skips the
//   release) `if (thing && def.GetSubDef<CEntitySoundDef>(&soundDef) && thing->HasTC(0x10)) { player = GetTC(0x10); if (pair =
//   map.GetPSoundPair(sound)) player->PlayAttachedSample(*pair, CSoundInit(), true, false, false); else if (criteria =
//   map.GetPCriteriaString(sound)) player->PlayAttachedCriteria(*criteria, true, false, false, -1); }` -- the criteria's
//   CCharString temporary is homed in the dead `sound` argument slot; the CSoundInit temporary is the 0x20-byte frame
//   block.  The interface map is reached through the non-const LowerBound copy although the thing is const (modelled
//   with a const_cast in the inline GetTC).
// Name: Ego_r.pdb `?PlayCameraSoundThroughObservedThing@?A0xa59f4854@@YIXPBVCThing@@ABVCCameraManagerDef@@ABVCCharString@@@Z`.
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"                             // NEEDS the ENullDef ctor added to the header (see Grade)
class CDefString {
public:
    long Index;
    operator CCharString() const;                           // 0x00415DD0
};
class CSoundPair {
public:
    long FirstSound;
    long LastSound;
};

class CSoundInit {                                          // 0x20 bytes
public:
    unsigned char Data[0x20];
    CSoundInit();                                           // 0x0041C930
};

class CSoundMap {
public:
    unsigned char Maps[0x28];                               // SoundMap +0, CriteriaMap +0x10 (two 0x10 CVectorMaps) + tail
    const CSoundPair* GetPSoundPair(const CCharString& name) const;        // 0x0065FBA4
    const CDefString* GetPCriteriaString(const CCharString& name) const;  // 0x0069D090
};

class CEntitySoundDef : public CSubDefClassBase {                    // RTTI .?AVCEntitySoundDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CEntitySoundDef", -1); }   // inline
    virtual void Transfer(CPersistContext&);                      // ?Transfer@CEntitySoundDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase*);                      // ?Copy@CEntitySoundDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CEntitySoundDef@@UAEKXZ
    unsigned char _pad25[0x28 - 0x25];
    CSoundMap     SoundMap;                                       // +0x28
};

class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (protected non-const overload; hidden result pointer)
};

// The retail GetSubDef<CEntitySoundDef> body (landed 0x0065F604), visible in this TU and called out of line: VC7.1 then
// knows the escaped soundDef handle is only written through `out`, and keeps the pointee in ecx for the closing release.
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CEntitySoundDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

class CCameraManagerDef : public CDefClassBase { };

class CTCSoundPlayer {
public:
    unsigned long PlayAttachedSample(const CSoundPair& sound, const CSoundInit& init, bool flag, bool loop,
                                     bool attached);        // 0x0066B280
    unsigned long PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d);   // 0x0066BD00
};

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020 (the non-const copy)
};

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                        // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                            // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline void* GetTC(long type) const
    {
        long key = type;
        CInterfaceMap* map = const_cast<CInterfaceMap*>(&m_interfaces);
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

namespace {
void __fastcall PlayCameraSoundThroughObservedThing(const CThing* thing, const CCameraManagerDef& def,
                                                    const CCharString& sound)
{
    CDefPointer<const CEntitySoundDef> soundDef(NULL_DEF);
    if (thing && def.GetSubDef<CEntitySoundDef>(&soundDef) && thing->HasTC(0x10)) {
        CTCSoundPlayer* player = (CTCSoundPlayer*)thing->GetTC(0x10);
        const CSoundMap& map = soundDef->SoundMap;
        const CSoundPair* pair = map.GetPSoundPair(sound);
        if (pair)
            player->PlayAttachedSample(*pair, CSoundInit(), true, false, false);
        else {
            const CDefString* criteria = map.GetPCriteriaString(sound);
            if (criteria)
                player->PlayAttachedCriteria(*criteria, true, false, false, -1);
        }
    }
}
}

void __fastcall CallPlayCameraSoundThroughObservedThing(const CThing* thing, const CCameraManagerDef& def,
                                                        const CCharString& sound)
{
    PlayCameraSoundThroughObservedThing(thing, def, sound);
}
