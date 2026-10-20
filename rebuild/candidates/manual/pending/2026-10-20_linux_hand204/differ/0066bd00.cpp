// Grade: DIFFER(231v231) (retry 9, differ/) -- VC7.1 expands an inline callee at every site while its cost is below a band,
//   exactly once (end-of-block site first, the early return then calls the out-of-line copy 0x00414EA0) in the next band,
//   never above it. The plain dtor sits in the always band; a dead member store after the free (`_M_start = 0`, DSE'd)
//   moves it into the once band and gives retail's site assignment and length, and the guard written as an `if` body
//   (`if (enabled && state) { ... } return invalid;`) merges the refusal exits into the shared tail with all four
//   callee-saved pushes in the prologue. Left: the lane keeps the zero register for the final `_M_start` null test
//   (`xor esi,esi ; cmp edi,esi` v `test esi,esi`) and colours the dtor cursor edi instead of esi. The dead store is not
//   in the retail out-of-line copy, so this stays in differ/.
// CTCSoundPlayer::PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d) -- expand the criteria into its candidate sound-event names and play the first one that starts, attached to the owner; the invalid handle when sound is off or nothing plays
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066BD00, 229 bytes (+ int3 padding), `ret 14h`. Ego_r.pdb
//   `?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z`; the catalog label "CSoundInfo::PlaySound" is a
//   propagated name (landed callers declare this address as `PlaySound(const CCharString&, bool, long, long, long)`).
// Leaf: PlayAttachedCriteria
// Callees: CThing::GetSampleBank() const @ 0x004C7A80 (PDB; reads the global sample bank), CTCSoundPlayer::
//   GetPotentialCriteriaStrings(const CCharString&, vector<CCharString>&) @ 0x0066BC10 and InternalPlaySoundEvent(CASoundBank&,
//   const CCharString&, bool, bool, bool, long) @ 0x0066B120 (PDB names; manifest "Sound_AppendAbilityNameToDescription" /
//   "CTCSoundPlayer_GetSoundState"), ~CCharString @ 0x0099EAE0, vector<CCharString>::~vector @ 0x00414EA0, free @ 0x00BFEA14.
// Globals: the sound-enabled byte @ 0x0137E974 (set by the `__fastcall` setter 0x0088D700), the game state @ 0x013B8394
//   (no sound while it is null), the invalid sound handle @ 0x0129D25C.
// Shape: the bank reference is fetched before the vector is built (its zero stores sink below the argument pushes); the
//   walk keeps the cursor in edi and re-reads end() after each call. The vector's in-class inline destructor is inlined
//   on the fall-through path (element loop re-reading _M_finish, then the null-tested free) and called out of line on the
//   early return of the first handle that plays.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14

struct CCharString {
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};

struct CCharStringVector {                             // STLport vector<CCharString>: 12 bytes
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CCharStringVector()                               // 0x00414EA0 when out of line
    {
        for (CCharString* p = _M_start; p != _M_finish; ++p)
            p->~CCharString();
        if (_M_start)
            free(_M_start);
        _M_start = 0;
    }
    CCharString* begin() { return _M_start; }
    CCharString* end() { return _M_finish; }
};

class CASoundBank;
class CThing {
public:
    CASoundBank& GetSampleBank() const;                // 0x004C7A80
};

extern bool          g_bSoundEnabled_0137e974;         // 0x0137E974
extern void*         g_pGameState;                     // 0x013B8394
extern unsigned long g_InvalidSoundHandle;             // 0x0129D25C

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                               // +0x04
};

class CTCSoundPlayer : public CTCBase {
public:
    void GetPotentialCriteriaStrings(const CCharString& criteria, CCharStringVector& names);   // 0x0066BC10
    unsigned long InternalPlaySoundEvent(CASoundBank& bank, const CCharString& name, bool a, bool b, bool c, long d);   // 0x0066B120
    unsigned long PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d);
};

unsigned long CTCSoundPlayer::PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d)
{
    if (g_bSoundEnabled_0137e974 && g_pGameState) {
        CASoundBank& bank = ParentThing->GetSampleBank();
        CCharStringVector names;
        GetPotentialCriteriaStrings(criteria, names);
        for (CCharString* it = names.begin(); it != names.end(); ++it) {
            unsigned long handle = InternalPlaySoundEvent(bank, *it, a, b, c, d);
            if (handle != g_InvalidSoundHandle)
                return handle;
        }
    }
    return g_InvalidSoundHandle;
}
