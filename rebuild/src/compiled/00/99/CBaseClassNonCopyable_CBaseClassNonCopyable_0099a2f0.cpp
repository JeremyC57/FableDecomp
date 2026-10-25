// CBaseClassNonCopyable::CBaseClassNonCopyable() -- protected default constructor: install the CBaseClassNonCopyable vtable and return this
// Leaf: ??0CBaseClassNonCopyable@@IAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099A2F0, 9 bytes, thiscall: `mov eax,ecx ; mov [eax],0129A7C4h ; ret`.
// Identity: the stored vtable 0x0129A7C4 is CBaseClassNonCopyable's (rebuild/manifest/rtti-vtables.tsv; slot 0 =
//   0x0099A470); Ego_r.pdb's name set has `??0CBaseClassNonCopyable@@IAE@XZ` (protected) and the protected virtual
//   destructor `??1CBaseClassNonCopyable@@MAE@XZ`. 364 call sites: every non-copyable engine object's constructor
//   runs it first.
// Fix (sweep2 1g): the landed text modelled a `class CBase` with no `// Leaf:` line, so grading fell back to the only
//   function in the object; the model now names the real class, access and leaf.
class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                   // vtable slot 0 (scalar deleting dtor 0x0099A470)
};

CBaseClassNonCopyable::CBaseClassNonCopyable()
{
}
