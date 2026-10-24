// NProgressDisplay::GetProgressDisplay() -- return a counted reference to the global progress display
// Retail 0x009EA060, 28 bytes. Ego_r.pdb: ?GetProgressDisplay@NProgressDisplay@@YI?AV?$CCountedPointer@
// VCAProgressDisplay@@@@XZ, i.e. `CCountedPointer<CAProgressDisplay> __fastcall GetProgressDisplay()`:
// the by-value counted pointer comes back through the hidden result pointer in ecx (`mov eax,ecx`
// returns it). The body is `return PProgressDisplay;` -- the namespace-scope static counted
// pointer NProgressDisplay::PProgressDisplay (PDB: "NProgressDisplay::`dynamic initializer for
// 'PProgressDisplay''"; retail storage 0x013CAA38 Data / 0x013CAA3C Info, the pair
// SetProgressDisplay 0x009E9FD0 assigns and the atexit release 0x0122A890 drops).
// Shape: CCountedPointer's add-ref copy constructor copied out of the global object: Data, then
// Info (loaded into edx, copied to ecx for the null test), then `++Info->owners`. Building the
// result from the two words through a separate two-argument constructor hoists the Info load
// above the Data store (26 v 28), so the global must be the counted-pointer object itself.
// Leaf: ?GetProgressDisplay@NProgressDisplay@@YI

class CAProgressDisplay;

typedef void (__fastcall *FableDestroyReferencedObject)(void* object);

struct FableReferenceCount              // CCountedPointer count block
{
    long owners;                        // +0x00
    FableDestroyReferencedObject destroy;   // +0x04
    void* object;                       // +0x08
};

template <class T>
class CCountedPointer
{
public:
    T* Data;                            // +0x00
    FableReferenceCount* Info;          // +0x04

    CCountedPointer(const CCountedPointer& other)
        : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->owners;
    }
    ~CCountedPointer();
};

namespace NProgressDisplay
{
    extern CCountedPointer<CAProgressDisplay> PProgressDisplay;      // 0x013CAA38 {Data, Info}

    CCountedPointer<CAProgressDisplay> __fastcall GetProgressDisplay();

    CCountedPointer<CAProgressDisplay> __fastcall GetProgressDisplay()
    {
        return PProgressDisplay;
    }
}
