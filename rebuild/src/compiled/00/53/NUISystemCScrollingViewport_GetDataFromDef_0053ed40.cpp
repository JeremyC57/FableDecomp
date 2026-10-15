// NUISystem::CScrollingViewport::GetDataFromDef() -- fetch the component's UI def and copy its scrolling speed and text window into the viewport, then release the def
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053ED40, 94 bytes (`?GetDataFromDef@CScrollingViewport@NUISystem@@QAEXXZ`,
// Ego_r.pdb; called by the landed constructor 0x0053F120).
// Callees: CComponent::LoadAndReturnDef() const (primary vtable slot 108, +0x1B0; returns
// CDefPointer<const CUIDef> through the hidden pointer = the `push ecx` frame slot), the def's virtual
// slot 1 (destroy) when the last reference goes.
//
// Shape: the def pointer is read back from the frame slot, ScrollingSpeed copied as a dword and the
// C2DBoxF window block-copied through `lea ecx,[eax+0A4h]` + edx (a struct member assignment); the
// temporary's destructor releases through the ref-count base's two inline accessors (DecRef, then
// GetRefCount) -- the `mov esi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec esi ; mov edx,esi ; test edx,edx`
// form of GOTCHAS wave 18 (model of the landed CDefinitionManager::GetDef 0x0083E0D0). The destructor's
// `if (Data)` test folds away after the dereferences.
// Retail layouts: CScrollingViewport = Ego_r.pdb minus 0x34 (ScrollingSpeed PDB +0x1A8 -> +0x174, Window
// +0x1AC -> +0x178); CUIDef minus 0x1C (TextWindow PDB +0xC0 -> +0xA4, ScrollingSpeed +0xDC -> +0xC0: the
// PDB vectors are 16 bytes, the retail ones 12). Engine headers carry the PDB offsets, so the classes are
// modelled locally with their real member names.
// Leaf: ?GetDataFromDef@CScrollingViewport
struct CRefCounted {                             // second base of CDefBase: the count at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                      // slot 1
};
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
};
struct CUIDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0xa4 - 0x08];
    C2DBoxF TextWindow;          // +0xa4
    unsigned char _pad_0xb4[0xc0 - 0xb4];
    float ScrollingSpeed;        // +0xc0
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CDefPointer_CUIDef {                      // CDefPointer<const NUISystem::CUIDef>
    const CUIDef* Data;
    ~CDefPointer_CUIDef() { if (Data) const_cast<CUIDef*>(Data)->ReleaseTemporary(); }
    const CUIDef* operator->() const { return Data; }
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
    virtual CDefPointer_CUIDef LoadAndReturnDef() const;   // slot 108 (+0x1B0)
};
struct CScrollingViewport : CComponentInterface {
    unsigned char _pad_0x04[0x174 - 0x04];
    float   ScrollingSpeed;          // +0x174
    C2DBoxF Window;                  // +0x178
    void GetDataFromDef();
};
void CScrollingViewport::GetDataFromDef()
{
    CDefPointer_CUIDef def = LoadAndReturnDef();
    ScrollingSpeed = def->ScrollingSpeed;
    Window = def->TextWindow;
}
}