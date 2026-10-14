// CThingCreature::EndCutFade() -- forwards to the manager at +0x1a8 (helper @ 0x006D5AB0) and raises property change 0x104
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082FE00, length 22 bytes.
// Callees: forwarder @ 0x006D5AB0 (`mov ecx,[ecx+1A8h] ; jmp CManager::GetEmbeddedEventNames`-shaped tail), CBaseObject::
// OnPropertyChanged @ 0x004C9840 (one immediate argument 0x104).
// Class attribution: manifest label "CGameScriptInterface::EndCutFade" is propagated; the +0x1a8 manager pointer and the
// CBaseObject callee point at a thing class -- provisional. Shape: two thiscalls on `this` kept in esi.
struct CThingCreature {
    void NotifyManager();                           // 0x006D5AB0
    void OnPropertyChanged(long property);          // 0x004C9840
    void EndCutFade();
};

void CThingCreature::EndCutFade()
{
    NotifyManager();
    OnPropertyChanged(0x104);
}
