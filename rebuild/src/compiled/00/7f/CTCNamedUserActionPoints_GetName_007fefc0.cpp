class CCharString { public:
    char* p;
    void Assign(const CCharString& src);
};

class CTCNamedUserActionPoints { public:
    char pad[0x34];
    CCharString* arr;
    void GetName(long index, CCharString* out);
};

void CTCNamedUserActionPoints::GetName(long index, CCharString* out)
{
    out->Assign(this->arr[index]);
}