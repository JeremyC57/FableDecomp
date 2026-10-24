// NGraphicAppearance::CTCGraphicAppearance::OnCreate() -- vtable slot 4: cache the owner's animation component and, when the owner's def has an appearance sub-def, take its lighting channel
// Grade: DIFFER(ours v retail) -- 144v144: the GetInterface component load lands in ecx (`mov ecx,[eax+4] ; mov [ebp+24h],ecx`), retail eax; all else matches (tried: member out pointer, ternary vs if-assign GetInterface, a local out + assign (fixes eax, rotates the later edx/al/ecx), HasTC+GetTC)
// Leaf: ?OnCreate@CTCGraphicAppearance@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BFC50, 144 bytes.
// Name: slot 4 of the vtable 0x0123986C (rtti-vtables.tsv); slot 4 of the sibling component vtables is the landed
// CTCGraphicAppearanceSprite::OnCreate 0x0077C480 and the OnCreate rows 0x007E1570 / 0x007A6740 of this chunk.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound(const long*) @ 0x0040F020 (the inlined GetInterface, type
// 0x5A = bit 26 of the bitset word +0x28), CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2 (landed as
// GetSubDef_0042b0a2(CDefPointer&)), CTCGraphicAppearance::SetLightingChannel(const CEnginePrimitiveThemeChannelDesc&,
// bool, bool) @ 0x004BFB40 (landed), the def's virtual slot 1 (release to zero).
// Layout: CTCBase ParentThing +4; PTCAnimation +0x24 (Ego_r.pdb +0x2C less the two 4-byte-smaller retail maps); retail
// CThing bitset +0x20, interface map +0x44, base def +0x70 (as the landed CatchMessage 0x004BFD10); retail CAppearanceDef
// LightingChannel +0x58 (PDB +0x5C, the def base 4 bytes smaller); CEnginePrimitiveThemeChannelDesc = four bytes
// (ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad).
// Shape: GetInterface takes the MEMBER's address as its out pointer (`mov [ebp+24h],eax`); the zeroed def pointer, the
// lookup key and the channel desc all share the one `push ecx` frame word; the desc is built by its inline constructor
// (three byte stores in the body, Pad untouched) after the pushes; the def is released by the pointer's inline
// destructor (`dec [esi+4] ; jne ; call [edx+4]`). Plain `ret`.
class CTCBase;
class CTCAnimationBase;

struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                                 // slot 1 (+0x04)
    long RefCount;                                          // +0x04
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x58 - 0x08];
    long          LightingChannel;                          // +0x58 ELightingChannel
};
struct CDefPointer {                                        // CDefPointer<const CAppearanceDef>
    CAppearanceDef* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                if (--m_p->RefCount == 0)
                    m_p->Destroy();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->RefCount;
        }
        return *this;
    }
    ~CDefPointer()
    {
        if (m_p) {
            if (--m_p->RefCount == 0)
                m_p->Destroy();
        }
    }
    CAppearanceDef* operator->() const { return m_p; }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CDefinitionManager {
    CDefPointer GetPDefFromGlobalIndex(long index);         // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual long* GetSubDefIndex(const CCharString& name);  // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 0x04];
    CDefinitionManager* m_pManager;                         // +0x1c
    bool GetSubDef_0042b0a2(CDefPointer& out);              // 0x0042B0A2
};
#pragma auto_inline(off)
// Same-TU stand-in for CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (its landed body, as the landed
// CatchMessage 0x004BFD10 does): VC7.1 then knows the def pointer's address does not escape, so the def is
// held in esi after the call and its frame word is reused for the channel desc. Only the target row is graded.
bool CDefClassBase::GetSubDef_0042b0a2(CDefPointer& out)
{
    long* pIndex;
    {
        CCharString name("CAppearanceDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                        // +0x20
    CInterfaceMap  m_interfaces;                            // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PBaseDef;                                // +0x70 CDefPointer<CThingBaseDef const>::Object
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

struct CEnginePrimitiveThemeChannelDesc {
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

struct CTCGraphicAppearance {
    virtual void v00();
    CThing*           ParentThing;                          // +0x04
    unsigned char     _pad08[0x24 - 0x08];                  // ToBeRemoved.., MeshEffects, TintColours
    CTCAnimationBase* PTCAnimation;                         // +0x24
    void SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool includeCarried, bool includeWeapons);   // 0x004BFB40
    virtual void OnCreate();
};

void CTCGraphicAppearance::OnCreate()
{
    CThing* parent = ParentThing;
    parent->GetInterface(&PTCAnimation, 0x5a);
    CDefPointer def;
    if (parent->PBaseDef->GetSubDef_0042b0a2(def))
        SetLightingChannel(CEnginePrimitiveThemeChannelDesc((unsigned char)def->LightingChannel), true, true);
}

} // namespace NGraphicAppearance
