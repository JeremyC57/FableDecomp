// Grade: DIFFER(1468v1447) 344 difflines (wave 2 sec3: first reconstruction from the disassembly; was the empty skeleton, 3v1447 / 462 lines)
// Tried: g1 straight draft (1475v1447/431); g2 HasTC as the `unsigned long` mask (retail's `mov ebp,80000000h; test
//   ebp,ecx`), copy-then-test def pointer, the CCharString name in its own block before GetCurrentMode (1468v1447/362);
//   g3 the map key as a literal temporary (this file, 1468v1447/344).
// Residue: frame 0x48 against 0x40; retail keeps `this` in a stack slot ([esp+14h], reloaded for every virtual call) and
//   the loop iterator in ebp, ours keeps `this` in ebp; the hit-location def AddRef is a load/inc/store in retail (ours
//   `inc [edi+4]`) and the first one the reverse; the push_back null test is `test eax,eax` in retail (ours `cmp eax,edi`).
// Next: drop the unused `target` local (try the PTarget test as a bare expression statement), reuse the parameter slot
//   for the 0x31 lookup (retail's `[esp+54h]`), and model the three vectors on a shared STLport header if one lands.
// CCreatureAction_InterruptableMidAttackAutoTurn::SetupTurningAnimation(float turn) -- choose the turning strike
//   animations: the melee ability's named animation when one is set; else split the action's candidate animations
//   (CAnimComponentCombatMisc flags +8 augment-only / +9) into three vectors, keep the augment-only ones when the
//   weapon in hand has augmentation 0x200, sort them by turn (or keep the best one), else drop the ones that do not
//   fit the turn, and hand the result to SetCombatAnimations
// Leaf: SetupTurningAnimation
// Model: the landed sibling FrameUpdate 0x0087CB10 (CCombatActionBase PCreature +0x08, PMeleeAbilityDef +0xB8); the
//   candidate vector +0xF0; PTarget +0xA8.
// Callees: CDefString::operator!=(const char*) 0x005FA740; CDefStringTable::GetString 0x009D49B0 on g_DefStringTable
//   0x013CA828; CTCCreatureModeManager::GetCurrentMode 0x0073A6E0; CThingCreatureBase::GetCombatAnimationSet 0x00663260,
//   GetPHeroCombatDef 0x00663340; CAnimationTable::GetAnimation 0x005DC340; CCombatActionBase::SetCombatAnimations
//   0x00857DC0; CTCHitLocations::GetHitLocation 0x00814BF0; map find 0x008699A0; CTCCarrying::GetThingInPrimarySlot
//   0x006A1440; IS_THING_ALIVE<CThing*> 0x0040DCE0; CTCObjectAugmentations::HasAugmentationOfType 0x007660C0;
//   CAnimationEntry::GetComponent 0x00738A00; STLport vector _M_insert_overflow 0x005E1420, push_back 0x005E1EB0, copy
//   0x0062F590, operator= 0x00669BB0; std::sort 0x0087E610, __find_if 0x0087DB50, remove_copy_if 0x0087DDD0; memmove,
//   free (MSVCR71).
extern "C" void __cdecl free(void*);
extern "C" void* __cdecl memmove(void*, const void*, unsigned int);
#pragma intrinsic(memmove)

class CTCBase;
class CBaseObject;
class CBaseObjectPointer;
struct __true_type { };
class CCharString {
public:
    void* PData;
    ~CCharString();                                      // 0x0099EAE0
};
class CDefStringTable {
public:
    CCharString GetString(long tablePos) const;          // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                 // 0x013CA828
class CDefString {
public:
    long TablePos;
    bool operator!=(const char* text) const;             // 0x005FA740
};

template <class T> class vector {                        // STLport 4.6 vector<T>, malloc allocator
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    vector(const vector& other);                         // 0x0062F590
    __forceinline ~vector() { if (_M_start) free(_M_start); }
    vector& operator=(const vector& other);              // 0x00669BB0
    __forceinline T* begin() const { return _M_start; }
    __forceinline T* end() const { return _M_finish; }
    __forceinline bool empty() const { return _M_start == _M_finish; }
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int n, bool atend);   // 0x005E1420
    void push_back_call(const T& x);                     // 0x005E1EB0 (out of line)
    __forceinline void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    __forceinline void erase(T* first, T* last)
    {
        if (last != _M_finish)
            memmove(first, last, (char*)_M_finish - (char*)last);
        _M_finish = first + (_M_finish - last);
    }
};

