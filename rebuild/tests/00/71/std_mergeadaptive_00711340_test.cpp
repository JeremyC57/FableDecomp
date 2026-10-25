// Behaviour test for 0x00711340 std::__merge_adaptive over list<CAnimSet>::const_iterator with
// CAnimSetItPriorityLessThan: merging two sorted runs is ordered and stable for every non-empty buffer size (whole-run buffer
// merge forward or backward, and the divide-and-conquer recursion through lower/upper_bound + __rotate_adaptive);
// a zero-size buffer never reaches this body (stable_sort takes __inplace_stable_sort instead), and loops if forced
#include <cstdio>

namespace model {

struct CAnimSet { int Priority; int Tag; };
struct It { const CAnimSet* Node; };

bool less(const It& a, const It& b) { return a.Node->Priority < b.Node->Priority; }   // CAnimSetItPriorityLessThan

It* copy(It* first, It* last, It* result) { for (int n = (int)(last - first); n > 0; --n) *result++ = *first++; return result; }
It* copy_backward(It* first, It* last, It* result) { for (int n = (int)(last - first); n > 0; --n) *--result = *--last; return result; }
It* merge(It* f1, It* l1, It* f2, It* l2, It* out)
{
    while (f1 != l1 && f2 != l2) {
        if (less(*f2, *f1)) *out++ = *f2++;
        else *out++ = *f1++;
    }
    out = copy(f1, l1, out);
    return copy(f2, l2, out);
}
It* merge_backward(It* f1, It* l1, It* f2, It* l2, It* result)
{
    if (f1 == l1) return copy_backward(f2, l2, result);
    if (f2 == l2) return copy_backward(f1, l1, result);
    --l1; --l2;
    for (;;) {
        if (less(*l2, *l1)) {
            *--result = *l1;
            if (f1 == l1) return copy_backward(f2, ++l2, result);
            --l1;
        }
        else {
            *--result = *l2;
            if (f2 == l2) return copy_backward(f1, ++l1, result);
            --l2;
        }
    }
}
It* lower_bound(It* first, It* last, const It& v) { while (first != last && less(*first, v)) ++first; return first; }
It* upper_bound(It* first, It* last, const It& v) { while (first != last && !less(v, *first)) ++first; return first; }
It* rotate_adaptive(It* first, It* middle, It* last, int len1, int len2, It* buffer, int buffer_size)
{
    It* buffer_end;
    if (len1 > len2 && len2 <= buffer_size) {
        buffer_end = copy(middle, last, buffer);
        copy_backward(first, middle, last);
        return copy(buffer, buffer_end, first);
    }
    else if (len1 <= buffer_size) {
        buffer_end = copy(first, middle, buffer);
        copy(middle, last, first);
        return copy_backward(buffer, buffer_end, last);
    }
    It tmp[64];
    It* e = copy(middle, last, tmp);
    e = copy(first, middle, e);
    copy(tmp, e, first);
    return first + (last - middle);
}

void merge_adaptive(It* first, It* middle, It* last, int len1, int len2, It* buffer, int buffer_size)
{
    if (len1 <= len2 && len1 <= buffer_size) {
        It* buffer_end = copy(first, middle, buffer);
        merge(buffer, buffer_end, middle, last, first);
    }
    else if (len2 <= buffer_size) {
        It* buffer_end = copy(middle, last, buffer);
        merge_backward(first, middle, buffer, buffer_end, last);
    }
    else {
        It* first_cut = first;
        It* second_cut = middle;
        int len11 = 0;
        int len22 = 0;
        if (len1 > len2) {
            len11 = len1 / 2;
            first_cut += len11;
            second_cut = lower_bound(middle, last, *first_cut);
            len22 = (int)(second_cut - middle);
        }
        else {
            len22 = len2 / 2;
            second_cut += len22;
            first_cut = upper_bound(first, middle, *second_cut);
            len11 = (int)(first_cut - first);
        }
        It* new_middle = rotate_adaptive(first_cut, middle, second_cut, len1 - len11, len22, buffer, buffer_size);
        merge_adaptive(first, first_cut, new_middle, len11, len22, buffer, buffer_size);
        merge_adaptive(new_middle, second_cut, last, len1 - len11, len2 - len22, buffer, buffer_size);
    }
}

}

int main()
{
    using namespace model;
    CAnimSet sets[40];
    int bad = 0;
    for (int len1 = 0; len1 <= 20; len1 += 4)
        for (int len2 = 0; len2 <= 20; len2 += 5)
            for (int bufsize = 1; bufsize <= 25; bufsize += 2) {   // the stable_sort caller only merges with a non-empty buffer
                int n = len1 + len2;
                It v[40], buffer[40];
                for (int i = 0; i < len1; ++i) { sets[i].Priority = (i * 7) / 5; sets[i].Tag = i; }
                for (int i = 0; i < len2; ++i) { sets[len1 + i].Priority = (i * 3) / 2; sets[len1 + i].Tag = len1 + i; }
                for (int i = 0; i < n; ++i) v[i].Node = &sets[i];
                merge_adaptive(v, v + len1, v + n, len1, len2, buffer, bufsize);
                int seen = 0;
                for (int i = 0; i < n; ++i) {
                    seen += v[i].Node->Tag;
                    if (i > 0) {
                        if (v[i].Node->Priority < v[i - 1].Node->Priority) bad = 1;
                        if (v[i].Node->Priority == v[i - 1].Node->Priority && v[i].Node->Tag < v[i - 1].Node->Tag) bad = 2;   // stable
                    }
                }
                if (seen != n * (n - 1) / 2) bad = 3;
            }
    if (bad) { std::printf("00711340_TEST FAIL\n"); return 1; }
    std::printf("00711340_TEST PASS\n");
    return 0;
}