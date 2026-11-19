// Grade: DIFFER(625v620) -- retail is link-time-code-generated: the second mask loop calls FindFirstSetBit 0x00A3F860 once, hoisted out of the loop with a register (ecx/edx) convention and its 1<<bit hoisted too (the call is known pure only under LTCG); also this kept in ebp and the play params placed above the other locals
// CSoundEmitterLH::vfunc_3(CSoundUser* user, unsigned int id, bool notPositional, const CSoundRequest* request, bool atListener) -- start (or re-trigger) sound id for an emitter: build the play parameters from the request, find or create the voice, record a new instance and mark the emitter dirty
// Leaf: vfunc_3
// Layout: descriptive (no PDB class for the LH sound library): emitter Manager +4, Position +8, Instances +0x14,
//   Positional +0x18, Dirty +0x19, Group +0x1c; manager Enabled +0x24, System +0x40, ListenerPosition +0x74;
//   request Volume +4, Priority +0xc, FlagMask +0x14, ExtraMask +0x18, Group +0x1c; play params 0x28 bytes.
struct C3DVector { float X, Y, Z; };
int __fastcall FindFirstSetBit(unsigned int mask, unsigned int bytes);   // 0x00A3F860
float __fastcall GFGetTime();                                            // 0x009E1BC0
inline int FindFirstSetBitInline(unsigned int mask, unsigned int bytes)
{
    for (unsigned int i = 0; i < bytes * 8; ++i)
        if (mask & (1 << i)) return (int)i;
    return -1;
}
struct CPlayParams {
    int Group;
    int Pad04[2];
    int Volume;
    bool Positional;
    C3DVector Position;
    int Pad20;
    bool AtListener;
    int Pad28[13];
    CPlayParams();                          // 0x00C0A790
    void SetPriority(int p);                // 0x00C0A860
    void SetReuse(bool b);                  // 0x00C0A880
    void SetFlag(int bit, bool on);         // 0x00C0A8A0
    void SetExtraFlag(int bit, bool on);    // 0x00C0A8C0
};
struct CBankRef {
    CBankRef(void* bank, int n);            // 0x00C08750
    ~CBankRef();                            // 0x00C08780
    int Pad[5];
};
struct CPlayer {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
    virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual int Reuse(void* bank, int a, int b, CPlayParams* p);
    virtual void v15();
    virtual int Create(const CBankRef& ref, unsigned int id, CPlayParams* p);
};
struct CSystem {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
    virtual CPlayer* GetPlayer();
};
struct CBank {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual int Find(unsigned int id, int* a, int* b);
};
struct CSoundUser { int Pad[4]; CBank* Bank; };
struct CSoundRequest { int Pad0; int Volume; int Pad8; int Priority; int Pad10; unsigned int FlagMask; unsigned int ExtraMask; int Group; };
struct CManager { char Pad[0x24]; bool Enabled; char Pad25[0x1b]; CSystem* System; char Pad44[0x30]; C3DVector ListenerPosition; };
struct CSoundInstance {
    CSoundInstance(int handle, CSoundUser* user, unsigned int id, float time, bool positional, bool atListener);  // 0x00AAB850
    int Pad[9];
};
struct CInstancePtr {
    CInstancePtr(CSoundInstance* p);        // 0x00AAC820
    ~CInstancePtr();                        // 0x00AAC870
    int Pad[2];
};
struct CInstanceList { void push_back(const CInstancePtr& p); int Pad; };   // 0x00AACCC0
struct CSoundEmitterLH {
    virtual void v0();
    CManager* Manager;
    C3DVector Position;
    CInstanceList Instances;
    bool Positional;
    bool Dirty;
    int Group;
    virtual int vfunc_3(CSoundUser* user, unsigned int id, bool notPositional, const CSoundRequest* request, bool atListener);
};
int CSoundEmitterLH::vfunc_3(CSoundUser* user, unsigned int id, bool notPositional, const CSoundRequest* request, bool atListener)
{
    if (!Manager->System || !Manager->Enabled || id > 100000)
        return -1;
    CPlayParams p;
    p.SetPriority(request->Priority == -1 ? -1 : request->Priority - 1);
    p.Volume = request->Volume;
    p.Positional = notPositional ? false : Positional;
    p.AtListener = atListener;
    unsigned int mask = request->FlagMask;
    while (mask) {
        int bit = FindFirstSetBitInline(mask, 16);
        p.SetFlag(bit, true);
        mask ^= 1 << bit;
    }
    unsigned int extra = request->ExtraMask;
    while (extra) {
        int bit = FindFirstSetBit(mask, 16);
        p.SetExtraFlag(bit, true);
        extra ^= 1 << bit;
    }
    if (p.AtListener)
        p.Position = Manager->ListenerPosition;
    else if (p.Positional)
        p.Position = Position;
    p.Group = request->Group != -1 ? request->Group : Group;
    int a, b;
    int handle = user->Bank->Find(id, &a, &b);
    if (handle <= -1) {
        handle = Manager->System->GetPlayer()->Create(CBankRef(user->Bank, 0), id, &p);
    } else {
        p.SetReuse(true);
        handle = Manager->System->GetPlayer()->Reuse(user->Bank, a, b, &p);
    }
    if (handle >= 0)
        Instances.push_back(CInstancePtr(new CSoundInstance(handle, user, id, GFGetTime(), p.Positional, p.AtListener)));
    Dirty = true;
    return handle;
}
