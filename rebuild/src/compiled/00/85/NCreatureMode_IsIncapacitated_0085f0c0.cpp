// NCreatureMode::IsIncapacitated(const CThing& thing) -- true when the current creature mode is Stunned (0x1D) or Dazed (0x43)
// Leaf: ?IsIncapacitated@NCreatureMode@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085F0C0, length 89 bytes. `__fastcall` free function (ecx = the thing), bool in al.
// Mangled (Ego_r.pdb): ?IsIncapacitated@NCreatureMode@@YI_NABVCThing@@@Z.
// Callees: CVectorMap LowerBound 0x004365B0 (const), CTCCreatureModeManager::GetCurrentMode 0x0073A6E0, the mode's slot 1.
// Shape: the `||` is returned as a value (`mov eax,1` / `xor eax,eax` full-register exits); the no-manager path is `xor al,al`.
// Layout (retail, local model; no retail-accurate CThing header exists): CThing type tag +0x10 (2 = creature), interface
//   present-bit words +0x20 (word = type >> 5), interface map {first,last,end} +0x44, ThingTypeFlags +0x6C (bit 1 physical,
//   bit 3 creature), PBaseDef +0x70, scan bits +0x91 (Dead = bit 0), CThingPhysical flag byte +0xBC, CThingCreatureBase
//   PCurrentAction +0x100 (CCountedPointer<CCreatureActionBase>::Object) and movement state +0x200 (landed models 0x0085EE90,
//   0x006E0DF0, 0x004497C0). The interface map is CVectorMap<ETCInterfaceType,CTCBase*>: const LowerBound 0x004365B0,
//   non-const 0x0040F020. Interface 0x31 = CTCCreatureModeManager (GetCurrentMode 0x0073A6E0, IsModeActive 0x0073A8A0,
//   RemoveMode 0x0073ABA0; the manifest labels "CPixelShader::CountReferences" / "CPlayer::HasMode" are propagated twins).
//   A mode's vtable slot 1 (+4) returns its NCreatureMode::EMode number (read from each CMode*::vfunc_1 body).
class CTCBase;
struct CTCInterfaceEntry {
    long     type;
    CTCBase* component;
};
class CTCInterfaceMap {                                 // CVectorMap<ETCInterfaceType, CTCBase*> (retail vector: first/last/end)
public:
    CTCInterfaceEntry* first;
    CTCInterfaceEntry* last;
    CTCInterfaceEntry* end;
    CTCInterfaceEntry* LowerBound(const long& type) const;   // 0x004365B0
    CTCInterfaceEntry* LowerBound(const long& type);         // 0x0040F020
};
class CModeBase {                                       // CTCCreatureModeManager::CModeBase
public:
    virtual void v00();
    virtual long GetType() const;                       // slot 1 (+4): NCreatureMode::EMode
};
class CTCCreatureModeManager {                          // interface 0x31
public:
    CModeBase& GetCurrentMode() const;                  // 0x0073A6E0
    bool IsModeActive(unsigned long mode) const;        // 0x0073A8A0
    void RemoveMode(unsigned long mode);                // 0x0073ABA0
};
#define VF(n) virtual void v##n();
class CCreatureActionBase {
public:
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09)
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19)
    VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29)
    VF(30) VF(31)
    virtual bool IsPhysicsPushableByHero() const;                 // slot 32 (+0x80)
    virtual bool IsAlwaysPhysicsPushableBySelfAvoidance() const;  // slot 33 (+0x84)
    VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40) VF(41) VF(42) VF(43) VF(44)
    virtual bool IsToEvadeAllHits() const;              // slot 45 (+0xB4)
    virtual bool IsToCauseRecoil() const;               // slot 46 (+0xB8)
    virtual bool IsAbleToBeAttacked() const;            // slot 47 (+0xBC)
    virtual bool IsPlayHitResponse() const;             // slot 48 (+0xC0)
    VF(49)
    virtual bool IsPreventingLookTurning() const;       // slot 50 (+0xC8)
};
class CPlayer;
class CDefClassBase;
enum ETargetingType { TARGETING_TYPE_NONE };
class CThing {
public:
    VF(t00) VF(t01) VF(t02) VF(t03) VF(t04) VF(t05) VF(t06) VF(t07) VF(t08) VF(t09)
    VF(t10) VF(t11) VF(t12) VF(t13) VF(t14) VF(t15) VF(t16) VF(t17) VF(t18) VF(t19)
    VF(t20) VF(t21) VF(t22) VF(t23) VF(t24) VF(t25) VF(t26) VF(t27) VF(t28) VF(t29)
    VF(t30) VF(t31) VF(t32) VF(t33) VF(t34) VF(t35) VF(t36) VF(t37) VF(t38) VF(t39)
    VF(t40) VF(t41) VF(t42) VF(t43) VF(t44) VF(t45) VF(t46) VF(t47) VF(t48) VF(t49)
    VF(t50) VF(t51) VF(t52) VF(t53) VF(t54) VF(t55) VF(t56) VF(t57) VF(t58) VF(t59)
    VF(t60) VF(t61) VF(t62) VF(t63) VF(t64) VF(t65) VF(t66) VF(t67) VF(t68) VF(t69)
    VF(t70) VF(t71) VF(t72) VF(t73) VF(t74) VF(t75) VF(t76)
    virtual bool IsPlayer() const;                      // slot 77 (+0x134): CThingPlayerCreature true, CThingAICreature false
    unsigned char   _pad04[0x10 - 0x04];
    unsigned char   ThingTypeTag;                       // +0x10 (2 = creature)
    unsigned char   _pad11[0x20 - 0x11];
    unsigned long   m_tcPresent[9];                     // +0x20 interface present bits
    CTCInterfaceMap m_tcs;                              // +0x44
    unsigned char   _pad50[0x6c - 0x50];
    unsigned long   ThingTypeFlags;                     // +0x6C THING_TYPE_* bits (1 physical, 3 creature)
    CDefClassBase*  PBaseDef;                           // +0x70 CDefPointer<const CThingDef>::Object
    unsigned char   _pad74[0x91 - 0x74];
    unsigned char   Dead : 1;                           // +0x91 bit 0
    unsigned char   _bits91 : 7;
    unsigned char   _pad92[0xbc - 0x92];
    unsigned char   PhysicalFlags;                      // +0xBC (CThingPhysical)
    unsigned char   _padbd[0xc0 - 0xbd];
    __forceinline unsigned long HasTC(long type) const { return m_tcPresent[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CTCInterfaceMap* map = &m_tcs;
        CTCInterfaceEntry* it = map->LowerBound(key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CTCInterfaceMap* map = &m_tcs;
        CTCInterfaceEntry* it = map->LowerBound(key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long type) const
    {
        if (HasTC(type)) {
            long key = type;
            const CTCInterfaceMap* map = &m_tcs;
            CTCInterfaceEntry* it = map->LowerBound(key);
            *ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    CPlayer& GetPlayer();                               // 0x004C7A10
    template<class T> __forceinline bool GetInterface(T** ppOut, long type)
    {
        if (HasTC(type)) {
            long key = type;
            CTCInterfaceMap* map = &m_tcs;
            CTCInterfaceEntry* it = map->LowerBound(key);
            *ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    __forceinline const CTCCreatureModeManager* GetModeManager() const { return (const CTCCreatureModeManager*)GetTC(0x31); }
    __forceinline CTCCreatureModeManager* GetModeManager() { return (CTCCreatureModeManager*)GetTC(0x31); }
};
class CThingCreatureBase : public CThing {
public:
    unsigned char        _padc0[0x100 - 0xc0];
    CCreatureActionBase* PCurrentAction;                // +0x100
    unsigned char        _pad104[0x200 - 0x104];
    long                 MovementState;                 // +0x200 (3 tested by the sprint check)
    bool IsPerformingAnyAction() const;                 // 0x006622A0
};
namespace NCreatureMode {
bool __fastcall IsIncapacitated(const CThing& thing);
}

bool __fastcall NCreatureMode::IsIncapacitated(const CThing& thing)
{
    if (thing.HasTC(0x31)) {
        long mode = thing.GetModeManager()->GetCurrentMode().GetType();
        return mode == 0x1d || mode == 0x43;
    }
    return false;
}