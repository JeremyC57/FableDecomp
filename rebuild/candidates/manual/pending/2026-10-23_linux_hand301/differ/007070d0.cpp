// Grade: DIFFER(910v868) -- first pass: changed flag, graphic-list clear (_Move + _Destroy), HeroMorph TC 3 lookup, layer/entry/graphic loops, GetDef + two GetSubDef calls and both push_back sites match in shape; retail does the flag as an or-byte to memory, keeps ebp=0 and ebx=&ModifierGraphics, and gives each push_back its own CModifierGraphic temporary (+8 frame); ours splits the flag RMW, swaps the zero/this registers and shares the temporaries
// CTCHeroAttachableAppearanceModifiers::UpdateGraphics() -- flags the appearance as changed, clears the graphic list, then for every visible modifier on every layer pushes each of its def's graphics (morph-gated ones only while the hero's morph strength lies in the graphic's [min, max) window)
class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;
    mutable long RefCount;
};
template<class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CGraphicEntry {
    long _pad00;
    long MorphType;                                     // +0x04
    long GraphicA;                                      // +0x08
    long GraphicB;                                      // +0x0C
    float MinStrength;                                  // +0x10
    float MaxStrength;                                  // +0x14
};
template<class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
};
class CGraphicDef : public CDefClassBase {};
class CAppearanceModifierDef : public CDefClassBase {
public:
    unsigned char _pad08[0x34 - 0x08];
    vector<CGraphicEntry> Graphics;                     // +0x34
};
class CDefinition : public CDefClassBase {
public:
    bool GetSubDef(CDefPointer<const CAppearanceModifierDef>* out) const;   // 0x0057FF92
    bool GetSubDef(CDefPointer<const CGraphicDef>* out) const;              // 0x007086F0
};
class CDefinitionManager {
public:
    bool GetDef(long index, CDefPointer<const CDefinition>* out) const;     // 0x0042B040
};
// Same-TU stand-ins: link-time codegen knows the out pointers do not escape.
#pragma auto_inline(off)
bool CDefinitionManager::GetDef(long index, CDefPointer<const CDefinition>* out) const
{
    out->Object = (const CDefinition*)index;
    return index != 0;
}
bool CDefinition::GetSubDef(CDefPointer<const CAppearanceModifierDef>* out) const
{
    out->Object = (const CAppearanceModifierDef*)this;
    return true;
}
bool CDefinition::GetSubDef(CDefPointer<const CGraphicDef>* out) const
{
    out->Object = (const CGraphicDef*)this;
    return true;
}
#pragma auto_inline(on)
struct __false_type {};
inline void* operator new(unsigned int, void* where) throw() { return where; }
class CModifierGraphic {
public:
    CDefPointer<const CGraphicDef> Def;
    long GraphicA;
    long GraphicB;
    __forceinline CModifierGraphic(CDefPointer<const CGraphicDef> def, long a, long b) : Def(def), GraphicA(a), GraphicB(b) {}
    __forceinline ~CModifierGraphic()
    {
        if (Def.Object) {
            if (--Def.Object->RefCount == 0)
                Def.Object->Destroy();
            Def.Object = 0;
        }
    }
};
CModifierGraphic* __fastcall MoveGraphics(CModifierGraphic* first, CModifierGraphic* last, CModifierGraphic* result, const __false_type&, long);   // 0x00708C20
void __fastcall DestroyGraphics(CModifierGraphic* first, CModifierGraphic* last, const __false_type&);   // 0x00709350
struct CModifierGraphicVector {
    CModifierGraphic* _M_start;
    CModifierGraphic* _M_finish;
    CModifierGraphic* _M_end_of_storage;
    void _M_insert_overflow(CModifierGraphic* pos, const CModifierGraphic& x, const __false_type&, unsigned int fill, bool atEnd);   // 0x00709840
    __forceinline void clear()
    {
        __false_type moveTag;
        CModifierGraphic* i = MoveGraphics(_M_finish, _M_finish, _M_start, moveTag, 0);
        __false_type destroyTag;
        DestroyGraphics(i, _M_finish, destroyTag);
        _M_finish = i;
    }
    __forceinline void push_back(const CModifierGraphic& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CModifierGraphic(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
class CTCHeroMorph {
public:
    float GetMorphStrength(long type) const;            // 0x0071BA60
};
struct CTCPair { long Type; void* TC; };
struct CTCMap {
    CTCPair* _M_start;
    CTCPair* _M_finish;
    CTCPair* LowerBound(const long& key);               // 0x0040F020
    __forceinline void* Find(long type)
    {
        long key = type;
        CTCPair* it = LowerBound(key);
        if (it == _M_finish || it->Type > type)
            it = _M_finish;
        return it->TC;
    }
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long PresentBits[9];                       // +0x20
    CTCMap TCs;                                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
};
struct CModifierEntry {
    long DefIndex;
    bool Visible;                                       // +0x04
};
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
    const CDefinitionManager* GetDefinitions() const;   // 0x006869D0
};
class CTCHeroAttachableAppearanceModifiers : public CTCBase {
public:
    unsigned char _pad0c[0x14 - 0x0c];
    unsigned long Flags;                                // +0x14 (bit 1: graphics changed)
    unsigned char _pad18[0x30 - 0x18];
    vector< vector<CModifierEntry> > AppearanceModifiers;   // +0x30
    CModifierGraphicVector ModifierGraphics;            // +0x3C
    void UpdateGraphics();
};

void CTCHeroAttachableAppearanceModifiers::UpdateGraphics()
{
    Flags |= 2;
    ModifierGraphics.clear();
    CTCHeroMorph* morph = 0;
    if (Thing->HasTC(3))
        morph = (CTCHeroMorph*)Thing->TCs.Find(3);
    for (unsigned int layer = 0; layer < AppearanceModifiers.size(); ++layer) {
        vector<CModifierEntry>& entries = AppearanceModifiers._M_start[layer];
        for (CModifierEntry* e = entries._M_start; e != entries._M_finish; ++e) {
            if (!e->Visible)
                continue;
            CDefPointer<const CDefinition> def;
            GetDefinitions()->GetDef(e->DefIndex, &def);
            CDefPointer<const CAppearanceModifierDef> mod;
            def->GetSubDef(&mod);
            CDefPointer<const CGraphicDef> graphic;
            def->GetSubDef(&graphic);
            for (long i = 0; i < (long)mod->Graphics.size(); ++i) {
                CGraphicEntry& g = mod->Graphics._M_start[i];
                if (g.MorphType == 0) {
                    ModifierGraphics.push_back(CModifierGraphic(graphic, g.GraphicA, g.GraphicB));
                } else {
                    float strength = morph->GetMorphStrength(g.MorphType);
                    if (strength >= g.MinStrength && g.MaxStrength > strength)
                        ModifierGraphics.push_back(CModifierGraphic(graphic, g.GraphicA, g.GraphicB));
                }
            }
        }
    }
}
