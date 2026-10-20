// Grade: DIFFER(299v300) -- ebx/ebp swap only: retail keeps this in ebx and the tattoo iterator in ebp (`mov esi,[ebp]` is one byte longer); every other instruction matches
// CTCHeroMorph::AddTattoo(CDefPointer<const CThingBaseDef>& def) -- add a tattoo item to the hero: a "removes all tattoos" def clears the list, otherwise every worn tattoo whose body area clashes with the new one is taken off and the new def appended; either way the morph is marked changed at the current world frame
// Leaf: ?AddTattoo@CTCHeroMorph@@QAEXAAV?$CDefPointer
//
// Retail 0x0071D940, 304 bytes incl. padding (`ret 4`). Ego_r.pdb
// `?AddTattoo@CTCHeroMorph@@QAEXAAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z` (of the three overloads, the CDefPointer one).
// Callees: CDefClassBase::GetSubDef<CTattooDef>(CDefPointer<const CTattooDef>*) const @ 0x005B1EFC (catalog label
// "GetSubDef<CAbilityDef>" is a byte-twin propagation), SGI `__copy(first, last, result, const random_access_iterator_tag&,
// long*)` @ 0x0071FA60 and `_Destroy_range(first, last, const allocator&)` @ 0x005B26E9 (both /Gr `__fastcall`; the
// inlined clear(), tag and allocator temporaries homed in the dead `def` slot; labels "_Move<CMeleeCombatAbilityDef>" and
// "_Destroy_range<CFlash>" are propagated), vector<CDefPointer<const CThingBaseDef>>::erase(iterator) @ 0x00720810 (landed),
// ::push_back(const CDefPointer&) @ 0x005B3325 (out of line), CTCBase::GetWorld() const @ 0x00686A80, CWorld::GetFrame()
// @ 0x0049D870 (a static: the GetWorld call is still emitted), the defs' slot 1 on release.
// Layout: retail CTCHeroMorph Tattoos +0x1C (12-byte vector), FrameTattoosChanged +0x34, Changed +0x3D; CTattooDef
// RemovesAllTattoos +0x3C, CoversBodyAreaFlags +0x40, SpecificCoversBodyAreaFlags +0x48.
// Shape: two tattoos clash when their CoversBodyAreaFlags overlap, unless the overlap is exactly 8 and both carry
// disjoint, non-zero specific flags; each worn def is copied into a counted local (add-ref) before its own GetSubDef.
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy() const;                           // slot 1 (+0x04)
    mutable long RefCount;                                  // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CTattooDef : CDefPointeeBase {
    unsigned char _pad08[0x3c - 0x08];
    bool RemovesAllTattoos;                                 // +0x3c
    bool Permanent;                                         // +0x3d
    long CoversBodyAreaFlags;                               // +0x40
    float VisibilityMultiplier;                             // +0x44
    long SpecificCoversBodyAreaFlags;                       // +0x48
};
struct CThingBaseDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<const CTattooDef>* out) const;       // 0x005B1EFC
};
const CTattooDef* FindDef_GetSubDef_CTattooDef(const CThingBaseDef* def);    // stand-in callee of the stand-in body below
#pragma auto_inline(off)
// Same-TU stand-in for CDefClassBase::GetSubDef<CTattooDef> 0x005B1EFC (writes the out slot only inline): it tells VC7.1,
// as retail's link-time code generation knew, that the out pointer's address does not escape, so the def read after the
// call stays in edi to the release. Only AddTattoo is graded.
bool CThingBaseDef::GetSubDef(CDefPointer<const CTattooDef>* out) const
{
    const CTattooDef* def = FindDef_GetSubDef_CTattooDef(this);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)
typedef CDefPointer<const CThingBaseDef> CThingDefPointer;
struct CDefPointerAllocator { CDefPointerAllocator() { } };
CThingDefPointer* __fastcall copy_sgi(CThingDefPointer* first, CThingDefPointer* last, CThingDefPointer* result,
                                      const random_access_iterator_tag& tag, long* distance);   // 0x0071FA60
void __fastcall destroy_range(CThingDefPointer* first, CThingDefPointer* last, const CDefPointerAllocator& alloc);   // 0x005B26E9
struct CThingDefVector {                                    // STLport vector (12 bytes)
    CThingDefPointer* _M_start;
    CThingDefPointer* _M_finish;
    CThingDefPointer* _M_end_of_storage;
    CThingDefPointer* begin() { return _M_start; }
    CThingDefPointer* end() { return _M_finish; }
    CThingDefPointer* erase(CThingDefPointer* pos);                 // 0x00720810
    void push_back(const CThingDefPointer& value);                  // 0x005B3325
    CThingDefPointer* erase(CThingDefPointer* first, CThingDefPointer* last)
    {
        CThingDefPointer* i = copy_sgi(last, _M_finish, first, random_access_iterator_tag(), (long*)0);
        destroy_range(i, _M_finish, CDefPointerAllocator());
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
struct CWorld {
    static long GetFrame();                                 // 0x0049D870 (returns the global @ 0x013B89BC)
};
class CTCHeroMorph {
public:
    virtual void v00();
    unsigned char _pad04[0x1c - 0x04];
    CThingDefVector Tattoos;                                // +0x1c
    unsigned char _pad28[0x34 - 0x28];
    long FrameTattoosChanged;                               // +0x34
    long FrameHeroBecameIdle;                               // +0x38
    bool IdleParticlesAdded;                                // +0x3c
    bool Changed;                                           // +0x3d
    CWorld* GetWorld() const;                               // 0x00686A80 (CTCBase)
    void AddTattoo(CThingDefPointer& def);
};

void CTCHeroMorph::AddTattoo(CThingDefPointer& def)
{
    CDefPointer<const CTattooDef> tattoo;
    if (def->GetSubDef(&tattoo)) {
        if (tattoo->RemovesAllTattoos) {
            Tattoos.clear();
        } else {
            for (CThingDefPointer* it = Tattoos.begin(); it != Tattoos.end(); ) {
                CDefPointer<const CTattooDef> worn;
                CThingDefPointer wornDef = *it;
                long overlap;
                if (wornDef->GetSubDef(&worn) &&
                    (overlap = worn->CoversBodyAreaFlags & tattoo->CoversBodyAreaFlags) != 0 &&
                    (overlap != 8 ||
                     (worn->SpecificCoversBodyAreaFlags & tattoo->SpecificCoversBodyAreaFlags) != 0 ||
                     worn->SpecificCoversBodyAreaFlags == 0 || tattoo->SpecificCoversBodyAreaFlags == 0))
                    it = Tattoos.erase(it);
                else
                    ++it;
            }
            Tattoos.push_back(def);
        }
        Changed = true;
        FrameTattoosChanged = GetWorld()->GetFrame();
    }
}
