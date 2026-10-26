class CCharString { public:
    void* p;
};

class CDefString { public:
    void* p;
    bool operator==(const CCharString& rhs) const;
};

class CThing { public:
    CDefString GetDefName() const;
};

class CThingFilter_HasDefName { public:
    CCharString m_name;
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_HasDefName::operator()(const CThing& thing) const
{
    return thing.GetDefName() == m_name;
}