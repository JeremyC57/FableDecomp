// CBaseObject::CBaseObject(const CBaseObject&) -- copy constructor: a copy observes nobody's pointer
// Leaf: ??0CBaseObject@@IAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A390, oracle length 16 bytes. Callees: none.
//
// Retail bytes:  mov eax,ecx ; mov dword ptr [eax],offset CBaseObject::`vftable' ; mov dword ptr [eax+4],0 ; ret 4
// Identity: the vtable stored is 0x0129A7D8 = CBaseObject (rtti-vtables.tsv); Ego_r.pdb (0x00B01FE0) and FableWin.pdb
// name the protected copy constructor ??0CBaseObject@@IAE@ABV0@@Z, right after the default one (0x0099A380 here).
// The IntelligentPointer record (+0x04) is NOT shared with the copy, it starts out detached; the argument is unused.
// The protected virtual destructor is only declared, so no `??_G` is emitted next to the constructor.
class CBaseObjectPointer;

class CBaseObject {                                    // vtable 0x0129A7D8
protected:
    CBaseObject(const CBaseObject& other);             // 0x0099A390
    virtual ~CBaseObject();                            // 0x0099A430
public:
    CBaseObjectPointer* IntelligentPointer;            // +0x04
};

CBaseObject::CBaseObject(const CBaseObject& other)
    : IntelligentPointer(0)
{
}
