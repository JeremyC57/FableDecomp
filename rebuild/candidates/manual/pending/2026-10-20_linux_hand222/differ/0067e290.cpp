// Grade: DIFFER(276v273) -- register rotation from the second Transfer on (retail eax/edx for the enum save/load arms and eax/ecx for the third bool, the lane edx/eax and ecx/edx), the sort call operand loads, and the fix-up loop: retail hoists a DWORD load of GraphicType above the loop entry (`jmp` over aligned padding), the lane loads the byte inside the loop. Fixed by this form: the vector transfer as `switch { case 2: In; default: Out; }` (`sub eax,2 ; je`). Tried: if/else vector, enum vs long traits, non-static TransferBinaryOut.
// CDegradableDef::Transfer(CPersistContext& context) -- persist Degradable, GraphicType, InitiallyBlocksNavigation and the Degradations vector; after a mode-0 transfer the degradations are sorted and each one's engine graphic takes the def's graphic type
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067E290, length 271 bytes (virtual, `ret 4`).
// Callees: CPersistContext::CheckCRC @ 0x00404500 (x4, the empty key "" @ 0x0122D70E), CPersistTraits<bool>::TransferBinaryIn
// @ 0x00403EB0, CDataOutputStream::WriteBool @ 0x00993E30, the graphic-type traits load @ 0x00632470 (manifest
// "CPersistTraits<float>::TransferBinaryIn": an ICF-shared 4-byte body), CDataOutputStream::WriteLong @ 0x00993EB0,
// GFSerialiseVectorBinaryIn/Out<CDegradableInfo> @ 0x0067F430 / 0x0067E720 (`__fastcall` stream, vector; the In label names
// CBrainUpdateZone by propagation), the SGI sort<CDegradableInfo*, comparator> @ 0x0067F3D0 (`/Gr`: first in ecx, last in edx,
// the empty comparator by value on the stack).
// Layout: PDB CDegradableDef (Degradable +0x25, GraphicType +0x28, InitiallyBlocksNavigation +0x2c, Degradations +0x30; the
// retail vector is 12 bytes) and CDegradableInfo (0x1c bytes; EngineGraphic +4 whose graphic-type byte sits at +0x11).
// Shape: the landed CActionUseDef::Transfer 0x006773F0 Transfer<T> template for the scalars; the vector overload loads on mode 2
// and saves otherwise; the comparator temporary lands in the dead `context` argument slot; the fix-up loop keeps end() in edi.
struct CMemoryDataInputStream;

struct CDataOutputStream {
    void WriteLong(long value);                         // 0x00993EB0
    void WriteBool(bool value);                         // 0x00993E30
};

enum EEngineGraphicType { ENGINE_GRAPHIC_TYPE_NONE = 0 };

#pragma pack(push, 2)
struct CEngineGraphic {                                 // 14 bytes (PDB)
    unsigned char _pad00[0xd];
    unsigned char GraphicType;                          // +0x0d
    void SetGraphicType(EEngineGraphicType type) { GraphicType = (unsigned char)type; }
};
#pragma pack(pop)

struct CDegradableInfo {                                // 0x1c bytes
    float          HealthPercentage;                    // +0x00
    CEngineGraphic EngineGraphic;                       // +0x04
    long           SmashParticleEmitter;                // +0x14
    bool           BlocksNavigation;                    // +0x18
};

struct CDegradableInfoVector {                          // STLport vector<CDegradableInfo> (12 bytes)
    CDegradableInfo* _M_start;
    CDegradableInfo* _M_finish;
    CDegradableInfo* _M_end_of_storage;
    CDegradableInfo* begin() { return _M_start; }
    CDegradableInfo* end() { return _M_finish; }
};

void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream* stream, CDegradableInfoVector* vector);   // 0x0067F430
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream* stream, CDegradableInfoVector* vector);       // 0x0067E720

struct CDegradableInfoLess {
    CDegradableInfoLess() { }
};
void __fastcall sort(CDegradableInfo* first, CDegradableInfo* last, CDegradableInfoLess comp);   // 0x0067F3D0

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<bool> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, bool* value);   // 0x00403EB0
    static void TransferBinaryOut(CDataOutputStream* stream, const bool& value) { stream->WriteBool(value); }
};
template<> struct CPersistTraits<EEngineGraphicType> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, EEngineGraphicType* value);   // 0x00632470
    static void TransferBinaryOut(CDataOutputStream* stream, const EEngineGraphicType& value) { stream->WriteLong(value); }
};

struct CPersistContext {
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    void CheckCRC(const char* name);                    // 0x00404500
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value);
            break;
        }
    }
    void Transfer(const char* name, CDegradableInfoVector& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            GFSerialiseVectorBinaryIn(m_pLoadStream, &value);
            break;
        default:
            GFSerialiseVectorBinaryOut(m_pSaveStream, &value);
            break;
        }
    }
};

struct CDegradableDef {
    unsigned char         _base[0x25];                  // CDefClassBase (vptr, def manager, index, flags)
    bool                  Degradable;                   // +0x25
    unsigned char         _pad26[2];
    EEngineGraphicType    GraphicType;                  // +0x28
    bool                  InitiallyBlocksNavigation;    // +0x2c
    unsigned char         _pad2d[3];
    CDegradableInfoVector Degradations;                 // +0x30
    void Transfer(CPersistContext& context);
};

void CDegradableDef::Transfer(CPersistContext& context)
{
    context.Transfer("", Degradable);
    context.Transfer("", GraphicType);
    context.Transfer("", InitiallyBlocksNavigation);
    context.Transfer("", Degradations);
    if (context.m_mode == 0) {
        sort(Degradations.begin(), Degradations.end(), CDegradableInfoLess());
        CDegradableInfo* last = Degradations.end();
        for (CDegradableInfo* it = Degradations.begin(); it != last; ++it)
            it->EngineGraphic.SetGraphicType(GraphicType);
    }
}
