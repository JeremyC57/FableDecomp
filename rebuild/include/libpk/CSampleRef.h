#ifndef FABLE_LIBPK_CSAMPLEREF_H
#define FABLE_LIBPK_CSAMPLEREF_H
// Descriptive library names; native C25C50/C25C70/C25CB0, existing CSampleRef destructor binding.
class CSoundSample;
class CSampleRef {
public:
 CSampleRef();
 ~CSampleRef();
 void Bind(CSoundSample*,long,long,long);
 void Release();
 CSoundSample* Sample;
 long Key1,Key2,Key3;
};
#endif
