// CPersistContext::Transfer(const char* name, COpinionTransientOffset& value) -- transfer a COpinionTransientOffset with an all-zero default
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006861C0, length 53 bytes (+ int3 padding), `ret 8`.
// Callees: CPersistContext::Transfer<COpinionTransientOffset>(const char*, COpinionTransientOffset&, const COpinionTransientOffset&)
//   @ 0x00685F80 (landed; its catalog label "Transfer<signed char>" is propagated). The catalog label of this row,
//   "vector<pair<long,CCountedPointer<CContainmentVolume>>>::_Ucopy", is propagated too: the body is the two-argument
//   persist overload (no callers in the image: COpinionReactionManager::TransferOpinionData calls the three-argument one).
// Shape: a 0x18-byte frame temporary built by the zeroing default ctor (`xor eax,eax` then six dword stores; POD
//   value-initialisation `T()` schedules the value load after the stores instead), its address
//   pushed, then the value and name slots; `this` stays in ecx; `add esp,18h ; ret 8`.
// Layout: COpinionTransientOffset from the PDB (Ego_r.pdb, 0x18 bytes); CPersistContext from the PDB (0x2c bytes). No engine
//   header exists for either, so local models are used.
struct CDefinitionManager;
struct CSymbolMap;
struct CStringParser;
struct CCharString;
struct CMemoryDataInputStream;
struct CDataOutputStream;

enum EOpinion { OPINION_NULL = 0 };

// COpinionTransientOffset (0x18 bytes, PDB layout Ego_r.pdb)
struct COpinionTransientOffset {
    EOpinion Opinion;                 // +0x00
    float    OffsetPerFrameRunIn;     // +0x04
    long     FramesToRunIn;           // +0x08
    long     FramesOfCappedPeak;      // +0x0c
    float    OffsetPerFrameRunOut;    // +0x10
    long     FramesToRunOut;          // +0x14

    COpinionTransientOffset()
        : Opinion(OPINION_NULL), OffsetPerFrameRunIn(0.0f), FramesToRunIn(0), FramesOfCappedPeak(0),
          OffsetPerFrameRunOut(0.0f), FramesToRunOut(0) {}
};

// CPersistContext (0x2c bytes, PDB layout Ego_r.pdb; no engine header)
struct CPersistContext {
    CDefinitionManager*     PDefinitionManager;           // +0x00
    CSymbolMap*             PSymbolMap;                   // +0x04
    CStringParser*          PParser;                      // +0x08
    void (__cdecl*          ErrorCallback)(CCharString*); // +0x0c
    CCharString*            PStringToClean;               // +0x10
    bool                    UseDefaults;                  // +0x14
    bool                    UseCRCCheck;                  // +0x15
    bool                    SingleTransfer;               // +0x16
    bool                    Finished;                     // +0x17
    long                    Mode;                         // +0x18
    bool                    Binary;                       // +0x1c
    CCharString*            PStringToSaveTo;              // +0x20
    CMemoryDataInputStream* PLoadStream;                  // +0x24
    CDataOutputStream*      PSaveStream;                  // +0x28

    void Transfer(const char* name, COpinionTransientOffset& value, const COpinionTransientOffset& defaultValue);   // 0x00685F80
    void Transfer(const char* name, COpinionTransientOffset& value);
};

void CPersistContext::Transfer(const char* name, COpinionTransientOffset& value)
{
    Transfer(name, value, COpinionTransientOffset());
}