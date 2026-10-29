// CTCBattleCharge::CleanUpEffects() -- kill the live shield effect and re-attach the live left/right feet particle effects to the charging thing
// Leaf: CleanUpEffects
// Retail 0x007A74A0, 336 bytes, `ret`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::Kill(bool) 0x004C9B80, CVectorMap<ETCInterfaceType,CTCBase*>::
// LowerBound 0x0040F020 (interface 0x4B, the particle emitter), CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E),
// ~CCharString 0x0099EAE0, CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) 0x006E0BE0.
// Layout = Ego_r.pdb CTCBattleCharge: ParentThing +4, ShieldEffect +0x50, LeftFeetEffect +0x58, RightFeetEffect +0x60
// (CIntelligentPointer<CThing>); CThing interface bitset +0x20, map +0x44, dead flag +0x91 bit 0.
// Shape: IS_THING_ALIVE (two GetPItem calls) then a third GetPItem for the use; one emitter local at function scope filled
// by the out-parameter GetInterface (left as is when the component is absent: retail then reuses the previous value or the
// shared key slot); the "" helper name is an implicit-conversion temporary.
class CTCBase;
class CBaseObject;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    void Kill(bool immediately);                        // 0x004C9B80
    bool IsDead() const { return (m_flags & 1) != 0; }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CBaseIntelligentPointer {
    void*        __vftable;
    CBaseObject* PData;
    CThing* GetPItem() const;                           // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
class CTCBattleCharge {
public:
    void*                   __vftable;                  // +0x00
    CThing*                 ParentThing;                // +0x04
    unsigned char           _pad08[0x50 - 0x08];
    CBaseIntelligentPointer ShieldEffect;               // +0x50
    CBaseIntelligentPointer LeftFeetEffect;             // +0x58
    CBaseIntelligentPointer RightFeetEffect;            // +0x60
    void CleanUpEffects();
};

void CTCBattleCharge::CleanUpEffects()
{
    if (IS_THING_ALIVE(ShieldEffect))
        ShieldEffect.GetPItem()->Kill(true);
    CThing* owner = ParentThing;
    CTCDParticleEmitter* emitter;
    if (IS_THING_ALIVE(LeftFeetEffect)) {
        LeftFeetEffect.GetPItem()->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(owner, 1, "", 0, 0.0f);
    }
    if (IS_THING_ALIVE(RightFeetEffect)) {
        RightFeetEffect.GetPItem()->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(owner, 1, "", 0, 0.0f);
    }
}