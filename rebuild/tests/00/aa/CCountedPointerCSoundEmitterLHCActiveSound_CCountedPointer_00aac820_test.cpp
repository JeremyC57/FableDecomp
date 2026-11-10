// Behaviour test for 0x00AAC820 CCountedPointer<CSoundEmitterLH::CActiveSound>::CCountedPointer(CActiveSound*): a non-null sound gets
// a count block {1, DeleteData, Data}; a null one leaves Info null; the stored deleter destroys the sound (virtual destructor).
#include <stdio.h>
class CSoundEmitterLH {
public:
    class CActiveSound {
    public:
        static int Live;
        CActiveSound() { ++Live; }
        virtual ~CActiveSound() { --Live; }
    };
};
int CSoundEmitterLH::CActiveSound::Live = 0;
struct CCPPointerInfo {
    unsigned long RefCount; void (*DeleteFunc)(void*); void* Data;
    CCPPointerInfo(unsigned long r, void (*f)(void*), void* d) : RefCount(r), DeleteFunc(f), Data(d) { }
};
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    static void DeleteData(void* data) { delete static_cast<T*>(data); }
    CCountedPointer(T* data) : Data(data) { if (data) Info = new CCPPointerInfo(1, DeleteData, (void*)Data); else Info = 0; }
};
int main()
{
    typedef CSoundEmitterLH::CActiveSound CActiveSound;
    CActiveSound* s = new CActiveSound;
    CCountedPointer<CActiveSound> p(s);
    bool ok = p.Data == s && p.Info && p.Info->RefCount == 1 && p.Info->Data == s && CActiveSound::Live == 1;
    p.Info->DeleteFunc(p.Info->Data);
    ok = ok && CActiveSound::Live == 0;
    delete p.Info;
    CCountedPointer<CActiveSound> q(0);
    ok = ok && q.Data == 0 && q.Info == 0;
    printf(ok ? "00aac820_TEST PASS\n" : "00aac820_TEST FAIL\n");
    return ok ? 0 : 1;
}
