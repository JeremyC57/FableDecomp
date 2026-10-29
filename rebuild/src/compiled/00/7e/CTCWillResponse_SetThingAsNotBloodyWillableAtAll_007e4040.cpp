// CTCWillResponse::SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable) -- set the thing's IsNotBloodyWillable will-response flag, adding the CTCWillResponse component when it is missing; clearing it on an existing component removes the component again once every response flag is back at its default
// Leaf: ?SetThingAsNotBloodyWillableAtAll@CTCWillResponse@@SIXPAVCThing@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E4040, 237 bytes. FableWin.pdb
//   `?SetThingAsNotBloodyWillableAtAll@CTCWillResponse@@SIXPAVCThing@@_N@Z` = public static void __fastcall (CThing*, bool)
//   (ecx thing, dl flag); the private inline members `SetThingAsNotBloodyWillableAtAll(bool)` (`AAEX_N`) and
//   `Cleanup()` (`AAEXXZ`) are expanded here.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the non-const copy), CCharString::CCharString
//   (const char*, long) @ 0x0099EBF0 ("CTCWillResponse" @ 0x0123E30C), CThing::AddTC(const CCharString&, bool,
//   CTCParamsBase*) @ 0x004C9D60, ~CCharString @ 0x0099EAE0, CThing::RemoveTC(long) @ 0x004C9840.
// Layout: CTCWillResponse (Ego_r.pdb, 0x14 bytes): ParentThing +4, response bits at +0x10 in PDB field order; interface
//   type 0x9c = presence word +0x30 bit 28; the thing is the retail local model of landed 0x00664D00.
// Shape: the presence word is hoisted above the flag test (both arms test it). The set arm stores the variable through
//   VC's xor bit insert; the clear arm passes a literal `false` (`and bl,0F7h`) and Cleanup tests the six default-true
//   bits then the cleared one in PDB order before `ParentThing->RemoveTC(0x9c)`. The AddTC name is the implicit
//   `const char*` -> CCharString conversion (address re-taken with `lea` after the ctor). GetTC is the one-ternary pick
//   `((it == last || kind < it->kind) ? last : it)->component`, which leaves the clear arm's component in ecx (the if-assign
//   form puts it in eax and swaps al/cl in the bit tests).
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
    void RemoveTC(long kind);                   // 0x004C9840
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
    void SetThingAsNotBloodyWillableAtAll(bool notWillable) { IsNotBloodyWillable = notWillable; }
    void Cleanup()
    {
        if (IsForcePushable && IsLifeDrainable && IsLightningable && IsAbleToBeLifted && IsEpicPowerExplodable &&
            IsEpicSpellable && !IsNotBloodyWillable)
            ParentThing->RemoveTC(INTERFACE_TYPE);
    }
    static void __fastcall SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable);
};

void __fastcall CTCWillResponse::SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable)
{
    if (notWillable) {
        CTCWillResponse* tc;
        if (thing->HasTC(INTERFACE_TYPE))
            tc = thing->GetTC<CTCWillResponse>();
        else
            tc = (CTCWillResponse*)thing->AddTC("CTCWillResponse", true, 0);
        tc->SetThingAsNotBloodyWillableAtAll(notWillable);
    } else if (thing->HasTC(INTERFACE_TYPE)) {
        CTCWillResponse* tc = thing->GetTC<CTCWillResponse>();
        tc->SetThingAsNotBloodyWillableAtAll(false);
        tc->Cleanup();
    }
}