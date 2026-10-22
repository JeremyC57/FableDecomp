// CPlayer::RemoveMode(EPlayerMode mode) -- take a player mode off the mode stack: when it is the active (top) mode of a local player, uninitialise it, pop it and initialise the mode below; any other occurrence is erased from the list (searching from the top); with the player-mode debug switch on, the mode list is formatted into a trace string before and after
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00634190, length 628 bytes, `ret 4`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / operator+=(const CCharString&) 0x0099F0A0
// / operator+=(const char*) 0x0099F100, GFIntToCharString 0x0099F830, operator+(const CCharString&, const char*)
// 0x0099F600, CPlayer::UnInitActiveMode 0x00633BF0, CPlayer::InitActiveMode 0x00633CB0, free 0x00BFEA14 (the list node).
// Data: the player-mode debug switch 0x013B86D6; strings "PlayerModes", "RemoveModeStart", "RemoveModeEnd",
// " - transient mode: ", " : all modes are - ", ", ".
// Layout (retail; engine/CPlayer.h carries the 12-byte PDB list, so the members after +0x210 sit 8 bytes high there):
// Local +0x208, PlayerModes +0x210 (list<EPlayerMode> = the sentinel node pointer), SuppressFullModeRemoval +0x214,
// DisallowModeChanges +0x215.
// Shape: the trace string is built in full (the output call is compiled out of the release build); the STLport list
// pop_back / erase(--rit.base()) are inline node unlinks released with free.
typedef long EPlayerMode;
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
    CCharString& operator+=(const char* text);          // 0x0099F100
};
CCharString __fastcall GFIntToCharString(long value);                      // 0x0099F830
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14 (MSVCR71)
extern bool g_DebugPlayerModes;                         // 0x013B86D6

struct SModeListNode {                                  // list<EPlayerMode> node
    SModeListNode* Next;
    SModeListNode* Prev;
    EPlayerMode    Value;
};

struct SModeReverseIterator {                           // reverse_iterator<list<EPlayerMode>::iterator>
    SModeListNode* current;
    explicit SModeReverseIterator(SModeListNode* node) : current(node) { }
    SModeListNode* base() const { return current; }
    const EPlayerMode& operator*() const { return current->Prev->Value; }
    SModeReverseIterator& operator++() { current = current->Prev; return *this; }
    bool operator!=(const SModeReverseIterator& other) const { return current != other.current; }
};
template <class It, class T> inline It find(It first, It last, const T& value)
{
    while (first != last && !(*first == value))
        ++first;
    return first;
}

struct CPlayer {
    unsigned char  _pad00[0x208];
    bool           Local;                               // +0x208
    unsigned char  _pad209[0x210 - 0x209];
    SModeListNode* PlayerModes;                         // +0x210 list<EPlayerMode> (sentinel)
    bool           SuppressFullModeRemoval;             // +0x214
    bool           DisallowModeChanges;                 // +0x215
    void UnInitActiveMode(EPlayerMode mode);            // 0x00633BF0
    void InitActiveMode(EPlayerMode mode);              // 0x00633CB0
    void RemoveMode(EPlayerMode mode);
};

void CPlayer::RemoveMode(EPlayerMode mode)
{
    if (g_DebugPlayerModes) {
        CCharString text("PlayerModes");
        text += CCharString("RemoveModeStart");
        text += " - transient mode: ";
        text += GFIntToCharString(mode);
        text += " : all modes are - ";
        for (SModeListNode* node = PlayerModes->Next; node != PlayerModes; node = node->Next)
            text += GFIntToCharString(node->Value) + ", ";
    }
    if (!DisallowModeChanges) {
        SModeListNode* top = PlayerModes->Prev;
        if (top->Value == mode) {
            if (Local) {
                if (!SuppressFullModeRemoval) {
                    SuppressFullModeRemoval = true;
                    UnInitActiveMode(PlayerModes->Prev->Prev->Value);
                    SModeListNode* last = PlayerModes->Prev;
                    if (last->Value == mode) {
                        SModeListNode* next = last->Next;
                        SModeListNode* prev = last->Prev;
                        prev->Next = next;
                        next->Prev = prev;
                        free(last);
                    }
                    InitActiveMode(mode);
                    SuppressFullModeRemoval = false;
                } else {
                    SModeListNode* next = top->Next;
                    SModeListNode* prev = top->Prev;
                    prev->Next = next;
                    next->Prev = prev;
                    free(top);
                }
            }
        } else {
            SModeReverseIterator it = find(SModeReverseIterator(PlayerModes), SModeReverseIterator(PlayerModes->Next), mode);
            if (it != SModeReverseIterator(PlayerModes->Next)) {
                SModeListNode* node = it.base()->Prev;
                SModeListNode* next = node->Next;
                SModeListNode* prev = node->Prev;
                prev->Next = next;
                next->Prev = prev;
                free(node);
            }
        }
    }
    if (g_DebugPlayerModes) {
        CCharString text("PlayerModes");
        text += CCharString("RemoveModeEnd");
        text += " - transient mode: ";
        text += GFIntToCharString(mode);
        text += " : all modes are - ";
        for (SModeListNode* node = PlayerModes->Next; node != PlayerModes; node = node->Next)
            text += GFIntToCharString(node->Value) + ", ";
    }
}