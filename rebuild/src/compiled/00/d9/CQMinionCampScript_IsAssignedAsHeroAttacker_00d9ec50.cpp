// CQ_MinionCampScript::IsAssignedAsHeroAttacker(const CScriptThing& thing) -- true when `thing` is one of the two live hero attackers (HeroAttackerOne / HeroAttackerTwo) and the script thread is not terminating
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9EC50, length 147 bytes (padded to 160), `ret 4`.
// Callees: CScriptThing virtual slots 77 IsNull (+0x134; 0x004AB150), 75 IsAlive (+0x12c; 0x004AB130), and 78 IsEqualTo
// (+0x138; 0x004AA8A0) called on the argument's implementation object (PImp_Data, +4) with the attacker's PImp_Data;
// CScriptBase::IsActiveThreadTerminating @ 0x00CB7940.
// Layout (retail = Ego_r PDB offsets -0x14): HeroAttackerOne +0x78, HeroAttackerTwo +0x84 (CScriptThing, 12 bytes:
// vptr, PImp = CCountedPointer<CScriptThing> {Data +4, Info +8} as in the PDB). engine/CScriptThing.h flattens the
// vptr to a `void*` and PImp to two words, so the virtual calls need this local model.
// Shape: one `||` of two `&&` triples; the comparison is an inline CScriptThing::operator== forwarding to the
// implementation's IsEqualTo through CCountedPointer's inline operator-> / operator* (`PImp->IsEqualTo(&*other.PImp)`;
// reading `.Data` directly swaps eax/edx in the IsAlive and second IsNull vtable loads); the true path is `IsActiveThreadTerminating() ? false : true` (8-bit neg/sbb/inc).
template <class T> struct CCountedPointer {       // CCountedPointer<T> (8 bytes)
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
};
struct CScriptThing {                              // vtable 0x01238C8C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
    virtual void v076();
    virtual bool IsNull() const;   // slot 77 (+0x134)
    virtual bool IsEqualTo(const CScriptThing* other) const;   // slot 78 (+0x138)
    CCountedPointer<CScriptThing> PImp;            // +0x04 (Data +4, Info +8)
    bool operator==(const CScriptThing& other) const { return PImp->IsEqualTo(&*other.PImp); }
};
struct CScriptBase {
    bool IsActiveThreadTerminating();              // 0x00CB7940
};
struct CQ_MinionCampScript : CScriptBase {
    unsigned char _pad_0x00[0x78];
    CScriptThing  HeroAttackerOne;                 // +0x78
    CScriptThing  HeroAttackerTwo;                 // +0x84
    bool IsAssignedAsHeroAttacker(const CScriptThing& thing);
};
bool CQ_MinionCampScript::IsAssignedAsHeroAttacker(const CScriptThing& thing)
{
    if ((!HeroAttackerOne.IsNull() && thing == HeroAttackerOne && HeroAttackerOne.IsAlive())
        || (!HeroAttackerTwo.IsNull() && thing == HeroAttackerTwo && HeroAttackerTwo.IsAlive()))
        return IsActiveThreadTerminating() ? false : true;
    return false;
}