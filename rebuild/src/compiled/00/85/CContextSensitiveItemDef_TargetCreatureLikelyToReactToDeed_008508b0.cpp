// CContextSensitiveItemDef::TargetCreatureLikelyToReactToDeed(CThingPlayerCreature*, CThingAICreature*, EOpinionDeedType) const -- false when the target is busy (0x1F component), does not react to this deed (0x4F reaction table), is not in
// Leaf: TargetCreatureLikelyToReactToDeed
// Grade: RELOCATION_MATCH -- ternary GetInterface store (landed 0x007A0490 form) frees ebp (retail reloads deed from its slot); same-TU const LowerBound stand-in (landed 0x008330D0 idiom) and local CDefPointer model kept: the engine CDefPointer header version grades 453v457
// SG_DEED_REACTION_ROOT, or (for deed-reaction item types 1..0x5B) has an idle 0x0F component and the hero's CTCHeroOpinionDeedLog (0x51)
// deed-effects def for this deed lacks AllowSpeechOnNonPureAISpeaker (+0x7D bit 0); otherwise true
// Layout: CContextSensitiveItemDef ExtraData2 +0x38 (Ego_r.pdb = retail); CThing interface bitset +0x20, map +0x44.
// Shape: the const interface lookups use a same-TU const LowerBound stand-in (0x004365B0, the landed 0x008330D0 idiom);
//   the hero's deed def is a by-value CDefPointer, a conditionally constructed temporary inside the final test.
class CTCBase;
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
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
enum EOpinionDeedType { OPINION_DEED_TYPE_NONE = 0 };
class CDefPointeeBase { public:
    virtual void v000();
    virtual void DestroyDef() const;                  // slot 1
    void Release() const
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
    mutable long m_refCount;                          // +4
};
class COpinionDeedEffectsDef : public CDefPointeeBase { public:    // retail (STLport) layout, 0x20 below Ego_r.pdb
    unsigned char _pad08[0x7d - 8];
    bool AllowSpeechOnNonPureAISpeaker : 1;           // +0x7d bit 0
};
template<class T> class CDefPointer { public:
    T* Object;
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
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
class CTCHeroOpinionDeedLog { public:                        // interface 0x51 on the hero
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
            *ppOut = (const T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
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
class CContextSensitiveItemDef { public:
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
    if (!target->IsInScriptGroup("SG_DEED_REACTION_ROOT"))
        return false;
    const CTCHeroOpinionDeedLog* c51 = 0;
    player->GetInterface(&c51, 0x51);
    if (ExtraData2 > 0 && ExtraData2 < 0x5c) {
        const CTC0F* c0f;
        if (target->GetInterface(&c0f, 0xf) && !c0f->Busy && (!c51 || !c51->PeekPDeedEffectsDef(deed)->AllowSpeechOnNonPureAISpeaker))
            return false;
    }
    return true;
}