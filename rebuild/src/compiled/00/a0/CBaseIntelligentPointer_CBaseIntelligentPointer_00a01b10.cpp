// CBaseIntelligentPointer::CBaseIntelligentPointer() -- sets the vtable (0x0129C95C) and nulls the record pointer at +4
// Leaf: ??0CBaseIntelligentPointer@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01B10, oracle length 16 bytes.
// Identity: Ego_r.pdb + FableWin.pdb ??0CBaseIntelligentPointer@@QAE@XZ; the vtable stored is 0x0129C95C
//   (RTTI .?AVCBaseIntelligentPointer@@). Ego_r layout: vptr + CBaseObjectPointer* PData (+0x04).
class CBaseObject;
class CBaseObjectPointer {                             // Ego_r.pdb, 8 bytes
public:
    CBaseObject*  object;                              // +0x00 the observed object, 0 once it died
    unsigned long refcount;                            // +0x04 intelligent pointers sharing this record
};
class CBaseIntelligentPointer {                        // vtable 0x0129C95C
public:
    CBaseIntelligentPointer();                         // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
private:
    mutable CBaseObjectPointer* PData;                 // +0x04
};

CBaseIntelligentPointer::CBaseIntelligentPointer()
    : PData(0)
{
}
