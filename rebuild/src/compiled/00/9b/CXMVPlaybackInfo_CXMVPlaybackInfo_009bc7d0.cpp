// CXMVPlaybackInfo::CXMVPlaybackInfo() -- default constructor: clears LazyInstallerNeedsUnpause only
// Retail: Fable.exe 0x009BC7D0, 7 bytes: `mov eax,ecx ; mov byte ptr [eax+14h],0 ; ret`.
// Identity: Ego_r.pdb ??0CXMVPlaybackInfo@@QAE@XZ; the only callers are CMovie's constructors (0x005482D0 / 0x00548350),
// which build their CXMVPlaybackInfo member at +0x178 with it. PDB layout (0x18 bytes): PDecoder +0, PDraw +4,
// PShow +8, SurfaceChanged +0xC, Flags +0x10, LazyInstallerNeedsUnpause +0x14 -- the PC build initialises only the bool.
// Leaf: ??0CXMVPlaybackInfo@@QAE@XZ
class XMVDecoder;
class CTexture;

class CXMVPlaybackInfo {
public:
    CXMVPlaybackInfo();
    XMVDecoder*   PDecoder;                    // +0x00
    CTexture*     PDraw;                       // +0x04
    CTexture*     PShow;                       // +0x08
    bool          SurfaceChanged;              // +0x0C
    unsigned long Flags;                       // +0x10
    bool          LazyInstallerNeedsUnpause;   // +0x14
};

CXMVPlaybackInfo::CXMVPlaybackInfo()
    : LazyInstallerNeedsUnpause(false)
{
}