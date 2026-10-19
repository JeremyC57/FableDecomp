// Grade: DIFFER(190v190) -- one register: the vptr for the FindFirstChildOfTypeNonRecursive call (slot 97) is loaded into edx where retail reuses the dead eax
// NUISystem::CMenuEntry::MakeUnavailable() -- grey out an available menu entry: run the base behaviour, make its text child unavailable and send the PC arrow child state request 1
// Leaf: MakeUnavailable@CMenuEntry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053E0C0, length 190 bytes (`ret`, CMenuEntry vtable 0x0124662C slot 92).
// Callees: the base (CComponent-level) MakeUnavailable @ 0x00530A40 (qualified, non-virtual), CComponent virtual slots 4 FindChildForward
// (0x00530830), 8 FindChildNonRecursive (0x005308F0), 9 FindFirstChildOfType (0x00530460), 97 FindFirstChildOfTypeNonRecursive (0x00530610),
// 92 MakeUnavailable (on the text child), 62 (0x00531E90, takes the 8-byte request by value); CCharString(const char*, long) @ 0x0099EBF0
// (by-value names built in the argument area), malloc @ 0x00BFEA0E (the request's list sentinel), the request's copy ctor @ 0x0042CD84
// and its list's dtor @ 0x0042ABCA.
// Layout: the Available flag at retail +0x198. Slot names from vtable 0x0124662C (manifest labels at those addresses).
// Shape: the request is a local {Type = 1, an empty one-pointer list}; its by-value copy reuses the un-popped malloc argument slot plus a
// `push ecx` word as the 8-byte argument area; the list's inline ctor self-links a 12-byte sentinel.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30 (declared: by-value arguments are built in place)
    ~CCharString();                                     // 0x0099EAE0 (run by the callee)
};
struct CStateArgumentList {                     // one-pointer list (12-byte sentinel)
    struct node { node* next; node* prev; long value; };
    node* head;
    CStateArgumentList() { node* n = (node*)malloc(sizeof(node)); n->next = n; n->prev = n; head = n; }
    ~CStateArgumentList();                      // 0x0042ABCA
};
struct CStateRequest {                          // 8 bytes, passed by value
    unsigned long      Type;
    CStateArgumentList Arguments;
    CStateRequest(unsigned long type) { Type = type; }
    CStateRequest(const CStateRequest& other);  // 0x0042CD84
};
namespace NUISystem {
struct CComponent {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual CComponent* FindChildForward(CCharString name);           // slot 4 (+0x10)
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual CComponent* FindChildNonRecursive(CCharString name);      // slot 8 (+0x20)
    virtual CComponent* FindFirstChildOfType(long type);              // slot 9 (+0x24)
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void        RequestState(CStateRequest request);          // slot 62 (+0xf8)
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void        MakeUnavailable();                            // slot 92 (+0x170)
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual CComponent* FindFirstChildOfTypeNonRecursive(long type);  // slot 97 (+0x184)
};
struct CDynamicListEntry : CComponent {
    unsigned char _body[0x198 - 4];
    void MakeUnavailable();                     // 0x00530A40
};
struct CMenuEntry : CDynamicListEntry {
    bool Available;                             // +0x198
    void MakeUnavailable();
};

void CMenuEntry::MakeUnavailable()
{
    if (!Available)
        return;
    CDynamicListEntry::MakeUnavailable();
    CComponent* text = FindFirstChildOfType(6);
    if (!text)
        text = FindChildForward(CCharString("MENU_ENTRY_TEXT", -1));
    if (!text)
        text = FindFirstChildOfTypeNonRecursive(6);
    if (text)
        text->MakeUnavailable();
    CComponent* arrow = FindChildNonRecursive(CCharString("PC_MENU_ENTRY_ARROW", -1));
    if (arrow) {
        CStateRequest request(1);
        arrow->RequestState(request);
    }
    Available = false;
}
}
