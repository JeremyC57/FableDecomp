// NUISystem::CComponent::GetType() const -- the component's UI type (the Type byte widened to NUISystem::EType)
// Leaf: ?GetType@CComponent@NUISystem@@UBE?AW4EType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052EFE0, 8 bytes: movzx eax,byte ptr [ecx+12Dh] ; ret.
// Identity: NUISystem::CComponent vtable 0x0124608C slot 65 (inherited by 42 more UI vtables, CSprite's 0x0122F5D4 among
//   them; the catalog "CSprite::vfunc_65" is the slot name). The landed CComponent::GetDataFromDef 0x005331A0 stores the
//   def's Type into the byte at +0x12D; Ego_r.pdb ?GetType@CComponent@NUISystem@@UBE?AW4EType@2@XZ is the only
//   CComponent virtual returning the type (FindFirstChildOfType etc. take a NUISystem::EType).
// Layout: retail CComponent as the landed 0x005331A0 / 0x005339B0 (Ego_r.pdb minus 0x28 past the STLport containers):
//   Flags +0x12C, Type +0x12D, FlagsSetTwo +0x12E, Layer +0x12F, DefIndex +0x130. No engine header for the class.
// Shape: the unsigned char member converted to the enum is a plain movzx.
namespace NUISystem {

enum EType { ETYPE_NONE = 0 };

class CComponent {
public:
    virtual ~CComponent();                      // slot 0
    virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005();
    virtual void _v006(); virtual void _v007(); virtual void _v008(); virtual void _v009(); virtual void _v010();
    virtual void _v011(); virtual void _v012(); virtual void _v013(); virtual void _v014(); virtual void _v015();
    virtual void _v016(); virtual void _v017(); virtual void _v018(); virtual void _v019(); virtual void _v020();
    virtual void _v021(); virtual void _v022(); virtual void _v023(); virtual void _v024(); virtual void _v025();
    virtual void _v026(); virtual void _v027(); virtual void _v028(); virtual void _v029(); virtual void _v030();
    virtual void _v031(); virtual void _v032(); virtual void _v033(); virtual void _v034(); virtual void _v035();
    virtual void _v036(); virtual void _v037(); virtual void _v038(); virtual void _v039(); virtual void _v040();
    virtual void _v041(); virtual void _v042(); virtual void _v043(); virtual void _v044(); virtual void _v045();
    virtual void _v046(); virtual void _v047(); virtual void _v048(); virtual void _v049(); virtual void _v050();
    virtual void _v051(); virtual void _v052(); virtual void _v053(); virtual void _v054(); virtual void _v055();
    virtual void _v056(); virtual void _v057(); virtual void _v058(); virtual void _v059(); virtual void _v060();
    virtual void _v061(); virtual void _v062(); virtual void _v063(); virtual void _v064();
    virtual EType GetType() const;              // slot 65 (+0x104)
   
    virtual void _v066(); virtual void _v067(); virtual void _v068(); virtual void _v069(); virtual void _v070();
    virtual void _v071(); virtual void _v072(); virtual void _v073(); virtual void _v074(); virtual void _v075();
    virtual void _v076(); virtual void _v077(); virtual void _v078(); virtual void _v079(); virtual void _v080();
    virtual void _v081(); virtual void _v082(); virtual void _v083(); virtual void _v084(); virtual void _v085();
    virtual void _v086(); virtual void _v087(); virtual void _v088(); virtual void _v089(); virtual void _v090();
    virtual void _v091(); virtual void _v092(); virtual void _v093(); virtual void _v094(); virtual void _v095();
    virtual void _v096(); virtual void _v097(); virtual void _v098(); virtual void _v099();
    unsigned char _pad004[0x12c - 0x4];
    unsigned char Flags;                        // +0x12C (bits 0-4 the def type, 5 LayerIndependant, 6 Independant, 7 BastardChild)
    unsigned char Type;                         // +0x12D
    unsigned char FlagsSetTwo;                  // +0x12E
    char          Layer;                        // +0x12F
    long          DefIndex;                     // +0x130
};

} // namespace NUISystem

NUISystem::EType NUISystem::CComponent::GetType() const
{
    return (EType)Type;
}