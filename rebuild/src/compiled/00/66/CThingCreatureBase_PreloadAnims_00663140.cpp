// CThingCreatureBase::PreloadAnims() -- fetch the creature's appearance sub-def and touch (preload) every animation of its animation set whose entry flags have bit 2 set
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663140, 117 bytes.
// Callees: CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>*) @ 0x0042B0A2 (byte-twin label
//   GetSubDef<CAbilityDef>), CThing::GetWorld @ 0x004C7AB0 (label "CViewBase::GetDisplayEngine" propagated),
//   CAnimationEntry::GetFlags @ 0x005DC210 (label "CCombatActionBase::GetAnimationTransitionInTime" propagated),
//   the mesh bank's GetAnimation(long) @ 0x00A242C0 (label "GetAbilityLevel" propagated, as in the landed 0x0070B4A0),
//   C3DAnimationInfo::TouchAnimation @ 0x00A26A50.
// Layout: retail CThing PBaseDef (CDefPointer object word) +0x70; retail CWorld +0x3c = the main mesh bank;
//   CAppearanceDef Animation (CAnimationSet, its CVectorMap's vector first/last at +0x34/+0x38, retail see 0x004717C7)
//   holds 20-byte pairs {unsigned long key; CAnimationEntry entry (animation index first)}; the def's ref count is at +4
//   with virtual Destroy in slot 1.
// Shape: the sub-def is read once from the out slot into a holder (ebx) whose dtor releases it; the bank is a named
//   local (ebp); the loop re-reads the vector end each pass.
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
struct C3DAnimationInfo {
    void TouchAnimation();                      // 0x00A26A50
};
struct CMeshBank {
    C3DAnimationInfo* GetAnimation(long index); // 0x00A242C0
};
struct CWorld {
    unsigned char _pad[0x3c];
    CMeshBank* PMainMeshBank;                   // +0x3c
};
struct CAnimationEntry {                        // 16 bytes
    long AnimIndex;                             // +0x00
    unsigned long _rest[3];
    long GetFlags() const;                      // 0x005DC210
};
struct CAnimationPair {                         // 20 bytes
    unsigned long Key;
    CAnimationEntry Entry;
};
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
struct CAppearanceDef : CDefClassBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationPair* AnimsFirst;                 // +0x34 Animation.Anims vector
    CAnimationPair* AnimsLast;                  // +0x38
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template<class T> struct CDefHolder {          // releases a def taken out of a CDefPointer slot (kept in one register)
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --((CDefClassBase*)Object)->RefCount == 0)
            ((CDefClassBase*)Object)->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CThingBaseDef {
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;      // 0x0042B0A2
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template<class T> __declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)

struct CThingCreatureBase {
    unsigned char _pad00[0x70];
    const CThingBaseDef* PBaseDef;              // +0x70
    CWorld* GetWorld();                         // 0x004C7AB0
    void PreloadAnims();
};

void CThingCreatureBase::PreloadAnims()
{
    CDefPointer<const CAppearanceDef> out;
    bool found = PBaseDef->GetSubDef(&out);
    CDefHolder<const CAppearanceDef> def(out.Object);
    if (found) {
        CMeshBank* bank = GetWorld()->PMainMeshBank;
        for (CAnimationPair* it = def->AnimsFirst; it != def->AnimsLast; ++it) {
            if (it->Entry.GetFlags() & 4)
                bank->GetAnimation(it->Entry.AnimIndex)->TouchAnimation();
        }
    }
}