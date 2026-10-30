// CContextSensitiveItemDef::TargetCreatureLikelyToReactToDeed(CThingPlayerCreature*, CThingAICreature*, EOpinionDeedType)
// Grade: DIFFER(453v457) -- header-moved (engine/CDefPointer.h verify block, CDefClassBase): deed parameter still cached in ebp (retail reloads from its slot)
// const -- false when the target is busy (0x1F component), does not react to this deed (0x4F reaction table), is not in
// SG_DEED_REACTION_ROOT, or (for deed-reaction item types 1..0x5B) has an idle 0x0F component and the hero's 0x51 deed
// def for this deed lacks bit 0 of +0x7D; otherwise true
// Leaf: TargetCreatureLikelyToReactToDeed
// Layout: CContextSensitiveItemDef ExtraData2 +0x38 (Ego_r.pdb = retail); CThing interface bitset +0x20, map +0x44.
// Shape: the const interface lookups use a same-TU const LowerBound stand-in (0x004365B0, the landed 0x008330D0 idiom);
//   the hero's deed def is a by-value CDefPointer, a conditionally constructed temporary inside the final test.
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#define FABLE_DEFPTR_INLINE __forceinline
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
template <class T> class CDefPointer {
public:
    CDefPointer(ENullDef) : Object(0) { }                               // PROPOSED header addition (FableWin ??0..QAE@W4ENullDef@@@Z)
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};

enum EOpinionDeedType { OPINION_DEED_TYPE_NONE = 0 };
class COpinionDeedEffectsDef : public CDefClassBase {      // Ego_r.pdb COpinionDeedEffectsDef (retail STLport members: 0x20 smaller)
public:
    unsigned char _padx[0x7d - sizeof(CDefClassBase)];
    bool AllowSpeechOnNonPureAISpeaker : 1;           // +0x7d bit 0 (Ego_r +0x91 bit 0)
};
struct CTC1F {                                        // interface 0x1F on the target
    unsigned char _pad00[0x18];
    bool          Flag18;                             // +0x18
    unsigned char _pad19[3];
    long          Level1C;                            // +0x1c
    bool IsRunning() const;                           // 0x007122C0
};
struct CTC4F {                                        // interface 0x4F on the target
    unsigned char  _pad00[0x3c];
    unsigned char* ReactsToDeed;                      // +0x3c, indexed by EOpinionDeedType
};
struct CTC0F {                                        // interface 0x0F on the target
    unsigned char _pad00[0x34];
    bool          Busy;                               // +0x34
};
class CTCHeroOpinionDeedLog {                          // interface 0x51 on the hero
public:
    CDefPointer<const COpinionDeedEffectsDef> PeekPDeedEffectsDef(EOpinionDeedType deed) const;   // 0x006C5630
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                   // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                       // +0x44
    template <class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing {};
class CThingAICreature : public CThing {
public:
    bool IsInScriptGroup(const CCharString& group) const;   // 0x008302C0
};
struct CContextSensitiveItemDef {
    unsigned char _pad00[0x28];
    long Type;                                        // +0x28
    long Priority;                                    // +0x2c
    long SlotIndex;                                   // +0x30
    long ExtraData1;                                  // +0x34
    long ExtraData2;                                  // +0x38
    bool TargetCreatureLikelyToReactToDeed(CThingPlayerCreature* player, CThingAICreature* target,
                                           EOpinionDeedType deed) const;
};

bool CContextSensitiveItemDef::TargetCreatureLikelyToReactToDeed(CThingPlayerCreature* player, CThingAICreature* target,
                                                                 EOpinionDeedType deed) const
{
    const CTC1F* c1f;
    const CTC4F* c4f;
    if ((target->GetInterface(&c1f, 0x1f) && (c1f->Flag18 || !c1f->IsRunning()) && c1f->Level1C >= 4) ||
        (target->GetInterface(&c4f, 0x4f) && !c4f->ReactsToDeed[deed]))
        return false;
    if (!target->IsInScriptGroup(CCharString("SG_DEED_REACTION_ROOT", -1)))
        return false;
    const CTCHeroOpinionDeedLog* log = 0;
    player->GetInterface(&log, 0x51);
    if (ExtraData2 > 0 && ExtraData2 < 0x5c) {
        const CTC0F* c0f;
        if (target->GetInterface(&c0f, 0xf) && !c0f->Busy && (!log || !log->PeekPDeedEffectsDef(deed)->AllowSpeechOnNonPureAISpeaker))
            return false;
    }
    return true;
}
