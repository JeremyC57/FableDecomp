// CTCPhysicalShieldSpell::~CTCPhysicalShieldSpell() -- fade or kill the shield and swirly particle effects when they are still alive, then destroy the attacker set, the two effect pointers and the def pointer, and the CTCBase base
// Leaf: ??1CTCPhysicalShieldSpell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079E990, 268 bytes (+ int3 padding), plain `ret`; called only by the
//   scalar deleting destructor 0x0079F510. Label correction: the catalog row "CTCDParticleEmitter::Destructor" is
//   propagated; the vtable stored first, 0x0126C53C, is CTCPhysicalShieldSpell (rtti-vtables.tsv).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @
//   0x0040F020 (inlined lookup of CTCDParticleEmitter, interface 0x4b = word 2 bit 11), CTCDParticleEmitter::EnterTimelineEnd
//   @ 0x006E05F0 (manifest "EnterTimelineEnd" is propagated), the set's recursive erase @ 0x0079EEC0 (as the landed
//   tree destructor 0x0079F2E0), free @ 0x00BFEA14, ~CBaseIntelligentPointer @ 0x00A01C10 (x2), the def's virtual slot 1,
//   CTCBase::~CTCBase @ 0x00686830.
// Layout = Ego_r.pdb CTCPhysicalShieldSpell (0x58; retail 0x54, the STLport set 12 bytes): PDef +0x0c, ShieldEffect
//   +0x30, SwirlyEffect +0x38, Pausable +0x44, AllowReactionAttackers (set<CThing*>, header pointer + count) +0x48.
// Shape: the body is the `__forceinline` CTCSpecialAbilities::FadeOrKillEffect on each effect pointer (one GetPItem, the alive
//   test, a zero-preloaded HasTC/GetTC pick and the emitter call made even on null; the key 0x4b kept in ebx across
//   both); the members die in reverse order: the inline set destructor (erase, header reset, free), the out-of-line
//   intelligent-pointer destructors, the inline def release with its zero store; the base destructor is called.
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || wanted < it->first) ? map->last : it)->second;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCDParticleEmitter { public:                    // interface type 0x4b component
public:
    void EnterTimelineEnd();   // 0x006E05F0
};
struct CDefPointee {
    virtual void v000();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CShieldDefPointer {                      // CDefPointer<CSpecialAbilitiesPhysicalShieldDef const>
    CDefPointee* Object;
    ~CShieldDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
struct CThingPointerSet {                       // STLport set<CThing*>: {header, count}
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node* head;
    unsigned long size;
    void Erase(node* root);                     // 0x0079EEC0
    ~CThingPointerSet()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
};
class CTCBase { public:
    virtual ~CTCBase();                         // 0x00686830
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCPhysicalShieldSpell : public CTCBase { public:       // vtable 0x0126C53C
    CShieldDefPointer PDef;                     // +0x0c
    long  SpellLevel;                           // +0x10
    long  ShieldFadeOutTimer;                   // +0x14
    long  CurShieldFadeOutTimer;                // +0x18
    long  DurationTimer;                        // +0x1c
    float ShieldHealth;                         // +0x20
    float CurWobbleTimer;                       // +0x24
    float WobbleDelta;                          // +0x28
    unsigned long ShieldColor;                  // +0x2c CRGBColour
    CIntelligentPointer<CThing> ShieldEffect;       // +0x30
    CIntelligentPointer<CThing> SwirlyEffect;       // +0x38
    unsigned long LoopingSound;                 // +0x40
    bool  Pausable;                             // +0x44
    bool  Paused;                               // +0x45
    bool  CutSceneCancelled;                    // +0x46
    CThingPointerSet AllowReactionAttackers;    // +0x48
    static __forceinline void FadeOrKillEffect(CIntelligentPointer<CThing>& effect)   // CTCSpecialAbilities helper, inlined
    {
        CThing* thing = effect.GetPItem();
        if (thing && !thing->IsDead()) {
            CTCDParticleEmitter* emitter = 0;
            if (thing->HasTC(0x4b))
                emitter = thing->GetTC<CTCDParticleEmitter>(0x4b);
            emitter->EnterTimelineEnd();
        }
    }
    virtual ~CTCPhysicalShieldSpell();
};

CTCPhysicalShieldSpell::~CTCPhysicalShieldSpell()
{
    FadeOrKillEffect(ShieldEffect);
    FadeOrKillEffect(SwirlyEffect);
}