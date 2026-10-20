// Grade: DIFFER(300v277) -- all but the padding loop: retail hoists the null-def zero store above the loop and emits NO destructor for it (VC proved it still null); ours re-zeroes and runs the inline release every pass. Tried: const named null local, same-TU push_back stand-in (no effect).
// CTCInventoryAbilities::GetPrioritisedAbilityDefs(vector<CDefPointer<const CHeroAbilityDef>>* defs) -- for an adult hero, lists the received abilities' defs in order, keeps them up to the last received one and pads that list with null defs to a whole palette row
// Retail 0x005F4B30, 277 bytes, `ret 4`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CThingCreatureBase::IsChild 0x00661F70 (on ParentThing), GetAbilityPDef(EHeroAbility) const 0x005EDFD9
// (CDefPointer by value), vector<CDefPointer<const CHeroAbilityDef>>::push_back(const value_type&) 0x005FA3A5,
// GetNumAbilitiesPerRow() const 0x005EDCF0 (manifest "_Dest_val<...>" is propagated), vector::operator= 0x005F9F5A,
// ~vector 0x005F9F14 (manifest "CActiveFile::OnReadFinished" is a byte twin).
// Shape: one appended def per ReceivedAbilityOrder slot (the by-value def temporary is released inside its null
// test through the ref-count base's DecRef()/GetRefCount() pair -- `dec [eax] ; cmp [eax],ebx` with the shared zero
// register -- and zeroed), remembering the last non-empty slot; a local vector copies the defs up to it and is padded
// with null-def temporaries (dtor folded away) while its size is not a multiple of the row width (the bound re-read
// every pass), then assigned back.
// Layout (retail; Ego_r.pdb names): ParentThing +0x04, ReceivedAbilityOrder vector<EHeroAbility> +0x154.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CThingCreatureBase {
    bool IsChild();                                  // 0x00661F70
};
struct CRefCountedBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CHeroAbilityDef : CRefCountedBase {};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other);
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
typedef CDefPointer<CHeroAbilityDef> CAbilityDefPointer;
struct CAbilityDefVector {                           // vector<CDefPointer<const CHeroAbilityDef>> (12 bytes)
    CAbilityDefPointer* _M_start;
    CAbilityDefPointer* _M_finish;
    CAbilityDefPointer* _M_end_of_storage;
    CAbilityDefVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CAbilityDefVector();                            // 0x005F9F14
    CAbilityDefVector& operator=(const CAbilityDefVector& other);   // 0x005F9F5A
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CAbilityDefPointer& operator[](unsigned long n) { return *(_M_start + n); }
    void push_back(const CAbilityDefPointer& value);  // 0x005FA3A5
};
enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };
struct CAbilityOrderVector {                         // vector<EHeroAbility>
    EHeroAbility* _M_start;
    EHeroAbility* _M_finish;
    EHeroAbility* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    EHeroAbility& operator[](unsigned long n) { return *(_M_start + n); }
};

struct CTCInventoryAbilities {
    void* __vftable;
    CThingCreatureBase* ParentThing;                 // +0x04
    unsigned char _pad_0x008[0x154 - 0x008];
    CAbilityOrderVector ReceivedAbilityOrder;        // +0x154
    CAbilityDefPointer GetAbilityPDef(EHeroAbility ability) const;   // 0x005EDFD9
    long GetNumAbilitiesPerRow() const;                               // 0x005EDCF0
    void GetPrioritisedAbilityDefs(CAbilityDefVector* defs);
};

void CTCInventoryAbilities::GetPrioritisedAbilityDefs(CAbilityDefVector* defs)
{
    if (ParentThing->IsChild())
        return;
    long lastReceived = -1;
    for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i) {
        defs->push_back(GetAbilityPDef(ReceivedAbilityOrder[i]));
        if (ReceivedAbilityOrder[i] != ABILITY_NULL)
            lastReceived = i;
    }
    CAbilityDefVector prioritised;
    long j;
    for (j = 0; j <= lastReceived; ++j)
        prioritised.push_back((*defs)[j]);
    for (unsigned long k = 0; k < prioritised.size() % GetNumAbilitiesPerRow(); ++k)
        prioritised.push_back(CAbilityDefPointer());
    *defs = prioritised;
}
