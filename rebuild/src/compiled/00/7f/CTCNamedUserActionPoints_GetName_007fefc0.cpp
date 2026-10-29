class CCharString { public:
    char* p;
    const CCharString& operator=(const CCharString& src);   // 0x0099EFB0
};

class CTCNamedUserActionPoints { public:
    char pad[0x34];
    CCharString* arr;
    void GetName(long index, CCharString* out);
};

void CTCNamedUserActionPoints::GetName(long index, CCharString* out)
{
    *out = this->arr[index];
}