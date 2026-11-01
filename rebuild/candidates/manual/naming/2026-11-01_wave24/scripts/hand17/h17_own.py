# h17_own.py: naming wave 17, family own -- own-row respells of c675 names:
#  0x00772110 CTCShop::GetNumShoppers() const (?GetNumShoppers@CTCShop@@QBEJXZ; called on the CTCShop component, TC 0x53, in
#    FindASimplyDelightfulShop 0x00946F90): Shoppers is the STLport vector<CIntelligentPointer<CThingCreatureBase> > at +0x18
#    (8-byte elements); was CShaderPreParser::GetConstantCount (a propagated shape label).
#  0x0049CF80 CRightHandedSet's copy constructor (??0CRightHandedSet@@QAE@ABV0@@Z: the by-value copy in GetRHSetFromOrientation
#    0x005D8EF0 and ~30 landed callers bind it): Up / Forward C3DVector memberwise; was CNearestBuildingWithProperty's.
# Name-only: the recheck proves the bytes.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import _rw, _w
p, rel, t = _rw('00772110', 'src')
if 'CShaderPreParser' in t:
    k = t.index('struct SElement')
    hdr = t[:k].replace('// CShaderPreParser::GetConstantCount() -- the element count of the 8-byte-element range at +0x18/+0x1c',
                        '// CTCShop::GetNumShoppers() const -- the number of shoppers: Shoppers.size(), the 8-byte CIntelligentPointer elements at +0x18/+0x1c')
    hdr += ('// Leaf: ?GetNumShoppers@CTCShop@@QBEJXZ\n'
            '// Identity (proven, chunk c675): Ego_r.pdb CTCShop::GetNumShoppers() const (long); FindASimplyDelightfulShop 0x00946F90 calls it\n'
            '//   on the CTCShop component (TC 0x53). Shoppers = Ego_r vector<CIntelligentPointer<CThingCreatureBase> > (STLport 12 bytes at +0x18).\n'
            '// Naming wave 17: landed as CShaderPreParser::GetConstantCount (shape label); respelled (name-only: the bytes are unchanged).\n')
    body = ('class CThingCreatureBase;\n'
            'template <class T> class CIntelligentPointer {         // 8 bytes: vptr + PData\n'
            'public:\n'
            '    virtual ~CIntelligentPointer();\n'
            '    T* PData;\n'
            '};\n\n'
            'class CTCShop { public:\n'
            '    unsigned char _pad[0x18];\n'
            '    CIntelligentPointer<CThingCreatureBase>* ShoppersStart;     // +0x18 Shoppers (STLport vector _M_start)\n'
            '    CIntelligentPointer<CThingCreatureBase>* ShoppersFinish;    // +0x1c Shoppers._M_finish\n'
            '    long GetNumShoppers() const;\n'
            '};\n\n'
            'long CTCShop::GetNumShoppers() const\n'
            '{\n'
            '    return ShoppersFinish - ShoppersStart;\n'
            '}')
    _w(p, hdr + body); print('ok 00772110 src')
p, rel, t = _rw('0049cf80', 'src')
if 'CNearestBuildingWithProperty' in t:
    _w(p, '''// CRightHandedSet::CRightHandedSet(const CRightHandedSet& other) -- copy constructor: Up and Forward memberwise (six dwords)
// Leaf: ??0CRightHandedSet@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049CF80, 24-byte memberwise copy.
// Identity (proven, chunk c675): the by-value CRightHandedSet copy in CTCInventoryItem::GetRHSetFromOrientation 0x005D8EF0 (and the
//   landed callers that bind ??0CRightHandedSet@@QAE@ABV0@@Z) reaches this address; Ego_r.pdb CRightHandedSet {C3DVector Up +0,
//   Forward +0xC}, 24 bytes.
// Naming wave 17: landed as CNearestBuildingWithProperty's copy constructor (six int members m0..m5); respelled on the PDB class
//   (name-only: the bytes are unchanged).
class C3DVector {                                       // Ego_r.pdb {X, Y, Z}, 12 bytes
public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class CRightHandedSet {
public:
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0C
    CRightHandedSet(const CRightHandedSet& other);
};
CRightHandedSet::CRightHandedSet(const CRightHandedSet& other)
    : Up(other.Up), Forward(other.Forward)
{
}
'''); print('ok 0049cf80 src')
