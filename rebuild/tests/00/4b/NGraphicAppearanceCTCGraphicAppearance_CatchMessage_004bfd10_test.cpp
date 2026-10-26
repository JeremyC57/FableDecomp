// Behaviour test for 004bfd10: src/004bfd10.cpp copied verbatim; the def lookup (through the same-TU GetSubDef body),
// the interface maps, the owned-thing list and SetLightingChannel stubbed so the override gate, the owner's channel
// copy to this appearance and to every live owned thing with an appearance, and the def release are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NGraphicAppearance::CTCGraphicAppearance::CatchMessage(const NTCMessages::CMessageOnSetThingOwner& message) -- unless the appearance def overrides its lighting channel, take the new owner's lighting channel (carried things included) and pass it to every live owned thing's appearance
// Retry 10b (getsw sweep, FIXES B-15): the base def is read through an inline getter `GetBaseDef()`; the plain member read gives the parked register/scheduling difference.
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
namespace NGraphicAppearance { class CTCGraphicAppearance; }

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CDefPointeeBase { public:
    virtual void Destroy0();
    virtual void Destroy();                                 // slot 1 (+0x04)
    long RefCount;                                          // +0x04
};
template <class T> class CDefPointer { public:
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
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x56 - 0x08];
    bool OverrideLightingChannel;                           // +0x56
};
class CDefinitionManager { public:
    CDefPointer<CAppearanceDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CThingBaseDef : public CDefPointeeBase { public:
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
    CThingBaseDef* GetBaseDef() const { return PBaseDef; }
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

class CEnginePrimitiveThemeChannelDesc { public:                   // one dword
    unsigned long Channel;
};

namespace NTCMessages {
class CMessageOnSetThingOwner { public:
    virtual void v00();
    CThing* NewOwner;                                       // +0x04
};
}

namespace NGraphicAppearance {

class CTCGraphicAppearance { public:
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
    if (!ParentThing->GetBaseDef()->GetSubDef(&def) || !def->OverrideLightingChannel) {
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
// ---- end source ----
void CThingBaseDef::v02() {} void CThingBaseDef::v03() {} void CThingBaseDef::v04() {} void CThingBaseDef::v05() {} void CThingBaseDef::v06() {} void CThingBaseDef::v07() {} void CThingBaseDef::v08() {} void CThingBaseDef::v09() {} void CThingBaseDef::v10() {} void CThingBaseDef::v11() {} void CThingBaseDef::v12() {} void CThingBaseDef::v13() {}
void CBaseIntelligentPointer::v00() {}
void NTCMessages::CMessageOnSetThingOwner::v00() {}
using namespace NGraphicAppearance;
static int g_destroys = 0;
void CDefPointeeBase::Destroy0() {}
void CDefPointeeBase::Destroy() { ++g_destroys; }
static long g_index = 5; static bool g_haveIndex = true;
long* CThingBaseDef::GetSubDefIndex(const CCharString&) const { return g_haveIndex ? &g_index : 0; }
static CAppearanceDef g_app;
CDefPointer<CAppearanceDef> CDefinitionManager::GetPDefFromGlobalIndex(long)
{ static void* s_holder; s_holder = &g_app; ++g_app.RefCount; return *(CDefPointer<CAppearanceDef>*)&s_holder; }
CCharString::CCharString(const char* t, long) : PStringData((void*)t) {}
CCharString::~CCharString() {}
static CInterfaceMap::SEntry* Find(CInterfaceMap* m, long t) { for (CInterfaceMap::SEntry* e = m->first; e != m->last; ++e) if (e->type >= t) return e; return m->last; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return Find(this, *type); }
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { return Find((CInterfaceMap*)this, *type); }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static CTCGraphicAppearance* g_set[8]; static unsigned long g_channels[8]; static int g_nSet = 0;
void CTCGraphicAppearance::SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& c, bool a, bool b)
{ if (a && b) { g_set[g_nSet] = this; g_channels[g_nSet++] = c.Channel; } }
int main()
{
    static CDefinitionManager dm; static CThingBaseDef baseDef; baseDef.m_pManager = &dm;
    g_app.RefCount = 1; g_app.OverrideLightingChannel = false;
    static CThing self, owner, child1, child2; memset(&self, 0, sizeof(self)); memset(&owner, 0, sizeof(owner)); memset(&child1, 0, sizeof(child1)); memset(&child2, 0, sizeof(child2));
    self.PBaseDef = &baseDef;
    static CTCGraphicAppearance selfApp, ownerApp, child1App; memset(&selfApp, 0, sizeof(selfApp)); memset(&ownerApp, 0, sizeof(ownerApp));
    selfApp.ParentThing = &self; ownerApp.LightingChannel.Channel = 0x33;
    static CInterfaceMap::SEntry ownerEntries[1] = { { 0x5b, (CTCBase*)&ownerApp } };
    owner.m_interfaces.first = ownerEntries; owner.m_interfaces.last = ownerEntries + 1; owner.m_presentBits[0x5b >> 5] |= 1UL << (0x5b & 31);
    static CTCOwnedThings owned; static SOwnedThingNode head, n1, n2;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &head; n1._M_data.m_pItem = &child1; n2._M_data.m_pItem = &child2;
    owned.OwnedThings = &head;
    static CInterfaceMap::SEntry selfEntries[1] = { { 0x21, (CTCBase*)&owned } };
    self.m_interfaces.first = selfEntries; self.m_interfaces.last = selfEntries + 1; self.m_presentBits[0x21 >> 5] |= 1UL << (0x21 & 31);
    child1.PGraphicAppearance = &child1App; child2.Flags91 = 1; child2.PGraphicAppearance = &child1App;   // child2 dead
    NTCMessages::CMessageOnSetThingOwner msg; msg.NewOwner = &owner;
    selfApp.CatchMessage(msg);
    CHECK(g_nSet == 2 && g_set[0] == &selfApp && g_set[1] == &child1App && g_channels[0] == 0x33 && g_channels[1] == 0x33);
    CHECK(g_app.RefCount == 1 && g_destroys == 0);
    g_app.OverrideLightingChannel = true; g_nSet = 0;
    selfApp.CatchMessage(msg); CHECK(g_nSet == 0 && g_app.RefCount == 1);      // the def overrides
    g_haveIndex = false;
    selfApp.CatchMessage(msg); CHECK(g_nSet == 2);                            // no def: copy again
    msg.NewOwner = 0; g_nSet = 0;
    selfApp.CatchMessage(msg); CHECK(g_nSet == 0);
    if (fails == 0) printf("004bfd10_TEST PASS\n"); else printf("004bfd10_TEST FAIL\n");
    return fails;
}