// CThingDeadCreature::PreloadDeadAnimation() -- run the "preload dead animation" functor over every entry of the appearance definition's animation set, resolving the matching entries through the world's main mesh bank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00835BD0, length 75 bytes (+ int3 padding).
// Callees: CThing::PeekWorld() const @ 0x004C7AB0 (`[g_main]+0x24`), CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer&)
// @ 0x0042B0A2 (landed), the SGI `for_each<const SAnimationSetEntry*, CPreloadDeadAnimation>` @ 0x008365F0 (a /Gr TU:
// __fastcall with the hidden result in ecx, `first` in edx, `last` and the by-value functor on the stack; manifest
// "CTCInventoryAbilities_PreloadDeadAnimation" is a propagated name), the def's virtual slot 1 (+4) at count zero.
// Layout (retail, local model): PBaseDef at +0x70 (landed CTCLook::OnCreate 0x00753040); CWorld's main mesh bank at +0x3c
// (landed CThingCreatureBase::GetAnimDuration 0x006627F0); the appearance definition's animation set vector at +0x34
// (first) / +0x38 (last); a counted definition holds its count at +4.
// Shape: the bank is a named local taken first (edi); the def pointer is zero-initialised in the `push ecx` slot, loaded
// ONCE into esi, and released through it by hand (a CDefPointer destructor re-reads the address-taken slot in the lane);
// the functor (a 4-byte class with a user constructor: returned through the hidden pointer, passed by value as a plain
// `push edi`) comes back into the def pointer's dead frame slot.
struct CMeshDataBank;
struct SAnimationSetEntry;

struct CDefPointeeBase {                        // a counted definition object
    virtual void v000();
    virtual void DestroyDef();                  // slot 1 (+4): called when the count drops to zero
    long m_refCount;                            // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};

template<class T> struct CDefPointer {          // CDefPointer<const T>: one counted pointer to a definition
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer() { if (m_pObject) m_pObject->ReleaseDef(); }
};

struct CAnimationSet {                          // vector<SAnimationSetEntry>: begin, end, end of storage
    SAnimationSetEntry* m_first;
    SAnimationSetEntry* m_last;
    SAnimationSetEntry* m_end;
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char       _pad08[0x2c];
    CAnimationSet       m_anims;                // +0x34
};

struct CDefClassBase {
    CAppearanceDef* FindAppearanceDef();        // stand-in callee of the stand-in body below
    bool GetSubDef(CDefPointer<CAppearanceDef>& out);       // 0x0042B0A2
};
// Stand-in GetSubDef body (the real one is 0x0042B0A2): it writes `out` only inline, so VC7.1 knows the def pointer's
// address does not escape (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAppearanceDef>& out)
{
    CAppearanceDef* def = FindAppearanceDef();
    if (!def)
        return false;
    ++def->m_refCount;
    out.m_pObject = def;
    return true;
}
#pragma auto_inline(on)

struct CWorld {
    unsigned char  _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;               // +0x3c
};

struct CPreloadDeadAnimation {                  // the per-entry functor: holds the mesh bank
    CMeshDataBank* m_bank;
    CPreloadDeadAnimation(CMeshDataBank* bank) : m_bank(bank) { }
};

void PreloadEntry(CMeshDataBank* bank, SAnimationSetEntry* entry);   // stand-in callees of the stand-in body below
SAnimationSetEntry* NextEntry(SAnimationSetEntry* entry);
// Stand-in for_each body (the real one is 0x008365F0): compiled above the caller so VC7.1 knows nothing escapes it.
#pragma auto_inline(off)
__declspec(noinline) CPreloadDeadAnimation __fastcall ForEachAnimation(SAnimationSetEntry* first, SAnimationSetEntry* last,
                                                  CPreloadDeadAnimation preload)      // 0x008365F0 (for_each)
{
    for (; first != last; first = NextEntry(first))
        PreloadEntry(preload.m_bank, first);
    return preload;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;                    // +0x70
    const CWorld& PeekWorld() const;            // 0x004C7AB0
};

struct CThingDeadCreature : CThing {
    void PreloadDeadAnimation();
};

void CThingDeadCreature::PreloadDeadAnimation()
{
    CMeshDataBank* bank = PeekWorld().PMainMeshBank;
    CDefPointer<CAppearanceDef> def;
    PBaseDef->GetSubDef(def);
    const CAnimationSet& anims = def.m_pObject->m_anims;
    ForEachAnimation(anims.m_first, anims.m_last, CPreloadDeadAnimation(bank));

}