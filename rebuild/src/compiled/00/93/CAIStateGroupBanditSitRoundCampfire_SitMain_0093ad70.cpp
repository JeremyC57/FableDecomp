// CAIStateGroup_BanditSitRoundCampfire::SitMain() -- keep picking a random campfire activity (idle / drink / sing when seated at the fire, idle / eat / drink otherwise) until one reports false
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093AD70, 260 bytes (`ret`). Ego_r.pdb: ?SitMain@CAIStateGroup_BanditSitRoundCampfire@@AAE_NXZ.
// Callees: CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFFloatRandom(float range, unsigned long* seed) 0x004978A0
// (`__fastcall`), Idle 0x0093A540, Eat 0x0093A5C0, Singsong 0x0093A7A0, Drink 0x0093A8C0 (this class); float pool
// 1.0f 0x01292444, 2.0f 0x01292440.
// Shape: the 0xc3 component lookup as an out-parameter GetInterface (presence byte test `test byte ptr [eax+38h],8`,
// the binary search written in place on ONE iterator variable -- a separate lower_bound helper copies its result
// esi -> eax -- with `len / 2` = `cdq ; sub ; sar`, then the end/greater select); a weighted pick on GFFloatRandom: `r < a`,
// else `r - a < 1.0f` (`fld a ; fsubr st(1)`), the four activity calls tail-merged into one `test al,al ; jne` loop test.
// Leaf: SitMain@CAIStateGroup_BanditSitRoundCampfire
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
};


// The component lookup of this TU inlines the STLport __lower_bound over the {type, component} vector.
struct CInlineInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
};
struct CThingI {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInlineInterfaceMap m_interfaces;           // +0x44
    bool HasTC(long type) const { if (m_presentBits[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long type)
    {
        if (HasTC(type)) {
            CInlineInterfaceMap::SEntry* last = m_interfaces.last;
            CInlineInterfaceMap::SEntry* it = m_interfaces.first;
            long len = last - it;
            while (len > 0) {
                long half = len / 2;
                if (it[half].type < type) {
                    it += half + 1;
                    len = len - half - 1;
                }
                else
                    len = half;
            }
            if (it == last || type < it->type)
                it = last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCCampfireSitter {                      // the 0xc3 component
    unsigned char _pad[0x0c];
    bool AtCampfire;                            // +0x0c
};

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct CAIStateGroupSeed : CAIStateGroupBase {
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
};

struct CAIStateGroup_BanditSitRoundCampfire : CAIStateGroupSeed {
    bool Idle();                                // 0x0093A540
    bool Eat();                                 // 0x0093A5C0
    bool Singsong();                            // 0x0093A7A0
    bool Drink();                               // 0x0093A8C0
    bool SitMain();
};

bool CAIStateGroup_BanditSitRoundCampfire::SitMain()
{
    bool ok;
    do {
        CTCCampfireSitter* sitter;
        if (((CThingI*)PBrain->PCreature)->GetInterface(&sitter, 0xc3) && sitter->AtCampfire) {
            float r = GFFloatRandom(3.0f, GetPWorldSeed());
            if (r < 1.0f)
                ok = Idle();
            else if (r - 1.0f < 1.0f)
                ok = Drink();
            else
                ok = Singsong();
        }
        else {
            float r = GFFloatRandom(5.0f, GetPWorldSeed());
            if (r < 2.0f)
                ok = Idle();
            else if (r - 2.0f < 1.0f)
                ok = Eat();
            else
                ok = Drink();
        }
    } while (ok);
    return ok;
}