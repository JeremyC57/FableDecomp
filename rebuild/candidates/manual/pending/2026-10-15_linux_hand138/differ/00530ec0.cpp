// CDefOwner00530ec0::GetDef() const -- look up this object's definition (global def index at +0x130) through the front end's definition-manager override, or the game definition manager when there is none, and return it as a counted def pointer
// Grade: DIFFER(99v98) -- only the release differs: retail keeps the def object in ecx (`mov ecx,[esp+4] ; ... ; mov edx,[ecx+4] ; inc edx ; mov eax,edx ; dec eax ; mov [ecx+4],edx ; mov [ecx+4],eax`), ours loads it into eax, copies it to ecx and steps the count through `add eax,4 ; inc edx ; mov [eax],edx ; dec edx ; mov [eax],edx`. Tried: CDefPointer local returned by value (copy + destructor: reloads the slot), ternary manager (Get() hoisted above the pushes), if/else on a manager local (manager in eax, pushes not hoisted), frontEnd local + hand release through an inline guard (this file: manager part byte-exact). Next: build the result from the slot directly (`CDefPointer(const SDefSlot&)`) or let the guard read the slot itself.
// Leaf: ?GetDef@CDefOwner00530ec0
// Retail: Fable.exe (TLC, VC7.1) @ 0x00530EC0, 98 bytes (+ int3 padding).
// Label correction: the manifest's "CEventKilledCreature::CEventKilledCreature" is wrong -- the body is a `__thiscall` with a
//   hidden return slot (`ret 4`, eax = the slot) that fills a def pointer; the owner class (def index at +0x130) is unknown.
// Callees: CFrontEndManager::GetInstance() @ 0x0041E5F2 (definition-manager override at +0x64), CGameDefinitionManager::Get()
//   @ 0x0044C6B0, CDefinitionManager::GetDef<T>(long, CDefPointer<T>*) @ 0x0042AEDA, the def's virtual slot 1 (release).
// Shape: if/else on `frontEnd->m_pDefinitionManager` with the two GetDef calls tail-merged (the `&def` push hoisted above the
//   test, Get() called after the index push); the returned pointer adds a reference and the local reference is dropped with
//   one shared null test.
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                      // slot 1
    long RefCount;                               // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(T* object) : Object(object) { if (object) ++object->RefCount; }
};
template <class T> struct SDefSlot {             // the out-slot GetDef fills (reference taken for the caller)
    T* Object;
    SDefSlot() : Object(0) {}
};
template <class T> struct CDefRelease {
    T* m_p;
    CDefRelease(T* p) : m_p(p) {}
    ~CDefRelease() { if (m_p) { if (--m_p->RefCount == 0) m_p->Destroy(); } }
};
struct CDefinitionManager {
    template <class T> bool GetDef(long globalIndex, SDefSlot<T>* out);   // 0x0042AEDA
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
struct CFrontEndManager {
    unsigned char _pad[0x64];
    CDefinitionManager* m_pDefinitionManager;    // +0x64
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
struct CSomeDef : CDefBase {};
struct CDefOwner00530ec0 {
    unsigned char _pad[0x130];
    long m_defIndex;                             // +0x130
    CDefPointer<CSomeDef> GetDef() const;
};
CDefPointer<CSomeDef> CDefOwner00530ec0::GetDef() const
{
    SDefSlot<CSomeDef> def;
    CFrontEndManager* frontEnd = CFrontEndManager::GetInstance();
    if (frontEnd->m_pDefinitionManager)
        frontEnd->m_pDefinitionManager->GetDef(m_defIndex, &def);
    else
        CGameDefinitionManager::Get().GetDef(m_defIndex, &def);
    CSomeDef* object = def.Object;
    CDefRelease<CSomeDef> release(object);
    return CDefPointer<CSomeDef>(object);
}
