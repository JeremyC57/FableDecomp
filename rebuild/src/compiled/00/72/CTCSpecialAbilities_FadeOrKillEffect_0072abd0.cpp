// CTCSpecialAbilities::FadeOrKillEffect(CIntelligentPointer<CThing>&) -- if the effect thing is alive, hand it to its particle-emitter component (interface 0x4b), which fades the emitter or kills the thing
// Grade: RELOCATION_MATCH (98v98) -- retry wave 2 sec2b: the dead test through CThing::IsDead() const (Ego_r ?IsDead@CThing@@QBE_NXZ)
//   inside the inline IS_THING_ALIVE gives retail's esi/edi colouring. Earlier note (was DIFFER 98v98, esi<->edi swapped): Tried: GetTC returning a null-
//   default pointer (94v98, flags word in edx), this out-parameter form with the local zeroed before the lookup (98v98,
//   best), a pointer-returning wrapper over it (94v98).
// Fable retry (2026-10-15): a reference local over the parameter (the 0x008DBA90 form) still grades 98v98 with esi/edi swapped; a named
//   `CThing* thing` local for the lookup drops to 94v98. The swap is a register-colouring gap in this standalone body.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072ABD0, length 98 bytes, `ret` with the pointer in ecx.
// Identity: Ego_r.pdb has `?FadeOrKillEffect@CTCSpecialAbilities@@SIXAAV?$CIntelligentPointer@VCThing@@@@@Z` (a static
//   __fastcall taking the effect pointer by reference); this body is called by CTCDivineWrath::FinishMove (0x0078F785)
//   and a dozen other spell TCs right after their own IS_THING_ALIVE test. The catalog label
//   "CCreatureAction_CastWillReclaimUndeadFollower::PerformAction" is propagated.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap::LowerBound @ 0x0040F020,
//          CTCDParticleEmitter::EnterTimelineEnd @ 0x006E05F0 (catalog + reloc_bindings; fades the emitter at +0x14 if set, else Kill(true) on the
//          parent thing; manifest label "EnterTimelineEnd" is propagated).
// Shape: IS_THING_ALIVE inlined (GetPItem null test, then the dead bit +0x91); the component accessor is the
//   GetTC form with a NULL default (`xor edi,edi` before the flags word +0x28 is loaded and `test ch,8` = bit 0x4b
//   tested), the vector-map lookup through a named map pointer in esi, and the emitter call made on the result
//   even when it is null (retail behaviour).
// Leaf: FadeOrKillEffect
class CTCBase;

enum ETCInterfaceType { };
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap {                                  // retail STLport vector<pair<K, V> > + comparator (only the used members)
public:
    typedef std::pair<K, V> value_type;
    value_type* _M_start;                           // +0x00
    value_type* _M_finish;                          // +0x04
    value_type* LowerBound(const K& key);     // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*> CInterfaceMap;

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }   // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
    template <class T> __forceinline void GetTC(T** ppOut, long interfaceType)   // leaves *ppOut alone when absent
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->_M_finish || interfaceType < it->first)
                it = map->_M_finish;
            *ppOut = (T*)it->second;
        }
    }
};

class CBaseObject;
class CBaseIntelligentPointer {             // CIntelligentPointer<T>: vptr + item
public:
    void*   __vftable;                      // +0x00
    CBaseObject* m_pItem;                   // +0x04
    CBaseObject* GetPItem() const;          // 0x00A01B50 Ego_r ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

template <class T> inline bool __fastcall IS_THING_ALIVE(const T& p)   // Ego_r ??$IS_THING_ALIVE@V?$CIntelligentPointer@...@@YI_NABV...@Z
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CTCDParticleEmitter {
public:                // interface type 0x4b component
    void EnterTimelineEnd();                // 0x006E05F0 (catalog + bound; fades the emitter at +0x14 if set, else Kill(true))
};

class CTCSpecialAbilities {
public:
    static void __fastcall FadeOrKillEffect(CIntelligentPointer<CThing>& effect);
};

void __fastcall CTCSpecialAbilities::FadeOrKillEffect(CIntelligentPointer<CThing>& effect)
{
    if (IS_THING_ALIVE(effect)) {
        CTCDParticleEmitter* emitter = 0;   // zeroed before the lookup: live across GetPItem, so it rides in edi
        effect.GetPItem()->GetTC(&emitter, 0x4b);
        emitter->EnterTimelineEnd();
    }
}