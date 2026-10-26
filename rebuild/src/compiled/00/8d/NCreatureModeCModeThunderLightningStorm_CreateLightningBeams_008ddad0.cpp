// NCreatureMode::CModeThunderLightningStorm::CreateLightningBeams() -- spawn NumberOfBeams LIGHTNING_BEAM_EFFECT things at the
//   caster (main beams, flagged MainBeamsCreated) and NumberOfBeams LIGHTNING_BEAM_END_EFFECT things, keeping intelligent pointers
//   to the live ones in the two emitter vectors; the special-effects sub-def reference is released at the end
// Leaf: CreateLightningBeams
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DDAD0, 661 bytes (`ret`).
// Matching: the first GetEffectFromString (0x00669D30) and the first push_back (0x004AD520) stay out of line; at the second site
//   VC7.1 inlines both (CRC Calc + map find, and the STLport placement-new push_back with the 0x004AC940 overflow). The inline
//   CRC needs `len` then `str` as named locals in that order (retail's `[ecx+esi]` index-first SIB); `y = Header; return y;` and
//   the caller's `!= Effects.end()` member re-read give retail's find tail.
class C3DVector { public:
    float X, Y, Z;
};
extern char g_emptyString[];                            // 0x0129AAF4
extern const unsigned long g_crcTable_0129a168[256];    // 0x0129A168
struct CStringData { char* Text; unsigned long Length; };
class CCharString { public:
    CStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    unsigned long Length() const { return PStringData == 0 ? 0 : PStringData->Length; }
    const char* c_str() const { return PStringData == 0 ? g_emptyString : PStringData->Text; }
};
__forceinline unsigned long CalcCRC(unsigned long seed, const char* data, unsigned long length)
{
    if (length == 0)
        return seed;
    for (unsigned long index = 0; index < length; ++index)
        seed = g_crcTable_0129a168[((const unsigned char*)data)[index] ^ (seed & 0xff)] ^ (seed >> 8);
    return seed;
}
struct CDefBase {
    virtual ~CDefBase();                        // slot 0
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
};
struct SEffectNode {                            // STLport _Rb_tree_node<pair<const unsigned long, long> >
    long          Color;
    SEffectNode*  Parent;                       // +0x04
    SEffectNode*  Left;                         // +0x08
    SEffectNode*  Right;                        // +0x0c
    unsigned long Key;                          // +0x10
    long          Value;                        // +0x14
};
struct CEffectMap {
    SEffectNode* Header;
    SEffectNode* end() const { return Header; }
    __forceinline SEffectNode* find(const unsigned long& key) const
    {
        SEffectNode* y = Header;
        SEffectNode* x = Header->Parent;
        while (x != 0) {
            if (!(x->Key < key)) {
                y = x;
                x = x->Left;
            } else
                x = x->Right;
        }
        if (y == Header || key < y->Key)
            y = Header;
        return y;
    }
};
class CSpecialEffectsDef : public CDefBase { public:
    unsigned char _pad08[0x28 - 0x08];
    CEffectMap    Effects;                      // +0x28 map<unsigned long, long> (header node pointer)
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
    __forceinline bool GetEffectFromStringInline(const CCharString& name, long& type) const
    {
        unsigned long len = name.Length();
        const char* str = name.c_str();
        unsigned long key = CalcCRC(0, str, len);
        SEffectNode* it = Effects.find(key);
        if (it != Effects.end()) {
            type = it->Value;
            return true;
        }
        return false;
    }
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
class CSpecialAbilitiesThunderLightningStormDef : public CDefClassBase { public:
    unsigned char _pad08[0x3c - 0x08];
    long          NumberOfBeams;                // +0x3c
};
class CThing { public:
    unsigned char  _pad0[0x91];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector& position, bool flag);   // 0x006E0880

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); }
};
inline void* operator new(unsigned int, void* place) { return place; }
struct __false_type { __false_type() {} };
struct CIntelligentPointerVector {                    // STLport vector<CIntelligentPointer<CThing> > (12 bytes)
    CIntelligentPointerThing* _M_start;
    CIntelligentPointerThing* _M_finish;
    CIntelligentPointerThing* _M_end_of_storage;
    void push_back(const CIntelligentPointerThing& x); // 0x004AD520
    void _M_insert_overflow(CIntelligentPointerThing* position, const CIntelligentPointerThing& x, const __false_type&,
                            unsigned int count, bool atEnd);   // 0x004AC940
    __forceinline void push_back_inline(const CIntelligentPointerThing& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CIntelligentPointerThing(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

struct CModeBase {
    virtual ~CModeBase();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x14];
};
namespace NCreatureMode {
struct CModeThunderLightningStorm : CModeBase {
    unsigned char             _pad1c[0x74 - 0x1c];
    CIntelligentPointerVector LightningBeamParticleEmitters;     // +0x74
    CIntelligentPointerVector LightningBeamEndParticleEmitters;  // +0x80
    CIntelligentPointerThing  PSkyBeam;                          // +0x8c
    CIntelligentPointerThing  PCreatorEffect;                    // +0x94
    CIntelligentPointerThing  PHandGlow;                         // +0x9c
    CSpecialAbilitiesThunderLightningStormDef* PLightningStormDef;   // +0xa4
    CIntelligentPointerVector HitThings;                         // +0xa8
    bool                      MainBeamsCreated;                  // +0xb4
    void CreateLightningBeams();
};

void CModeThunderLightningStorm::CreateLightningBeams()
{
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    PLightningStormDef->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects->GetEffectFromString("LIGHTNING_BEAM_EFFECT", effectType)) {
        MainBeamsCreated = true;
        C3DVector position = ParentThing->GetPosition();
        for (long i = 0; i < PLightningStormDef->NumberOfBeams; ++i) {
            CThing* beam = CreateEffectThing(effectType, position, false);
            if (beam && !(beam->m_flags91 & 1))
                LightningBeamParticleEmitters.push_back(CIntelligentPointerThing(beam));
        }
    }
    if (effects->GetEffectFromStringInline("LIGHTNING_BEAM_END_EFFECT", effectType)) {
        C3DVector position = ParentThing->GetPosition();
        for (long i = 0; i < PLightningStormDef->NumberOfBeams; ++i) {
            CThing* beam = CreateEffectThing(effectType, position, false);
            if (beam && !(beam->m_flags91 & 1))
                LightningBeamEndParticleEmitters.push_back_inline(CIntelligentPointerThing(beam));
        }
    }
    if (--effects->m_refCount == 0)
        effects->Destroy();
}
}