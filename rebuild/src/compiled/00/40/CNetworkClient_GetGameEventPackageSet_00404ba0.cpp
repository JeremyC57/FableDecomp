// CNetworkClient::GetGameEventPackageSet(int arg) -- when the client is live, defer to the real query; otherwise answer true
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404BA0, 15 bytes. Callees: the delegate query (same arguments).
//
// A guarded forwarder: the byte at +8 says whether this object is active, and only then does the
// question go to the real implementation. An inactive object answers "yes" so callers that loop on
// "is the package set / is it done" fall straight through instead of waiting on something that will
// never happen. The CNetworkClient name is what the symbol tables attach to this address; the shape is
// generic and the PDB's CNetworkClient has its embedded server object at +8, so the attribution is
// uncertain.
//
// Matching: `cmp byte ptr [ecx+8],0; je` then a tail `jmp` to the delegate with ecx/edx untouched;
// the true path is `mov al,1; ret`.
struct CNetworkClient {
    char pad0[8];
    char flag8;      // object is active: ask the real implementation
};

// The real query, called with the same arguments.
extern bool __fastcall CNetworkClient_GetGameEventPackageSet_delegate(CNetworkClient* self, int arg);

bool __fastcall CNetworkClient_GetGameEventPackageSet(CNetworkClient* self, int arg)
{
    if (self->flag8)
        return CNetworkClient_GetGameEventPackageSet_delegate(self, arg);
    return true;
}
