// CXMVPlayer::LoadXMVCode() -- load the shared XMV playback code (static loader) and count this player's reference
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BD090, 13 bytes: `push esi ; mov esi,ecx ; call 0x009BCF70 ; inc dword ptr
//   [esi+54h] ; pop esi ; ret`.
// Identity: Ego_r.pdb ?LoadXMVCode@CXMVPlayer@@QAEXXZ / ?ReleaseXMVCode@CXMVPlayer@@QAEXXZ next to ?StaticLoadXMVCode@
//   CXMVPlayer@@SIXXZ / ?StaticReleaseXMVCode@CXMVPlayer@@SIXXZ; retail 0x009BD090 calls StaticLoadXMVCode 0x009BCF70 and
//   increments +0x54, and its sibling 0x009BD0A0 decrements +0x54 then tail-jumps to StaticReleaseXMVCode 0x009BCFC0
//   (both static loaders already labelled in labels_callee_canon); Ego_r.pdb CXMVPlayer::LocalXMVPlayerRefCount is the
//   long at +0x54 (84). Not CTCTrophy::OnShowTrophyInitialise (a propagated byte-twin label). FINDINGS chunk 411.
// Leaf: ?LoadXMVCode@CXMVPlayer@@QAEXXZ
// Layout: no engine header; the class is modelled locally with the PDB field at its retail offset.
class CXMVPlayer {
public:
    unsigned char _pad00[0x54];
    long LocalXMVPlayerRefCount;                // +0x54
    static void __fastcall StaticLoadXMVCode();   // 0x009BCF70
    void LoadXMVCode();
};

void CXMVPlayer::LoadXMVCode()
{
    StaticLoadXMVCode();
    ++LocalXMVPlayerRefCount;
}
