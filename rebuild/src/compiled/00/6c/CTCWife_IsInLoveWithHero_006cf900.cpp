class CTCWife { public:
    char pad0[0x10];
    int state;
    int IsInLoveWithHero();
public:
    bool IsCourtingHero() const;   // 0x006CF890
};

int CTCWife::IsInLoveWithHero()
{
    if (IsCourtingHero())
    {
        if (this->state == 0xc)
            return 1;
    }
    return 0;
}