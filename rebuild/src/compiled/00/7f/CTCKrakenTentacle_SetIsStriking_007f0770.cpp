// CTCKrakenTentacle::SetIsStriking(bool striking) -- when the tentacle was striking, remember the current frame as LastStrikeTick; then store the new IsStriking state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F0770, length 40 bytes.
// Callees: the global frame counter getter @ 0x0049D870 (`return g_frame`; manifest "GFGetPreMainMemoryUsed" propagated).
// Layout (local model, no engine header): IsStriking +0x2a, LastStrikeTick +0x30 (PDB offsets).
// Shape: the bool store is tail-duplicated into both arms (al after the call, cl on the short path).
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                            // 0x0049D870

class CTCKrakenTentacle { public:
    unsigned char _pad_0x00[0x2a];
    bool          IsStriking;                      // +0x2a
    unsigned char _pad_0x2b[0x30 - 0x2b];
    long          LastStrikeTick;                  // +0x30
    void SetIsStriking(bool striking);
};

void CTCKrakenTentacle::SetIsStriking(bool striking)
{
    if (IsStriking)
        LastStrikeTick = CWorld::GetFrame();
    IsStriking = striking;
}