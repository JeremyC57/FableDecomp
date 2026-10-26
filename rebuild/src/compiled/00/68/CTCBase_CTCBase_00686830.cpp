// CTCBase::~CTCBase() -- the component base destructor: this class's vtable back in place, then the CBaseClassNonCopyable base destructor (tail jump)
// Leaf: ??1CTCBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686830, 11 bytes: mov dword ptr [ecx],0x0125BE0C ; jmp 0x0099A300.
// Identity (catalog CTCBase::~CTCBase already; the landed source was a `struct C` stand-in emitting "??1CChunkedFileChunk@";
//   FINDINGS chunk 478 "0x686830 = ~CTCBase (not ~CChunkedFileChunk)"): vtable 0x0125BE0C is CTCBase (rtti-vtables.tsv; the
//   landed CTCBase::CTCBase(CThing&) 0x00686800 stores the same one), 0x0099A300 is ~CBaseClassNonCopyable (sweep2:
//   protected virtual, `MAE`), Ego_r.pdb ??1CTCBase@@UAE@XZ; component destructors tail-jump here after their members
//   (e.g. ~CTCCreatureItemLevitationBase 0x004DE6ED destroys its list<CThing*> and jumps to 0x00686830).
class CThing;

class CBaseClassNonCopyable {                          // vtable 0x0129A7C4
protected:
    CBaseClassNonCopyable();                           // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                  // 0x0099A300
};

class CTCBase : public CBaseClassNonCopyable {         // vtable 0x0125BE0C
public:
    virtual ~CTCBase();
protected:
    CThing* ParentThing;                               // +0x04
    bool    ToBeRemoved;                               // +0x08
    bool    IsInGlobalUpdateTCs;                       // +0x09
};

CTCBase::~CTCBase()
{
}
