// Grade: DIFFER(326v297) -- frame 0xc v push-ecx: retail homes the scheduler sub-def in the dead group slot and lets the GetDef out word and the find iterator share one frame word; ours gives them separate words; the def releases are the long dec/test form (retail dec [ebp+4]; jne). A named iterator local read from memory matches retail's find compare (93 -> 59 diff lines)
// CTCIdleScheduler::RegisterIdleStateGroup(CAIStateGroupBase* group, EIdleStateGroup type) -- register an idle state group under its type with a random weight (the global default range, or the creature's idle-scheduler def entry for the type when it has one)
// Leaf: RegisterIdleStateGroup
//
// Retail 0x00801920, 304 bytes incl. padding (`ret 8`). Ego_r.pdb
// `?RegisterIdleStateGroup@CTCIdleScheduler@@QAEXPAVCAIStateGroupBase@@W4EIdleStateGroup@@@Z`.
// Callees: map<EIdleStateGroup,CAIStateGroupBase*>::operator[] @ 0x00802110, CTCBase::GetPRandomSeed @ 0x00686DA0 (twice),
// GFROR13 @ 0x00497890 (the first GFRandom expanded inline), map<EIdleStateGroup,long>::operator[] @ 0x00802190 (twice),
// CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef<T> @ 0x0042AF3C (label names the drunkenness def
// instantiation, a byte twin), CDefClassBase::GetSubDef<T> @ 0x00801670 (label names a byte twin), the sub-def map's find
// @ 0x008014F0 (twice), GFRandom(unsigned long, unsigned long*) @ 0x004CDAE0 (__fastcall, the second call out of line), the
// defs' virtual slot 1 (release to zero). Global: the default idle weight range @ 0x013BAF1C (name descriptive).
// Layout = Ego_r.pdb CTCIdleScheduler with retail 12-byte maps: IdleStates +0x0c, IdleStatesWeight +0x18; the parent
// thing's def index word +0x8c; the sub-def's weight map at +0x3c (STLport map, header first; node value +0x14).
// Shape: GFRandom is inline at its first call and out of line at the second (VC7.1's inline-once band: modelled as a
// `__forceinline` copy and the extern); the def out pointer is read once into a holder (ebp) so its frame word is reused by
// the find iterator; the weights map is a reference local (ebx); the sub def lives in the dead `group` slot.
class CAIStateGroupBase;
unsigned long __fastcall GFROR13(unsigned long value);      // 0x00497890
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
__forceinline unsigned long GFRandomInline(unsigned long range, unsigned long* seed)   // GFRandom, expanded at its first call
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
extern unsigned long g_defaultIdleWeightRange;              // 0x013BAF1C

struct CIdleStateMap {                                      // std::map<EIdleStateGroup, CAIStateGroupBase*> (retail 12 bytes)
    void* _M_header; unsigned long _M_node_count; unsigned long _M_compare;
    CAIStateGroupBase*& operator[](const long& type);      // 0x00802110
};
struct CIdleWeightMap {                                     // std::map<EIdleStateGroup, long> (retail 12 bytes)
    void* _M_header; unsigned long _M_node_count; unsigned long _M_compare;
    long& operator[](const long& type);                     // 0x00802190
};

struct SIdleWeightNode {                                    // STLport _Rb_tree_node<pair<const EIdleStateGroup, unsigned long>>
    long _M_color; SIdleWeightNode* _M_parent; SIdleWeightNode* _M_left; SIdleWeightNode* _M_right;
    long          first;                                    // +0x10
    unsigned long second;                                   // +0x14
};
struct CDefIdleWeightMap {                                  // the sub-def's map<EIdleStateGroup, unsigned long>
    SIdleWeightNode* _M_header; unsigned long _M_node_count; unsigned long _M_compare;
    struct iterator {
        SIdleWeightNode* _M_node;
        SIdleWeightNode* operator->() const { return _M_node; }
    };
    iterator find(const long& key);                         // 0x008014F0
};

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {                     // CDefPointer<const T>
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
template <class T> struct CDefOutSlot {                     // the GetDef out pointer, read once
    T* Object;
    CDefOutSlot() : Object(0) { }
};
template <class T> struct CDefHolder {                      // the def kept in a register and released at the end
    T* Object;
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};

struct CIdleSchedulerDef : CDefPointeeBase {
    unsigned char _pad08[0x3c - 0x08];
    CDefIdleWeightMap IdleWeights;                          // +0x3c
};
struct CIdleCreatureDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CIdleSchedulerDef>* out) const;   // 0x00801670
};
struct CDefinitionManager {
    bool GetDef(long index, CDefOutSlot<CIdleCreatureDef>* out) const;   // 0x0042AF3C
};

class CThing {
public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                                // +0x8c
};

struct CTCIdleScheduler {
    void*          __vftable;                               // +0x00
    CThing*        ParentThing;                             // +0x04 CTCBase
    bool           ToBeRemoved;                             // +0x08
    bool           IsInGlobalUpdateTCs;                     // +0x09
    unsigned char  _pad_0x0a[0x2];
    CIdleStateMap  IdleStates;                              // +0x0C
    CIdleWeightMap IdleStatesWeight;                        // +0x18
    long           Preferred;                               // +0x24 EIdleStateGroup
    long           FrameLastChecked;                        // +0x28
    CDefinitionManager* GetDefinitionManager();             // 0x006869D0 (CTCBase)
    unsigned long* GetPRandomSeed();                        // 0x00686DA0 (CTCBase)
    void RegisterIdleStateGroup(CAIStateGroupBase* group, long type);
};

void CTCIdleScheduler::RegisterIdleStateGroup(CAIStateGroupBase* group, long type)
{
    IdleStates[type] = group;
    IdleStatesWeight[type] = GFRandomInline(g_defaultIdleWeightRange, GetPRandomSeed());
    if (ParentThing->DefIndex != 0) {
        CDefPointer<CIdleSchedulerDef> scheduler;
        CDefHolder<CIdleCreatureDef> def;
        {
            CDefOutSlot<CIdleCreatureDef> out;
            GetDefinitionManager()->GetDef(ParentThing->DefIndex, &out);
            def.Object = out.Object;
        }
        if (def->GetSubDef(&scheduler)) {
            CDefIdleWeightMap& weights = scheduler->IdleWeights;
            CDefIdleWeightMap::iterator it = weights.find(type);
            if (it._M_node != weights._M_header) {
                it = weights.find(type);
                IdleStatesWeight[type] = GFRandom(it->second, GetPRandomSeed());
            }
        }
    }
}
