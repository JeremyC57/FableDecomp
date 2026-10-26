// NFire::CFirePoint::Reset() -- kill the live fire and singe effects; when the point was burning, spawn the "Flaming_Hands_Die" effect at the parent's position and attach its particle emitter to the parent's helper point; the point stops burning
// Leaf: Reset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00780700, length 249 bytes (+ int3 padding), plain `ret`.
// Signature from Ego_r.pdb: ?Reset@CFirePoint@NFire@@QAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80, CThing::GetPosition @ 0x004C73D0,
//   the __fastcall effect-thing factory Create(const CCharString&, const C3DVector&, bool) @ 0x006E0970 (landed callers
//   0x00781F10), CCharString(const char*, long) @ 0x0099EBF0 ("Flaming_Hands_Die" @ 0x0126B18C) / ~CCharString @ 0x0099EAE0,
//   the interface map LowerBound @ 0x0040F020, CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float)
//   @ 0x006E0BE0.
// Layout = Ego_r.pdb NFire::CFirePoint (44 bytes): HelperName +0, HelperIndex +4, Burning +0x10, PParentThing +0x18,
//   PEffect +0x1c, PSingeEffect +0x24 (CIntelligentPointer<CThing>); retail CThing interface bitset +0x20, map +0x44, dead
//   bit +0x91 bit 0.
// Shape: each effect is fetched three times through GetPItem (null test, dead test, Kill); the name is a named local in its
//   own block; the emitter lookup is `T* p = 0; if (HasTC(k)) p = GetTC(k);` (the preloaded `xor ecx,ecx`) and is used
//   unchecked; GetTC is `__forceinline` with a map pointer local (`add edi,44h`, `this` stays in esi) and the end/hit
//   choice as one ternary (the component lands straight in ecx); the Burning clear sits after the join.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    void Kill(bool immediately);                        // 0x004C9B80
    const C3DVector& GetPosition() const;               // 0x004C73D0
    bool HasTC(long type) const { if (m_presentBits[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
CThing* __fastcall Create(const CCharString& defName, const C3DVector& position, bool flag);   // 0x006E0970

class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float delay);   // 0x006E0BE0
};
struct CBaseIntelligentPointer {
    void*   _vptr;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};

namespace NFire {
class CFirePoint { public:
    CCharString             HelperName;                 // +0x00
    long                    HelperIndex;                // +0x04
    long                    EffectIndex;                // +0x08
    long                    Resistance;                 // +0x0c
    bool                    Burning;                    // +0x10
    long                    FireDamageAfterFrame;       // +0x14
    CThing*                 PParentThing;               // +0x18
    CBaseIntelligentPointer PEffect;                    // +0x1c
    CBaseIntelligentPointer PSingeEffect;               // +0x24
    void Reset();
};

void CFirePoint::Reset()
{
    if (PEffect.GetPItem() && !PEffect.GetPItem()->IsDead())
        PEffect.GetPItem()->Kill(true);
    if (PSingeEffect.GetPItem() && !PSingeEffect.GetPItem()->IsDead())
        PSingeEffect.GetPItem()->Kill(true);
    if (Burning) {
        CThing* effect;
        {
            CCharString name("Flaming_Hands_Die", -1);
            effect = Create(name, PParentThing->GetPosition(), false);
        }
        if (effect && !effect->IsDead()) {
            CTCDParticleEmitter* emitter = 0;
            if (effect->HasTC(0x4b))
                emitter = effect->GetTC<CTCDParticleEmitter>(0x4b);
            emitter->AttachToThing(PParentThing, 0x1c, HelperName, HelperIndex, 0.0f);
        }
    }
    Burning = false;
}
}