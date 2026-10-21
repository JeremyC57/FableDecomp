// Grade: DIFFER(440v442) -- register allocation: retail this=edi, zero/&First=esi, slot/count/first=ebx (late push); ours this=ebx, zero=edi, esi late; retail tail-merges the early-empty free into the final free call (push eax ; jmp), ours keeps its own call. Tried: if(!empty) wrap (427v442, worse), CSoundInit 0x20 fixes frame 0x3c.
// CTCInventoryAbilities::CycleAssignedExpressions(bool forward) -- page the assignable expression slots one page forward or
// back through the prioritised expression defs (wrapping at either end, giving up after a full turn without a usable def),
// re-assign the slots from the new first index and play the menu click
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F6793, 440 bytes, `ret 4` (frame pointer, `leave`).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CLongVector {                                        // STLport vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CLongVector()
    {
        if (_M_start)
            free(_M_start);
    }
    long* begin() { return _M_start; }
    long* end() { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    long size() const { return _M_finish - _M_start; }
};
struct CSoundInit {
    unsigned char _pad00[0x14];
    long          Flags;                                    // +0x14
    unsigned char _pad18[0x20 - 0x18];
    CSoundInit();                                           // 0x0041C930
};
class CASoundBank {
public:
    long GetSoundIndexFromSymbol(const CCharString& symbol);   // 0x00A01920
};
class CThing {
public:
    CASoundBank& GetSampleBank() const;                     // 0x004C7A80
};
struct CSoundPlayer2D {
    virtual void p00(); virtual void p01(); virtual void p02();
    virtual void Play(CASoundBank& bank, long sound, bool flag, CSoundInit* init, long a);   // slot 3 (+0x0C)
};
struct CGameState {
    virtual void g00(); virtual void g01(); virtual void g02(); virtual void g03(); virtual void g04(); virtual void g05(); virtual void g06(); virtual void g07(); virtual void g08(); 
    virtual CSoundPlayer2D* GetSoundPlayer();               // slot 9 (+0x24)
};
extern CGameState* g_pGameState;                            // 0x013B8394

class CTCInventoryAbilities {
public:
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04 CTCBase
    unsigned char _pad08[0x1B8 - 0x08];
    long          FirstAssignedExpression;                  // +0x1B8
    void GetPrioritisedExpressionDefs(CLongVector* defs);   // 0x005EED8C
    long GetNumberOfAssignableSlots();                      // 0x005EDCF0
    void SetAssignedExpression(long defIndex, long slot);   // 0x005F66D9
    void CycleAssignedExpressions(bool forward);
};

void CTCInventoryAbilities::CycleAssignedExpressions(bool forward)
{
    CLongVector defs;
    GetPrioritisedExpressionDefs(&defs);
    if (defs.empty())
        return;
    for (long slot = 0; slot < GetNumberOfAssignableSlots(); ++slot)
        SetAssignedExpression(0, slot);
    long original = Max(FirstAssignedExpression, -GetNumberOfAssignableSlots());
    long count = defs.size();
    long* first;
    for (;;) {
        if (forward)
            FirstAssignedExpression += GetNumberOfAssignableSlots();
        else
            FirstAssignedExpression -= GetNumberOfAssignableSlots();
        if (FirstAssignedExpression >= count)
            FirstAssignedExpression = Min(original, 0L);
        else if (FirstAssignedExpression < 0)
            FirstAssignedExpression = count - GetNumberOfAssignableSlots();
        first = defs.begin();
        if (FirstAssignedExpression > 0)
            first += FirstAssignedExpression;
        long* it = first;
        for (long checked = 0; checked < GetNumberOfAssignableSlots(); ++checked, ++it)
            if (it != defs.end() && *it > 0)
                goto found;
        if (FirstAssignedExpression == original)
            return;
    }
found:
    for (long slot = 0; first != defs.end(); ++first) {
        SetAssignedExpression(*first > 0 ? *first : 0, slot);
        if (++slot == GetNumberOfAssignableSlots())
            break;
    }
    if (g_pGameState) {
        CSoundInit init;
        init.Flags = 0x20;
        CASoundBank& bank = ParentThing->GetSampleBank();
        long sound = bank.GetSoundIndexFromSymbol(CCharString("SND_MENU_04", -1));
        g_pGameState->GetSoundPlayer()->Play(bank, sound, true, &init, 0);
    }
}
