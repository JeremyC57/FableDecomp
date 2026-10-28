// CPersistContext::Transfer<CRGBColour>(CRGBColour&) -- one-argument transfer helper (member template; see the landed 0x00431061 Transfer<float>):
//   CheckCRC("") (0x00404500, the empty key 0x0122D70E), then the binary load (mode 2) / save (mode 3) switch.
//   namepass: one name for 0x004595A3 -- its callers call CPersistContext::Transfer<T>; the catalog label
//   "CEngineLightingManager::UpdateShadowScene" / Transfer*_xxx was propagated or descriptive.
// Leaf: ??$Transfer@VCRGBColour@@@CPersistContext@@QAEXAAVCRGBColour@@@Z
#pragma optimize("s", on)
// CPersistContext::TransferColour_004595a3(CRGBColour*) -- CheckCRC(""), then in mode 2 (load) read a colour from the load stream into the value, in mode 3 (save) write the value to the save stream
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004595A3, length 64 bytes.
// Callees: CPersistContext::CheckCRC @ 0x00404500 (the empty key @ 0x0122D70E), CDataOutputStream::WriteColour @ 0x00993F30 (on the save stream +0x28, the value by
// reference), CDataInputStream::ReadColour @ 0x009945E0 (on the load stream +0x24, through a 4-byte local, the colour returned by reference and copied as one dword).
// Shape: the mode at +0x18 is switched with the size-optimised `dec ; dec ; je ; dec ; jne` ladder (cases 2 and 3, nothing for the rest); a frame pointer for the
// local. The chunk label "CPersistContext::UpdateShadowScene" is propagated from an unrelated body.
class CRGBColour { public: unsigned long m_argb; };
class CDataOutputStream { public: void WriteColour(const CRGBColour& colour); };          // 0x00993F30
class CDataInputStream { public: CRGBColour& ReadColour(CRGBColour& colour); };        // 0x009945E0

class CPersistContext { public:
    unsigned char     _pad0[0x18];
    long              m_mode;                      // +0x18 (2 = load, 3 = save)
    unsigned char     _pad1[8];
    CDataInputStream* m_pLoadStream;               // +0x24
    CDataOutputStream* m_pSaveStream;              // +0x28
    protected: void CheckCRC(const char* name); public:               // 0x00404500
    template <class T> void Transfer(T& value);
};

template <> void CPersistContext::Transfer<CRGBColour>(CRGBColour& value_)
{
    CRGBColour* value = &value_;
    CheckCRC("");
    switch (m_mode) {
    case 3:
        m_pSaveStream->WriteColour(*value);
        break;
    case 2: {
        CRGBColour colour;
        *value = m_pLoadStream->ReadColour(colour);
        break;
    }
    }
}