struct CAnimComponentCombatMisc { unsigned char _pad[8]; bool AugmentOnly; bool Flag9; };
class CAnimFilterBase;
class CAnimationEntry {
public:
    void GetComponent(const CAnimComponentCombatMisc** out) const;   // 0x00738A00
};
typedef vector<const CAnimationEntry*> CAnimationEntryVector;
struct CTurnCompare { float Turn; CTurnCompare(float t) : Turn(t) { } };
void __fastcall sort_turn(const CAnimationEntry** first, const CAnimationEntry** last, CTurnCompare pred);    // 0x0087E610
const CAnimationEntry** __fastcall find_if_turn(const CAnimationEntry** first, const CAnimationEntry** last, const CTurnCompare& pred); // 0x0087DB50
const CAnimationEntry** __fastcall remove_copy_if_turn(const CAnimationEntry** first, const CAnimationEntry** last, const CAnimationEntry** out, const CTurnCompare& pred); // 0x0087DDD0
__forceinline const CAnimationEntry** remove_if_turn(const CAnimationEntry** first, const CAnimationEntry** last, const CTurnCompare& pred)
{
    first = find_if_turn(first, last, pred);
    if (first == last)
        return first;
    const CAnimationEntry** next = first;
    return remove_copy_if_turn(++next, last, first, pred);
}

