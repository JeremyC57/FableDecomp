// test for 0041919c GFLimitReturn<double>
#include <stdio.h>
template <class T> const T& __fastcall GFLimitReturn(const T& value, const T& lower, const T& upper)
{
    return value < lower ? lower : value > upper ? upper : value;
}
template const double& __fastcall GFLimitReturn<double>(const double&, const double&, const double&);
int main()
{
    double lo = 1.0, hi = 5.0, a = 0.5, b = 3.0, c = 7.0, d = 5.0, e = 1.0;
    int ok = &GFLimitReturn(a, lo, hi) == &lo && &GFLimitReturn(b, lo, hi) == &b && &GFLimitReturn(c, lo, hi) == &hi
          && &GFLimitReturn(d, lo, hi) == &d && &GFLimitReturn(e, lo, hi) == &e;
    printf(ok ? "0041919c_TEST PASS\n" : "0041919c_TEST FAIL\n");
    return ok ? 0 : 1;
}