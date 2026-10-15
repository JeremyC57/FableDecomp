// CThingPlayerCreature::vfunc_69(const CCharString&, long) -- looks the animation up by name and hands its first word to the slot-68 overload
// Retail: Fable.exe 0x006635D0, 37 bytes (CThingPlayerCreature vtable 0x012457FC slot 69).
// Callees: CThingCreatureBase::GetAnimFromName(name, 0) 0x00662A00 (returns the CAnimationEntry*),
// vtable slot 68 (+0x110, 0x00663E90, `ret 8`) on this.
// Shape: name-to-entry overload forwarding the entry's handle and the second argument; the entry
// pointer is dereferenced after the call (`mov eax,[eax]`), the second argument re-read from its slot.
struct CCharString { void* m_pData; };

struct CAnimationEntry {
    long m_anim;                                         // +0x00
};

struct CThingPlayerCreature {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void vfunc_68(long anim, long mode);         // slot 68 (+0x110)
    virtual void vfunc_69(const CCharString& name, long mode);
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index);   // 0x00662A00
};

void CThingPlayerCreature::vfunc_69(const CCharString& name, long mode)
{
    vfunc_68(GetAnimFromName(name, 0)->m_anim, mode);
}