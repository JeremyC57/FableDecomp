// CTCIdleScheduler::ChooseNewIdle() -- stamp the frame; credit the outgoing preferred idle with its weight from the thing's idle-scheduler def; then prefer the runnable idle state group with the lowest accumulated weight
// Leaf: ChooseNewIdle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00801A50, 390 bytes (+ int3 padding), `ret`.
//   Ego_r.pdb: `?ChooseNewIdle@CTCIdleScheduler@@QAEXXZ`.
// Callees: CTCBase::GetWorld() @ 0x00686A70 (result unused), CWorld::GetWorldFrame() @ 0x0049D870 (static),
//   CTCBase::GetDefinitions() @ 0x006869D0, CDefinitionManager::GetDef<T>(long, CDefPointer<T>*) @ 0x0042AF3C,
//   CDefClassBase::GetSubDef<CIdleSchedulerDef> @ 0x00801670 (landed), map<EIdleStateGroup, long>::find @ 0x008014F0
//   (landed, iterator by hidden pointer), map<EIdleStateGroup, long>::operator[] @ 0x00802190, the state group's
//   virtual slot 11 (+0x2c, can-run test), the defs' virtual slot 1 (destroy at the last release).
// Shape: same-TU `auto_inline(off)` stand-ins for GetDef<T> and GetSubDef<CIdleSchedulerDef> (see below) keep both def
//   objects in ebp/ebx; the thing pointer named after the two def-pointer declarations re-reads the def index for
//   the push (`cmp word ptr [eax+8Ch],dx` ... `movzx edx,word ptr [eax+8Ch]`); the first find is an inline
//   `Contains` with a named iterator and the second a named iterator, both read back from the frame slot that the
//   thing def's out word vacated; `bestWeight` is declared before `best` (xor ebx before xor ebp).
// Layout: as the landed sibling 0x00801BE0 (IdleStates +0x0C, IdleStatesWeight +0x18, Preferred +0x24,
//   FrameLastChecked +0x28; retail 12-byte STLport maps); CThing def index +0x8C (unsigned short);
//   CIdleSchedulerDef +0x3C is its map<EIdleStateGroup, long> of idle weights.
struct CAIStateGroupBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual bool CanRun();                          // slot 11 (+0x2c)
};

struct CWorld {
    static long GetWorldFrame();                     // 0x0049D870
};

struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() { Object = 0; }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
};

struct SWeightNode {                                 // _Rb_tree_node<pair<const EIdleStateGroup, long> >
    long color;
    SWeightNode* parent;
    SWeightNode* left;
    SWeightNode* right;
    long first;                                      // +0x10
    long second;                                     // +0x14
};
struct CWeightMapIterator {
    SWeightNode* _M_node;
    CWeightMapIterator(SWeightNode* node) : _M_node(node) { }
};
struct CWeightMap {                                  // map<EIdleStateGroup, long> (retail 12 bytes)
    SWeightNode* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    CWeightMapIterator find(const long& key);        // 0x008014F0
    long& operator[](const long& key);               // 0x00802190
};

static inline bool Contains(CWeightMap& map, const long& key)
{
    CWeightMapIterator it = map.find(key);
    return it._M_node != map._M_header;
}

struct SIdleNode {                                   // _Rb_tree_node<pair<const EIdleStateGroup, CAIStateGroupBase*> >
    long color;
    SIdleNode* parent;
    SIdleNode* left;
    SIdleNode* right;
    long first;                                      // +0x10
    CAIStateGroupBase* second;                       // +0x14
};
struct CIdleMapIterator {
    SIdleNode* _M_node;
    CIdleMapIterator(SIdleNode* node) : _M_node(node) { }
    static SIdleNode* Increment(SIdleNode* node)     // STLport _Rb_global::_M_increment
    {
        if (node->right != 0) {
            node = node->right;
            while (node->left != 0)
                node = node->left;
        } else {
            SIdleNode* y = node->parent;
            while (node == y->right) {
                node = y;
                y = y->parent;
            }
            if (node->right != y)
                node = y;
        }
        return node;
    }
    void operator++() { _M_node = Increment(_M_node); }
    bool operator!=(const CIdleMapIterator& other) const { return _M_node != other._M_node; }
    SIdleNode* operator->() const { return _M_node; }
};
struct CIdleMap {                                    // map<EIdleStateGroup, CAIStateGroupBase*> (retail 12 bytes)
    SIdleNode* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    CIdleMapIterator begin() { return CIdleMapIterator(_M_header->left); }
    CIdleMapIterator end() { return CIdleMapIterator(_M_header); }
};

