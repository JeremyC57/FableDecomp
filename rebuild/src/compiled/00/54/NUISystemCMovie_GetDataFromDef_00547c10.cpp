// NUISystem::CMovie::GetDataFromDef() -- fetch the movie component's def and copy its size, centring flag, movie name and angle into the component, then release the def
// Leaf: ?GetDataFromDef@CMovie
// Retail: Fable.exe (TLC, VC7.1) @ 0x00547C10, 121 bytes (+ int3 padding). Called by both CMovie constructors (0x005482D0, 0x00548350).
// Callees: CComponent::LoadAndReturnDef() const (primary vtable slot 108, +0x1B0; CDefPointer returned through the hidden
//   pointer = the `push ecx` frame slot), __ftol2 @ 0x00BFEA70 (float -> long), CWideString::operator= @ 0x0099B7D0,
//   the def's virtual slot 1 (destroy) on the last release.
// Retail layout: CMovie = Ego_r.pdb minus 0x30 (the retail CChangingStateComponent base is 0x15C bytes, as in the landed
//   CTextContainer::GetDataFromDef 0x00546CD0): Width +0x168, Height +0x16c, PositionIsCenter +0x170, Movie +0x174,
//   Angle +0x190. The movie def (CUIDef-derived, retail offsets): movie name +0x54, the two size floats +0x58 / +0x5c,
//   angle +0xb8, centring flag +0xbc. Modelled locally with the real member names (the engine header carries the PDB offsets).
// Shape: same def temporary and inline release (DecRef / GetRefCount, no null test once the def is dereferenced) as 0x00546CD0;
//   Width is converted from the +0x5c float first; the flag is loaded before the string copy and stored after its `lea`.
struct CRefCounted {                               // second base of CDefBase: the count at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
struct CWideString {
    void* PStringData;
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
struct CMovieDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0x54 - 0x08];
    CWideString   Movie;                           // +0x54
    float         Height;                          // +0x58
    float         Width;                           // +0x5c
    unsigned char _pad_0x60[0xb8 - 0x60];
    float         Angle;                           // +0xb8
    bool          PositionIsCenter;                // +0xbc
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CDefPointer_CMovieDef {                     // CDefPointer<const CMovieDef>
    const CMovieDef* Data;
    ~CDefPointer_CMovieDef() { if (Data) const_cast<CMovieDef*>(Data)->ReleaseTemporary(); }
    const CMovieDef* operator->() const { return Data; }
};
namespace NUISystem {
struct CComponentInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107();
    virtual CDefPointer_CMovieDef LoadAndReturnDef() const;   // slot 108 (+0x1B0)
};
struct CMovie : CComponentInterface {
    unsigned char _pad_0x04[0x168 - 0x04];
    unsigned long Width;                           // +0x168
    unsigned long Height;                          // +0x16c
    bool          PositionIsCenter;                // +0x170
    unsigned char _pad_0x171[3];
    CWideString   Movie;                           // +0x174
    unsigned char Playback[0x18];                  // +0x178 CXMVPlaybackInfo
    float         Angle;                           // +0x190
    void GetDataFromDef();
};
void CMovie::GetDataFromDef()
{
    CDefPointer_CMovieDef def = LoadAndReturnDef();
    Width = (unsigned long)def->Width;
    Height = (unsigned long)def->Height;
    PositionIsCenter = def->PositionIsCenter;
    Movie = def->Movie;
    Angle = def->Angle;
}
}