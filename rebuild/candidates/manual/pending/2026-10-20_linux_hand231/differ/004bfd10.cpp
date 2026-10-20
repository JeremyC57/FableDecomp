// Grade: DIFFER(264v264) -- register choice only: the parent/def loads go through ecx/eax where retail uses eax/ecx/edx (mov eax,[edi+4] ; mov ecx,[eax+70h] ; lea edx), the owner reload uses edx not eax, two key leas swap eax/ecx/edx and the release's vtable goes through edx not eax
// NGraphicAppearance::CTCGraphicAppearance::CatchMessage(const NTCMessages::CMessageOnSetThingOwner& message) -- unless the appearance def overrides its lighting channel, take the new owner's lighting channel (carried things included) and pass it to every live owned thing's appearance
// Leaf: CatchMessage
//
// Retail 0x004BFD10, 272 bytes incl. padding (`ret 4`). Ego_r.pdb `?CatchMessage@CTCGraphicAppearance@NGraphicAppearance@@
// QAEXABVCMessageOnSetThingOwner@NTCMessages@@@Z` (the only CatchMessage of the class taking a thing message with an owner).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>*) const @ 0x0042B0A2 on the parent's
// base def, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (the owner's CTCGraphicAppearance, type 0x5B = bit
// 27 of +0x28) and the const copy @ 0x004365B0 (the parent's owned-things component, type 0x21 = bit 1 of +0x24),
// CTCGraphicAppearance::SetLightingChannel(const CEnginePrimitiveThemeChannelDesc&, bool, bool) @ 0x004BFB40 (twice),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the def's virtual slot 1 (release to zero). The GetSubDef body above the
// function is a same-TU stand-in (see its comment; CCharString 0x0099EBF0/0x0099EAE0 and GetPDefFromGlobalIndex 0x009ADA10
// are its callees, not this function's).
// Layout: retail CAppearanceDef OverrideLightingChannel +0x56 (PDB +0x5a, the def base 4 bytes smaller); the message's new
// owner +4 (landed ctor 0x007E8310); retail CThing bitset +0x20, interface map +0x44, base def +0x70, dead bit +0x91,
// graphic appearance +0x64; the type-0x21 component's owned-thing list header +0xc (STLport list of CIntelligentPointer<CThing>).
// Shape: the sub-def out pointer is a zeroed CDefPointer local released at the end through the pointee's inline Release
// (`dec [ecx+4] ; jne`); the owner's channel is a reference local (ebp) shared by both calls; each owned thing is read with
// ONE GetPItem; the parent's lookup goes through a const thing (const LowerBound).
class CTCBase;
namespace NGraphicAppearance { struct CTCGraphicAppearance; }

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                                 // slot 1 (+0x04)
    long RefCount;                                          // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x56 - 0x08];
    bool OverrideLightingChannel;                           // +0x56
};
struct CDefinitionManager {
    CDefPointer<CAppearanceDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CThingBaseDef : CDefPointeeBase {
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual long* GetSubDefIndex(const CCharString& name) const;   // slot 14 (+0x38)
    unsigned char _pad08[0x1c - 0x08];
    CDefinitionManager* m_pManager;                         // +0x1c
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const; // 0x0042B0A2
};
#pragma auto_inline(off)
// Same-TU stand-in for CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (its landed body, written through the out
// parameter): it tells VC7.1, as retail's link-time code generation knew, that the def pointer's address does not escape,
// so the release keeps the def in ecx after the decrement instead of re-reading the frame word.
bool CThingBaseDef::GetSubDef(CDefPointer<CAppearanceDef>* out) const
{
    long* pIndex;
    {
        CCharString name("CAppearanceDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CAppearanceDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
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
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    NGraphicAppearance::CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    unsigned char _pad68[0x70 - 0x68];
    CThingBaseDef* PBaseDef;                                // +0x70 CDefPointer<CThingBaseDef const>::Object
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                                  // +0x91 bit 0 Dead
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
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                        // +0x04
    CThing* GetPItem() const;                               // 0x00A01B50
};
struct SOwnedThingNode {                                    // STLport list node of CIntelligentPointer<CThing>
    SOwnedThingNode*        _M_next;
    SOwnedThingNode*        _M_prev;
    CBaseIntelligentPointer _M_data;                        // +0x08
};
struct CTCOwnedThings {                                     // the type-0x21 component (owner side of owned entities)
    unsigned char    _pad00[0x0c];
    SOwnedThingNode* OwnedThings;                           // +0x0c list header
};

struct CEnginePrimitiveThemeChannelDesc {                   // one dword
    unsigned long Channel;
};

namespace NTCMessages {
struct CMessageOnSetThingOwner {
    virtual void v00();
    CThing* NewOwner;                                       // +0x04
};
}

namespace NGraphicAppearance {

struct CTCGraphicAppearance {
    void*          __vftable;                               // +0x00 (CTCBase)
    CThing*        ParentThing;                             // +0x04
    unsigned char  _pad08[0x40 - 0x08];
    CEnginePrimitiveThemeChannelDesc LightingChannel;       // +0x40
    void SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool includeCarried, bool includeWeapons);   // 0x004BFB40
    void CatchMessage(const NTCMessages::CMessageOnSetThingOwner& message);
};

void CTCGraphicAppearance::CatchMessage(const NTCMessages::CMessageOnSetThingOwner& message)
{
    CDefPointer<CAppearanceDef> def;
    if (!ParentThing->PBaseDef->GetSubDef(&def) || !def->OverrideLightingChannel) {
        CThing* owner = message.NewOwner;
        CTCGraphicAppearance* ownerAppearance;
        if (owner && owner->GetInterface(&ownerAppearance, 0x5b)) {
            const CEnginePrimitiveThemeChannelDesc& channel = ownerAppearance->LightingChannel;
            SetLightingChannel(channel, true, true);
            const CThing* parent = ParentThing;
            CTCOwnedThings* owned;
            if (parent->GetInterface(&owned, 0x21)) {
                for (SOwnedThingNode* node = owned->OwnedThings->_M_next; node != owned->OwnedThings; node = node->_M_next) {
                    CThing* thing = node->_M_data.GetPItem();
                    if (thing && !(thing->Flags91 & 1) && thing->PGraphicAppearance)
                        thing->PGraphicAppearance->SetLightingChannel(channel, true, true);
                }
            }
        }
    }
}

}
