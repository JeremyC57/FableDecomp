// StrPtr::operator!=  retail 0x00c36150
// this in ecx points to a char* (the held string pointer); param is a char*.
// Body: r = (*g_cmp)(*this, rhs); return (r == 0x102) ? 1 : 0;

class StrPtr { public:
    char* p;
};

typedef int (__stdcall *CmpFn)(char*, char*);
CmpFn g_cmp;

int __fastcall StrPtr_operator_ne(StrPtr* ptr, int /*edx*/, char* rhs)
{
    return (*g_cmp)(ptr->p, rhs) == 0x102;
}