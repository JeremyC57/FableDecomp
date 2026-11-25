// CGameCameraManager::SetAutomaticCameraControl @ 00699090  __fastcall void(this)
// Literals (NF418, 2026-11-25): were extern data kStrA / kStrB (table ?kStrA@@3QBDB -> 0x0125C304, ?kStrB@@3QBDB -> 0x0125C360); now
//   spelled at the use sites: "CAMERA_LIST_PUSH_ERROR" / "SET_AUTO_CONTROL", retail's pooled literals (/GF-merged ??_C@).

struct ScopedGuard {
    void* p;                 // 4-byte object
    ScopedGuard(const char* s, int n);
    ~ScopedGuard();
};

struct Inner {
    char x;
};

struct Sub100 {
    Inner* get(ScopedGuard* g);   // __fastcall, g on stack, returns Inner*
};


class CGameCameraManager { public:
    char pad[0x100];
    Sub100 sub100;           // 0x100
    char pad1[0x2c - sizeof(Sub100)];
    char flag12c;            // 0x12c
    char flag12d;            // 0x12d
    char pad2[6];
    int  h134;               // 0x134
    void PushCurrentManualMode();   // 0x006988B0 ?PushCurrentManualMode@CGameCameraManager@@QAEXXZ (Ego_r / FableWin; the donor SetAutomaticCameraControl calls it)
    void SetAutomaticCameraControl();
};

extern "C" void __fastcall Inner_call_edx(Inner* self, int edx);

void CGameCameraManager::SetAutomaticCameraControl()
{
    if (this->flag12c == 0) {
        this->flag12c = 0;
        ScopedGuard g("CAMERA_LIST_PUSH_ERROR", -1);
        int h = this->h134;
        Inner* p = this->sub100.get(&g);
        Inner_call_edx(p, h);
        return;
    }
    {
        ScopedGuard g("SET_AUTO_CONTROL", -1);
        int h = this->h134;
        Inner* p = this->sub100.get(&g);
        Inner_call_edx(p, h);
    }
    this->PushCurrentManualMode();
    this->flag12d = 0;
    this->flag12c = 0;
}