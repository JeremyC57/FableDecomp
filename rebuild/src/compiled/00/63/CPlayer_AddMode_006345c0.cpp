// CPlayer::AddMode(EPlayerMode mode, bool clearOthers) -- push a player mode onto the mode stack of a local player: optionally uninitialise the active mode and reset the stack to the null mode first, then uninitialise the active mode, push the new one and initialise it (when no full mode change is already in progress); traced like RemoveMode under the player-mode debug switch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006345C0, length 682 bytes (+ int3 padding), `ret 8`.
// Callees: CPlayer::InitialiseIfUninitialised 0x00488A80; CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0 / operator+=(const CCharString&) 0x0099F0A0 / operator+=(const char*) 0x0099F100, GFIntToCharString
// 0x0099F830, operator+(const CCharString&, const char*) 0x0099F600, CPlayer::UnInitActiveMode 0x00633BF0,
// CPlayer::InitActiveMode 0x00633CB0, list<EPlayerMode>::clear 0x0044B770 (out of line), malloc 0x00BFEA0E (the node).
// Data: the player-mode debug switch 0x013B86D6; strings "PlayerModes", "AddModeStart", "AddModeEnd",
// " - transient mode: ", " : all modes are - ", ", ".
// Layout (retail; engine/CPlayer.h carries the 12-byte PDB list, so the members after +0x210 sit 8 bytes high there):
// Local +0x208, PlayerModes +0x210 (list<EPlayerMode> = the sentinel node pointer), SuppressFullModeRemoval +0x214,
// DisallowModeChanges +0x215.
// Shape: the sibling RemoveMode 0x00634190 trace block; the STLport insert is inline (node malloc, placement
// construction with its null test, four link stores); the previous mode is kept in the dead `mode` argument slot.
typedef long EPlayerMode;
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
    const CCharString& operator+=(const char* text);          // 0x0099F100
};
CCharString __fastcall GFIntToCharString(long value);                      // 0x0099F830
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14 (MSVCR71)
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E (MSVCR71)
inline void* operator new(unsigned int, void* where) { return where; }
extern bool g_DebugPlayerModes;                         // 0x013B86D6

struct SModeListNode {                                  // list<EPlayerMode> node
    SModeListNode* Next;
    SModeListNode* Prev;
    EPlayerMode    Value;
};

struct _List_iterator {                                 // list<EPlayerMode>::iterator
    SModeListNode* _M_node;
    _List_iterator(SModeListNode* n) : _M_node(n) {}
    _List_iterator(const _List_iterator& o) : _M_node(o._M_node) {}
};
struct CModeList {                                      // STLport list<EPlayerMode>: the sentinel node pointer
    SModeListNode* _M_node;
    _List_iterator begin() { return _List_iterator(_M_node->Next); }
    _List_iterator end() { return _List_iterator(_M_node); }
    EPlayerMode& back() { return _M_node->Prev->Value; }
    SModeListNode* _M_create_node(const EPlayerMode& x)
    {
        SModeListNode* tmp = (SModeListNode*)malloc(sizeof(SModeListNode));
        new (&tmp->Value) EPlayerMode(x);
        return tmp;
    }
    void insert(_List_iterator position, const EPlayerMode& x)
    {
        SModeListNode* tmp = _M_create_node(x);
        SModeListNode* n = position._M_node;
        SModeListNode* prev = n->Prev;
        tmp->Next = n;
        tmp->Prev = prev;
        prev->Next = tmp;
        n->Prev = tmp;
    }
    void push_front(const EPlayerMode& x) { insert(begin(), x); }
    void push_back(const EPlayerMode& x) { insert(end(), x); }
    void clear();                                       // 0x0044B770
};

class CPlayer { public:
    unsigned char  _pad00[0x208];
    bool           Local;                               // +0x208
    unsigned char  _pad209[0x210 - 0x209];
    CModeList      PlayerModes;                         // +0x210
    bool           SuppressFullModeRemoval;             // +0x214
    bool           DisallowModeChanges;                 // +0x215
    void InitialiseIfUninitialised();                   // 0x00488A80
    EPlayerMode GetCurrentMode() { return PlayerModes.back(); }
    void UnInitActiveMode(EPlayerMode mode);            // 0x00633BF0
    void InitActiveMode(EPlayerMode mode);              // 0x00633CB0
    void AddMode(EPlayerMode mode, bool clearOthers);
};

void CPlayer::AddMode(EPlayerMode mode, bool clearOthers)
{
    InitialiseIfUninitialised();
    if (g_DebugPlayerModes) {
        CCharString text("PlayerModes");
        text += CCharString("AddModeStart");
        text += " - transient mode: ";
        text += GFIntToCharString(mode);
        text += " : all modes are - ";
        for (SModeListNode* node = PlayerModes._M_node->Next; node != PlayerModes._M_node; node = node->Next)
            text += GFIntToCharString(node->Value) + ", ";
    }
    if (!DisallowModeChanges && Local) {
        EPlayerMode previous = GetCurrentMode();
        if (clearOthers) {
            UnInitActiveMode(mode);
            PlayerModes.clear();
            PlayerModes.push_front(0);
        }
        if (!SuppressFullModeRemoval) {
            SuppressFullModeRemoval = true;
            UnInitActiveMode(mode);
            PlayerModes.push_back(mode);
            InitActiveMode(previous);
            SuppressFullModeRemoval = false;
        } else {
            PlayerModes.push_back(mode);
        }
    }
    if (g_DebugPlayerModes) {
        CCharString text("PlayerModes");
        text += CCharString("AddModeEnd");
        text += " - transient mode: ";
        text += GFIntToCharString(mode);
        text += " : all modes are - ";
        for (SModeListNode* node = PlayerModes._M_node->Next; node != PlayerModes._M_node; node = node->Next)
            text += GFIntToCharString(node->Value) + ", ";
    }
}