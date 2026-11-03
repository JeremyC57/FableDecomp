#ifndef FABLE_ENGINE_C2DEXTENTSF_H
#define FABLE_ENGINE_C2DEXTENTSF_H
// New header (chunk 718, row 0x00487570 CPlayer::DrawFreeCamDebug). Ego_r.pdb C2DExtentsF: public float X +0, Y +4, sizeof 8
// (struct_layouts_egor.tsv); FableWin.pdb ??0C2DExtentsF@@QAE@XZ / ??0C2DExtentsF@@QAE@MM@Z / ??0C2DExtentsF@@QAE@ABV0@@Z (the user
// copy constructor, as C2DExtentsI's: a C2DExtentsF result travels through a hidden pointer). Returned by the CFontBank virtual
// GetStringExtents (slot 1) and the inline GetVStringExtents.
class C2DExtentsF {
public:
    float X, Y;
    C2DExtentsF() { }
    C2DExtentsF(float x, float y) : X(x), Y(y) { }
    C2DExtentsF(const C2DExtentsF& other) : X(other.X), Y(other.Y) { }
#ifdef FABLE_RETAIL_C2DEXTENTSF
    // Retail opt-in (chunk 733, CUserProfileManager::ApplyVideoValues 0x0040A7F0): FableWin.pdb ?Initialise@C2DExtentsF@@QAEXMM@Z
    // (inline in retail: the aspect-ratio switch stores two immediates).
    void Initialise(float x, float y) { X = x; Y = y; }
#endif
};
#endif