class CAnimationTable {
public:
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;  // 0x005DC340
};
class CCreatureMode {
public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual const CAnimFilterBase& GetAnimFilter() const;   // slot 4 (+0x10)
};
class CTCCreatureModeManager {
public:
    CCreatureMode* GetCurrentMode();                     // 0x0073A6E0
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                              // slot 1
    long IVRefCount;                                     // +4
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object && --Object->IVRefCount == 0) Object->Destroy(); }
    __forceinline T* operator->() const { return Object; }
    __forceinline bool IsValid() const { return Object != 0; }
};
struct CMeleeAbilityDef : CDefBase { unsigned char _pad08[0x3c - 0x08]; CDefString AnimName; };   // +0x3C
struct CMapNode;
struct CHitLocationMap {
    CMapNode* _M_header;
    CMapNode* find(const long& key, CMapNode** out) const;   // 0x008699A0 (iterator out)
};
struct CHitLocationDef : CDefBase { unsigned char _pad08[0x3c - 0x08]; CHitLocationMap Augments; };   // +0x3C
struct CHeroCombatDef : CDefBase { unsigned char _pad08[0xd8 - 0x08]; bool KeepAllTurns; };          // +0xD8
struct CHitLocation { unsigned char _pad[8]; CDefPointer<CHitLocationDef> PDef; };                  // +0x08
class CTCHitLocations { public: CHitLocation* GetHitLocation(); };            // 0x00814BF0
class CThing;
class CTCCarrying { public: CThing* GetThingInPrimarySlot(); };              // 0x006A1440
class CTCObjectAugmentations { public: bool HasAugmentationOfType(long type) const; };   // 0x007660C0
bool __fastcall IS_THING_ALIVE(CThing* const& thing);    // 0x0040DCE0

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                // 0x0040F020
    SEntry* LowerBoundConst(const long* type) const;     // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                      // +0x20
    CInterfaceMap m_interfaces;                          // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char m_flags6c;                             // +0x6C bit 3: creature
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                             // +0x91 bit 0: dead
    const CAnimationTable* GetCombatAnimationSet() const;    // 0x00663260
    CDefPointer<CHeroCombatDef> GetPHeroCombatDef() const;   // 0x00663340
    __forceinline unsigned long HasTC(long t) const { return m_presentBits[t >> 5] & (1UL << (t & 31)); }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline T* GetTCConst(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBoundConst(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline bool GetInterfaceConst(T** out, long interfaceType) const
    {
        if (HasTC(interfaceType)) {
            *out = GetTCConst<T>(interfaceType);
            return true;
        }
        return false;
    }
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CCombatActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual bool v17(); virtual void v18();
    virtual bool v19();                                  // slot 19 (+0x4c)
    virtual void v20();
    virtual bool v21();                                  // slot 21 (+0x54)
    unsigned char                 _pad04[4];
    CIntelligentPointer<CThing>   PCreature;             // +0x08
    unsigned char                 _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing>   PTarget;               // +0xa8
    unsigned char                 _padb0[0xb8 - 0xb0];
    CDefPointer<CMeleeAbilityDef> PMeleeAbilityDef;      // +0xb8
    unsigned char                 _padbc[0xf0 - 0xbc];
    CAnimationEntryVector         Animations;            // +0xf0
    unsigned char                 _padfc[0x114 - 0xfc];
    void SetCombatAnimations(const CAnimationEntryVector& anims);   // 0x00857DC0
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_InterruptableMidAttackAutoTurn : public CCombatActionBase { public:
    void SetupTurningAnimation(float turn);
};

void CCreatureAction_InterruptableMidAttackAutoTurn::SetupTurningAnimation(float turn)
{
    CThing* target = 0;
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1) && (PTarget.GetPItem()->m_flags6c & 8))
        target = PTarget.GetPItem();

    if (v21()) {
        CDefPointer<CMeleeAbilityDef> def = PMeleeAbilityDef;
        if (def.IsValid() && def->AnimName != "") {
            CTCCreatureModeManager* modes;
            GetCreature()->GetInterfaceConst(&modes, 0x31);
            const CAnimationEntry* entry;
            {
                CCharString name = g_DefStringTable.GetString(def->AnimName.TablePos);
                CCreatureMode* mode = modes->GetCurrentMode();
                CThing* creature = GetCreature();
                entry = creature->GetCombatAnimationSet()->GetAnimation(name, 0, mode->GetAnimFilter());
            }
            CAnimationEntryVector anims;
            anims.push_back_call(entry);
            SetCombatAnimations(anims);
            return;
        }
    }

    bool augmented = false;
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1) && PTarget.GetPItem()->HasTC(0x7f)) {
        CHitLocation* location = PTarget.GetPItem()->GetTC<CTCHitLocations>(0x7f)->GetHitLocation();
        if (location && location->PDef.IsValid()) {
            CDefPointer<CHitLocationDef> def = location->PDef;
            CMapNode* it;
            def->Augments.find(0x200, &it);
            if (it != def->Augments._M_header) {
                CThing* creature = GetCreature();
                if (creature->HasTC(0x46)) {
                    CThing* weapon = creature->GetTCConst<CTCCarrying>(0x46)->GetThingInPrimarySlot();
                    if (IS_THING_ALIVE(weapon) && weapon->HasTC(0xbf)
                        && weapon->GetTCConst<CTCObjectAugmentations>(0xbf)->HasAugmentationOfType(0x200))
                        augmented = true;
                }
            }
        }
    }

    CAnimationEntryVector normal;
    CAnimationEntryVector special;
    CAnimationEntryVector augmentOnly;
    for (const CAnimationEntry** it = Animations.begin(); it != Animations.end(); ++it) {
        const CAnimationEntry* entry = *it;
        const CAnimComponentCombatMisc* misc;
        entry->GetComponent(&misc);
        bool augmentFlag = misc->AugmentOnly;
        bool flag9 = misc->Flag9;
        bool use = true;
        if (augmentFlag && !augmented)
            use = false;
        if (!v21() && augmentFlag)
            use = false;
        if (flag9 && !v21() && !v19())
            continue;
        if (!use)
            continue;
        if (augmentFlag)
            augmentOnly.push_back(entry);
        else if (flag9)
            special.push_back(entry);
        else
            normal.push_back(entry);
    }

    if (!augmentOnly.empty()) {
        CDefPointer<CHeroCombatDef> heroDef = GetCreature()->GetPHeroCombatDef();
        if (!heroDef->KeepAllTurns) {
            sort_turn(augmentOnly.begin(), augmentOnly.end(), CTurnCompare(turn));
            augmentOnly._M_finish = augmentOnly._M_start + 1;
        }
        SetCombatAnimations(augmentOnly);
    } else {
        CAnimationEntryVector chosen(!special.empty() ? special : normal);
        chosen.erase(remove_if_turn(chosen.begin(), chosen.end(), CTurnCompare(turn)), chosen.end());
        if (chosen.empty())
            chosen = !special.empty() ? special : normal;
        SetCombatAnimations(chosen);
    }
}
