%{
#include "rawld.h"
using namespace sword;
%}

class RawLD : public SWLD {
public:
    RawLD (const char *ipath, const char *iname = 0, const char *idesc =  0, SWDisplay * idisp = 0, SWTextEncoding encoding = ENC_UNKNOWN, SWTextDirection dir = DIRECTION_LTR, SWTextMarkup markup = FMT_UNKNOWN, const char* ilang = 0);
    virtual ~RawLD ();
    
    static char createModule (const char *path);
};

