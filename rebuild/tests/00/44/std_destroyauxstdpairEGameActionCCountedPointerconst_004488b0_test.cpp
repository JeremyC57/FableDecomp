#include <cstdio>
class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CInputTypeBase;
enum EGameAction { GAME_ACTION_NONE };
namespace std {
struct __false_type {};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class ForwardIter>
void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template void __fastcall __destroy_aux<std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >*>(std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >*, std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >*, const __false_type&);
}


#include <cstring>
static int g_deleted;
static void __fastcall DeleteIt(void*) { ++g_deleted; }
typedef std::pair<EGameAction, CCountedPointer<const CInputTypeBase> > Elem;
int main()
{
    static Elem e[3]; std::memset(e, 0, sizeof e);
    CCPPointerInfo* a = new CCPPointerInfo; a->RefCount = 1; a->DeleteFunc = DeleteIt; a->Data = 0;
    CCPPointerInfo b; b.RefCount = 2; b.DeleteFunc = DeleteIt; b.Data = 0;
    int k = 0; e[k].second.Info = a; k = 1; e[k].second.Info = &b; k = 2; e[k].second.Info = 0;
    std::__destroy_aux(e, e + 3, std::__false_type());
    bool ok = g_deleted == 1 && b.RefCount == 1;
    for (k = 0; k < 3; ++k) ok = ok && e[k].second.Info == 0;
    std::__destroy_aux(e, e, std::__false_type());
    if (ok) std::printf("004488b0_TEST PASS\n");
    return 0;
}