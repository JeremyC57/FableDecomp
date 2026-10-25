// Behaviour test for 0x005BC68F NInventory::CTCInventoryBase::ProcessButtonXReleased: the method comes from the source object;
// every other slot records its index.
#include <cstdio>
namespace NInventory {

class CTCInventoryBase {
public:
    CTCInventoryBase() : Hits(0), LastSlot(-1) {}
    virtual ~CTCInventoryBase() {}
    virtual void _v01() { ++Hits; LastSlot = 1; }
    virtual void _v02() { ++Hits; LastSlot = 2; }
    virtual void _v03() { ++Hits; LastSlot = 3; }
    virtual void _v04() { ++Hits; LastSlot = 4; }
    virtual void _v05() { ++Hits; LastSlot = 5; }
    virtual void _v06() { ++Hits; LastSlot = 6; }
    virtual void _v07() { ++Hits; LastSlot = 7; }
    virtual void _v08() { ++Hits; LastSlot = 8; }
    virtual void _v09() { ++Hits; LastSlot = 9; }
    virtual void _v10() { ++Hits; LastSlot = 10; }
    virtual void _v11() { ++Hits; LastSlot = 11; }
    virtual void _v12() { ++Hits; LastSlot = 12; }
    virtual void _v13() { ++Hits; LastSlot = 13; }
    virtual void _v14() { ++Hits; LastSlot = 14; }
    virtual void _v15() { ++Hits; LastSlot = 15; }
    virtual void _v16() { ++Hits; LastSlot = 16; }
    virtual void _v17() { ++Hits; LastSlot = 17; }
    virtual void _v18() { ++Hits; LastSlot = 18; }
    virtual void _v19() { ++Hits; LastSlot = 19; }
    virtual void _v20() { ++Hits; LastSlot = 20; }
    virtual void _v21() { ++Hits; LastSlot = 21; }
    virtual void _v22() { ++Hits; LastSlot = 22; }
    virtual void _v23() { ++Hits; LastSlot = 23; }
    virtual void _v24() { ++Hits; LastSlot = 24; }
    virtual void _v25() { ++Hits; LastSlot = 25; }
    virtual void _v26() { ++Hits; LastSlot = 26; }
    virtual void _v27() { ++Hits; LastSlot = 27; }
    virtual void _v28() { ++Hits; LastSlot = 28; }
    virtual void _v29() { ++Hits; LastSlot = 29; }
    virtual void _v30() { ++Hits; LastSlot = 30; }
    virtual void _v31() { ++Hits; LastSlot = 31; }
    virtual void _v32() { ++Hits; LastSlot = 32; }
    virtual void _v33() { ++Hits; LastSlot = 33; }
    virtual void _v34() { ++Hits; LastSlot = 34; }
    virtual void _v35() { ++Hits; LastSlot = 35; }
    virtual void _v36() { ++Hits; LastSlot = 36; }
    virtual void _v37() { ++Hits; LastSlot = 37; }
    virtual void ProcessInventoryInputUp() { ++Hits; LastSlot = 38; } // slot 38 (+0x98)
    virtual void ProcessInventoryInputDown() { ++Hits; LastSlot = 39; } // slot 39 (+0x9c)
    virtual void ProcessInventoryInputLeft() { ++Hits; LastSlot = 40; } // slot 40 (+0xa0)
    virtual void ProcessInventoryInputRight() { ++Hits; LastSlot = 41; } // slot 41 (+0xa4)
    virtual void _v42() { ++Hits; LastSlot = 42; }
    virtual void _v43() { ++Hits; LastSlot = 43; }
    virtual void _v44() { ++Hits; LastSlot = 44; }
    virtual void _v45() { ++Hits; LastSlot = 45; }
    virtual void _v46() { ++Hits; LastSlot = 46; }
    virtual void _v47() { ++Hits; LastSlot = 47; }
    virtual void _v48() { ++Hits; LastSlot = 48; }
    virtual void _v49() { ++Hits; LastSlot = 49; }
    virtual void ProcessButtonAPressed() { ++Hits; LastSlot = 50; } // slot 50 (+0xc8)
    virtual void ProcessButtonBPressed() { ++Hits; LastSlot = 51; } // slot 51 (+0xcc)
    virtual void ProcessButtonXPressed() { ++Hits; LastSlot = 52; } // slot 52 (+0xd0)
    virtual void ProcessButtonYPressed() { ++Hits; LastSlot = 53; } // slot 53 (+0xd4)
    virtual void ProcessButtonAHeld() { ++Hits; LastSlot = 54; } // slot 54 (+0xd8)
    virtual void ProcessButtonBHeld() { ++Hits; LastSlot = 55; } // slot 55 (+0xdc)
    virtual void ProcessButtonXHeld() { ++Hits; LastSlot = 56; } // slot 56 (+0xe0)
    virtual void ProcessButtonYHeld() { ++Hits; LastSlot = 57; } // slot 57 (+0xe4)
    virtual void ProcessButtonAReleased() { ++Hits; LastSlot = 58; } // slot 58 (+0xe8)
    virtual void ProcessButtonBReleased() { ++Hits; LastSlot = 59; } // slot 59 (+0xec)
    virtual void ProcessButtonXReleased();                 // slot 60 (+0xf0)
    virtual void ProcessButtonYReleased() { ++Hits; LastSlot = 61; } // slot 61 (+0xf4)
    virtual void ProcessDPadUpReleased() { ++Hits; LastSlot = 62; } // slot 62 (+0xf8)
    virtual void ProcessDPadDownReleased() { ++Hits; LastSlot = 63; } // slot 63 (+0xfc)
    virtual void ProcessDPadLeftReleased() { ++Hits; LastSlot = 64; } // slot 64 (+0x100)
    virtual void ProcessDPadRightReleased() { ++Hits; LastSlot = 65; } // slot 65 (+0x104)
    int Hits; int LastSlot;
};

} // namespace NInventory
int main()
{
    NInventory::CTCInventoryBase b; int ok = 1;
    b.ProcessButtonXReleased(); ok &= b.Hits == 0 && b.LastSlot == -1;
    if (!ok) { std::printf("005bc68f_TEST FAIL\n"); return 1; }
    std::printf("005bc68f_TEST PASS\n");
    return 0;
}
