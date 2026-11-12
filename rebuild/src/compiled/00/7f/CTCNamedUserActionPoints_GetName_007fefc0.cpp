// CTCNamedUserActionPoints::GetName(long index, CCharString& name) -- copy the name of the indexed action point into `name`
// Leaf: ?GetName@CTCNamedUserActionPoints@@QAEXJAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FEFC0, 23 bytes (`ret 8`).
// Flags: none
// Name-fix pass (2026-11-12): Ego_r.pdb 0x007F2830 + FableWin.pdb ?GetName@CTCNamedUserActionPoints@@QAEXJAAVCCharString@@@Z (the out
//   parameter is a reference; CTCTavernTable::GetEatingPoint 0x007B8B10 passes the address of a CCharString local and binds this symbol);
//   was spelled GetName(long, CCharString*).
class CCharString { public:
    char* p;
    const CCharString& operator=(const CCharString& src);   // 0x0099EFB0
};

class CTCNamedUserActionPoints { public:
    char pad[0x34];
    CCharString* arr;
    void GetName(long index, CCharString& out);
};

void CTCNamedUserActionPoints::GetName(long index, CCharString& out)
{
    out = this->arr[index];
}