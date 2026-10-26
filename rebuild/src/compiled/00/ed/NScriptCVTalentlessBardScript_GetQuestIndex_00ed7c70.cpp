// CV_TalentlessBardScript::GetQuestIndex(CCharString name) -- index of the named quest in the script's twelve 8-byte quest entries at +0x74; -1 when absent (and when the thread is terminating)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED7C70, length 95 bytes.
// Callees: CCharString::operator!=(const CCharString&) const @ 0x0099E900 (manifest "less<CCharString>::
// operator()" is propagated: the body is `!(a == b)` with the sete tail), CScriptBase::IsActiveThreadTerminating
// @ 0x00CB7940, CCharString::~CCharString @ 0x0099EAE0 (the by-value parameter destroyed by the callee on
// each exit path).
// Shape: the counted loop (`cmp esi,0Ch ; jl`, signed index) walks the entries by pointer (edi += 8) comparing
// each entry's name with the parameter's slot address; on a miss at 12 the terminating test returns -1
// directly (`or eax,-1`), otherwise the index becomes -1 (`or esi,-1`) and the common tail destroys the
// parameter and returns esi; `ret 4`.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* m_pStringData;
    CCharString(const CCharString& other);         // 0x0099E450 (never called here: it makes the by-value parameter a callee-destroyed copy)
    ~CCharString();                                // 0x0099EAE0
    bool operator!=(const CCharString& other) const;   // 0x0099E900
};

struct SQuestEntry {                               // 8 bytes
    CCharString m_name;                            // +0
    long        m_state;                           // +4
};

namespace NScript {
class CV_TalentlessBardScript {
public:
    virtual void v00();
    unsigned char _pad04[0x70];                    // +0x04..+0x73
    SQuestEntry   m_quests[12];                    // +0x74
    unsigned char IsActiveThreadTerminating();     // 0x00CB7940 (CScriptBase)
    long GetQuestIndex(CCharString name);
};
} // namespace NScript
using NScript::CV_TalentlessBardScript;

long NScript::CV_TalentlessBardScript::GetQuestIndex(CCharString name)
{
    long i;
    for (i = 0; i < 12; ++i) {
        if (!(m_quests[i].m_name != name))
            break;
    }
    if (i == 12) {
        if (IsActiveThreadTerminating())
            return -1;
        i = -1;
    }
    return i;
}