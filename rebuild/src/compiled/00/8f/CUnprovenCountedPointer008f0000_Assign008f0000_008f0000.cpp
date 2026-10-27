// CUnprovenCountedPointer_008f0000::Assign_008f0000(const CUnprovenCountedPointer_008f0000&) -- share another holder's counted object, releasing the old one
// Leaf: ?Assign_008f0000@CUnprovenCountedPointer_008f0000@@QAEXABV1@@Z
// Retail: Fable.exe @ 0x008F0000, 52 bytes, thiscall `ret 4`, returns nothing (eax is left holding the new object, not this).
// Identity (descriptive): unreferenced, and one of 187 byte-identical unreferenced copies (0x0040FD80 .. 0x00B66BB0, mostly
// game-side TUs), none bound or landed. The pointee keeps its count at +4 and is released through virtual slot 1 with no
// argument, which is neither CIVCountedPointeeBase (Ego_r.pdb: one virtual, the dtor, slot 0 in rtti-vtables) nor
// CCountedPointer (separate 12-byte count block, see the landed 0x00BB64A0); PDB counted-pointer assignment operators all
// return a reference, so no PDB name fits. Type, method and pointee are descriptive.
// Callees: the pointee's virtual slot 1 (release on last reference).
// Layout: holder = one pointer (+0); pointee = vptr (+0), long count (+4). Local polymorphic model (header debt: no header).
// Shape: an identity test first; the old object is released (count decremented in place, slot 1 on zero) and the holder
// nulled; the source pointer is then re-read, stored, and its count bumped when non-null.
class CUnprovenCountee_008f0000 {
public:
    virtual void Slot0_008f0000();
    virtual void Slot1_008f0000();                          // released on the last reference
    long RefCount;                                          // +0x04
};

class CUnprovenCountedPointer_008f0000 {
public:
    void Assign_008f0000(const CUnprovenCountedPointer_008f0000& other);

    CUnprovenCountee_008f0000* Object;                      // +0x00
};

void CUnprovenCountedPointer_008f0000::Assign_008f0000(const CUnprovenCountedPointer_008f0000& other)
{
    CUnprovenCountee_008f0000* current = Object;
    if (current != other.Object) {
        if (current) {
            if (--current->RefCount == 0)
                current->Slot1_008f0000();
            Object = 0;
        }
        Object = other.Object;
        if (Object)
            ++Object->RefCount;
    }
}