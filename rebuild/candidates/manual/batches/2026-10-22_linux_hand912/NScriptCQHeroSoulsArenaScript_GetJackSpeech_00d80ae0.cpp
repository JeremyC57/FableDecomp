// NScript::CQ_HeroSoulsArenaScript::GetJackSpeech(long wave) -- the text key of Jack's taunt for arena wave 1..7, the arena-victory key for 8, the wave-one key otherwise
// Leaf: GetJackSpeech
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D80AE0, 160 bytes + the 8-entry jump table (0x00D80B80), `ret 8`.
// PDB: ?GetJackSpeech@CQ_HeroSoulsArenaScript@NScript@@QAE?AVCCharString@@J@Z (CCharString returned through the hidden
// result pointer; `this` is never read).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CCharString::CCharString(const CCharString&)
// 0x0099EC30, CCharString::~CCharString 0x0099EAE0 (manifest label `_Cons_val<...>` is propagated).
// Keys: TEXT_QST_B04_JACK_WAVE_ONE..SEVEN (0x012D41A0..0x012D40F0), TEXT_QST_B04_JACK_ARENA_VICTORY (0x012D40D0).
// Body: a `speech` local starting at the wave-one key is built first and never used again (destroyed before the
// return); every arm then constructs its key straight into the result (all arms share one `push -1 ; push key`
// / constructor tail, wave 1 and the default arm share the wave-one block).
// Shape: the dead local is a COPY of a constructed temporary (ctor into one frame word, copy ctor into the other,
// dtor of the first). VC7.1 elides `CCharString s = CCharString("..")` and `CCharString s = ".."`; an inline helper
// returning a NAMED local (VC7.1 has no NRVO) reproduces the ctor / copy / dtor sequence. Module built without
// C++ exceptions (no EH frame).
class CCharString {
public:
    struct SBlock* m_pBlock;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};

static inline CCharString FirstWaveSpeech()
{
    CCharString speech("TEXT_QST_B04_JACK_WAVE_ONE");
    return speech;
}

namespace NScript {
class CQ_HeroSoulsArenaScript {
public:
    CCharString GetJackSpeech(long wave);
};

CCharString CQ_HeroSoulsArenaScript::GetJackSpeech(long wave)
{
    CCharString speech = FirstWaveSpeech();
    switch (wave) {
    case 1: return CCharString("TEXT_QST_B04_JACK_WAVE_ONE");
    case 2: return CCharString("TEXT_QST_B04_JACK_WAVE_TWO");
    case 3: return CCharString("TEXT_QST_B04_JACK_WAVE_THREE");
    case 4: return CCharString("TEXT_QST_B04_JACK_WAVE_FOUR");
    case 5: return CCharString("TEXT_QST_B04_JACK_WAVE_FIVE");
    case 6: return CCharString("TEXT_QST_B04_JACK_WAVE_SIX");
    case 7: return CCharString("TEXT_QST_B04_JACK_WAVE_SEVEN");
    case 8: return CCharString("TEXT_QST_B04_JACK_ARENA_VICTORY");
    }
    return CCharString("TEXT_QST_B04_JACK_WAVE_ONE");
}
}
