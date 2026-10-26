// CPersistContext_TransferableVector<CHeroMarriageInfo>::TransferOut(CPersistContext* context, CCharString* out, long index) -- writes marriage record `index` as text through a default CPersistTraits<CHeroMarriageInfo>
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057F754, 59 bytes, `leave ; ret 0Ch`.
// Callee: 0x0057F78F CPersistTraits<CHeroMarriageInfo>::TransferOut(CPersistContext*, CCharString*, CHeroMarriageInfo*)
// (thiscall, `ret 0Ch`; manifest label `Inventory_FormatItemTransferDetails` is a guess; its read twin 0x00580194 is
// labelled CPersistTraits<CHeroMarriageInfo>::TransferIn).
// Persist WRITE hook (vtable 0x0124F350 slot 2; slot 3 is TransferIn 0x00580159, slot 0 the count 0x0057F745) of
// CPersistContext_TransferableVector<CHeroMarriageInfo> { vptr; vector<CHeroMarriageInfo>* TransVector; }. The
// manifest's `<long>` is propagated: the 24-byte element stride (`lea eax,[eax+eax*2] ; lea eax,[ecx+eax*8]`) and the
// traits frame name CHeroMarriageInfo (Ego_r.pdb 0x18 bytes: WifeUID, MaritalHomeUID (u64), State, bool GayMarriage).
// The traits constructor zeroes the default record member by member through one register: five dwords then the
// GayMarriage byte (`mov byte ptr [ebp-4],al`; the 3 pad bytes are untouched). No destructor runs.
// EBP frame + size TU (optimize("y", off), optimize("s", on)).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CHeroMarriageInfo.h"

class CPersistContext;
class CStringParser;
class CCharString;

template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return *(m_pFirst + index); }
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CHeroMarriageInfo> {
    CHeroMarriageInfo DefaultVal;
    CPersistTraits()
    {
        DefaultVal.WifeUID = 0;
        DefaultVal.MaritalHomeUID = 0;
        *(long*)DefaultVal.State = 0;                   // EMarriageState (opaque 4 bytes in the header)
        DefaultVal.GayMarriage = false;
    }
    void TransferOut(CPersistContext* context, CCharString* out, CHeroMarriageInfo* value);       // 0x0057F78F
    void TransferIn(CPersistContext* context, CStringParser* parser, CHeroMarriageInfo* value);   // 0x00580194
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<CHeroMarriageInfo>* TransVector;     // +0x4
};

void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    CPersistTraits<CHeroMarriageInfo> traits;
    traits.TransferOut(context, out, &(*TransVector)[index]);
}