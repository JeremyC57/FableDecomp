// CCS_OakValeRevisitedScript::OakValeFire() -- Oakvale-revisited cutscene thread: wait until the "fire" flag is set,
// gather the Q_REVISITED_FIREPOINT things, size Fires to match and create an OAKVALE_BURNING_PATCH effect at every fire
// point; then wait until the flag is cleared again and remove every fire effect
// Leaf: OakValeFire
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE8870, 862 bytes (866 with padding).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0, map<CCharString,bool>::operator[] 0x008ADF10, CScriptThing
//   default ctor 0x006E7B40 / dtor 0x004AA840 / slot 6 GetPos, vector<CScriptThing>::resize(n, const T&) 0x00D34AC0
//   (the one-argument resize is inline), CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slots
//   7 NewScriptFrame, 75 GetAllThingsWithScriptName, 100 CreateEffect, 108 RemoveThing. The by-value CreateEffect result is
//   assigned with CScriptThing's implicit operator= (CCountedPointer::operator= expanded inline).
//   Literals "fire" 0x012C1978, "Q_REVISITED_FIREPOINT" 0x012F5458, "FirePoints" 0x012F544C, "OAKVALE_BURNING_PATCH" 0x012F5434.
// Shape: retail expands CScriptThing::operator= here although its out-of-line copy exists; the lane only inlines it
//   with __forceinline (CCountedPointer::operator= reads the source block then the object first, the 0x008AB980 form
//   reversed). Element access is *(begin() + i): the start pointer is read before the CreateEffect call, the index added after.
// Layout: Ego_r.pdb CCS_OakValeRevisitedScript - 0x14 (retail CScriptBase is 0x40): FirePoint +0x48, Fires +0x54,
//   OakValeFlag +0x60 (map<CCharString,bool>), v1..v7 handles +0x78.. (GSI +0x40, Owner +0x44).
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {                                // CCountedPointer's shared block
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4
    void* Data;                                        // +8
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
struct CBase {
    virtual ~CBase();                                  // 0x0099A2E0
};
namespace N_006e7b40 { struct CScriptThing : CBase {                          // vtable 0x01238C8C, 12 bytes
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;           // slot 6 (+0x18)
    CCountedPointer<void> PImp;                        // +0x04 Data, +0x08 Info
    CScriptThing();                                    // 0x006E7B40
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }   // inline here (out-of-line copy 0x008AB980)
}; } using N_006e7b40::CScriptThing;
template <class T> struct vector {                     // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T& operator[](unsigned int i) { return *(begin() + i); }
    void resize(unsigned int n, const T& value);       // 0x00D34AC0
    void resize(unsigned int n) { resize(n, T()); }
};
struct CBoolMap {                                      // map<CCharString, bool>
    char _M_t[0xc];
    bool& operator[](const CCharString& key);          // 0x008ADF10
};
struct CGameScriptInterface {                          // vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, vector<CScriptThing>& things) const;   // slot 75 (+0x12c)
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091();
    virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual CScriptThing CreateEffect(const CCharString& name, const C3DVector& pos, const CCharString& group, float angle, bool a, bool b) const;   // slot 100 (+0x190)
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
};
struct CScriptBase {                                   // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                  // 0x00CB7940
};
struct CCS_OakValeRevisitedScript : CScriptBase {
    CGameScriptInterface* GSI;                         // +0x40
    void*                 Owner;                       // +0x44
    vector<CScriptThing>  FirePoint;                   // +0x48
    vector<CScriptThing>  Fires;                       // +0x54
    CBoolMap              OakValeFlag;                 // +0x60
    void OakValeFire();
};

void CCS_OakValeRevisitedScript::OakValeFire()
{
    while (OakValeFlag["fire"] != true) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->GetAllThingsWithScriptName("Q_REVISITED_FIREPOINT", FirePoint);
    Fires.resize(FirePoint.size());
    for (unsigned int i = 0; i < FirePoint.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        Fires[i] = GSI->CreateEffect("OAKVALE_BURNING_PATCH", FirePoint[i].GetPos(), "FirePoints", 0.0f, false, false);
    }
    if (IsActiveThreadTerminating())
        return;
    while (OakValeFlag["fire"] != false) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Fires.resize(FirePoint.size());
    for (unsigned int j = 0; j < Fires.size(); ++j) {
        if (IsActiveThreadTerminating())
            return;
        GSI->RemoveThing(Fires[j], false, true);
    }
    if (IsActiveThreadTerminating())
        return;
}