// CBriarRose::DoOnSpeakToSpeech() -- Briar Rose's reply when spoken to: picks one of three lines by the minion-camp script's timer (more than 48: _START, more than 24: _MIDDLE, else _END) and says it through DoSpeech unless the script is done
// Leaf: DoOnSpeakToSpeech
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA27B0, 176 bytes incl. padding (plain `ret`).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (line prefix 0x012D7138), the timer service (global
//          0x0143E8F8) slot 90 (+0x168, GetTimer), IsDone @ 0x00F35B30 (manifest label propagated), CCharString::operator+=
//          (const char*) @ 0x0099F100 (suffixes 0x012CD3B8 / 0x012D7114 / 0x012D710C), CBriarRose::DoSpeech @ 0x00DA2610,
//          CCharString::~CCharString @ 0x0099EAE0.
// Layout = Ego_r.pdb CBriarRose (80 bytes, the landed DoSpeech model): ParentClass +0x14 (CQ_MinionCampScript*), whose
//          timer sits at +0x60.
// Shape: the parent's timer is read through an inline CTimer getter taking the timer by reference (`add eax,60h ; mov eax,[eax]`).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator+=(const char* text);          // 0x0099F100
};
struct CTimerManager {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088();
    virtual void SetTimer(long index, long time);   // slot 89 (+0x164)
    virtual long GetTimer(long index);        // slot 90 (+0x168)
};
extern CTimerManager* g_pScriptTimerManager;  // 0x0143E8F8
struct CTimer {
    long TimerIndex;                          // +0x00
};
inline long GetTime(const CTimer& timer) { return g_pScriptTimerManager->GetTimer(timer.TimerIndex); }

struct CQ_MinionCampScript {
    unsigned char _pad[0x60];
    CTimer        CampTimer;                  // +0x60
};

class CBriarRose {
public:
    void*                 __vftable;          // +0x00
    void*                 Game;               // +0x04
    unsigned char         Me[0x0c];           // +0x08
    CQ_MinionCampScript*  ParentClass;        // +0x14
    void*                 PMasterData;        // +0x18
    bool IsDone();                            // 0x00F35B30
    void DoSpeech(const CCharString& line);   // 0x00DA2610
    void DoOnSpeakToSpeech();
};

void CBriarRose::DoOnSpeakToSpeech()
{
    CCharString line("TEXT_QST_B08_BRIAR_ONSPEAKTO");  // 0x012D7138
    if (GetTime(ParentClass->CampTimer) > 48) {
        if (IsDone())
            return;
        line += "_START";                         // 0x012CD3B8
    } else if (GetTime(ParentClass->CampTimer) > 24) {
        if (IsDone())
            return;
        line += "_MIDDLE";                         // 0x012D7114
    } else {
        if (IsDone())
            return;
        line += "_END";                         // 0x012D710C
    }
    DoSpeech(line);
}
