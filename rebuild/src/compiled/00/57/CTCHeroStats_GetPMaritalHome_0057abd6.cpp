// CTCHeroStats::GetPMaritalHome(const CThing*) const -- the building the hero shares with this spouse (first marriage record naming her whose home still exists as a building), else null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057ABD6, 83 bytes.
// Callees: CTCBase::GetWorld() @ 0x00686AF0 (returns the world global @ 0x013B8A1C),
//          CWorld::GetThingFromUID(unsigned __int64) @ 0x0051ED60 (thing map at +0x58; manifest label
//          "CStopWatch::GetTicks" is propagated).
//
// Shape: a list walk over Marriages (retail head-node pointer +0x1D8): `node = head->next`, the empty test before the
// spouse argument is loaded into ebp; a 64-bit WifeUID == UniqueID compare (`jne` on each half); the home lookup pushes
// the by-value MaritalHomeUID BEFORE fetching the world (`GetWorld()->GetThingFromUID(uid)`: arguments first, then
// the object expression); a found thing whose type byte (+0x10) is 3 is returned straight from eax, the loop exit
// shares `xor eax,eax`. Size TU: the UID halves are pushed straight from memory (`push dword ptr [esi+14h]`) and the
// register colouring (ebx = this, edi = head, ebp = spouse) follows; the speed form loads through edx/eax and
// colours edi = this.
// Layout: Marriages is +0x1D8 in retail (PDB +0x1F0; see HasMarriageInState 0x0057AC29, landed); CHeroMarriageInfo
// from the PDB (WifeUID +0x00, MaritalHomeUID +0x08 -> node +0x08/+0x10); CThing::UniqueID +0x18 (PDB).
#pragma optimize("s", on)

class CThing { public:
    unsigned char    _pad_0x00[0x10];
    unsigned char    ThingType;                // +0x10 (3 = building)
    unsigned char    _pad_0x11[0x7];
    unsigned __int64 UniqueID;                 // +0x18
};

class CWorld { public:
    CThing* GetThingFromUID(unsigned __int64 uid);   // 0x0051ED60
};

struct CHeroMarriageInfoNode {                 // std::list<CHeroMarriageInfo> node
    CHeroMarriageInfoNode* Next;               // +0x00
    CHeroMarriageInfoNode* Prev;               // +0x04
    unsigned __int64       WifeUID;            // +0x08
    unsigned __int64       MaritalHomeUID;     // +0x10
    long                   State;              // +0x18 EMarriageState
    bool                   GayMarriage;        // +0x1C
};

struct CTCHeroStats_Retail {
    unsigned char          _pad_0x00[0x1D8];
    CHeroMarriageInfoNode* Marriages_Head;     // +0x1D8 (PDB +0x1F0)

    CWorld* GetWorld() const;                  // 0x00686AF0
    CThing* GetPMaritalHome(const CThing* wife) const;
};

CThing* CTCHeroStats_Retail::GetPMaritalHome(const CThing* wife) const
{
    CHeroMarriageInfoNode* head = Marriages_Head;
    for (CHeroMarriageInfoNode* node = head->Next; node != head; node = node->Next) {
        if (node->WifeUID == wife->UniqueID) {
            CThing* home = GetWorld()->GetThingFromUID(node->MaritalHomeUID);
            if (home && home->ThingType == 3)
                return home;
        }
    }
    return 0;
}