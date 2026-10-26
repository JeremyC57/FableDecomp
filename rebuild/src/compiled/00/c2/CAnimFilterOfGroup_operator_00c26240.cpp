// CAnimFilterOfGroup::operator() — retail 0x00c26240
// __fastcall bool operator()(CAnimFilterOfGroup* this, CAnimationEntry* entry)

struct SubObjectAt360 {
    // free-standing helper, __fastcall(self, edx, arg) models __fastcall(ecx=self, arg)
    int Compute(int a);
};

class CAnimationEntry { public:
    char pad[0x360];
    SubObjectAt360 sub; // at +0x360
};

class CAnimFilterOfGroup { public:
    int m0;   // +0
    int m4;   // +4
    int GroupCRC;   // +8
    int Call(CAnimationEntry* entry);
};

int CAnimFilterOfGroup::Call(CAnimationEntry* entry)
{
    return entry->sub.Compute(this->m4) + this->GroupCRC;
}