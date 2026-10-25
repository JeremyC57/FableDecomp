// CSubtitleRenderer::CSetTextInfo::~CSetTextInfo() -- destroy the four text strings (Button3Text, Button2Text, Button1Text, Text)
// Leaf: ??1CSetTextInfo@CSubtitleRenderer@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043E3D0, 35 bytes, thiscall; four CWideString::~CWideString (0x0099B510)
//   calls on +0xC, +8, +4 and a tail jump on +0.
// Identity (sweep1, 2026-10-25): FINDINGS chunk 454 / hand454 README ("0x0043E3D0 is labelled _Dest_val<CMacro> but is
//   ~CSetTextInfo"); Ego_r.pdb CSubtitleRenderer::CSetTextInfo (28 bytes) is Text +0, Button1Text +4, Button2Text +8,
//   Button3Text +0xC (all CWideString), then DisplayFlags, DisplayTime and two bools (no destructors); the callee
//   0x0099B510 is the landed CWideString::~CWideString. Callers: the landed deque<CSubtitleRenderer::CSetTextInfo>
//   _M_pop_front_aux 0x00493C25 (at 0x00493C2A, element stride 0x1C) and 0x004929B8, 0x00493D04, 0x0049415E in the
//   CSubtitleRenderer run. The landed row modelled the element destructor as CMacro (0x0055B060) at a stale slice address
//   0x004404B0 and the catalog name `Dtor_CCharStringBase_0043e3d0` named the wrong string class.
// Shape: the implicit member-wise destructor (members destroyed in reverse order), spelled out so it is emitted.

class CWideString {
public:
    void* PStringData;
    ~CWideString();                                       // 0x0099B510
};

class CSubtitleRenderer {
public:
    class CSetTextInfo {                                  // 28 bytes (class: the PDB mangles it V)
    public:
        CWideString   Text;                               // +0x00
        CWideString   Button1Text;                        // +0x04
        CWideString   Button2Text;                        // +0x08
        CWideString   Button3Text;                        // +0x0C
        unsigned long DisplayFlags;                       // +0x10
        float         DisplayTime;                        // +0x14
        bool          UpdateWhilePaused;                  // +0x18
        bool          ChangePlayerMode;                   // +0x19
        ~CSetTextInfo();
    };
};

CSubtitleRenderer::CSetTextInfo::~CSetTextInfo()
{
}
