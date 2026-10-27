// std::__copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result, const random_access_iterator_tag&, _Distance*) -- STLport random-access copy: assign last - first elements forward, return the end of the output (CInputEvent)
// Leaf: ??$__copy@PAVCInputEvent@@PAV1@H@std@@YIPAVCInputEvent@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009f6120, 68 bytes, __fastcall (first in ecx, last in edx), `ret 0Ch`.
// Identity (probable): no call, jump or data reference reaches it (unreferenced COMDAT); 52-byte stride = Ego_r.pdb CInputEvent (CInputManager::ProcessedEvents / EventStore are vector<CInputEvent>), in the CInputManager code (after StreamInputsFromFile 0x009F5E10); the byte-identical 0x009F6170 is the other source constness -- the split between the two is by position only.
// Callees: none.
// Shape (family L058): n = (last - first) / 52 by the magic divide; each trivial assignment is `mov ecx,13 ; rep movsd`
//   with the two cursors stepped beside it.
enum EInputDeviceType { };
enum EInputEventType { };
union UInputEventData {                                    // Ego_r.pdb, 32 bytes (the per-device payload)
    unsigned long Words[8];
};
class CInputEvent {                                        // Ego_r.pdb, 52 bytes
public:
    UInputEventData Data;                                 // +0x00
    EInputDeviceType DeviceType;                          // +0x20
    long DeviceNumber;                                    // +0x24
    EInputEventType Type;                                 // +0x28
    float CurrentTime;                                    // +0x2C
    float StartTime;                                      // +0x30
};
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}
template CInputEvent* __fastcall std::__copy(CInputEvent*, CInputEvent*, CInputEvent*, const std::random_access_iterator_tag&, int*);