// Behaviour test for 0x005E9820 CPersistContext_TransferableVector<unsigned long>::Resize(long) (agentFAM05 respell; the catalog test
// linked the old local symbol vec_Umove (its __fastcall shim does not build under g++)). Self-contained on both compilers: the row's own body over the standard library
// vector (the row object's STLport resize and _M_fill_insert are not linkable from a test TU), checking that Resize grows the
// transferred vector with unsigned long() zeros and shrinks it in place.
#include <cstdio>
#include <vector>
class CPersistContext_TransferableVectorGenericType {
public:
    virtual long Size() = 0;
    virtual void Resize(long) = 0;
};
template<class T> class CPersistContext_TransferableVector : public CPersistContext_TransferableVectorGenericType {
public:
    CPersistContext_TransferableVector(std::vector<T>& vector) : TransVector(&vector) {}
private:
    virtual long Size() { return (long)TransVector->size(); }
    virtual void Resize(long size) { TransVector->resize(size, T()); }
    std::vector<T>* TransVector;
};
int main()
{
    std::vector<unsigned long> values;
    values.push_back(25ul);
    CPersistContext_TransferableVector<unsigned long> transfer(values);
    CPersistContext_TransferableVectorGenericType& generic = transfer;
    generic.Resize(3);
    bool ok = values.size() == 3 && values[0] == 25ul && values[1] == 0ul && values[2] == 0ul;
    generic.Resize(1);
    ok = ok && values.size() == 1 && values[0] == 25ul && generic.Size() == 1;
    std::printf(ok ? "005e9820_TEST PASS\n" : "005e9820_TEST FAIL\n");
    return ok ? 0 : 1;
}
