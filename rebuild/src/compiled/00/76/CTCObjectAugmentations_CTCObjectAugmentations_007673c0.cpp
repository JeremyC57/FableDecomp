// CTCObjectAugmentations::CTCObjectAugmentations(CThing* thing) -- empty augmentation slots, sound maps, effect lists and trail pointer; then the thing definition's object-augmentations sub-def is fetched and the slot vector sized to its NumberOfSlots
// Leaf: ??0CTCObjectAugmentations
// Retail: Fable.exe (TLC, VC7.1) @ 0x007673C0, 222 bytes (`ret 4`). Vtable 0x0126A3A4 (CTCObjectAugmentations).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (the two map header nodes, 0x20 each),
//   CBaseIntelligentPointer() @ 0x00A01B10 + SetPItem @ 0x00A01B90 (vtable 0x01238C6C = CIntelligentPointer<CThing>),
//   CDefClassBase::GetSubDef<CObjectAugmentationsDef> @ 0x0042AF9E (labelled GetSubDef<CAbilityDef>), vector<CDefPointer<
//   const CThingBaseDef> >::resize(n, const T&) @ 0x0076BF80 (labelled vector<unsigned int>::resize).
// Layout: Ego_r.pdb CTCObjectAugmentations with retail 12-byte STLport containers: PDef +0x0c, LoadedFromSaveGame +0x10,
//   Augmentations +0x14, InitSounds +0x20, LoopingSounds +0x2c, ParticleEffects +0x38, AttackParticleEffects +0x44,
//   CurrentRunningAugEffect +0x50, CurrentRunningAugEffectIndex +0x5c, CurPeriod +0x60, InStrikeAction +0x64,
//   ProjectileTrailEffect +0x65 (the PDB's odd offset: pack(1) on the intelligent-pointer classes only). CThing PDef +0x70; CObjectAugmentationsDef NumberOfSlots +0x28.
// Shape: the containers are the real STLport shapes -- `_Vector_base` with the end pointer in an alloc proxy, `_Rb_tree_base`
//   allocating the header in its body (header zeroed first, then assigned) and `_Rb_tree` zeroing the count and initialising
//   the empty header through the re-read member; the slot resize is the inline STLport `resize(n)` forwarding `T()` to the
//   out-of-line `resize(n, const T&)`, which puts the temporary's zero store AFTER the argument pushes (in the dead `thing`
//   parameter slot), where `resize(n, T())` written in the body stores it first.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
class CThing;
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                    // +0x04
};
template <class T> class CDefPointer { public:
    const T* Object;
    CDefPointer() : Object(0) { }
};
class CObjectAugmentationsDef : public CDefBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long NumberOfSlots;                                 // +0x28
    long MaximumNumberOfSlots;                          // +0x2c
};
class CThingBaseDef : public CDefBase { public: };
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out) const;   // 0x0042AF9E (T = CObjectAugmentationsDef)
};
class CThing { public:
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70 CDefPointer<const CThingDef>::Object
};
template <class P> struct CAllocProxy {                 // STLport _STLP_alloc_proxy<P, ...> over an empty allocator
    P _M_data;
    CAllocProxy(P p) : _M_data(p) { }
};
template <class T> struct vector {                      // STLport vector: _Vector_base, 12 bytes
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T*> _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void resize(unsigned long count, const T& value);   // 0x0076BF80
    void resize(unsigned long count) { resize(count, T()); }
};
class CCharString { public: void* PStringData; };
class CObjectAugmentationSoundSet { public:                    // 0x0c
    bool          Played;
    CCharString   Sound;
    unsigned long Handle;
};
struct SRbTreeNodeBase {                                // STLport _Rb_tree_node_base
    bool             _M_color;                          // +0x00
    SRbTreeNodeBase* _M_parent;                         // +0x04
    SRbTreeNodeBase* _M_left;                           // +0x08
    SRbTreeNodeBase* _M_right;                          // +0x0c
};
struct SSoundSetNode : SRbTreeNodeBase {                // 0x20 bytes
    long                        first;                  // EObjectAugmentationType
    CObjectAugmentationSoundSet second;
};
template <class N> struct CNodeAllocProxy {
    N* _M_data;
    CNodeAllocProxy(N* p) : _M_data(p) { }
    N* allocate(unsigned int n) { return n != 0 ? (N*)malloc(n * sizeof(N)) : 0; }
};
struct CSoundSetTreeBase {                              // STLport _Rb_tree_base
    CNodeAllocProxy<SSoundSetNode> _M_header;
    CSoundSetTreeBase() : _M_header(0) { _M_header._M_data = _M_header.allocate(1); }
};
struct SLess { };
struct CSoundSetMap : CSoundSetTreeBase {               // map<EObjectAugmentationType, CObjectAugmentationSoundSet>, 12 bytes
    unsigned long _M_node_count;
    SLess         _M_key_compare;
    CSoundSetMap() : _M_node_count(0)
    {
        _M_header._M_data->_M_color = false;
        _M_header._M_data->_M_parent = 0;
        _M_header._M_data->_M_left = _M_header._M_data;
        _M_header._M_data->_M_right = _M_header._M_data;
    }
};
#pragma pack(push, 1)                                   // the PDB puts ProjectileTrailEffect at the odd offset +0x65
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    virtual ~CIntelligentPointer();

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)
struct ParticleAndEndPoints;
struct AttackParticleAndEndPoints;
class CTCBase { public:
    CTCBase(CThing* thing);                             // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};
class CTCObjectAugmentations : public CTCBase { public:               // vtable 0x0126A3A4
    CDefPointer<CObjectAugmentationsDef> PDef;          // +0x0c
    bool          LoadedFromSaveGame;                   // +0x10
    vector< CDefPointer<CThingBaseDef> > Augmentations; // +0x14
    CSoundSetMap  InitSounds;                           // +0x20
    CSoundSetMap  LoopingSounds;                        // +0x2c
    vector< vector<ParticleAndEndPoints> > ParticleEffects;         // +0x38
    vector< vector<AttackParticleAndEndPoints> > AttackParticleEffects;   // +0x44
    vector< CIntelligentPointer<CThing> > CurrentRunningAugEffect;  // +0x50
    long          CurrentRunningAugEffectIndex;         // +0x5c
    long          CurPeriod;                            // +0x60
    bool          InStrikeAction;                       // +0x64
    CIntelligentPointer<CThing> ProjectileTrailEffect;  // +0x65
    CTCObjectAugmentations(CThing* thing);
    virtual ~CTCObjectAugmentations();
};

CTCObjectAugmentations::CTCObjectAugmentations(CThing* thing)
    : CTCBase(thing), LoadedFromSaveGame(false), CurrentRunningAugEffectIndex(0), CurPeriod(0), InStrikeAction(false),
      ProjectileTrailEffect(0)
{
    ParentThing->PDef->GetSubDef(PDef);
    Augmentations.resize(PDef.Object->NumberOfSlots);
}