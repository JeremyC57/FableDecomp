// CCountedPointer::SetStaticCast(const CCountedPointer<U>& other) -- share another counted pointer's object and reference block (static cast, zero adjustment): release the current block unless it is the same one, take the other's data and info, add a reference
// Leaf: ??$SetStaticCast@VCUnprovenSource_00abe910@@@?$CCountedPointer@VCUnprovenTarget_00abe910@@@@QAEXABV?$CCountedPointer@VCUnprovenSource_00abe910@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABE910, 70 bytes, thiscall `ret 4`, no result.
// Identity (probable, descriptive types): a CCountedPointer<T> member taking `const CCountedPointer<U>&` and returning
//   nothing is, in the PDB name sets, only the member template SetStaticCast<U> (74 instantiations, almost all
//   CCountedPointer<C3DMeshFile*Chunk / C3DAnimFile*Chunk>::SetStaticCast<CChunkedFileChunk>, the mesh/anim chunk readers
//   this address sits among). No call or pointer reaches 0x00ABE910 (the "abs" hits 0x00437E6C / 0x00496BB7 /
//   0x0061A784 are bytes inside other instructions), so T and U are unproven: CUnprovenTarget_00abe910 /
//   CUnprovenSource_00abe910 stand in (T derives from U at offset 0, so the static cast moves the pointer unchanged).
// Callee: operator delete @ 0x00BFE9BC (the reference block). The block's delete function is called through +4 with
//   the object (+8) in ecx (__fastcall).
// Layout: CCountedPointer<T> (8 bytes): Data +0, Info +4 (CCPPointerInfo: RefCount +0, DeleteFunc +4, Data +8).
// Shape: SetStaticCast forwards to the inline ShareData(static_cast<T*>(other.Data), other.Info) (the landed
//   CCountedPointer<CScriptThing>::ShareData 0x00A6E1D0 body): both source words are loaded first, the block re-read after
//   the decrement, the new block tested from the argument register.
class CCPPointerInfo {
public:
    unsigned long RefCount;                              // +0x00
    void (__fastcall* DeleteFunc)(void* data);           // +0x04
    void*         Data;                                  // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            if (Info) {
                if (--Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)
    {
        ShareData(static_cast<T*>(other.Data), other.Info);
    }
};

class CUnprovenSource_00abe910 { };
class CUnprovenTarget_00abe910 : public CUnprovenSource_00abe910 { };

template void CCountedPointer<CUnprovenTarget_00abe910>::SetStaticCast<CUnprovenSource_00abe910>(const CCountedPointer<CUnprovenSource_00abe910>& other);