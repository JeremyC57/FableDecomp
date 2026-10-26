// CCreatureAction_Read::Clone() const -- `return new CCreatureAction_Read(*this);` with the implicit copy constructor inline: the CCreatureActionBase copy, the own vtable store, then the TextTag member's out-of-line copy constructor; null when the allocation fails
// Leaf: ?Clone@CCreatureAction_Read@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E82C0, 65 bytes (+ int3 padding to 80), thiscall, plain `ret`, returns the copy.
// Identity (proven): CCreatureAction_Read::Clone() const (Ego_r.pdb ?Clone@CCreatureAction_Read@@UBEPAVCCreatureActionBase@@XZ); evidence: it is
//   slot 11 of the CCreatureAction_Read vtable 0x01270AC4 (RTTI .?AVCCreatureAction_Read@@), the same vtable it stores into the 0xAC-byte copy.
// Label: the manifest's "CCreatureAction_Read::Clone" is propagated across byte-identical bodies (family J017: six creature-action Clone
//   bodies with one out-of-line member copy; retail has no identical-code folding, so each address is one class's Clone).
// Callees: operator new(unsigned int) (import thunk 0x00BFEA1A), CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) 0x0062EBC0
//   (??0CCreatureActionBase@@QAE@ABV0@@Z, catalog-bound and in Ego_r.pdb), CWideString::CWideString(const CWideString&) 0x0099B720 (??0CWideString@@QAE@ABV0@@Z, catalog-bound among stand-ins, Ego_r.pdb spelling).
// Layout: retail CCreatureActionBase is 0xA8 bytes (12-byte STLport containers; Ego_r.pdb 0xB4), so TextTag (Ego_r.pdb's only own
//   member, CWideString) sits at +0xA8 and the class is 0xAC bytes. Local model: the generated engine headers for these
//   classes carry the Ego_r (Dinkumware) layout and a data-member vptr, which cannot give the retail offsets or a vtable store.

class CWideStringData;
class CWideString {                            // 4 bytes (Ego_r.pdb)
public:
    CWideString(const CWideString& other);      // 0x0099B720
    CWideStringData* PStringData;
};
class CCreatureActionBase {                         // 0xA8 bytes in retail (vptr first)
public:
    CCreatureActionBase(const CCreatureActionBase& other);   // 0x0062EBC0
    virtual ~CCreatureActionBase();
    unsigned char _body[0xA8 - 4];
};

class CCreatureAction_Read : public CCreatureActionBase {
public:
    virtual ~CCreatureAction_Read();
    virtual CCreatureActionBase* Clone() const;     // slot 11
    CWideString TextTag;   // +0xA8
};

CCreatureActionBase* CCreatureAction_Read::Clone() const
{
    return new CCreatureAction_Read(*this);
}