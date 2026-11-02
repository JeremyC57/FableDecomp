// CTCCreatureModeManager::CModeBase::$E_NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS() -- dynamic initializer of the static long CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS: twelve seconds in frames (CMainGameComponent::GetConstantFPS() * 12)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F7E0, 17 bytes, in `.text$yc`, reached from the .CRT$XCU table entry
//   0x01374690, plain `ret`: `mov eax,[ConstantFPS] ; lea eax,[eax+eax*2] ; shl eax,2 ; mov [0x013BAFE8],eax`.
// Identity (proven, two sources): (1) Ego_r.pdb 0x01011F00..0x01011F80 and FableWin.pdb 0x03FF7320..0x03FF73C0 run, in one TU,
//   JACK_TOTAL_DURATION, JACK_GLOW_START_FRAME, JACK_FADE_DOWN_TIME (anonymous namespace), then
//   CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_COMB_IDLE_ANIMS, ..._IDLE_ANIMS, ..._WAVE_ANIMS; retail's .CRT$XCU
//   runs 0x0121F780 (0x013BAFD8 = A + B), 0x0121F7A0 (0x013BAFDC = 0x013BAFD8 - B), 0x0121F7C0 (0x013BAFE0 = B), 0x0121F7D0
//   (0x013BAFE4 = fps * 6), this 0x0121F7E0 (0x013BAFE8 = fps * 12), 0x0121F800 (0x013BAFEC = fps * 3): the fifth, the idle-anims
//   delay (Ego_r's three CModeBase copies are 16 / 18 / 14 bytes, retail's 16 / 17 / 14); (2) its one reader is the landed
//   CTCCreatureModeManager::CModeBase::GetNoFramesBeforePlayingIdleAnims 0x0073A760 (Ego_r + FableWin
//   ?GetNoFramesBeforePlayingIdleAnims@CModeBase@CTCCreatureModeManager@@IAEJW4EMode@NCreatureMode@@@Z), whose default delay it is
//   (`mov eax,[0x013BAFE8]`), as 0x013BAFE4 / 0x013BAFEC are for the combination-idle / wave twins 0x0073A6F0 / 0x0073A7D0.
//   Catalog row "_gapscan::sub_0121f7e0" carried no name; the landed 0x0073A760 binds 0x013BAFE8 as an invented
//   `?g_DefaultFrames_013bafe8@@3JA` (NAME_FIXES.tsv).
// Data symbol: ?NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS@CModeBase@CTCCreatureModeManager@@2JB -- static long (the reader's return
//   type `J`); public and `const` assumed (house style for class statics without a PDB data-symbol dump: retail never writes it
//   after this initializer). The nested class is declared here for its static only (class keys V/V, pdb_class_keys.tsv).
// Data: CMainGameComponent::ConstantFPS 0x01375550 (?ConstantFPS@CMainGameComponent@@1JA) through the inline GetConstantFPS() (naming wave 18).
// Shape: `* 12` is strength-reduced to `lea eax,[eax+eax*2] ; shl eax,2` (the landed `* 6` twin 0x0121F7D0: `shl eax,1`).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CTCCreatureModeManager {
public:
    class CModeBase {
    public:
        static const long NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS;   // 0x013BAFE8
    };
};

const long CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS = CMainGameComponent::GetConstantFPS() * 12;