struct CIdleSchedulerDef : CDefClassBase {
    unsigned char _pad08[0x3c - 0x08];
    CWeightMap IdleWeights;                          // +0x3c
};
struct CThingDef : CDefClassBase {
    bool GetSubDef(CDefPointer<CIdleSchedulerDef>* out);   // 0x00801670 (GetSubDef<CIdleSchedulerDef>)
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingDef>* out);  // 0x0042AF3C
};
struct CThing {
    unsigned char _pad00[0x8c];
    unsigned short DefIndex;                         // +0x8c
    long GetDefIndex() const { return DefIndex; }
};

struct CTCIdleScheduler {
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    bool          ToBeRemoved;                       // +0x08
    bool          IsInGlobalUpdateTCs;               // +0x09
    unsigned char _pad_0x0a[0x2];
    CIdleMap      IdleStates;                        // +0x0C
    CWeightMap    IdleStatesWeight;                  // +0x18
    long          Preferred;                         // +0x24 EIdleStateGroup
    long          FrameLastChecked;                  // +0x28

    CWorld* GetWorld() const;                        // 0x00686A70 (CTCBase)
    CDefinitionManager* GetDefinitions() const;      // 0x006869D0 (CTCBase)
    void ChooseNewIdle();
};

// Same-TU stand-ins (retail 0x00801670 sits directly above this function; GetDef<T> is a COMDAT instantiation):
// with their bodies visible VC7.1 sees that the out pointers do not escape and keeps both def objects in registers
// (ebp / ebx) for the short `dec [reg+4] ; jne` releases, as retail's LTCG did.
CDefClassBase* LookUpDef(long index);               // the manager's raw lookup (stand-in only)
CDefClassBase* LookUpSubDef(const CDefClassBase* def);
#pragma auto_inline(off)
bool CDefinitionManager::GetDef(long index, CDefPointer<CThingDef>* out)
{
    CThingDef* def = (CThingDef*)LookUpDef(index);
    out->Object = def;
    return def != 0;
}
bool CThingDef::GetSubDef(CDefPointer<CIdleSchedulerDef>* out)
{
    CIdleSchedulerDef* def = (CIdleSchedulerDef*)LookUpSubDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)

void CTCIdleScheduler::ChooseNewIdle()
{
    FrameLastChecked = GetWorld()->GetWorldFrame();
    if (Preferred != 0 && ParentThing->DefIndex != 0) {
        CDefPointer<CIdleSchedulerDef> idleDef;
        CDefPointer<CThingDef> thingDef;
        CThing* thing = ParentThing;
        GetDefinitions()->GetDef(thing->GetDefIndex(), &thingDef);
        if (thingDef.Object->GetSubDef(&idleDef)) {
            CWeightMap& weights = idleDef.Object->IdleWeights;
            if (Contains(weights, Preferred)) {
                long& total = IdleStatesWeight[Preferred];
                CWeightMapIterator found = weights.find(Preferred);
                total += found._M_node->second;
            }
        }
    }
    Preferred = 1;
    long bestWeight = 0;
    long best = 0;
    for (CIdleMapIterator it = IdleStates.begin(); it != IdleStates.end(); ++it) {
        if (best == 0 || IdleStatesWeight[it->first] < bestWeight) {
            if (it->second->CanRun()) {
                best = it->first;
                bestWeight = IdleStatesWeight[it->first];
            }
        }
    }
    Preferred = best;
}