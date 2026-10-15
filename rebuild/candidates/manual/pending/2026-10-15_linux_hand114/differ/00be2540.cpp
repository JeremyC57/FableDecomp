// CVO_OracleMarker::Init() -- (constructor twin: returns this, `mov eax,ecx`) zero the two leading words, the 16-byte record at +0x08 and the two flag bytes at +0x28/+0x29
// Grade: DIFFER(38v56) -- retail keeps a 0x10-byte frame temporary whose four zero stores survive AFTER the member
// copy (sub esp,10h ; xor eax,eax ; mov esi,eax ; lea edx,[ecx+8] ; four `mov [edx+n],esi` ; then [esp+4..10h]=eax);
// every source form here SROAs the temporary (no frame) or keeps it as immediate float stores with reloads.
// Retail: Fable.exe @ 0x00BE2540, 56 bytes. No callees, no references found (xref.py: none).
// Model: no engine header; Ego_r.pdb's CVO_OracleMarker (0x30 bytes) does not fit these offsets (label propagated).
// Tried: aggregate/brace local, member-wise local, (x,y,z,w) ctor temporary, implicit/user copy ctor, user operator=,
// memset local, long[4] array member, const-ref inline setter with a temporary / a named local, float fields (frame kept but immediate stores).
#include <string.h>
struct SQuad {
    long a, b, c, d;
    SQuad() { }
};
struct CVO_OracleMarker {
    long m_first;                  // +0x00
    long m_second;                 // +0x04
    SQuad m_quad;                  // +0x08
    unsigned char _pad18[0x10];    // +0x18
    bool m_flagA;                  // +0x28
    bool m_flagB;                  // +0x29
    CVO_OracleMarker* Init();
};
CVO_OracleMarker* CVO_OracleMarker::Init()
{
    SQuad q;
    memset(&q, 0, sizeof(q));
    m_quad = q;
    m_first = 0;
    m_second = 0;
    m_flagA = false;
    m_flagB = false;
    return this;
}
