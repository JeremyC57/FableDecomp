// CPlayer::InitialiseIfUninitialised()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00488A80, oracle length 45 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov al,[esi+216h] ; test al,al ; jne R ; mov eax,[esi+14h] ; mov ecx,[eax+28h] ; call <loading query> ; test al,al ; jne R ; mov byte ptr [esi+216h],1 ; mov ecx,esi ; pop esi ; jmp CPlayer::SetupInitialPlayerMode ; R: pop esi ; ret
// engine/CPlayer.h (PDB): +0x14 Component (CMainGameComponent*), +0x21E bool InitialPlayerModeSet.
// The std::list PlayerModes (+0x210) is 8 bytes smaller in retail, so the flag is at +0x216,
// which the header only covers as an unnamed pad. Once the component's loader (+0x28 of the component)
// reports that nothing is still loading (0x004349E0, __fastcall, bool; its manifest label is a
// propagated name), the player marks itself initialised and sets up its initial mode
// (0x004887C0, manifest: CPlayer::SetupInitialPlayerMode, tail call).
#include "engine/CPlayer.h"

struct CLoaderCalls {
    bool IsLoading();               // 0x004349E0
};

struct CMainGameComponentCalls {
    unsigned char _pad_0x00[0x28];
    CLoaderCalls* Loader;           // +0x28 (name inferred)
};

struct CPlayer_Methods : CPlayer {
    void SetupInitialPlayerMode();  // 0x004887C0
    void InitialiseIfUninitialised();
};

void CPlayer_Methods::InitialiseIfUninitialised()
{
    bool* initialPlayerModeSet = (bool*)&this->_pad_0x20d[0x216 - 0x20D];   // +0x216 InitialPlayerModeSet (PDB +0x21E)
    if (!*initialPlayerModeSet) {
        if (!reinterpret_cast<CMainGameComponentCalls*>(this->Component)->Loader->IsLoading()) {
            *initialPlayerModeSet = true;
            SetupInitialPlayerMode();
        }
    }
}