// NScript::CQ_FireHeartScript::IncNumberOfSuns() -- one more pad turned to a sun: with two moons left trigger the "NEARLYFREE" prophet comment, after the round's second new sun (with more moons left) the "NEWSUNS" one, each at most once per round
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D38310, length 231 bytes, plain `ret`.
// Identity: Ego_r.pdb `?IncNumberOfSuns@CQ_FireHeartScript@NScript@@QAEXXZ`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CQ_FireHeartScript::TriggerProphetComment(const
// CCharString&) 0x00D38400 (Ego_r.pdb `?TriggerProphetComment@CQ_FireHeartScript@NScript@@QAEXABVCCharString@@@Z`;
// catalog label "CTCGameModeController_UpdateSunCount" is propagated); CCharString(const char*, long) 0x0099EBF0,
// ~CCharString 0x0099EAE0. Strings: "NEARLYFREE" @ 0x012CD10C, "NEWSUNS" @ 0x012CD104.
// Layout (retail, every STLport container 12 bytes, vector<bool> one byte per element): StartNumberOfSuns +0x6C,
// NumberOfPads +0x7C, NumberOfSuns +0x80, CommentGivenThisRound +0xF8.
// Shape: the moon count is a local kept in edi; both arms open with a termination return, which VC hoists above the
// arm test (one call, `test al,al` duplicated into each arm); the new-sun difference is written out twice (a named
// local changes the register rotation of the block after it); every comment is guarded by a second termination
// test and built from an implicit conversion temporary (re-addressed with `lea`).
class CCharString { public:
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
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {

class CQ_FireHeartScript : public CScriptBase { public:
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
    void IncNumberOfSuns();
};
void CQ_FireHeartScript::IncNumberOfSuns()
{
    long moons = NumberOfPads - ++NumberOfSuns;
    if (moons == 2) {
        if (IsActiveThreadTerminating())
            return;
        if (!CommentGivenThisRound[2]) {
            if (IsActiveThreadTerminating())
                return;
            TriggerProphetComment("NEARLYFREE");
            CommentGivenThisRound[2] = true;
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        if (NumberOfSuns - StartNumberOfSuns == 2 && moons > NumberOfSuns - StartNumberOfSuns) {
            if (IsActiveThreadTerminating())
                return;
            if (!CommentGivenThisRound[0]) {
                if (IsActiveThreadTerminating())
                    return;
                TriggerProphetComment("NEWSUNS");
                CommentGivenThisRound[0] = true;
            }
        }
    }
}
}