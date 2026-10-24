// CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ReadFromFile(CAFile&, CASuspendableProcess*) -- read the level record; a record of an older version is read into a scratch record, upgraded to the current layout and copied over this one
// Grade: DIFFER(ours v retail) -- 164v175: retail keeps a 4-byte frame word (`push ecx`) that receives the zero-extended version (`movzx eax,al ; mov [esp+14h],eax`) on the old-format path and is never read, and compares the version through al (`mov al,[v] ; cmp al,4`); every other instruction matches
// Leaf: ReadFromFile
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051A240, 175 bytes (+1 int3 padding), `__thiscall`, `ret 8`.
// Callees: CAFile::ReadVar<int> @ 0x00516D90, CAFile::ReadVar<unsigned char> @ 0x00516E10, CAFile virtual slots 7 (+0x1C,
//   position), 5 (+0x14, seek) and 3 (+0x0C, Read), operator new[] @ 0x00BFEB22, the record's in-place upgrade from the
//   old format @ 0x00519510 (it converts when the version byte at +4 is not 4), operator delete[] @ 0x00BFEB1C.
// Identity: FableWin.pdb ?ReadFromFile@?$CReplaceSerialise@VCFileFormatLevel@@VCOldFileFormatLevel3@@@@QAEXAAVCAFile@@PAVCASuspendableProcess@@@Z;
//   the propagated label names the EditMapBrushHeader instantiation. The scratch block is sizeof(CFileFormatLevel) = 25
//   bytes (Ego_r.pdb: CReplaceSerialise base {ByteLength, SerialiseVersionNumber} + five long offsets), the current
//   version is 4, and the callers are the level loaders (CWorldMap::GetLevelDimensions 0x00506BF3 ..).
// Shape: length and version are homed in the dead argument slots; the current-version path stores ByteLength and reads
//   `length - 4` bytes from +4. The old path reads into a `new T[1]`, sets its ByteLength, upgrades it, copies the 25
//   bytes over *this (`rep movsd` x6 + `movsb`) and frees it with delete[].
class CASuspendableProcess;
void* __cdecl operator new[](unsigned int size);         // 0x00BFEB22 (the lane <new> lacks it)
void __cdecl operator delete[](void* block);              // 0x00BFEB1C

class CAFile {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);   // slot 3 (+0x0C)
    virtual void v04();
    virtual void SetPosition(long position);                                    // slot 5 (+0x14)
    virtual void v06();
    virtual long GetPosition();                                                 // slot 7 (+0x1C)
    void ReadVar(int* value, CASuspendableProcess* process);                    // 0x00516D90 ReadVar<int>
    void ReadVar(unsigned char* value, CASuspendableProcess* process);          // 0x00516E10 ReadVar<unsigned char>
};

#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                                                   // +0x00
    unsigned char SerialiseVersionNumber;                                       // +0x04
    void ReadFromFile(CAFile& file, CASuspendableProcess* process);
    void UpgradeFromOldVersion();                                               // 0x00519510
};

class COldFileFormatLevel3;
class CFileFormatLevel : public CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3> {   // 25 bytes
public:
    enum { VERSION = 4 };
    long MapDataOffset;                                                         // +0x05
    long ThingDataOffset;                                                       // +0x09
    long BrushDataOffset;                                                       // +0x0D
    long NavigationDataOffset;                                                  // +0x11
    long NavigationSectionsOffset;                                              // +0x15
};
#pragma pack(pop)

template <class T, class TOld> void CReplaceSerialise<T, TOld>::ReadFromFile(CAFile& file, CASuspendableProcess* process)
{
    int length;
    file.ReadVar(&length, process);
    long position = file.GetPosition();
    unsigned char version;
    file.ReadVar(&version, process);
    file.SetPosition(position);
    if (version == T::VERSION) {
        ByteLength = length;
        file.Read(&SerialiseVersionNumber, length - 4, process);
        return;
    }
    T* old = new T[1];
    file.Read(&old->SerialiseVersionNumber, length - 4, process);
    old->ByteLength = length;
    old->UpgradeFromOldVersion();
    *static_cast<T*>(this) = *old;
    delete[] old;
}

template void CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3>::ReadFromFile(CAFile& file, CASuspendableProcess* process);
