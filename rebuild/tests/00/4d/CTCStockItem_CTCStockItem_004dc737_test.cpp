// Behaviour test for CTCStockItem::CTCStockItem (0x004DC737): for sale, stealable, not initialised, zero price, empty handle.
#include <cstdio>

class CThing { public: int id; };
static CThing* g_baseParent = 0;
static int g_handleBaseCtors = 0;

class CTCBase { public:
    CThing* ParentThing;
    virtual ~CTCBase() { }
    CTCBase(CThing* parent) : ParentThing(parent) { g_baseParent = parent; }
};

class CBaseClass { public:
    CBaseClass() { ++g_handleBaseCtors; }
    virtual ~CBaseClass() { }
};

class CEnginePersistentPrimitiveBase;
struct CIVCountedPointer {
    CEnginePersistentPrimitiveBase* m_pObject;
    CIVCountedPointer() : m_pObject(0) { }
};

class CEnginePrimitiveHandle : public CBaseClass { public:
    CIVCountedPointer Data;
    CEnginePrimitiveHandle() { }
};

class CTCStockItem : public CTCBase { public:
    bool                   ForSale;
    bool                   Stealable;
    bool                   Initialised;
    long                   Price;
    CEnginePrimitiveHandle PrimHandle;
    CTCStockItem(CThing* parent);
};

CTCStockItem::CTCStockItem(CThing* parent)
    : CTCBase(parent)
    , ForSale(true)
    , Stealable(true)
    , Initialised(false)
    , Price(0)
{
}

int main()
{
    CThing thing;
    CTCStockItem item(&thing);
    if (g_baseParent != &thing || !item.ForSale || !item.Stealable || item.Initialised || item.Price != 0
        || g_handleBaseCtors != 1 || item.PrimHandle.Data.m_pObject != 0) {
        std::printf("004dc737_TEST FAIL\n"); return 1;
    }
    std::printf("004dc737_TEST PASS\n");
    return 0;
}