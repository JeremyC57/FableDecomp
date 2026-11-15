#ifndef FABLE_LIBPK_PKCDEFAULTFILEMANAGER_H
#define FABLE_LIBPK_PKCDEFAULTFILEMANAGER_H
class CAudioFile;
// Real library class: native trace PKCDefaultFileManager.cpp and factory00C1ABB0.
// External declarations only; no manager fields or complete size are asserted.
class PKCDefaultFileManager {
public:
    bool ReleaseFile(unsigned long key); // native00C1A870; existing landed method name.
    void ReleaseFileObject(CAudioFile* file); // native00C19FA0; descriptive member name.
};
#endif
