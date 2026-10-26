// CCreatureAction_SitDown::Clone() const -- `return new CCreatureAction_SitDown(*this);` with the implicit copy constructor inline: the CCreatureActionBase copy, the own vtable store, then the PSitMode member's out-of-line copy constructor; null when the allocation fails
// Leaf: ?Clone@CCreatureAction_SitDown@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AE2A0, 65 bytes (+ int3 padding to 80), thiscall, plain `ret`, returns the copy.
// Identity (proven): CCreatureAction_SitDown::Clone() const (Ego_r.pdb ?Clone@CCreatureAction_SitDown@@UBEPAVCCreatureActionBase@@XZ); evidence: it is
//   slot 11 of the CCreatureAction_SitDown vtable 0x012781FC (RTTI .?AVCCreatureAction_SitDown@@), the same vtable it stores into the 0xB0-byte copy.
// Label: the manifest's "CCreatureAction_SitDown::Clone" is propagated across byte-identical bodies (family J017: six creature-action Clone
//   bodies with one out-of-line member copy; retail has no identical-code folding, so each address is one class's Clone).
// Callees: operator new(unsigned int) (import thunk 0x00BFEA1A), CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) 0x0062EBC0
//   (??0CCreatureActionBase@@QAE@ABV0@@Z, catalog-bound and in Ego_r.pdb), CCountedPointer<NCreatureMode::CModeSitBase>::CCountedPointer(const CCountedPointer&) 0x008AA6A0 (??0?$CCountedPointer@VCModeSitBase@NCreatureMode@@@@QAE@ABV0@@Z, unbound in the catalog; FableWin.pdb spelling).
// Layout: retail CCreatureActionBase is 0xA8 bytes (12-byte STLport containers; Ego_r.pdb 0xB4), so PSitMode (Ego_r.pdb's only own
//   member, CCountedPointer<NCreatureMode::CModeSitBase>) sits at +0xA8 and the class is 0xB0 bytes. Local model: the generated engine headers for these
//   classes carry the Ego_r (Dinkumware) layout and a data-member vptr, which cannot give the retail offsets or a vtable store.
namespace NCreatureMode { class CModeSitBase; }
class CCPPointerInfo;
template <class T> class CCountedPointer {      // 8 bytes (Ego_r.pdb)
public:
    CCountedPointer(const CCountedPointer<T>& other);   // out of line
    T*              Data;
    CCPPointerInfo* Info;
};
class CCreatureActionBase {                         // 0xA8 bytes in retail (vptr first)
public:
    CCreatureActionBase(const CCreatureActionBase& other);   // 0x0062EBC0
    virtual ~CCreatureActionBase();
    unsigned char _body[0xA8 - 4];
};

class CCreatureAction_SitDown : public CCreatureActionBase {
public:
    virtual ~CCreatureAction_SitDown();
    virtual CCreatureActionBase* Clone() const;     // slot 11
    CCountedPointer<NCreatureMode::CModeSitBase> PSitMode;   // +0xA8
};

CCreatureActionBase* CCreatureAction_SitDown::Clone() const
{
    return new CCreatureAction_SitDown(*this);
}