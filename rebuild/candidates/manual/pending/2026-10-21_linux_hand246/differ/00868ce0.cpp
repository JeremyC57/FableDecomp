// Grade: DIFFER(361v360) -- frame 0x1C v 0x18: retail overlays the intelligent-pointer temporary, the GetTC key and both def out words on one word ([esp+4]) and zero-inits the vector after the lookup (ebp zero register, pushes after LowerBound); ours keeps the pointer temporary in its own 8 bytes. Tried: nested if, same-TU LowerBound/GetDef/GetSubDef stand-ins (fixed the out-word sharing: 388->361 bytes), component local (worse), block-scoped assignment
// CHitParameters::SetWeapon(CThing* weapon) -- remember the weapon and, when it carries object augmentations (component 0xBF), append the augmentation type of every augmentation definition's ability sub-definition to the hit's Augmentations list
// Leaf: SetWeapon
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00868CE0, length 358 bytes, `ret 4`.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
//   (temporary vtable 0x01238C6C), the component map's LowerBound @ 0x004365B0, the augmentations component's list getter
//   @ 0x00766920 (manifest "CParticleMorphs_InitFromDef" is propagated), CThing::GetDefinitionManager @ 0x004C7990 (landed
//   name), CDefinitionManager::GetDef<CObjectAugmentationDef>(long, CDefPointer*) @ 0x0042AF3C and
//   CDefClassBase::GetSubDef<CAbilityDef>(CDefPointer*) @ 0x0042B248 (manifest labels name other instantiations),
//   the Augmentations vector's _M_insert_overflow @ 0x00869AC0, free @ 0x00BFEA14, the defs' virtual slot 1 (delete).
// Layout: Ego_r.pdb CHitParameters with retail 12-byte containers: Augmentations (vector<EObjectAugmentationType>) +0x3C,
//   PWeapon (CIntelligentPointer<CThing>) +0x58 (PDB +0x40 / +0x60); CThing component bitset +0x20 (0xBF = sign bit of
//   +0x34) and map +0x44; the ability def's augmentation type at +0x2C.
// Shape: each def is fetched into a block-scoped plain out word (both share one frame slot) and copied into a small
//   holder whose destructor does the short release (the def stays in edi / esi); the weapon is re-read from its argument
//   slot for each GetDefinitionManager call.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
struct __true_type { __true_type() { } };
struct CDefBase {
    virtual void v00();
    virtual void Delete();                         // slot 1
    long          RefCount;                        // +0x04
    void Release() { if (--RefCount == 0) Delete(); }
};
struct CAbilityDef : CDefBase {
    unsigned char _pad08[0x2c - 0x08];
    long          AugmentationType;                // +0x2c
};
struct DefOut {
    CDefBase* Object;
    DefOut() : Object(0) { }
};
struct DefHolder {
    CDefBase* Object;
    DefHolder(CDefBase* object) : Object(object) { }
    ~DefHolder() { if (Object) Object->Release(); }
};
struct CDefClassBase : CDefBase {
    bool GetSubDef(DefOut* out);                   // 0x0042B248
};
struct CDefinitionManager {
    bool GetDef(long index, DefOut* out);          // 0x0042AF3C
};
template <class T> struct CRetailVector {          // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRetailVector() { if (_M_start) free(_M_start); }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill, bool atEnd);   // 0x00869AC0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CTCObjectAugmentations {
    void GetAugmentations(CRetailVector<long>& out);   // 0x00766920
};
struct STCEntry {
    long                    m_kind;
    CTCObjectAugmentations* m_pTC;
};
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x004365B0
    __forceinline CTCObjectAugmentations* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                     // +0x20
    CTCMap        m_tcs;                           // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCObjectAugmentations* GetTC(long kind) { return m_tcs.Find(kind); }
    CDefinitionManager* GetDefinitionManager();    // 0x004C7990
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* PData;
    CBaseIntelligentPointer();                     // 0x00A01B10
    ~CBaseIntelligentPointer();                    // 0x00A01C10
    CThing* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CThing* item);                   // 0x00A01B90
};
struct CThingPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CThingPointer(CThing* item = 0) : CBaseIntelligentPointer() { SetPItem(item); }
    CThingPointer& operator=(const CThingPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
// Same-TU stand-ins (retail's LTCG knowledge of these callees): compiled above the caller under auto_inline(off) they let
// VC7.1 reuse the dead lookup-key / out words for later temporaries, as in the landed chunk 240-242 rows.
CDefBase* LookupDefByIndex(long index);
#pragma auto_inline(off)
STCEntry* CTCMap::LowerBound(const unsigned long& kind)
{
    STCEntry* first = m_pFirst;
    long count = m_pLast - m_pFirst;
    while (count > 0) {
        long half = count >> 1;
        STCEntry* middle = first + half;
        if ((unsigned long)middle->m_kind < kind) {
            first = middle + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return first;
}
bool CDefinitionManager::GetDef(long index, DefOut* out)
{
    CDefBase* old = out->Object;
    out->Object = LookupDefByIndex(index);
    if (out->Object)
        ++out->Object->RefCount;
    if (old)
        old->Release();
    return out->Object != 0;
}
bool CDefClassBase::GetSubDef(DefOut* out)
{
    CDefBase* old = out->Object;
    out->Object = LookupDefByIndex(RefCount);
    if (out->Object)
        ++out->Object->RefCount;
    if (old)
        old->Release();
    return out->Object != 0;
}
#pragma auto_inline(on)

class CHitParameters {
public:
    unsigned char       _pad00[0x3c];
    CRetailVector<long> Augmentations;             // +0x3c
    unsigned char       _pad48[0x58 - 0x48];
    CThingPointer       PWeapon;                   // +0x58
    void SetWeapon(CThing* weapon);
};

void CHitParameters::SetWeapon(CThing* weapon)
{
    {
        PWeapon = CThingPointer(weapon);
    }
    CThing* thing = PWeapon.GetPItem();
    if (thing->HasTC(0xbf)) {
        CRetailVector<long> augmentations;
        thing->GetTC(0xbf)->GetAugmentations(augmentations);
        for (long* it = augmentations._M_start; it != augmentations._M_finish; ++it) {
            bool found;
            CDefBase* object;
            {
                DefOut out;
                found = weapon->GetDefinitionManager()->GetDef(*it, &out);
                object = out.Object;
            }
            DefHolder def(object);
            if (found) {
                bool hasAbility;
                CDefBase* abilityObject;
                {
                    DefOut sub;
                    hasAbility = static_cast<CDefClassBase*>(def.Object)->GetSubDef(&sub);
                    abilityObject = sub.Object;
                }
                DefHolder ability(abilityObject);
                if (hasAbility)
                    Augmentations.push_back(static_cast<CAbilityDef*>(ability.Object)->AugmentationType);
            }
        }
    }
}
