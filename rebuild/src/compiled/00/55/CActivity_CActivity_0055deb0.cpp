// CActivity::CActivity() -- construct the three activity strings (name, description, unit) and clear the decimal-places count and the lower-is-better flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055DEB0, length 36 bytes. Callees: CCharString::CCharString() @ 0x0099E4B0 (x3).
// Label correction: the manifest calls this a CRegister constructor (a label propagated over four unrelated bodies; the
// PDB CRegister is {SymbolName, Name, bool Free}, 12 bytes, and would clear one byte, not two). The only Ego_r.pdb record
// with three CCharStrings at +0/+4/+8 and two byte members at +0x0c/+0x0d is CActivity (16 bytes): Name,
// Description, UnitOfMeasurement, uchar NumDecPlacesToShow, bool LowerIsBetter. No direct caller or address reference
// exists in the image (an unreferenced inline-constructor copy), so the attribution rests on the layout alone.
// Shape: three member constructor calls in declaration order, then both byte members cleared through al.
// Leaf: ??0CActivity
class CCharString { public:
    void* m_pStringData;
    CCharString();                                      // 0x0099E4B0
};

struct CActivity {
    CCharString   Name;                                 // +0x00
    CCharString   Description;                          // +0x04
    CCharString   UnitOfMeasurement;                    // +0x08
    unsigned char NumDecPlacesToShow;                   // +0x0c
    bool          LowerIsBetter;                        // +0x0d
    CActivity();
};

CActivity::CActivity()
    : NumDecPlacesToShow(0), LowerIsBetter(false)
{
}