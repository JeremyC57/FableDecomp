// CTCWillResponse::SetThingIsEpicSpellable(CThing* thing, bool value) -- set the thing's IsEpicSpellable will-response flag: clearing it adds the CTCWillResponse component when it is missing (every response defaults to allowed); setting it on an existing component removes the component again once every response flag is back at its default
// Leaf: ?SetThingIsEpicSpellable@CTCWillResponse@@SIXPAVCThing@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E4330, 241 bytes. Ego_r.pdb / FableWin.pdb
//   `?SetThingIsEpicSpellable@CTCWillResponse@@SIXPAVCThing@@_N@Z` = public static void __fastcall (CThing*, bool) (ecx thing,
//   dl flag); the private inline members `SetThingIsEpicSpellable(bool)` (`AAEX_N`) and `Cleanup()` are expanded here.
// Label correction: the catalog's "SetThingIsAbleToBeLifted" is wrong: the body clears / inserts bit 6 (`and 0BFh`,
//   `shl bl,6`), which is IsEpicSpellable in the PDB field order; it is the last setter of the PDB static order.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the non-const copy), CCharString::CCharString
//   (const char*, long) @ 0x0099EBF0 ("CTCWillResponse" @ 0x0123E30C), CThing::AddTC(const CCharString&, bool,
//   CTCParamsBase*) @ 0x004C9D60, ~CCharString @ 0x0099EAE0, CThing::RemoveTC(long) @ 0x004C9840.
// Layout: CTCWillResponse (Ego_r.pdb, 0x14 bytes): ParentThing +4, response bits at +0x10 in PDB field order (IsEpicSpellable
//   bit 6); interface type 0x9c = presence word +0x30 bit 28; the thing is the retail local model of the landed
//   sibling CTCWillResponse::SetThingAsNotBloodyWillableAtAll 0x007E4040.
// Shape: the sibling's body with the polarity inverted (the flags default to true): `if (!value)` get-or-add and store a
//   literal `false` (`and byte ptr [esi+10h], mask` in both tail-duplicated arms), else on an existing component store the
//   variable through VC's xor bit insert and run Cleanup (the six default-true bits, then the cleared one, before
//   `ParentThing->RemoveTC(0x9c)`).
class CTCBase;
class CTCParamsBase;

class CCharString {
public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;

template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const ETCInterfaceType& kind);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline T* GetTC()
    {
        CTCMap* map = &m_tcs;
        STCEntry* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
        return (T*)((it == map->m_pLast || T::INTERFACE_TYPE < it->first) ? map->m_pLast : it)->second;
    }
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
    void RemoveTC(ETCInterfaceType kind);                   // 0x004C9840
};

class CTCWillResponse {
public:
    enum { INTERFACE_TYPE = 0x9c };
    void*         _vfptr;
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[4];
    void*         PDef;                         // +0x0c
    bool IsForcePushable : 1;                   // +0x10 bit 0
    bool IsLifeDrainable : 1;
    bool IsLightningable : 1;
    bool IsNotBloodyWillable : 1;
    bool IsAbleToBeLifted : 1;
    bool IsEpicPowerExplodable : 1;
    bool IsEpicSpellable : 1;                   // bit 6
    void SetThingIsEpicSpellable(bool value) { IsEpicSpellable = value; }
    void Cleanup()
    {
        if (IsForcePushable && IsLifeDrainable && IsLightningable && IsAbleToBeLifted && IsEpicPowerExplodable &&
            IsEpicSpellable && !IsNotBloodyWillable)
            ParentThing->RemoveTC((ETCInterfaceType)(INTERFACE_TYPE));
    }
    static void __fastcall SetThingIsEpicSpellable(CThing* thing, bool value);
};

void __fastcall CTCWillResponse::SetThingIsEpicSpellable(CThing* thing, bool value)
{
    if (!value) {
        CTCWillResponse* tc;
        if (thing->HasTC(INTERFACE_TYPE))
            tc = thing->GetTC<CTCWillResponse>();
        else
            tc = (CTCWillResponse*)thing->AddTC("CTCWillResponse", true, 0);
        tc->SetThingIsEpicSpellable(false);
    } else if (thing->HasTC(INTERFACE_TYPE)) {
        CTCWillResponse* tc = thing->GetTC<CTCWillResponse>();
        tc->SetThingIsEpicSpellable(value);
        tc->Cleanup();
    }
}