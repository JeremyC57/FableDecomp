// NScript::CQ_FireHeartScript::DecNumberOfSuns() -- one pad turned back to a moon: with two suns left trigger the "NEARLYDEAD" prophet comment, after the round's second new moon (with more suns left) the "NEWMOONS" one, each at most once per round
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D38AA0, length 233 bytes, plain `ret`.
// Identity: Ego_r.pdb `?DecNumberOfSuns@CQ_FireHeartScript@NScript@@QAEXXZ` (the twin of IncNumberOfSuns 0x00D38310).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CQ_FireHeartScript::TriggerProphetComment(const
// CCharString&) 0x00D38400 (Ego_r.pdb; catalog label "CTCGameModeController_UpdateSunCount" is propagated);
// CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0. Strings: "NEARLYDEAD" @ 0x012CD194,
// "NEWMOONS" @ 0x012CD188.
// Layout (retail, every STLport container 12 bytes, vector<bool> one byte per element): StartNumberOfSuns +0x6C,
// NumberOfSuns +0x80, CommentGivenThisRound +0xF8.
// Shape: the decremented count is compared straight from the register (`dec edx ; mov eax,edx ; cmp eax,2`); each
// arm opens with its own termination return (no common value is live across the call here, so VC does not hoist
// it); the new-moon difference is written out twice (a named local swaps eax/ecx in the compare and the
// registers of the block after it); every comment is guarded by a second termination test and built from an implicit conversion temporary.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

namespace std {
template <class T> struct vector {                      // retail STLport vector, 12 bytes (vector<bool> is byte-based)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
}

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

struct CQ_FireHeartScript : CScriptBase {
    void*         GSI;                                  // +0x40
    void*         PMasterData;                          // +0x44
    unsigned char _pad048[0x24];                        // +0x48 .. +0x6c (mission flags, cutscene maps, prophet counts)
    long StartNumberOfSuns;                             // +0x6c
    long NumberOfRounds;                                // +0x70
    long NumberOfPatterns;                              // +0x74
    long NumberOfProphets;                              // +0x78
    long NumberOfPads;                                  // +0x7c
    long NumberOfSuns;                                  // +0x80
    unsigned char _pad084[0x74];                        // +0x84 .. +0xf8 (round state, pad effects, pattern flags)
    std::vector<bool> CommentGivenThisRound;            // +0xf8
    void TriggerProphetComment(const CCharString& comment);   // 0x00D38400
    void DecNumberOfSuns();
};
void CQ_FireHeartScript::DecNumberOfSuns()
{
    if (--NumberOfSuns == 2) {
        if (IsActiveThreadTerminating())
            return;
        if (!CommentGivenThisRound[3]) {
            if (IsActiveThreadTerminating())
                return;
            TriggerProphetComment("NEARLYDEAD");
            CommentGivenThisRound[3] = true;
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        if (StartNumberOfSuns - NumberOfSuns == 2 && NumberOfSuns > StartNumberOfSuns - NumberOfSuns) {
            if (IsActiveThreadTerminating())
                return;
            if (!CommentGivenThisRound[1]) {
                if (IsActiveThreadTerminating())
                    return;
                TriggerProphetComment("NEWMOONS");
                CommentGivenThisRound[1] = true;
            }
        }
    }
}
}
